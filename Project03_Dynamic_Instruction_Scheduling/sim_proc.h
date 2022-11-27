#ifndef SIM_PROC_H
#define SIM_PROC_H

//#include "queue.h"
#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

typedef unsigned long int uint32;
typedef struct proc_params{
    unsigned long int rob_size;
    unsigned long int iq_size;
    unsigned long int width;
}proc_params;

// Put additional data structures here as per your requirement

typedef struct PIPE_Info{
        int bc;  // begin-cycle
        int dur; // duration
}PIPE_Info;

typedef struct INST_INFO{
    int      seq;
    int      op;
    int      src1;
    int      src2;
    int      dst;
    PIPE_Info   FE;
    PIPE_Info   DE;
    PIPE_Info   RN;
    PIPE_Info   RR;
    PIPE_Info   DI;
    PIPE_Info   IS;
    PIPE_Info   EX;
    PIPE_Info   WB;
    PIPE_Info   RT;
}INST_INFO;

typedef struct PIPE_REG{
    int seq; // seq is used to reconized and update info.
    int op;
    int src1;
    int src2;
    int dst;
    bool rob1;
    bool rob2;
}PIPE_REG;

// Rename Map Table
typedef struct Rmt_REG{
    bool      v;
    int      Rob_tag;
}Rmt_REG;

// Reorder Buffer
typedef struct Rob_REG{
    bool        rdy;
    int         seq;
    int         dst;       // -1: no dst, 0-66: reg tag
}Rob_REG;

class ROB{
    vector<Rob_REG> cqueue; // circular queue
    int Next(int idx){ return (idx + 1) % p.rob_size;};
public:
    proc_params p;
    int Head;
    int Tail;
    bool Full;
    ROB(){};
    ROB(proc_params);
    void Allocate(int, int, bool);
    void SetRdy(int idx){ cqueue[idx].rdy = true; };
    bool GetRdy(int idx){
        //cout<<"Rob get [ "<<idx<<" ].rdy= "<<cqueue[idx].rdy<<endl;
        return cqueue[idx].rdy; };
    int GetDst(int idx){ return cqueue[idx].dst; };
    void Inv(int idx){ cqueue[idx] = (Rob_REG){.rdy=false}; };
    //Rob_REG NextRob(){ return cqueue[ Next(Head) ]; }
    Rob_REG CurrentRob(){ return cqueue[ Head ]; };
    int GetHead(){ return Head; };
    int GetTail(){ return Tail; };
    bool Retire();
    bool RobIsFull();
    void PrintRob();
    void PrintRT();
};
ROB::ROB(proc_params pp):p(pp){
    // Head and Tail can be initialized from 0 to rob_size-1
    Head = Tail = p.rob_size -1;
    Full = false;
    cqueue = vector<Rob_REG>(pp.rob_size);
}
bool ROB::RobIsFull(){
    // if Rob is already full
    if ( Full ) return true;
    int next_tail=Tail;
    // if Rob is not full, test availbility for next WIDTH -1 INSTs
    // we dont test the first INST since the availbility of first INST
    // is guaranty by "Full" flag
    for(int jj=0;jj<(int)p.width-1;jj++){
        next_tail = Next(next_tail);
        if( Head == next_tail ) return true;
    }
    return false;
}

void ROB::Allocate(int seq, int dst, bool rdy){
    cqueue[ Tail ] = (Rob_REG){ .rdy=rdy, .seq=seq, .dst=dst };
    Tail = Next(Tail);
    Full = ( Tail == Head );
}
bool ROB::Retire(){
    Head = Next(Head);
    //Inv(Head);
    Full = false;
    return ( Head == Tail );
};

void ROB::PrintRob(){
    cout<<"ROB head "<<Head<<" next head "<<Next(Head)<<" Tail "<<Tail<<endl;
    for(int k=0;k<(int)p.rob_size;k++)
        cout<<"Rob "<<k<<" rdy: "<<cqueue[k].rdy<<" dst: "<<cqueue[k].dst<<" seq: "<<cqueue[k].seq<<endl;
}
void ROB::PrintRT(){
    cout<<"[ RT ]: ";
    for(int k= Next(Head); k!=Tail;k=Next(k)){
        if( cqueue[k].rdy ) cout<<cqueue[k].seq<<" ";
    }
    cout<<endl;
}
typedef struct EXC_REG{
    bool start;
    int seq;
    int cnt;        // meaning when cpu check cnt value
                    // -1: empty,  0 : done this cycle, 1-5: wait
    int dst_tag;    // -1: no dst, 0-66: rob tag
}EXC_REG;

typedef struct WB_REG{
    bool v;
    int seq;
    int Rob_tag;
}WB_REG;

