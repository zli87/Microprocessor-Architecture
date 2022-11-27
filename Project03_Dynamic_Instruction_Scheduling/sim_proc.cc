#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sim_proc.h"
#include "cpu.cc"

/*  argc holds the number of command line arguments
    argv[] holds the commands themselves

    Example:-
    sim 256 32 4 gcc_trace.txt
    argc = 5
    argv[0] = "sim"
    argv[1] = "256"
    argv[2] = "32"
    ... and so on
*/
int main (int argc, char* argv[])
{
    FILE *FP;               // File handler
    char *trace_file;       // Variable that holds trace file name;
    proc_params params;       // look at sim_bp.h header file for the the definition of struct proc_params
    int op_type, dest, src1, src2;  // Variables are read from trace file
    uint32 pc; // Variable holds the pc read from input file

    if (argc != 5)
    {
        printf("Error: Wrong number of inputs:%d\n", argc-1);
        exit(EXIT_FAILURE);
    }

    params.rob_size     = strtoul(argv[1], NULL, 10);
    params.iq_size      = strtoul(argv[2], NULL, 10);
    params.width        = strtoul(argv[3], NULL, 10);
    trace_file          = argv[4];
    /*printf("rob_size:%lu "
            "iq_size:%lu "
            "width:%lu "
            "tracefile:%s\n", params.rob_size, params.iq_size, params.width, trace_file);
    */
    // Open trace_file in read mode
    FP = fopen(trace_file, "r");
    if(FP == NULL)
    {
        // Throw error and exit if fopen() failed
        printf("Error: Unable to open file %s\n", trace_file);
        exit(EXIT_FAILURE);
    }

    CPU_OOO* pro = new CPU_OOO(params);

    INST_INFO* inst_input;
    inst_input = new INST_INFO[params.width]();
    int seq = 0;
    uint32 last = 0;

    while(last == 0){
        for(int k=0; k < (int)params.width ; k++)
        {
            if(fscanf(FP, "%lx %d %d %d %d", &pc, &op_type, &dest, &src1, &src2) != EOF)
            {
                inst_input[k] = (INST_INFO){.seq=seq, .op=op_type, .src1=src1, .src2=src2, .dst=dest };
                last = 0;
                seq++;
            }else
            {
                inst_input[k] = (INST_INFO){.seq= -1 , .op=op_type, .src1=src1, .src2=src2, .dst=dest };
                last = 1;
            }

        } // end for k


        do{
            pro->UpdInfo();
            //pro->PrintState();

            pro->Retire();
            pro->Writeback();
            pro->Execute();
            pro->Issue();
            pro->Dispatch();
            pro->RegRead();
            pro->Rename();
            pro->Decode();
            pro->Fetch(inst_input);
            pro->UpdPrintFetch();
        }while( pro->Advance_Cycle() );

    }
    //cout<<"Second Loop\n";
    do{
        for(int k=0; k < (int)params.width ; k++)
            inst_input[k] = (INST_INFO){.seq= -1 , .op=op_type, .src1=src1, .src2=src2, .dst=dest };

        pro->UpdInfo();
        //pro->PrintState();

        pro->Retire();
        pro->Writeback();
        pro->Execute();
        pro->Issue();
        pro->Dispatch();
        pro->RegRead();
        pro->Rename();
        pro->Decode();
        pro->Fetch(inst_input);
        pro->UpdPrintFetch();
        //cout<<"done flag: "<<pro->done_flag<<endl;
        pro->Advance_Cycle();
    }while( !pro->done_flag );

    cout<<"# === Simulator Command =========\n";
    cout<<"# "<<argv[0]<<" "<<argv[1]<<" "<<argv[2]<<" "<<argv[3]<<" "<<argv[4]<<endl;
    pro->PrintConfig();
    pro->PrintResult();

    //cout<<fixed<<setprecision(2)<<pro->ipc<<endl;
    return 0;
}
