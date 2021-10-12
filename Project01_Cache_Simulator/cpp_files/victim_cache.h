#ifndef VICTIM_CACHE_H
#define VICTIM_CACHE_H

#include "basic_cache.h"
#include <iostream>
using namespace std;

class Victim_Cache : public Cache{
    tuple<rtn_t,uint32> request(rw_t, uint32){ return make_tuple(MISS,0xFFFFFFFF); };
public:
    using Cache::Cache;
    Victim_Cache(char* name, uint32 block_size, uint32 assoc,cache_t cache_type);
    tuple<rtn_t,uint32> swap_request(Cache *C_L1, uint32 addr_X);
};
// victim cache is fully associative, therefore the meaning of parameter "assoc" is swith to number of blocks.
Victim_Cache::Victim_Cache(char* name, uint32 block_size, uint32 vc_num_blocks,cache_t cache_type):\
Cache(name, block_size, (uint32)(block_size*vc_num_blocks), vc_num_blocks, cache_type){
#ifdef DEBUG_ENABLED
    printf("[info] victim cache %s initial!!\n",name );
#endif
}

tuple<rtn_t,uint32> Victim_Cache::swap_request(Cache *C_L1, uint32 addr_X){
    if(VICTIM==cache_type){
        access++;
        // for search blk in VC
        uint32 tag = (addr_X) >> BLK_OFST_BIT;
        // for blk X in L1
        uint32 idx_L1 = (addr_X & C_L1->IDX_MASK) >> C_L1->BLK_OFST_BIT;
        l_blk_it it_X_L1 = C_L1->sets[idx_L1].begin();
        uint32 tag_v_VC = (C_L1->addr_evict)>>BLK_OFST_BIT;
        d_bit_t d_v_L1 = C_L1->blk_v.d;
#ifdef DEBUG_ENABLED
        //cout<<" VC swap req: ["<<hex<<addr_X<<", "<<C_L1->addr_evict<<dec<<"]"<<endl;
#endif
        l_blk_it it_vc,it_begin,it_end;

        it_begin = sets[0].begin();
        it_end = sets[0].end();
        for (it_vc = it_begin; it_vc != it_end; it_vc++) {
            if ( (tag == it_vc->tag) && (VALID == it_vc->v) ){
                // find blk X in VC, swap [blk X in VC] and [blk V in L1]
                swaps++;
                it_X_L1->d = (DIRTY == it_X_L1->d)? DIRTY : it_vc->d ;
                sets[0].insert(it_begin,block{.tag=tag_v_VC,.v=VALID,.d=d_v_L1});
                sets[0].erase(it_vc);
                return make_tuple(HIT,0xFFFFFFFF);
            }
        } // end of loop it_evicted
        // VC miss blk_X, L1 transfer blk V to VC
        it_evicted = --(sets[0].end());
        blk_v = *it_evicted;
        addr_evict = ((it_evicted->tag)<<BLK_OFST_BIT);

        sets[0].insert(it_begin,block{.tag=tag_v_VC,.v=VALID,.d=d_v_L1});
        sets[0].erase(it_evicted);
        //it_begin = sets[0].begin();
        if(DIRTY == blk_v.d){
            writebacks++;
            return make_tuple(EVICT_DIRTY, addr_evict);
        }else
            return make_tuple(EVICT_CLEAN, addr_evict);
    }
    // if not VC cache, do nothing
    return make_tuple(MISS,0xFFFFFFFF);
}

#endif