class EXC_LIST{
    EXC_REG* fu;
public:
    proc_params p;
    EXC_LIST(){};
    EXC_LIST(proc_params);
    EXC_REG GetFu(int);
    int FindEmpty();
    bool ExcIsFull(){ return (FindEmpty() == -1);};
    int Add(int, int, int);
    void DecCnt();
    //bool FindWb(int);
    void PrintEx(int);
    void PrintExSeq();
    void PrintExSeq2();
};

EXC_LIST::EXC_LIST(proc_params p):p(p){
    fu = new EXC_REG[5*p.width]();
    for(int k=0;k<5*(int)p.width;k++)
    {    fu[k].cnt = -1;       }
}
EXC_REG EXC_LIST::GetFu(int idx){
    return fu[idx];
}
int EXC_LIST::FindEmpty(){
    // find cnt = 0 or -1
    for(int k=0; k < 5*(int)p.width;k++)
        if( (fu[k].cnt == -1) || (fu[k].cnt == 0) )
            return k;
    return -1;
}
int EXC_LIST::Add(int seq, int op, int dst_tag){

    int idx = FindEmpty();
    if( idx == -1 ) return -1;

    int cycle;
    if(op == 2) cycle = 5;
    else if( op == 1 ) cycle = 2;
    else if( op == 0 ) cycle = 1;
    else { cout<<"error op code: "<<op<<" seq:"<<seq<<endl; exit(1);}

    fu[idx] = (EXC_REG){ .start=true, .seq=seq, .cnt=cycle, .dst_tag=dst_tag };
    return idx;
}
void EXC_LIST::DecCnt(){
    for(int k=0; k < 5*(int)p.width;k++){
        if( fu[k].start ) fu[k].start=false;
        //else
        if( fu[k].cnt >= 0) fu[k].cnt--;
    }
}
void EXC_LIST::PrintEx(int a){
    if(a==0) cout<<"before EX: seq,cnt,tag:";
    if(a==1) cout<<"after EX: seq,cnt,tag:";

    for(int k=0; k < 5*(int)p.width;k++)
        cout<<" ("<<fu[k].seq<<","<<fu[k].cnt<<","<<fu[k].dst_tag<<")";
    cout<<endl;
}

void EXC_LIST::PrintExSeq(){
    cout<<"[ EX ]: seq,cnt:";
    for(int k=0; k < 5*(int)p.width;k++)
    if(fu[k].cnt>0)
        cout<<" ("<<fu[k].seq<<","<<fu[k].cnt<<")";
    cout<<endl;
}
void EXC_LIST::PrintExSeq2(){
    cout<<"[ EX ]: seq,cnt:";
    for(int k=0; k < 5*(int)p.width;k++)
        cout<<" ("<<fu[k].seq<<","<<fu[k].cnt<<")";
    cout<<endl;
}

enum IQS{   // IQ state
        INV = 0,    // INV : already exec or empty, RDY: ready to exec, NRDY: not ready to exec
        RDY = 1,
        NRDY =2
};

// Issue Queue
typedef struct Iq_REG{
    int         v;
    int         seq;    // find the oldest instruction
    int         op;
    int         dst_tag;    // -1: no dst, 0-66: rob tag
    bool        rs1_rdy;
    int         rs1_tag;
    bool        rs2_rdy;
    int         rs2_tag;        // if -1: mean it is a value from ARF, if >=0: it is tag of Rob
}Iq_REG;

class IQ{
    vector<Iq_REG> queue;
public:
    int seq_max;
    uint32 cnt;
    proc_params p;
    //Iq_REG* mem;

    IQ(){};
    IQ(proc_params);
    bool IqIsFull();
    int FindInv();
    int FindRdy();
    void Invalidation(int);
    void IncCnt(){cnt++; seq_max++;};
    void Update(int);
    void SetIq(int, Iq_REG);
    Iq_REG GetIq(int);
    void PrintIq(int);
    void PrintIqSeq();
};

