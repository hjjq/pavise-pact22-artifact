#include "pin.H"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
using std::cerr;
using std::cout;
using std::string;
using std::endl;

/* ================================================================== */
// Global variables, data structure and  macro definitions
/* ================================================================== */

std::ostream * out = &cerr;
unsigned nt_cnt, clwb_cnt, sfence_cnt;
/* ===================================================================== */
// Command line switches
/* ===================================================================== */
KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE,  "pintool",
        "o", "", "specify file name for MyPinTool output");

/* ===================================================================== */
// Utilities
/* ===================================================================== */

/*!
 *  Print out help message.
 */
INT32 Usage()
{
    cerr << "Experimental pintool for Pavise" << endl;

    cerr << KNOB_BASE::StringKnobSummary() << endl;

    return -1;
}


void record_movnt(){
    nt_cnt++;
}
void record_clwb(){
    clwb_cnt++;
}
void record_sfence(){
    sfence_cnt++;
}

// Detect R/W to PMEM region - address starting from 0x10000000000 as set by PMEM_MMAP_HINT
// Also detect memcpy, memset, etc.

void Instruction(INS ins, void* v) {

    //*out << "Instruction addr: " << INS_Address(ins) << endl;
    string name = INS_Mnemonic(ins);
    if(name.find("MOVNT") != std::string::npos) {
        INS_InsertPredicatedCall(
                ins, IPOINT_BEFORE, (AFUNPTR) record_movnt,
                IARG_END);
    }
    else if(name.find("CLWB") != std::string::npos) {
        INS_InsertPredicatedCall(
                ins, IPOINT_BEFORE, (AFUNPTR) record_clwb,
                IARG_END);
    }
    else if(name.find("SFENCE") != std::string::npos) {
        INS_InsertPredicatedCall(
                ins, IPOINT_BEFORE, (AFUNPTR) record_sfence,
                IARG_END);
    }

}   

void Fini(INT32 code, VOID *v) {
    cerr << "MOVNT count = " << nt_cnt << ", clwb count = " << clwb_cnt << ", sfence count " << sfence_cnt << endl;
}

/*!
 * The main procedure of the tool.
 * This function is called when the application image is loaded but not yet started.
 * @param[in]   argc            total number of elements in the argv array
 * @param[in]   argv            array of command line arguments, 
 *                              including pin -t <toolname> -- ...
 */

int main(int argc, char *argv[])
{
    // Initialize PIN library. Print help message if -h(elp) is specified
    // in the command line or the command line is invalid 
    if( PIN_Init(argc,argv) )
    {
        return Usage();
    }

    PIN_InitSymbols();


    string fileName = KnobOutputFile.Value();

    if (!fileName.empty()) { out = new std::ofstream(fileName.c_str());}

    nt_cnt = clwb_cnt = sfence_cnt = 0;

    INS_AddInstrumentFunction(Instruction,0);
    PIN_AddFiniFunction(Fini, 0);

    cerr <<  "===============================================" << endl;
    cerr <<  "This application is instrumented by MyPinTool" << endl;
    if (!KnobOutputFile.Value().empty()) 
    {
        cerr << "See file " << KnobOutputFile.Value() << " for analysis results" << endl;
    }
    cerr <<  "===============================================" << endl;

    // Start the program, never returns
    PIN_StartProgram();

    return 0;
}

/* ===================================================================== */
/* eof */
/* ===================================================================== */
