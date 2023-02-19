//#define TEST

#ifdef TEST

#ifndef QUEUE
#include "queue.h"
#define QUEUE
#endif

#include <inttypes.h>
#include <iostream>
#include "FreeList.h"

using namespace std;
#define GBM_FULL -1
#define GBM_EMPTY 0

uint64_t getGBMnum(uint64_t GBM){
    uint64_t num=0;
	for ( uint64_t v = GBM; v; v &= v - 1 ) ++num;
	return num;
};

void print64bit(uint64_t GBM, uint64_t highlight ,const char* msg){
	cout<<"[info] "<<msg<<" = ";
	uint64_t branch_ID = 63;
    long one = 1;
	long mask = one << branch_ID;
    assertm( getGBMnum(mask)==1,"mask error!" );
	for ( ; branch_ID > 0 ; branch_ID-- ) {
		mask = one << branch_ID;
        assertm( getGBMnum(mask)==1,"mask error!" );
		uint64_t bit = ( (GBM & mask) > 0 )? 1 : 0;
        if( highlight == branch_ID ) cout<<"("<<bit<<")";
		else cout<<bit;
		if( branch_ID % 8 == 0 ) cout<<"_";
	}
	uint64_t bit = ( (GBM & 1) > 0 )? 1 : 0;
	cout<<bit;
	cout<<endl;
}
uint64_t checkpoint(uint64_t GBM){
    uint64_t branch_ID = 63;
    uint64_t mask = (long)1 << branch_ID;
    uint64_t GBM_inverse = ~GBM;
    assertm( getGBMnum(mask)==1,"mask error!" );
    while( mask > 0 ){
		if( ( GBM_inverse & mask ) > 0 ){

			// set bit from 0 to 1
			GBM = GBM ^ mask; // example: GBM = 1(0)10 , GBM_inv = 0(1)01, mask = 0100, GBM ^ mask = 1(1)10

			return branch_ID;
		}// end if
		branch_ID--;
		mask >>= 1;
        assertm( getGBMnum(mask)==1,"mask error!" );
	}// end while
    assertm( 0,"Can not find 0 bit in GBM!");
}
int main(){
    uint64_t GBM=-1;
    cout<<GBM<<endl;
    cout<<getGBMnum(GBM)<<endl;
    if(GBM == GBM_FULL)
        cout<<"full\n";
    assert(1); // will pass
    //cerr<<"will fail\n";
    //assertm(0,"will fail\n"); // will fail

    Queue<int> *mem;
    mem = new Queue<int>(5);
//    mem.reset(5);
    for(int i=0;i<5;i++)
        mem->Add(i+1);
    int a,b;
    b = *(mem->GetHead(a));
    cout<<"head ="<<a<<" "<<b<<endl;
    for(int i=0;i<5;i++){
        mem->Delete(a);
        cout<<"retire a= "<<a<<endl;
    }
    FreeList<int>* FL;
    FL = new FreeList<int>(3,10);

    uint64_t branch_ID = 63;
    long one = 1;
    uint64_t zero = 0;
    uint64_t mask = (long)1 << branch_ID;
    uint64_t GBM_test = -1;
    uint64_t GBM_inverse;
    uint64_t ofst = 7;
    uint64_t mask2 = (long)1 << ofst;
    GBM_test = GBM_test ^ mask2;
    GBM_inverse = ~GBM_test;
    print64bit(mask,ofst,"mask");
    print64bit(GBM_test,ofst,"GBM_test");
    print64bit(GBM_inverse,ofst,"GBM_inverse");
    GBM_inverse = ~zero;
    print64bit(GBM_inverse,ofst,"GBM_inverse");
    cout<<checkpoint(GBM_test)<<endl;
    return 0;
}
#endif