IQ::IQ(proc_params pp ):seq_max(1){
    p = pp;
    queue = vector<Iq_REG>(p.iq_size);
    for(uint32 k=0;k<p.iq_size;k++)
        Invalidation(k);
    cnt = 0;
}
Iq_REG IQ::GetIq(int idx){
    return queue[idx];
}
void IQ::SetIq(int idx, Iq_REG iq_reg){
    queue[idx] = iq_reg;
}
void IQ::Invalidation(int idx){
    cnt--;
    queue[idx] = (Iq_REG){ .v=INV, .seq=-1 };
};
bool IQ::IqIsFull(){
    for(uint32 k=0; k < p.width ;k++)
        if( (cnt+k) == (p.iq_size) )
            return true;
    return false;
}
int IQ::FindInv(){
    for(uint32 k=0;k<p.iq_size;k++)
        if(queue[k].v == INV)
            return (int) k;
    return -1;
}
int IQ::FindRdy(){
    // find the oldest ready instruction
    int seq = seq_max;
    int idx=p.iq_size;
    for(uint32 k=0;k<p.iq_size;k++)
        if((queue[k].v == RDY) && (queue[k].seq < seq) && (queue[k].seq != -1) ){
            seq = queue[k].seq;
            idx = k;
        }

    if( idx == (int)p.iq_size ) return -1;
    return idx;
}
void IQ::Update(int done_tag){
    for(uint32 k=0;k<p.iq_size;k++){
        if( queue[k].v == NRDY ){
            if ( !queue[k].rs1_rdy ) queue[k].rs1_rdy = ( queue[k].rs1_tag == done_tag );
            if ( !queue[k].rs2_rdy ) queue[k].rs2_rdy = ( queue[k].rs2_tag == done_tag );
            if( queue[k].rs1_rdy && queue[k].rs2_rdy ) queue[k].v = RDY;
        }
    }
    //cout<<"Update rob tag: "<<done_tag<<endl;
}
void IQ::PrintIq(int a){
    if(a==1) cout<<"Iq before DI\n";
    if(a==2) cout<<"Iq before IS\n";

    for(int k=0;k<(int)p.iq_size;k++){
        Iq_REG qq = queue[k];
        cout<<"v: "<<qq.v<<" seq: "<<qq.seq;
        cout<<" rdy1: "<<qq.rs1_rdy<<" rs1: "<<qq.rs1_tag;
        cout<<" rdy2: "<<qq.rs2_rdy<<" rs2: "<<qq.rs2_tag;
        cout<<" op: "<<qq.op<<" dst: "<<qq.dst_tag<<endl;
    }
    cout<<endl;
}
void IQ::PrintIqSeq(){
    cout<<"[ IS ]: ";
    for(int k=0;k<(int)p.iq_size;k++){
        cout<<queue[k].seq<<" ";
    }
    cout<<endl;
}

typedef struct AVAIL{
    bool FE;
    bool DE;
    bool RN;
    bool RR;
    bool DI;
    bool IS;
    bool EX;
}AVAIL;

class CPU_OOO{
public:
    //uint32 seq;
    uint32 cycle;      // clock cycle
    proc_params params;
    uint32 info_size;
    AVAIL Avail;
    bool done_flag;
    float ipc;
    uint32 last_cycle, last_seq;

    PIPE_REG*   De;
    Rmt_REG*    Rmt;
    PIPE_REG*   Rn;
    PIPE_REG*   Rr;
    PIPE_REG*   Di;
    //Iq_REG*     Iq;
    IQ          Iq;
    EXC_LIST    Exc_lst;    // execute_list
    WB_REG*     Wb;
    ROB         Rob;

    INST_INFO*  Rn_inf;
    INST_INFO*  De_inf;
    INST_INFO*  info;

    CPU_OOO(proc_params p);
    void Retire();
    void Writeback();
    void Execute();
    void Issue();
    void Dispatch();
    void RegRead();
    void Rename();
    void Decode();
    void Fetch(INST_INFO*);
    bool Advance_Cycle();
    int getWIDTH(){return (int) params.width;};
    int getIqSize(){return (int)params.iq_size;};
    uint32 getRobSize(){return params.rob_size;};
    uint32 getArfSize(){return 67;};
    int getInfoSize(){return (int)info_size;};
    int getInfoIdx(int seq){ return (seq == -1)? -1 : seq % getInfoSize();};
    void RstInfo(int);
    void PrintInfo(int);
    void PrintRmt();
    void PrintRmt(int);
    void PrintState();
    void UpdInfo();
    void PrintWb();
    bool FindWb(int);
    void UpdPrintFetch();
    void PrintResult();
    void PrintConfig();
};

bool CPU_OOO::FindWb(int src_tag){
    // find cnt = 0
    //cout<<"WB find done, tag: "<<src_tag<<" ";
    for(int k=0; k < 5*getWIDTH();k++)
        if( ( Wb[k].v ) && ( Wb[k].Rob_tag == src_tag) )
            { //cout<<"Find\n";
            return true;}
    //cout<<"Not found\n";
    return false;
}
void CPU_OOO::RstInfo( int idx ){
    PIPE_Info init = (PIPE_Info){ .bc=-1, .dur=-1 };
    info[ idx ] = (INST_INFO){.seq=-1,.op=-1,.src1=-1,.src2=-1,.dst=-1,
        .FE=init,.DE=init,.RN=init,.RR=init,.DI=init,.IS=init,.EX=init,.WB=init,.RT=init};
}
#endif
