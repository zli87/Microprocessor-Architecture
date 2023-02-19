#ifndef QUEUE
#define QUEUE
#include "queue.h"
#endif

template <class T>
class ActiveList :public Queue<T> {
public:
    uint64_t __Maxsize__;
    ActiveList(uint64_t);
    //!-- call Retire when commit
    T* Retire(T& x);
    //!-- call Add when dispatch_inst
    list_idx Add(T& x);
    //!-- call GetHead when precommit
    T* GetHead(T& x);
    //!-- call Restore when resolve
    void Restore(uint64_t __new_rear__){     Queue<T>::__rear__ = __new_rear__; };
    //!-- call Checkpoint when checkpoint
    list_idx Checkpoint(){ return Queue<T>::__rear__; };
    //!-- call Squash when squash
    void Squash(){ Queue<T>::__rear__ = Queue<T>::__front__; };
};
template <class T>
ActiveList<T>::ActiveList(uint64_t n_specul_regs):Queue<T>(n_specul_regs){
     __Maxsize__=n_specul_regs;
};

template <class T>
T* ActiveList<T>::Retire(T& x){
    assertm( !Queue<T>::IsEmpty(), "[My Error][ActiveList] Delete to Empty Active List!!");
    return Queue<T>::Delete(x);
};

template <class T>
list_idx ActiveList<T>::Add(T& x){
    assertm( !Queue<T>::IsFull(), "[My Error][ActiveList] Add to Full Active List!!");
    return Queue<T>::Add(x);
};
template <class T>
T* ActiveList<T>::GetHead(T& x){
    assertm( !Queue<T>::IsEmpty(), "[My Error][ActiveList] Try to Get Head from Active List!!");
    return Queue<T>::GetHead(x);
};
