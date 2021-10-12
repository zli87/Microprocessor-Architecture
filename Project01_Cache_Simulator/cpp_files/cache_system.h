#include "sim_cache.h"
#include "regular_cache.h"
#include "victim_cache.h"
#include <iostream>
#include <iomanip>
#include<tuple>

using namespace std;

#define DEFAULT_BLOCK_SIZE 1
#define DEFAULT_CACHE_SIZE 1
#define DEFAULT_ASSOCIATIVE 1

class CACHE_SYSTEM{
public:
    //Victim_Cache (char* name, uint32 block_size,uint32 vc_num_blocks) : Cache(name, block_size, vc_num_blocks*block_size, vc_num_blocks){};
    //friend class Cache;
    //friend class CACHE;
    //friend class VC; // victim cache
    bool flag_L1;
    bool flag_L2;
    bool flag_VC;
    Cache* L1;
    Cache* L2;
    Cache* VC;
    uint32 BLOCKSIZE,L1_SIZE,L1_ASSOC,VC_NUM_BLOCKS,L2_SIZE,L2_ASSOC;
    CACHE_SYSTEM(cache_params params);
    void request(rw_t rw, uint32 addr);
	void func0(rw_t rw,uint32 addr);
	void func1(rw_t rw,uint32 addr);
	void func2(rw_t rw,uint32 addr);
	void func3(rw_t rw,uint32 addr);
    void info();
    void results();
};

