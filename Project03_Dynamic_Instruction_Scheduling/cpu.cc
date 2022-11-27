

CPU_OOO::CPU_OOO(proc_params p):params(p){
    info_size = 2*getWIDTH() + getRobSize();
    Rmt = new Rmt_REG[getArfSize()]();
    Rob =  ROB( params );
    Iq = IQ( params );
    De = new PIPE_REG[getWIDTH()]();
    Rn = new PIPE_REG[getWIDTH()]();
    Rr = new PIPE_REG[getWIDTH()]();
    Di = new PIPE_REG[getWIDTH()]();
    Exc_lst = EXC_LIST( params );
    Wb = new WB_REG[5*getWIDTH()]();

    Rn_inf = new INST_INFO[getWIDTH()]();
    De_inf = new INST_INFO[getWIDTH()]();
    info = new INST_INFO[getInfoSize()]();

    cycle = last_cycle = last_seq = 0;
    Avail = (AVAIL){ true };
    done_flag = false;

    for(int k=0;k<getWIDTH();k++){
        De[k].seq=-1;
        Rn[k].seq=-1;
        Rr[k].seq=-1;
        Di[k].seq=-1;
    }
    PIPE_Info init = (PIPE_Info){ .bc=-1, .dur=-1 };
    for(int k=0;k<getInfoSize();k++){
        info[k] = (INST_INFO){.seq=-1,.op=-1,.src1=-1,.src2=-1,.dst=-1,
            .FE=init,.DE=init,.RN=init,.RR=init,.DI=init,.IS=init,.EX=init,.WB=init,.RT=init};
    }
}
void CPU_OOO::Fetch(INST_INFO* inst_input){

    if( Avail.DE ){
        Avail.FE = true;
        for(int k=0;k<getWIDTH();k++){

            INST_INFO ii= inst_input[k];
            De[k] = (PIPE_REG){ .seq = ii.seq , .op = ii.op ,.src1 = ii.src1 , .src2 = ii.src2 , .dst = ii.dst, .rob1=false, .rob2=false };

        }
    }else{  // if we stall,
        Avail.FE = false;
    }
}
void CPU_OOO::Decode(){

    if( Avail.RN ){
        Avail.DE = true;
        for(int k=0;k<getWIDTH();k++)
            Rn[k] = De[k];
    }else{
        Avail.DE = false;
    }
}

