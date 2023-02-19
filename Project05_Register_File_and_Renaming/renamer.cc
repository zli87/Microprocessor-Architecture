#include <renamer.h>

using namespace std;

//std::string to_string(int i){    out << i;    return out.str();}

bool renamer::RMT_Find(uint64_t x){
	for(uint64_t i=0; i < MT_SIZE; i++)
		if(x == RMT[i]) return true;
	return false;
}
renamer::renamer(uint64_t n_log_regs,
		uint64_t n_phys_regs,
		uint64_t n_branches){
	MAX_BRANCH_NUM = n_branches;
	LIST_SIZE = n_phys_regs - n_log_regs;
	MT_SIZE = n_log_regs;
	RMT = new uint64_t[MT_SIZE];
	AMT = new uint64_t[MT_SIZE];
	for(uint64_t i=0; i < MT_SIZE ;i++)
	{	RMT[i] = AMT[i] = i; }

	FL = new FreeList<uint64_t>( n_log_regs, LIST_SIZE );
	AL = new ActiveList<AL_EN>( LIST_SIZE );

	for(uint64_t i=1; i < n_phys_regs; i++)
		assertm( RMT_Find(i) ^ FL->Find(i) , "[My Error][renamer] one physical register exist in both RMT and FreeList!! " );

	PRF = new uint64_t[ n_phys_regs ];
	PRF_rdy = new bool[ n_phys_regs ];

	GBM = 0;
	for(uint64_t i=0; i < MAX_BRANCH_NUM ; i++)
		BRCP[i].RMT_shdw = new uint64_t[MT_SIZE];

	assertm( ( 1 <= n_branches ) && ( n_branches <= 64 ) ,"[My Error][renamer] number of branches is not in the range from 1 to 64 !!");
	assertm( n_phys_regs > n_log_regs , "[My Error][renamer] number pf physical registers less or equal than number of logical registers !!");
};
//
bool renamer::stall_reg(uint64_t bundle_dst)		{	return FL->IsEmpty( bundle_dst );								};

//https://stackoverflow.com/questions/26411131/iterate-through-bits-in-c
//https://stackoverflow.com/questions/20054846/how-to-loop-bit-by-bit-over-a-long-long-in-c
uint64_t renamer::getGBMnum(uint64_t _GBM_){
	uint64_t num=0;
	for ( uint64_t v = _GBM_; v; v &= v - (long)1 ) ++num;
	return num;
};
bool renamer::stall_branch(uint64_t bundle_branch)	{	return ( ( getGBMnum(GBM) + bundle_branch ) > MAX_BRANCH_NUM );	};
uint64_t renamer::get_branch_mask()					{	return GBM;														};

uint64_t renamer::rename_rsrc(uint64_t log_reg){
    return RMT[log_reg];
};

uint64_t renamer::rename_rdst(uint64_t log_reg){
	uint64_t x;
	FL->GetFreeReg(x);
	RMT[log_reg] = x;
	return x;
};
void renamer::print64bit(uint64_t _GBM_, uint64_t _highlight_, const char* _msg_){
/*	if(__PRINT__){
		cout<<"[info] "<<_msg_<<" = ";
		uint64_t branch_ID = MAX_BRANCH_NUM-1;
		uint64_t mask = (long)1 << branch_ID;
		for ( ; branch_ID > 0 ; branch_ID-- ) {
			mask = (long)1 << branch_ID;
			uint64_t bit = ( (_GBM_ & mask) > 0 )? 1 : 0;
			if( _highlight_ == branch_ID ) cout<<"("<<bit<<")";
			else cout<<bit;
			if( branch_ID % 8 == 0 ) cout<<"_";
		}
		uint64_t bit = ( (_GBM_ & 1) > 0 )? 1 : 0;
		cout<<bit<<endl;
	}*/
}
// return new branch's ID
uint64_t renamer::checkpoint(){
	uint64_t GBM_inverse = ~GBM;
	assertm( (GBM & GBM_inverse) == 0 ,"[My Error][checkpoint] GBM inverse generate error!" );
	uint64_t branch_ID = MAX_BRANCH_NUM-1;
	uint64_t mask = (long)1 << branch_ID;
	assertm( getGBMnum(mask)==1,"[My Error][checkpoint] mask generate error!" );
	// find fisrt 1 in inverse GBM == find first 0 in original GBM
	while( mask > 0 ){
		if( ( GBM_inverse & mask ) > 0 ){
			// 1. create checkpoint
			BRCP[branch_ID].GBM = GBM;	// store GBM before set branch's bit to 1, in case I forget to clear it when I resolve/recovery
			BRCP[branch_ID].FL_head = FL->Checkpoint();
			for(uint64_t i = 0 ; i < MT_SIZE ; i++ )
				BRCP[branch_ID].RMT_shdw[i] = RMT[i];
			// 2. set branch_ID bit from 0 to 1
			GBM = GBM ^ mask; // example: GBM = 1(0)10 , GBM_inv = 0(1)01, mask = 0100, GBM ^ mask = 1(1)10
			return branch_ID;
		}// end if
		branch_ID--;
		mask = mask >> (long)1;
		assertm( getGBMnum(mask)==1,"[My Error][checkpoint] mask generate error!" );
	}// end while
	assertm( 0 ,"[My Error][checkpoint] Can not create Checkpoints. No 0 bits !!");
    return 0;
};

bool renamer::stall_dispatch(uint64_t bundle_inst){
    return AL->IsFull( bundle_inst );
};

