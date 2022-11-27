#include "pred.h"
#include "sim_bp.h"

Predictor::Predictor(bp_params b){
    params = b;
    cnt_pred = 0;
    cnt_mispred = 0;
    size_pt = 0;
}
void Predictor::info(){
    float rate_mispred = (cnt_mispred*100.0)/(float)cnt_pred;
    cout<<"OUTPUT"<<endl;
     cout<<"number of predictions:    "<<    cnt_pred<<endl;
     cout<<"number of mispredictions: "<<cnt_mispred<<endl;
     cout<<"misprediction rate:  "<<fixed<<setprecision(2) <<rate_mispred<<"%"<<endl;
    content_pt();
}
void Predictor::content_pt(){
    for (size_t i = 0; i < size_pt; i++) {
        cout<<i<<" "<<(int)pt[i]<<endl;
    }
}
void Predictor::update_pt(uint32 idx, bool taken){
    if(pt[idx]==0){
        pt[idx] = ( taken )? 1 : 0;
    }else if(pt[idx]==1){
        pt[idx] = ( taken )? 2 : 0;
    }else if(pt[idx]==2){
        pt[idx] = ( taken )? 3 : 1;
    }else if(pt[idx]==3){
        pt[idx] = ( taken )? 3 : 2;
    }else{
        cout<<"Error: pt content out of bound: "<<pt[idx]<<endl;
        exit(1);
    }
}
bool Predictor::predict_pt(uint32 idx){
    return (pt[idx]>= 2)? TAKEN : NOT_TAKEN;
}
void Predictor::next_idx_outcome(uint32 idx, bool outcome){
    //idx = ( addr & mask_m_bit ) >> 2;
    bool pred = predict_pt(idx);

    update_pt(idx, outcome);

    cnt_pred++;
    if(pred!=outcome) cnt_mispred++;
}
void Predictor::print_miss_rate(){
    float rate_mispred = (cnt_mispred*100.0)/(float)cnt_pred;
    cout<<fixed<<setprecision(2)<<rate_mispred<<endl;
}
//===============================================================================

Pred_Bimodal::Pred_Bimodal(bp_params b):Predictor(b){
        size_pt = 1<<params.M2; // 2^m
        mask_m_bit = ( size_pt -1 )<<2;
        pt = (char*) malloc (size_pt*sizeof(char));
        memset(pt, '2'-48, size_pt*sizeof(char));
}
uint32 Pred_Bimodal::gen_idx(uint32 addr){
    return ( addr & mask_m_bit ) >> 2;
}
void Pred_Bimodal::next_pc_outcome(uint32 addr, bool outcome){
    //uint32 idx_bio = ( addr & mask_m_bit ) >> 2;
    uint32 idx_bio = gen_idx(addr);
#ifdef DEBUG_
    char before = pt[idx_bio];
#endif

    Predictor::next_idx_outcome(idx_bio,outcome);
#ifdef DEBUG_
    char oo = (outcome)? 't' : 'n';
    if(cnt_pred<20){
        cout<<"="<<cnt_pred<<" "<<hex<<addr<<dec<<" "<<oo<<endl;
        cout<<"BP "<<idx_bio<<" "<<(int)before<<endl;
        cout<<"BU "<<idx_bio<<" "<<(int)pt[idx_bio]<<endl;
    }
#endif
}
void Pred_Bimodal::content_pt(){
    cout<<"FINAL BIMODAL CONTENTS"<<endl;
    Predictor::content_pt();
}

//===============================================================================
Pred_Gshare::Pred_Gshare(bp_params b):Predictor(b){
        size_pt = 1 << params.M1; // 2^m
        mask_m_bit = ( size_pt -1 )<<2;
        pt = (char*) malloc (size_pt*sizeof(char));
        memset(pt, '2'-48, size_pt*sizeof(char));
        mask_n_bit = ( 1 << params.N ) -1 ;
        gbh = 0;
}

uint32 Pred_Gshare::get_gbh(){
    return gbh & mask_n_bit;
}
void Pred_Gshare::update_gbh(bool outcome){
    gbh = ( gbh >> 1 ) | ( outcome << (params.N-1) );
}
uint32 Pred_Gshare::gen_idx(uint32 addr){
    uint32 idx_m = ( addr & mask_m_bit ) >> 2;
    uint32 last_bit = params.M1 - params.N; // last m-n bits
    return ( get_gbh() << last_bit ) ^ idx_m;
}