void CPU_OOO::Rename(){

    if( !Avail.RR ){
        Avail.RN = false;
        // do nothing, keep value of pipeline register between RN and RR stage
    }else if( !Rob.RobIsFull() ){

        Avail.RN = true;

        for(int k=0;k<getWIDTH();k++){

            PIPE_REG rn = Rn[k];

            if( rn.seq != -1 ){
                // if find src in Rename Mapped Table, rename src
                if( rn.src1 != -1 ){
                    if( Rmt[ rn.src1 ].v ){
                        Rn[k].src1 = Rmt[ rn.src1 ].Rob_tag;
                        Rn[k].rob1 = true;
                        //Rr[k].rob1 = !Exc_lst.FindWb(Rr[k].src1); ???
                        //if( Rn[k].rob1 ) Rn[k].rob1 = !FindWb( Rn[k].src1 );
                    }
                }
                if( rn.src2 != -1 ){
                    if( Rmt[ rn.src2 ].v ){
                        Rn[k].src2 = Rmt[ rn.src2 ].Rob_tag;
                        Rn[k].rob2 = true;
                        //Rr[k].rob2 = !Exc_lst.FindWb(Rr[k].src2); ???
                        //if( Rn[k].rob2 ) Rn[k].rob2 = !FindWb( Rn[k].src2 );
                    }
                }

                // if inst has dst, update Rmt's dst entry
                if( rn.dst != -1 )
                    Rmt[ rn.dst ] = (Rmt_REG){ .v=true, .Rob_tag=Rob.GetTail() };

                // rename inst dst as rob tag
                Rn[k].dst = Rob.GetTail();

                // allocate a row in reorder buffer for this instruction
                // no matter inst has dst or not
                Rob.Allocate( rn.seq, rn.dst, false);
            }
            // assign pipeline register to Read Register
            Rr[k] = Rn[k];
        } // end for k
    }else{
        // if Rename stall, no inst for register read stage
        Avail.RN = false;
        for(int k=0;k<getWIDTH();k++)
            Rr[k] = (PIPE_REG){ .seq=-1, .op=-1,.src1=-1,.src2=-1,.dst=-1 };
    }
}
void CPU_OOO::RegRead(){
    //cout<<"< RR > ";
    for(int k=0;k<getWIDTH();k++){
        //cout<<" RR seq: "<<Rr[k].seq<<" rob1: "<<Rr[k].rob1<<" src1: "<<Rr[k].src1<<" rob2: "<<Rr[k].rob2<<" src2: "<<Rr[k].src2<<endl;
        // search write back result (Rob) and execute result (Wb)
        if( Rr[k].rob1 ) Rr[k].rob1 = ((!FindWb( Rr[k].src1 )) && (!Rob.GetRdy( Rr[k].src1 )));
        if( Rr[k].rob2 ) Rr[k].rob2 = ((!FindWb( Rr[k].src2 )) && (!Rob.GetRdy( Rr[k].src2 )));
    }

    if( Avail.DI ){
        Avail.RR = true;

        for(int k=0;k<getWIDTH();k++)
            Di[k] = Rr[k];

    } else {
        bool empty = true;
        for(int k=0;k<getWIDTH();k++)
            if( Rr[k].seq != -1) empty = false;

        if( empty )
            Avail.RR = true;
        else
            Avail.RR = false;
        //cout<<"RR stall\n";
    }

}
void CPU_OOO::Dispatch(){
    //if(cycle>200 &&cycle<300) Iq.PrintIq(1);

    for(int k=0;k<getWIDTH();k++){

        if( Di[k].seq == -1){
            // no instructions
        }else{
            bool rs1_rdy,rs2_rdy;
            rs1_rdy = rs2_rdy = true;

            rs1_rdy = (Di[k].rob1)? FindWb(Di[k].src1) || Rob.GetRdy( Di[k].src1 ) : true;
            rs2_rdy = (Di[k].rob2)? FindWb(Di[k].src2) || Rob.GetRdy( Di[k].src2 ) : true;
            Di[k].rob1 = !rs1_rdy;
            Di[k].rob2 = !rs2_rdy;
        }
    }
    if(Avail.IS){
        Avail.DI = true;
        // store instruction into Issue Queue
        for(int k=0;k<getWIDTH();k++){

            if( Di[k].seq == -1){
                // no instructions
            }else{
                    bool rs1_rdy = !Di[k].rob1;
                    bool rs2_rdy = !Di[k].rob2;

                    // allocate inst in issue queue
                    int idx_iq = Iq.FindInv();
                    if( idx_iq == -1 ){ cout<<"ERROR, issue queue is full !"<<endl; exit(0); }
                    IQS v_bit =  ( rs1_rdy && rs2_rdy )? RDY : NRDY;

                    Iq_REG iq_reg = (Iq_REG){ .v=v_bit, .seq=Di[k].seq , .op=Di[k].op,
                         .dst_tag=Di[k].dst,
                        .rs1_rdy= rs1_rdy, .rs1_tag=Di[k].src1,
                        .rs2_rdy= rs2_rdy, .rs2_tag=Di[k].src2,   };
                    Iq.SetIq( idx_iq, iq_reg);
                    Iq.IncCnt();
                }
        }// end for
    } else {
        bool empty=true;
        for(int k=0;k<getWIDTH();k++)
            if( Di[k].seq != -1) empty = false;

        if( empty )
            Avail.DI = true;
        else
            Avail.DI = false;
        //cout<<"DI stall\n";
    }
}
void CPU_OOO::Issue(){
    //if(cycle>200 &&cycle<300) Iq.PrintIq(2);

    // issue ready instruction from Issue Queue
    for(int k=0;k<getWIDTH();k++){
        int idx_iq = Iq.FindRdy();

        if( idx_iq != -1 ){
            Iq_REG reg = Iq.GetIq( idx_iq );
            //int seq = reg.seq;
            if( Exc_lst.Add(reg.seq, reg.op, reg.dst_tag) != -1 ){
                // if cpu has idle function unit, it add inst into execute list
                // then cpu remove inst from Issue Queue
                Iq.Invalidation( idx_iq );

            }
        }
    }// end for k

    if(Iq.IqIsFull()){
        Avail.IS = false;
        //cout<<"after IS, IQ is full\n";
    }else
        Avail.IS = true;

}