uint64_t renamer::dispatch_inst(bool dest_valid,
                       uint64_t log_reg,
                       uint64_t phys_reg,
                       bool load,
                       bool store,
                       bool branch,
                       bool amo,
                       bool csr,
                       uint64_t PC)
{
	assertm( !(AL->IsFull()) ,"[My Error][dispatch_inst] Dispatch(Add) to Full ActiveList!!");
	AL_EN AL_new_inst = (AL_EN){ .flg_dst=dest_valid, .dst_lgc=log_reg, .dst_phy=phys_reg,
		.completed=false, .exception=false, .load_viol=false, .br_misp=false, .val_misp=false,
		.load=load, .store=store, .branch=branch, .amo=amo, .csr=csr, .PC=PC };

	return AL->Add( AL_new_inst );
};

bool renamer::is_ready(uint64_t phys_reg)					{	return PRF_rdy[phys_reg];				};
void renamer::clear_ready(uint64_t phys_reg)				{	PRF_rdy[phys_reg] = false;				};
void renamer::set_ready(uint64_t phys_reg)					{	PRF_rdy[phys_reg] = true;				};
uint64_t renamer::read(uint64_t phys_reg)					{	return PRF[phys_reg];					};
void renamer::write(uint64_t phys_reg, uint64_t value)		{	PRF[phys_reg] = value;	PRF_rdy[phys_reg] = true;	};
void renamer::set_complete(uint64_t AL_index)				{	AL->queue[AL_index].completed = true;	};

void renamer::resolve(uint64_t AL_index,
         uint64_t branch_ID,
         bool correct){

	if( correct ){
		uint64_t mask = (long)1 << branch_ID;
		assertm( getGBMnum(mask)==1,"[My Error][resolve] mask generate error!" );
		// 1. clear branch's bit in GBM
		GBM = GBM ^ mask;
		// 2. clear branch's bit in all checkpoint GBMs
		for(uint64_t i=0; i < MAX_BRANCH_NUM ; i++)
			if( ( BRCP[i].GBM & mask ) > 0 ) // find checkpoint with branch's bit 1
				BRCP[i].GBM = BRCP[i].GBM ^ mask;
	} else {
		// resore GBM from checkpoint
		BRCP_EN checkpoint = BRCP[branch_ID];
		GBM = checkpoint.GBM;
		FL->Restore( checkpoint.FL_head );
		AL->Restore( AL_index );	// set tail pointer just after finish dispatch branch instruction
		for(uint64_t i = 0 ; i < MT_SIZE ; i++ )
			RMT[i] = checkpoint.RMT_shdw[i];
	}// end if/else correct
};

bool renamer::precommit(bool &completed, bool &exception, bool &load_viol, bool &br_misp, bool &val_misp,
                bool &load, bool &store, bool &branch, bool &amo, bool &csr, uint64_t &PC){
    if(AL->IsEmpty() ){ return false; }
	else{
		AL_EN AL_head;
		AL->GetHead(AL_head);
		completed = AL_head.completed;
		exception = AL_head.exception;
		load_viol = AL_head.load_viol;
		br_misp = AL_head.br_misp;
		val_misp = AL_head.val_misp;
		load = AL_head.load;
		store = AL_head.store;
		branch = AL_head.branch;
		amo = AL_head.amo;
		csr = AL_head.csr;
		PC = AL_head.PC;
		return true;
	}
};
void renamer::commit(){
	AL_EN AL_head;
	AL->GetHead(AL_head);
	assertm( AL_head.completed ,"[My Error][commit] ActiveList Head is not completed!!");
	assertm( !AL_head.exception ,"[My Error][commit] ActiveList Head is marked as an exception!!");
	assertm( !AL_head.load_viol ,"[My Error][commit] ActiveList Head is marked as an load violation!!");
	// if have destination
	if( AL_head.flg_dst ){
		// get previous physical mapping
		uint64_t previous_mapping = AMT[AL_head.dst_lgc];
		// commit new physical mapping to Architecture Mapping Table
		AMT[AL_head.dst_lgc] = AL_head.dst_phy;
		// free previous physical mapping
		FL->ReturnFreeReg( previous_mapping );
	}
	// Retire AL_head, increment head pointer
	AL->Retire(AL_head);
};

// reset to initial
void renamer::squash(){
	// 1. clear all mapping in ActiveList
	AL->Squash();
	// ??? how to recover freelist ????
	// 2. compare RMT to AMT
	// if same logic index has different physical mapping,
	// my method: 	reserve __rear__, force FreeList back to full status,
	// 				set "__front__" equals "(__rear__ + 1)% __Maxsize__"
	list_idx previous_rear = FL->rear();
	FL->Squash();
	assertm( FL->rear() == previous_rear,"[My Error][squash] squash should not change FreeList tail !!" );
	assertm( FL->IsFull(),"[My Error][squash] FreeList is not Full after squash!!" );

	// 3. clear RMT
	// method:	copy AMT to RMT
	for(uint64_t i=0; i < MT_SIZE ; i++)
		RMT[i] = AMT[i];
	// 4. clear GBM
	GBM = 0;
	// 5. clear checkpoints
	for(uint64_t i=0; i < MAX_BRANCH_NUM ;i++){
		BRCP[i].GBM = 0;
		BRCP[i].FL_head = 0;
		for(uint64_t k=0; k < MT_SIZE ;k++)
			BRCP[i].RMT_shdw[k] = 0;
	}

};

void renamer::set_exception(uint64_t AL_index)				{	AL->queue[AL_index].exception = true;	};
void renamer::set_load_violation(uint64_t AL_index)			{	AL->queue[AL_index].load_viol = true;	};
void renamer::set_branch_misprediction(uint64_t AL_index)	{	AL->queue[AL_index].br_misp = true;		};
void renamer::set_value_misprediction(uint64_t AL_index)	{	AL->queue[AL_index].val_misp = true;	};
bool renamer::get_exception(uint64_t AL_index)				{   return AL->queue[AL_index].exception;	};
