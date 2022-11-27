#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sim_bp.h"
#include "pred.cc"
#include <iostream>
using namespace std;
/*  argc holds the number of command line arguments
    argv[] holds the commands themselves

    Example:-
    sim bimodal 6 gcc_trace.txt
    argc = 4
    argv[0] = "sim"
    argv[1] = "bimodal"
    argv[2] = "6"
    ... and so on
*/
//#define EXP_

int main (int argc, char* argv[])
{
    FILE *FP;               // File handler
    char *trace_file;       // Variable that holds trace file name;
    bp_params params;       // look at sim_bp.h header file for the the definition of struct bp_params
    char outcome;           // Variable holds branch outcome
    unsigned long int addr; // Variable holds the address read from input file

    if (!(argc == 4 || argc == 5 || argc == 7))
    {
        printf("Error: Wrong number of inputs:%d\n", argc-1);
        exit(EXIT_FAILURE);
    }

    params.bp_name  = argv[1];

    // strtoul() converts char* to unsigned long. It is included in <stdlib.h>
    if(strcmp(params.bp_name, "bimodal") == 0)              // Bimodal
    {
        if(argc != 4)
        {
            printf("Error: %s wrong number of inputs:%d\n", params.bp_name, argc-1);
            exit(EXIT_FAILURE);
        }
        params.M2       = strtoul(argv[2], NULL, 10);
        trace_file      = argv[3];
#ifndef EXP_
        printf("COMMAND\n%s %s %lu %s\n", argv[0], params.bp_name, params.M2, trace_file);
#endif
    }
    else if(strcmp(params.bp_name, "gshare") == 0)          // Gshare
    {
        if(argc != 5)
        {
            printf("Error: %s wrong number of inputs:%d\n", params.bp_name, argc-1);
            exit(EXIT_FAILURE);
        }
        params.M1       = strtoul(argv[2], NULL, 10);
        params.N        = strtoul(argv[3], NULL, 10);
        trace_file      = argv[4];
#ifndef EXP_
        printf("COMMAND\n%s %s %lu %lu %s\n", argv[0], params.bp_name, params.M1, params.N, trace_file);
#endif
    }
    else if(strcmp(params.bp_name, "hybrid") == 0)          // Hybrid
    {
        if(argc != 7)
        {
            printf("Error: %s wrong number of inputs:%d\n", params.bp_name, argc-1);
            exit(EXIT_FAILURE);
        }
        params.K        = strtoul(argv[2], NULL, 10);
        params.M1       = strtoul(argv[3], NULL, 10);
        params.N        = strtoul(argv[4], NULL, 10);
        params.M2       = strtoul(argv[5], NULL, 10);
        trace_file      = argv[6];
#ifndef EXP_
        printf("COMMAND\n%s %s %lu %lu %lu %lu %s\n", argv[0], params.bp_name, params.K, params.M1, params.N, params.M2, trace_file);
#endif
    }
    else
    {
        printf("Error: Wrong branch predictor name:%s\n", params.bp_name);
        exit(EXIT_FAILURE);
    }

    // Open trace_file in read mode
    FP = fopen(trace_file, "r");
    if(FP == NULL)
    {
        // Throw error and exit if fopen() failed
        printf("Error: Unable to open file %s\n", trace_file);
        exit(EXIT_FAILURE);
    }
    // initialize predictor
    Predictor* pred;
    if(strcmp(params.bp_name, "bimodal") == 0)              // Bimodal
    {
        pred = new Pred_Bimodal(params);
    }
    else if(strcmp(params.bp_name, "gshare") == 0)          // Gshare
    {
        pred = new Pred_Gshare(params);
    }
    else if(strcmp(params.bp_name, "hybrid") == 0)          // Hybrid
    {
        pred = new Pred_Hybrid(params);
    }
    char str[2];
    int i=0;
    while(fscanf(FP, "%lx %s", &addr, str) != EOF)
    {
        ++i;
        outcome = str[0];
        if (outcome == 't'){
            pred->next_pc_outcome(addr,TAKEN);
            //if(i<10) printf("%lx %s\n", addr, "t");           // Print and test if file is read correctly
        }else if (outcome == 'n'){
            pred->next_pc_outcome(addr,NOT_TAKEN);
            //if(i<10) printf("%lx %s\n", addr, "n");          // Print and test if file is read correctly
        }/*************************************
            Add branch predictor code here
        **************************************/
    }
#ifndef EXP_
    pred->info();
#endif
#ifdef EXP_
    pred->print_miss_rate();
#endif
    return 0;
}