void CPU_OOO::Execute(){
    // Function Unit stage never stall

    Exc_lst.DecCnt();

    for(int k=0;k<5*(int)params.width;k++){
        EXC_REG ee = Exc_lst.GetFu(k);

        if( ee.cnt == 0){    // cnt == 0 means inst done in this cycle
            Iq.Update(ee.dst_tag);     // wakeup depence inst in Issue Queue
            // Also, wakeup depence inst in DI, RR
            // However, I implement this part at the beginnning of DI and RR stage

            //Add the instruction to WB
            Wb[k] = (WB_REG){ .v=true, .seq=ee.seq, .Rob_tag=ee.dst_tag };

        }else
            Wb[k] = (WB_REG){ .v=false };
    }
}
void CPU_OOO::Writeback(){
    // write back stage never stall
    for(int k=0;k<5*(int)params.width;k++){
        // for each inst in WB, mark the inst as ready in its ROB entry
        if( Wb[k].v ){
            // update ROB
            Rob.SetRdy( Wb[k].Rob_tag );
        }
    }
}
void CPU_OOO::Retire(){

    //if(cycle>200 && cycle<300) Rob.PrintRob();
    bool empty = false;
    for(int k=0;k<getWIDTH();k++){

        Rob_REG rob_reg = Rob.CurrentRob();

        if( !empty ){
            if( rob_reg.rdy ){      // this inst is ready to retire
                if( rob_reg.dst != -1 ){
                    // when a inst retire in this cycle and it has dst
                    // check its RMT entry, if the Rob_tag matchs, clear its valid bit and Rob_tag
                    if( Rmt[rob_reg.dst].v && (Rmt[rob_reg.dst].Rob_tag == Rob.GetHead() ))
                        Rmt[rob_reg.dst] = (Rmt_REG){ .v=false, .Rob_tag=0  };
                }
                done_flag = empty = Rob.Retire();
                // update & print out info
                uint32  idx = getInfoIdx( rob_reg.seq );
                info[ idx ].RT.dur = (cycle+1) - info[ idx ].RT.bc;
                if( done_flag ){
                    last_cycle = cycle+1;
                    last_seq = (uint32)( info[ idx ].seq + 1 );
                    ipc = float( last_seq )/float( last_cycle );
                }
                PrintInfo( idx );
                RstInfo(idx);

            } // end if rdy
        }else{
            // if ROB is empty, do nothing
        }

    }
}


bool CPU_OOO::Advance_Cycle(){

    cycle++;

    if(!Avail.FE)   // stall instruction input for 1 cycle
        return true;
    else
        return false;
}

void CPU_OOO::PrintInfo(int idx){
    INST_INFO ii = info[ idx ];
    cout<<ii.seq<<" fu{"<<ii.op<<"} src{"<<ii.src1<<","<<ii.src2<<"} ";
    cout<<"dst{"<<ii.dst<<"} ";
    cout<<"FE{"<<ii.FE.bc<<","<<ii.FE.dur<<"} ";
    cout<<"DE{"<<ii.DE.bc<<","<<ii.DE.dur<<"} ";
    cout<<"RN{"<<ii.RN.bc<<","<<ii.RN.dur<<"} ";
    cout<<"RR{"<<ii.RR.bc<<","<<ii.RR.dur<<"} ";
    cout<<"DI{"<<ii.DI.bc<<","<<ii.DI.dur<<"} ";
    cout<<"IS{"<<ii.IS.bc<<","<<ii.IS.dur<<"} ";
    cout<<"EX{"<<ii.EX.bc<<","<<ii.EX.dur<<"} ";
    cout<<"WB{"<<ii.WB.bc<<","<<ii.WB.dur<<"} ";
    cout<<"RT{"<<ii.RT.bc<<","<<ii.RT.dur<<"} ";
    cout<<endl;
}
void CPU_OOO::UpdInfo(){

    for(int k=0;k<getWIDTH();k++){
        int seq_de = getInfoIdx(De[k].seq);
        if( seq_de != -1 ){
            if(info[seq_de].DE.bc == -1){
                info[seq_de].DE.bc = cycle;
                info[seq_de].FE.dur = cycle - info[seq_de].FE.bc;
            }
        }
    }
    for(int k=0;k<getWIDTH();k++){
        int seq_rn = getInfoIdx(Rn[k].seq);
        if( seq_rn!= -1 ){
            if(info[seq_rn].RN.bc == -1){
                info[seq_rn].RN.bc = cycle;
                info[seq_rn].DE.dur = cycle - info[seq_rn].DE.bc;
            }
        }
    }
    for(int k=0;k<getWIDTH();k++){
        int seq_rr = getInfoIdx(Rr[k].seq);
        if( seq_rr != -1 ){
            if(info[seq_rr].RR.bc == -1){
                info[seq_rr].RR.bc = cycle;
                info[seq_rr].RN.dur = cycle - info[seq_rr].RN.bc;
            }
        }
    }
    for(int k=0;k<getWIDTH();k++){
        int seq_di = getInfoIdx(Di[k].seq);
        if( seq_di!= -1 ){
            if(info[seq_di].DI.bc == -1){
                info[seq_di].DI.bc = cycle;
                info[seq_di].RR.dur = cycle - info[seq_di].RR.bc;
            }
        }
    }
    for(int k=0;k<getIqSize();k++){
        int seq_iq = getInfoIdx(Iq.GetIq(k).seq);
        bool v_iq = Iq.GetIq(k).v;
        if( v_iq )
            if(info[seq_iq].IS.bc == -1){
                info[seq_iq].IS.bc = cycle;
                info[seq_iq].DI.dur = cycle - info[seq_iq].DI.bc;
            }
    }
    for(int k=0;k<5*getWIDTH();k++){
        int seq_ex = getInfoIdx(Exc_lst.GetFu(k).seq);
        bool v_ex = (Exc_lst.GetFu(k).cnt > 0);
        if( v_ex )
            if( info[seq_ex].EX.bc == -1 ){
                info[seq_ex].EX.bc = cycle;
                info[seq_ex].IS.dur = cycle - info[seq_ex].IS.bc;
            }
    }
    for(int k=0;k<5*getWIDTH();k++){
        int seq_wb = getInfoIdx(Wb[k].seq);
        bool v_wb = Wb[k].v;
        if( v_wb )
            if( info[seq_wb].WB.bc == -1){
                info[seq_wb].EX.dur = cycle - info[seq_wb].EX.bc;
                info[seq_wb].WB.bc = cycle;
                info[seq_wb].WB.dur = 1;
                info[seq_wb].RT.bc = cycle + 1;
            }
    }
}
void CPU_OOO::UpdPrintFetch(){
    //cout<<"[ FE ]: ";
    if(Avail.FE){
        for(int k=0;k<getWIDTH();k++){

            int seq_fe = getInfoIdx( De[k].seq );
            if( seq_fe != -1 )
            {
                //cout<<De[k].seq<<" ";
                if(info[seq_fe].FE.bc == -1){
                    info[seq_fe].seq = De[k].seq;
                    info[seq_fe].op = De[k].op;
                    info[seq_fe].src1 = De[k].src1;
                    info[seq_fe].src2 = De[k].src2;
                    info[seq_fe].dst = De[k].dst;

                    info[seq_fe].FE.bc = cycle;
                }
            }
        }
    }
    //cout<<endl;
}