CACHE_SYSTEM::CACHE_SYSTEM(cache_params params):\
BLOCKSIZE(params.block_size),L1_SIZE(params.l1_size),L1_ASSOC(params.l1_assoc),\
VC_NUM_BLOCKS(params.vc_num_blocks),L2_SIZE(params.l2_size),L2_ASSOC(params.l2_assoc){
    //if (0 >= L1_SIZE){
    //    cout<<"[ERROR] this project requires L1 CACHE !"<<endl;
    //    exit(0);
    //}
    flag_L1 = (0 < L1_SIZE)? 1 : 0;
    flag_L2 = (0 < L2_SIZE)? 1 : 0;
    flag_VC = (0 < VC_NUM_BLOCKS)? 1 : 0;
    
    if(flag_L1){
        L1 = new Regular_Cache((char*)"L1", params.block_size, params.l1_size, params.l1_assoc,NORMAL);
    }else{
        L1 = new Regular_Cache((char*)"L1", DEFAULT_BLOCK_SIZE, DEFAULT_CACHE_SIZE, DEFAULT_ASSOCIATIVE,NORMAL);
    }
    if(flag_L2){
        L2 = new Regular_Cache((char*)"L2", params.block_size, params.l2_size, params.l2_assoc,NORMAL);
    }else{
        L2 = new Regular_Cache((char*)"L2", DEFAULT_BLOCK_SIZE, DEFAULT_CACHE_SIZE, DEFAULT_ASSOCIATIVE,NORMAL);
    }
    if(flag_VC){
        VC = new Victim_Cache((char*)"VC", params.block_size, params.vc_num_blocks,VICTIM);
    }else{
        VC = new Victim_Cache((char*)"VC", DEFAULT_BLOCK_SIZE, DEFAULT_CACHE_SIZE, DEFAULT_ASSOCIATIVE,VICTIM);
    }
#ifdef DEBUG_ENABLED
    cout<<"[info] finish initial cache system"<<endl;
#endif
};
void CACHE_SYSTEM::request(rw_t rw, uint32 addr){
    switch ( (flag_L2)|(flag_VC<<1) ){
        case 0 :
            //cout<<"only L1"<<endl;
            func0(rw,addr);
            break;
        case 1 :
            //cout<<"L1 + L2"<<endl;
            func1(rw,addr);
            break;
        case 2 :
            func2(rw,addr);
            //cout<<"L1 + VC"<<endl;
            break;
        case 3 :
            func3(rw,addr);
            //cout<<"L1 + VC + L2"<<endl;
            break;
    }
};
void CACHE_SYSTEM::func0(rw_t rw,uint32 addr){
    // only L1 CACHE
    rtn_t rtn1;
    uint32 addr_rtn1;
    tie(rtn1,addr_rtn1) = L1->request(rw,addr);
};
void CACHE_SYSTEM::func1(rw_t rw,uint32 addr){
    // L1 + L2 CACHE
    rtn_t rtn1;
    uint32 addr_rtn1;
    tie(rtn1,addr_rtn1) = L1->request(rw,addr);
    if(EVICT_DIRTY == rtn1){
        // writeback evict first and then allocate block from L2
        L2->request(WRITE,addr_rtn1);
        L2->request(READ,addr);
    }else if((EVICT_CLEAN == rtn1)|| (MISS == rtn1)){
        L2->request(READ,addr);
    }
};
void CACHE_SYSTEM::func2(rw_t rw,uint32 addr){
    // L1 + VC CACHE
    rtn_t rtn1;
    uint32 addr_rtn1;
    tie(rtn1,addr_rtn1) = L1->request(rw,addr);
    if((EVICT_DIRTY == rtn1) || (EVICT_CLEAN == rtn1)){
        VC->swap_request(L1,addr);
    }else if(MISS ==rtn1){
        // L1 never tranfer blk to VC !!! L1 request to next level cache directly !!!
    }
};
void CACHE_SYSTEM::func3(rw_t rw,uint32 addr){
    // L1 + VC CACHE
    rtn_t rtn1,rtn2;
    uint32 addr_rtn1,addr_rtn2;
    tie(rtn1,addr_rtn1) = L1->request(rw,addr);
    if((EVICT_DIRTY == rtn1) || (EVICT_CLEAN == rtn1)){
        tie(rtn2,addr_rtn2) = VC->swap_request(L1,addr);
        if( EVICT_DIRTY== rtn2){
            L2->request(WRITE,addr_rtn2);
            L2->request(READ,addr);
        }else if(EVICT_CLEAN == rtn2){
            L2->request(READ,addr);
        }
    }else if(MISS ==rtn1){
        // L1 never tranfer blk to VC !!! L1 request to L2 cache directly !!!
        L2->request(READ,addr);
    }
};
void CACHE_SYSTEM::info(){
    L1->info();
    if(flag_VC) VC->info();
    if(flag_L2) L2->info();
};
void CACHE_SYSTEM::results(){
    cout<<" ===== Simulation results ====="<<endl;
    float L1_VC_miss_rate,swap_req_rate;
    uint32 L1_VC_writebacks, L1_VC_mem_traffic;
    uint32 swaps,swap_req;
    if(flag_VC){
        swaps = VC->swaps;
        swap_req = VC->access;
        L1_VC_writebacks = VC->writebacks;
        swap_req_rate = static_cast<float>(VC->access) / static_cast<float>(L1->reads + L1->writes);
        L1_VC_miss_rate = static_cast<float>(L1->rmiss + L1->wmiss - VC->swaps) / static_cast<float>(L1->reads + L1->writes);
        L1_VC_mem_traffic = L1->rmiss + L1->wmiss - VC->swaps + VC->writebacks;
    }else{
        swaps = 0;
        swap_req = 0;
        L1_VC_writebacks = L1->writebacks;
        swap_req_rate = (float) 0;
        L1_VC_miss_rate = static_cast<float>(L1->rmiss + L1->wmiss) / static_cast<float>(L1->reads + L1->writes);
        L1_VC_mem_traffic = L1->rmiss + L1->wmiss + L1->writebacks;
    }
    float L2_miss_rate = (flag_L2)? static_cast<float>(L2->rmiss) / static_cast<float>(L2->reads) : (float)0;
    uint32 L2_mem_traffic = L2->rmiss + L2->wmiss -L2->swaps +L2->writebacks;

    uint32 total_mem_traffice = (flag_L2)? L2_mem_traffic : L1_VC_mem_traffic;
    // L1
    cout<<"  a. number of L1 reads:"<<L1->reads<<endl;
    cout<<"  b. number of L1 read misses:"<<L1->rmiss<<endl;
    cout<<"  c. number of L1 writes:"<<L1->writes<<endl;
    cout<<"  d. number of L1 write misses:"<<L1->wmiss<<endl;
    // VC
    cout<<"  e. number of swap requests:"<<swap_req<<endl;
    cout<<"  f. swap request rate:"<<fixed<<setprecision(4)<<swap_req_rate<<endl;
    cout<<"  g. number of swaps: "<<swaps<<endl;
    // L1 + VC
    cout<<"  h. combined L1+VC miss rate:"<<fixed<<setprecision(4)<<L1_VC_miss_rate<<endl;
    cout<<"  i. number writebacks from L1/VC:"<<L1_VC_writebacks<<endl;
    // L2
    cout<<"  j. number of L2 reads:"<<L2->reads<<endl;
    cout<<"  k. number of L2 read misses:"<<L2->rmiss<<endl;
    cout<<"  l. number of L2 writes:"<<L2->writes<<endl;
    cout<<"  m. number of L2 write misses:"<<L2->wmiss<<endl;
    cout<<"  n. L2 miss rate:"<<fixed<<setprecision(4)<<L2_miss_rate<<endl;
    cout<<"  o. number of writebacks from L2:"<<L2->writebacks<<endl;
    cout<<"  p. total memory traffic:"<<total_mem_traffice<<endl;
};
