#include "sim_bp.h"
#include <iostream>
#include <iomanip>
#include <cstring>
using namespace std;

//#define DEBUG_
#define TAKEN true
#define NOT_TAKEN false

class Predictor{
protected:
    uint32 cnt_pred;
    uint32 cnt_mispred;
    bp_params params;
    //uint32 m; // prediction table index., max m = 30 bits, which use PC[31:2]
    //uint32 n; // n-bit global branch history register, max n == m;
    uint32 size_pt; // size of prediction table
    uint32 mask_m_bit;
    uint32 mask_n_bit;
#ifdef DEBUG_
public:
#endif
    char *pt; // 8-bits
public:
    Predictor(bp_params);
    virtual uint32 gen_idx(uint32 addr)=0;
    virtual void update_pt(uint32 idx, bool taken);
    virtual bool predict_pt(uint32 idx);
    virtual void next_idx_outcome(uint32 idx, bool outcome);
    virtual void next_pc_outcome(uint32 addr, bool outcome)=0;
    virtual void info();
    virtual void content_pt();
    virtual void print_miss_rate();
};


class Pred_Bimodal : public Predictor{
public:
     Pred_Bimodal(bp_params);
     uint32 gen_idx(uint32 addr);
     void next_pc_outcome(uint32 addr, bool outcome);
     void content_pt();
};



class Pred_Gshare : public Predictor{
    uint32 gbh; // global branch history;
    uint32 mask_n_bit;
    uint32 get_gbh();

public:
     Pred_Gshare(bp_params);
     uint32 gen_idx(uint32 addr);
     void update_gbh(bool outcome);
     void next_pc_outcome(uint32 addr, bool outcome);
     void content_pt();
};


class Pred_Hybrid : public Predictor{
    uint32 gen_idx(uint32 addr);
    Pred_Bimodal* pred_bio;
    Pred_Gshare* pred_gsh;
public:
    Pred_Hybrid(bp_params);
    void next_pc_outcome(uint32 addr, bool outcome);
    void content_pt();
    void info();
};
