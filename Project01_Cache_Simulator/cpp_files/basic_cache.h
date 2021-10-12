
#ifndef BASIC_CACHE_H
#define BASIC_CACHE_H

#include "sim_cache.h"
#include <iostream>
using namespace std;

class Cache{
public:
    // initial params
    char *name;
    const uint32 block_size,cache_size,assoc;
    const cache_t cache_type;

    uint32 SET_NUM;
    uint32 BLK_OFST_BIT, IDX_BIT, TAG_BIT;
    uint32 BLK_OFST_MASK, IDX_MASK, TAG_MASK;
    // data structures
    //list <int> lru;
    vector< list<block>> sets;
    l_blk_it it_evicted;
    block blk_v;
    uint32 addr_evict;
    // record numbers
    uint32 reads,rmiss,writes,wmiss,hits,writebacks,swaps,access;
    // constructors & functions
    Cache(char* name, uint32 block_size, uint32 cache_size, uint32 assoc,cache_t cache_type);
    // virtual fucntions that child class must implement
    virtual tuple<rtn_t,uint32> request(rw_t, uint32) =0;
    virtual tuple<rtn_t,uint32> swap_request(Cache *C_L1, uint32 addr_X) =0;
    void info();
    void debug_info(rtn_t hm, rw_t rw, vc_t vc, l_blk_it it1 ,l_blk_it it_evicted ,uint32 idx);
};

Cache::Cache(char* name, uint32 block_size, uint32 cache_size, uint32 assoc,cache_t cache_type):\
name(name), block_size(block_size), cache_size(cache_size), assoc(assoc), cache_type(cache_type),\
reads(0),rmiss(0),writes(0),wmiss(0),hits(0),writebacks(0),swaps(0),access(0){

    SET_NUM = cache_size/( assoc * block_size );
    BLK_OFST_BIT = log2(block_size);
    IDX_BIT = log2(SET_NUM);
    //TAG_BIT = 32 - BLK_OFST_BIT - IDX_BIT;
    //printf("BLK_OFST_BIT: %lu IDX_BIT: %lu TAG_BIT: %lu\n",BLK_OFST_BIT, IDX_BIT, TAG_BIT);
    BLK_OFST_MASK = (block_size-1);
    IDX_MASK = (SET_NUM-1) << BLK_OFST_BIT;
    TAG_MASK = 0xFFFFFFFF ^ (IDX_MASK | BLK_OFST_MASK);
    //cout<<name<<" BLK_OFST_BIT: "<<BLK_OFST_BIT<<" IDX_BIT: "<<IDX_BIT<<endl;
#ifdef DEBUG_ENABLED
    //printf("[info] cache SET_NUM :%lu\n ",SET_NUM);
    //printf("[info]BLK_OFST_MASK %lx IDX_MASK %lx TAG_MASK %lx\n",BLK_OFST_MASK,IDX_MASK,TAG_MASK );
#endif
    for (size_t i = 0; i < SET_NUM; i++) {
        list<block> one_set;
        for (size_t ass = 0; ass < assoc; ass++) {
            one_set.push_front( block{.tag= ass, .v=INVALID, .d=CLEAN } );
        }
        sets.push_back(one_set);
    }
    //Cache::info();
};

void Cache::info(){
    cout<<"===== "<<name<<" contents ====="<<endl;
    for (size_t i = 0; i < SET_NUM; i++) {
        cout<<" set "<<i<<":";
        for (auto & blk: sets[i]) {
            char dirty_char=( DIRTY == blk.d )? 'D' : ' ';
            if( blk.v == VALID ){
                cout<<" "<<hex<<blk.tag<<dec<<" "<<dirty_char;
            }
        }
        cout<<endl;
    }
    cout<<endl;
};

void Cache::debug_info(rtn_t rtn, rw_t rw, vc_t vc, l_blk_it it1 ,l_blk_it it_evicted ,uint32 idx){
#ifdef DEBUG_ENABLED
    uint32 addr = ADDR(idx,it1->tag) ;
    const char *rw_char = ( rw )? "write" : "read" ;
    const char *rtn_char = (MISS==rtn)? "miss": (HIT==rtn)? "hit":"other";
    const char *vc_dirty = (vc)? (it_evicted->d)? "dirty": "clean": " ";
    char vc_info[100] = "none";
    if(vc){
        uint32 addr_vc = ADDR(idx,it_evicted->tag) ;
        sprintf(vc_info,"%lx (tag %lx, index %lu, %s)", addr_vc, it_evicted->tag,idx,vc_dirty);
    }//else sprintf(vc_info,"none");
    char name2[] = "L1";
    if(name[1] == name2[1])
    {
        printf("----------------------------------------\n" );
        printf("# %lu: %s %lx\n", access,rw_char,addr);
    }
    printf(" %s %s: %lx (tag %lx, index %lu)\n", name,rw_char, addr,it1->tag,idx);
    printf(" %s %s\n",name,rtn_char );
    if(MISS==rtn) printf(" %s victim: %s\n",name,vc_info );
    printf(" %s update LRU\n",name );
    if((WRITE==rw) &&(DIRTY==it1->d)) printf(" %s set dirty\n",name );
#endif
};

#endif
