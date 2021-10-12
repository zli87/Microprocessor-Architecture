
#ifndef REGULAR_CACHE_H
#define REGULAR_CACHE_H

#include "basic_cache.h"
#include <iostream>
using namespace std;

class Regular_Cache : public Cache{
    tuple<rtn_t,uint32> swap_request(Cache *C_L1, uint32 addr_X){ return make_tuple(MISS,0xFFFFFFFF); };
public:
    using Cache::Cache;
    Regular_Cache(char* name, uint32 block_size, uint32 cache_size, uint32 assoc,cache_t cache_type);
    tuple<rtn_t,uint32> request(rw_t, uint32);
};
Regular_Cache::Regular_Cache(char* name, uint32 block_size, uint32 cache_size, uint32 assoc,cache_t cache_type):\
Cache(name, block_size, cache_size, assoc, cache_type){
#ifdef DEBUG_ENABLED
    printf("[info] regular cache %s initial!!\n",name );
#endif
}

// CACHE POLICY: WBWA write back write allocate
// write back: write to the next level of cache ONLY WHEN a dirty block is evicted from current cache
// write allocate: when write miss, allocate blocks from next level cache first, and then write to blocks.

// data structure of set[N]
// set N : blk 0[tag,v,d] blk 1[tag,v,d] ... blk M[tag,v,d]
//          MRU <----------------------------> LRU
//  set 0 :   [acdd05644,1,0] ..... [aedc6999,0,0]
// notice that once you find a invalid blk, the rest blk will be all invalid !
tuple<rtn_t,uint32> Regular_Cache::request(rw_t rw,uint32 addr){
    //printf("%d %lx\n", rw, addr);
    uint32 tag,idx;
    d_bit_t d;

    if(rw)  writes++;
    else    reads++;
    access += 1;
    tag = ((addr & TAG_MASK) >> IDX_BIT) >> BLK_OFST_BIT;
    idx = (addr & IDX_MASK) >> BLK_OFST_BIT;

    l_blk_it it1,it2,it3,it_begin,it_end;
    it_begin = sets[idx].begin();
    it_end = sets[idx].end();
    for (it1 = it_begin; it1 != it_end; it1++) {
        if( INVALID == it1->v ){
            // cache miss but has invalid blk
            // 1. insert new X blk to MRU blk in list
            // 2. delete current invalid blk
            d = (rw)? DIRTY : CLEAN ;
            sets[idx].insert(it_begin,block{ .tag=tag, .v=VALID, .d=d });
            sets[idx].erase(it1);
            if(rw)  wmiss++;
            else    rmiss++;
#ifdef DEBUG_ENABLED
            debug_info( MISS, rw, NO_VC , sets[idx].begin() ,it_end ,idx);
#endif
            return make_tuple(MISS, addr);

        }else if(( tag == it1->tag )){
            // cache hit
            // 1. update LRU list, dirty bit
            // 2. nothing with VC cache
            d = (rw)? DIRTY : it1->d ;
            sets[idx].insert(it_begin,block{ .tag=it1->tag, .v=VALID, .d=d });
            sets[idx].erase(it1);
#ifdef DEBUG_ENABLED
            debug_info( HIT, rw, NO_VC , sets[idx].begin() ,it_end ,idx);
#endif
            return make_tuple(HIT, 0xFFFFFFFF);
        }
    } // end of it1 for loop
    // cache miss, no more invalid blk in cache
    // 1. insert new X blk to MRU blk in list
    // 2. return V blk

    it_evicted = --(sets[idx].end());
    blk_v = *it_evicted;
    addr_evict = ((((it_evicted->tag)<<IDX_BIT)|(idx))<<BLK_OFST_BIT);

    d = (rw)? DIRTY : CLEAN ;
    sets[idx].insert(it_begin,block{ .tag=tag, .v=VALID, .d=d });
    sets[idx].erase(it_evicted);

    if(rw)  wmiss++;
    else    rmiss++;
#ifdef DEBUG_ENABLED
    debug_info( MISS, rw, VC , sets[idx].begin() ,it_evicted ,idx);
#endif
    if(DIRTY == blk_v.d){
        writebacks++;
        return make_tuple(EVICT_DIRTY, addr_evict);
    }else
        return make_tuple(EVICT_CLEAN, addr_evict);
};
#endif
