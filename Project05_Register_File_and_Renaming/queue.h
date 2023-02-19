
#ifndef ASSERT
#define ASSERT
// uncomment to disable assert()
// #define NDEBUG
#include <cassert>

// Use (void) to silent unused warnings.
#define assertm(exp, msg) assert(((void)msg, exp))
#endif

#include <inttypes.h>
#include <iostream>

using namespace std;

typedef uint64_t list_idx;

template <class T>
class Queue {
public:
    list_idx __front__, __rear__;
    uint64_t __Maxsize__;
    T* queue;
    Queue(uint64_t);
    ~Queue();
    bool IsFull();
    bool IsFull(uint64_t);
    bool IsEmpty();
    bool IsEmpty(uint64_t);// input bundle_dst, number of instruction
    list_idx Add(const T& item);
    T* Delete(T&);
    T* GetHead(T&);
    list_idx front(){ return __front__; };
    list_idx rear(){ return __rear__; };
    void Reset();
    bool Find(T&);
};
template <class T>
void Queue<T>::Reset(){
    __front__ = __rear__ = 0;
    for( uint64_t i=0 ; i < __Maxsize__ ; i++ )
        queue[i] = 0;
};

template <class T>
Queue<T>::Queue(uint64_t Max):__Maxsize__(Max+1){
    queue = new T[__Maxsize__];
    __front__ = __rear__ = 0;
}


template <class T>
Queue<T>::~Queue(){ delete [] queue; }

template <class T>
bool Queue<T>::IsFull(){
    list_idx new___rear__ = (__rear__ +1) % __Maxsize__;
    //cout<<"new __rear__= "<<new___rear__<<" __front__= "<<(__front__+__Maxsize__)%__Maxsize__<<endl;
    return ( ( ( __front__ + __Maxsize__ ) % __Maxsize__) == new___rear__ ) ;
}

template <class T>
bool Queue<T>::IsFull(uint64_t bundle_dst){
    // !!! NOT Allow __front__ == __rear__ after Add !!!
    list_idx __new_rear__ = __rear__;
    for(uint64_t i=0; i < bundle_dst; i++){
        __new_rear__ = (__new_rear__ +1) % __Maxsize__;
        if ( ( ( __front__ + __Maxsize__ ) % __Maxsize__) == __new_rear__ ) return true;
    }
    return false;
}

template <class T>
bool Queue<T>::IsEmpty(){
    return ( __front__ == __rear__ );
}
template <class T>
bool Queue<T>::IsEmpty(uint64_t bundle_dst ){
    // test if it is empty after delete <bundle_dst> times
    // __current_front__ + bundle_dst => __new_front__ after <bundle_dst> times delete
    // !!! allow empty after delete !!!
    list_idx __new_front__ = __front__;
    for(uint64_t i=0; i < bundle_dst; i++){
        if ( __new_front__ == __rear__ ) return true;
        __new_front__ = (__new_front__+ 1 ) % __Maxsize__;
    }
    return false;
}

template <class T>
list_idx Queue<T>::Add(const T& x){
    assertm( !IsFull(), "[My Error] Add to Full Queue!!");
    __rear__ = (__rear__+1) % __Maxsize__;
    queue[__rear__] = x;
    return __rear__;
}

template <class T>
T*  Queue<T>::Delete(T& x){
    assertm( !IsEmpty(), "[My Error] Delete/Pop from Empty Queue!!");
    __front__ = (__front__+1) % __Maxsize__;
    x=queue[__front__];
    return &x;
}

template <class T>
T*  Queue<T>::GetHead(T& x){
    assertm( !IsEmpty(), "[My Error] Try to GetHead from Empty Queue!!");
    list_idx __next_front__ = (__front__+1) % __Maxsize__;
    x=queue[__next_front__];
    return &x;
}

template <class T>
bool  Queue<T>::Find(T& x){
    for(list_idx idx=0; idx<__Maxsize__;idx++)
        if( x == queue[idx] ) return true;
    return false;
}