void Pred_Gshare::next_pc_outcome(uint32 addr, bool outcome){
    uint32 idx_gshare = gen_idx(addr);
#ifdef DEBUG_
    char before = pt[idx_gshare];
#endif
    Predictor::next_idx_outcome( idx_gshare, outcome );

#ifdef DEBUG_
    char oo = (outcome)? 't' : 'n';
    if(cnt_pred<20){
        cout<<"="<<cnt_pred<<" "<<hex<<addr<<dec<<" "<<oo<<endl;
        cout<<"GP "<<idx_gshare<<" "<<(int)before<<endl;
        cout<<"GU "<<idx_gshare<<" "<<(int)pt[idx_gshare]<<endl;
    }
#endif
    update_gbh(outcome);
}
void Pred_Gshare::content_pt(){
    cout<<"FINAL GSHARE CONTENTS"<<endl;
    Predictor::content_pt();
}



//===============================================================================

Pred_Hybrid::Pred_Hybrid(bp_params b):Predictor(b){
        pred_bio = new Pred_Bimodal(b);
        pred_gsh = new Pred_Gshare(b);
        size_pt = 1 << params.K; // size of chooser table = 2^k
        mask_m_bit = ( size_pt -1 )<<2;
        pt = (char*) malloc (size_pt*sizeof(char));
        memset(pt, '1'-48, size_pt*sizeof(char));
        //mask_n_bit = ( 1 << params.N ) -1 ;
}
uint32 Pred_Hybrid::gen_idx(uint32 addr){
    return ( addr & mask_m_bit ) >> 2;
}
void Pred_Hybrid::next_pc_outcome(uint32 addr, bool outcome){

#ifdef DEBUG_
    char oo = (outcome)? 't' : 'n';
    if(cnt_pred<100) cout<<"="<<cnt_pred<<" "<<hex<<addr<<dec<<" "<<oo<<endl;
#endif
    // step 1: obtain two predictions
    uint32 idx_bio = pred_bio->gen_idx(addr);
    uint32 idx_gsh = pred_gsh->gen_idx(addr);
    bool prediction_bio = pred_bio->predict_pt( idx_bio );
    bool prediction_gsh = pred_gsh->predict_pt( idx_gsh );
#ifdef DEBUG_
        if(cnt_pred<100) cout<<"GP "<<idx_gsh<<" "<<(int)pred_gsh->pt[idx_gsh]<<endl;
        if(cnt_pred<100) cout<<"BP "<<idx_bio<<" "<<(int)pred_bio->pt[idx_bio]<<endl;
#endif
    // step 2: determine branch's index
    uint32 idx_ct = gen_idx(addr);

    // step 3: make an overall prediction
    bool choose_gsh =predict_pt(idx_ct);
    if(choose_gsh){
        if(prediction_gsh != outcome)   cnt_mispred++;
    }else{
        if(prediction_bio != outcome)   cnt_mispred++;
    }
    cnt_pred++;
#ifdef DEBUG_
        if(cnt_pred<100) cout<<"CP "<<idx_ct<<" "<<(int)pt[idx_ct]<<endl;
#endif
    // step 4: update selected branch predictor
    if(choose_gsh){
        pred_gsh->update_pt(idx_gsh, outcome);
    }else{
        pred_bio->update_pt(idx_bio, outcome);
    }
#ifdef DEBUG_
    if(choose_gsh){ if(cnt_pred<100)  cout<<"GU "<<idx_gsh<<" "<<(int)pred_gsh->pt[idx_gsh]<<endl; }
    if(!choose_gsh){ if(cnt_pred<100)  cout<<"BU "<<idx_bio<<" "<<(int)pred_bio->pt[idx_bio]<<endl; }
#endif
    // step 5: update global branch history
    pred_gsh->update_gbh(outcome);

    // step 6: update branch choose counter
    if( (prediction_gsh == outcome) && (prediction_bio != outcome) ){
        update_pt(idx_ct, TAKEN);
    }else if( (prediction_gsh != outcome) && (prediction_bio == outcome) ){
        update_pt(idx_ct, NOT_TAKEN);
    }
#ifdef DEBUG_
    if( (prediction_gsh == outcome) && (prediction_bio != outcome) ){
        if(cnt_pred<100) cout<<"CU "<<idx_ct<<" "<<(int)pt[idx_ct]<<endl;
    }else if( (prediction_gsh != outcome) && (prediction_bio == outcome) ){
        if(cnt_pred<100) cout<<"CU "<<idx_ct<<" "<<(int)pt[idx_ct]<<endl;
    }
#endif
}
void Pred_Hybrid::info(){
    float rate_mispred = (cnt_mispred*100.0)/(float)cnt_pred;
    cout<<"OUTPUT"<<endl;
     cout<<"number of predictions:    "<<    cnt_pred<<endl;
     cout<<"number of mispredictions: "<<cnt_mispred<<endl;
     cout<<"misprediction rate:  "<<fixed<<setprecision(2) <<rate_mispred<<"%"<<endl;
    content_pt();
}
void Pred_Hybrid::content_pt(){
    cout<<"FINAL CHOOSER CONTENTS"<<endl;
    Predictor::content_pt();
    pred_gsh->content_pt();
    pred_bio->content_pt();
}
