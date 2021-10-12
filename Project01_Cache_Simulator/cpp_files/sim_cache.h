//#define DEBUG_ENABLED

#ifndef SIM_CACHE_H
#define SIM_CACHE_H

#include <iostream>
using namespace std;
#include <math.h>
#include <list>
#include <vector>
#include <iterator>
#include<tuple>

using namespace std;

typedef unsigned long int uint32;
typedef unsigned int uint16;

typedef struct cache_params{
    unsigned long int block_size;
    unsigned long int l1_size;
    unsigned long int l1_assoc;
    unsigned long int vc_num_blocks;
    unsigned long int l2_size;
    unsigned long int l2_assoc;
}cache_params;

// Put additional data structures here as per your requirement
// #set
// cache = set[#set]
//  set = block[#ACCSO]
//  block[#].tag block[#].dirty block[#].valid
//enum hm_type { HIT=(bool)1, MISS=0 };
enum vc_t { VC=(bool)1, NO_VC=0 };
enum rw_t { WRITE=(bool)1, READ=0 };
enum v_bit_t { VALID=(bool)1, INVALID=0 };
enum d_bit_t { DIRTY=(bool)1, CLEAN=0 };
enum rtn_t { HIT=3, MISS=2, EVICT_DIRTY=1, EVICT_CLEAN=0 };
enum cache_t {NORMAL=0,VICTIM=1};

typedef struct block{
    unsigned long int tag;
    v_bit_t v;
    d_bit_t d;
}block;
#define ADDR(idx,tag) ((((tag)<<IDX_BIT)|(idx))<<BLK_OFST_BIT)

typedef list<block>::iterator l_blk_it;

#endif
