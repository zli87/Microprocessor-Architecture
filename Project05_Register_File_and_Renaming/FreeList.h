#ifndef QUEUE
#define QUEUE
#include "queue.h"
#endif

template <class T>
class FreeList :public Queue<T> {
public:
    uint64_t __Maxsize__;
    uint64_t n_log_regs;
    FreeList(uint64_t,uint64_t);
    //!-- call GetFreeReg when rename destination register
    T* GetFreeReg(T& x);
    //!-- call ReturnFreeReg when Active list retire register and free previous_mapping destination register
    void ReturnFreeReg(T& x);
    //!-- call restore when active list doing exception restore
    void Restore(list_idx __new_front__){     Queue<T>::__front__ = __new_front__; };
    list_idx Checkpoint(){ return Queue<T>::__front__; };
    void Squash(){ Queue<T>::__front__ = (Queue<T>::__rear__ + 1 ) % Queue<T>::__Maxsize__; };
};

template <class T>
FreeList<T>::FreeList(uint64_t n_log_regs, uint64_t n_specul_regs) : Queue<T>(n_specul_regs){

    __Maxsize__ = n_specul_regs;

    for(int i=0; i < __Maxsize__ ; i++)
        Queue<T>::Add( (T)( n_log_regs + i ) );

//    cout<<__Maxsize__<<"   "<<Queue<T>::__Maxsize__<<endl;
//    for(int i=0; i < __Maxsize__ ; i++){        T x;        Queue<T>::Delete(x);        cout<<"pop = "<<x<<endl;    }
}
template <class T>
T* FreeList<T>::GetFreeReg(T& x){
    if(Queue<T>::IsEmpty()) { std::cerr<<"[My Error] Delete to Empty Free List!!"; assert(false); return 0;}
    return Queue<T>::Delete(x);
};

template <class T>
void FreeList<T>::ReturnFreeReg(T& x){
    if(Queue<T>::IsFull()) { std::cerr<<"[My Error] Add to Full Free List!!"; assert(false); }
    Queue<T>::Add(x);
};