void CPU_OOO::PrintConfig(){
    cout<<"# === Processor Configuration ==="<<endl;
    cout<<"# ROB_SIZE = "<<getRobSize()<<endl;
    cout<<"# IQ_SIZE  = "<<getIqSize()<<endl;
    cout<<"# WIDTH    = "<<getWIDTH()<<endl;
}
void CPU_OOO::PrintResult(){
    cout<<"# === Simulation Results ========\n";
    cout<<"# Dynamic Instruction Count    = "<<last_seq<<endl;
    cout<<"# Cycles                       = "<<last_cycle<<endl;
    cout<<"# Instructions Per Cycle (IPC) = "<<fixed<<setprecision(2)<<ipc<<endl;
}

// additional function print out information only for debugging

void CPU_OOO::PrintState(){
    cout<<"\nCycle: "<<cycle<<endl;

    cout<<"[ DE ]: ";
    for(int k=0;k<getWIDTH();k++)
        cout<<De[k].seq<<" ";
    cout<<endl;
    cout<<"[ RN ]: ";
    for(int k=0;k<getWIDTH();k++)
        cout<<Rn[k].seq<<" ";
    cout<<endl;
    cout<<"[ RR ]: ";
    for(int k=0;k<getWIDTH();k++)
        cout<<Rr[k].seq<<" ";
    cout<<endl;
    cout<<"[ DI ]: ";
    for(int k=0;k<getWIDTH();k++)
        cout<<Di[k].seq<<" ";
    cout<<endl;
    Iq.PrintIqSeq();
    Exc_lst.PrintExSeq();
    PrintWb();
    Rob.PrintRT();
}
void CPU_OOO::PrintRmt(){
    for(int k=0;k<30;k++)
        cout<<"Rmt "<<k<<", v: "<<Rmt[k].v<<" rob_tag: "<<Rmt[k].Rob_tag<<endl;
}
void CPU_OOO::PrintRmt(int k){
        cout<<"< Rmt > "<<k<<", v: "<<Rmt[k].v<<" rob_tag: "<<Rmt[k].Rob_tag<<endl;
}
void CPU_OOO::PrintWb(){
    cout<<"[ WB ]: ";
    for(int k=0; k < 5*(int)params.width;k++)
    if(Wb[k].v)
        cout<<Wb[k].seq<<" ";
    cout<<endl;
}
