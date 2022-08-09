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

#define PM_ADDR_BASE 0x10000000000ULL
#define PM_ADDR_SIZE 0x10000000000ULL
std::ostream * out = &cout;
unsigned nt_cnt, clwb_cnt, sfence_cnt;

// Holds instruction count for a single procedure
typedef struct RtnCount
{
    string _name;
    string _image;
    ADDRINT _address;
    RTN _rtn;
    UINT64 _rtnCount;
    UINT64 _icount;
    struct RtnCount * _next;
} RTN_COUNT;

// Linked list of instruction counts for each routine
RTN_COUNT * RtnList = 0;
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
    *out << "NTStore"<< endl;
    nt_cnt++;
}
void record_clwb(){
    *out << "CLWB"<< endl;
    clwb_cnt++;
}
void record_sfence(){
    *out << "========= SFENCE ========"<< endl;
    sfence_cnt++;
}

void print_rtn_name(RTN_COUNT* rc){
    *out << "Calling " << rc->_name << endl;
}

// This function is called before every instruction is executed
VOID docount(UINT64 * counter)
{
    (*counter)++;
}
    
const char * StripPath(const char * path)
{
    const char * file = strrchr(path,'/');
    if (file)
        return file+1;
    else
        return path;
}

void record_pmread (ADDRINT ip, void* addr) {

    if ((uint64_t) addr >= PM_ADDR_BASE &&
            (uint64_t) addr <= PM_ADDR_BASE + PM_ADDR_SIZE ){
        *out << "PM READ at addr: " << addr << endl;
    } 
} 
void record_pmwrite (ADDRINT ip, void* addr) {

    if ((uint64_t) addr >= PM_ADDR_BASE &&
            (uint64_t) addr <= PM_ADDR_BASE + PM_ADDR_SIZE ){

        *out << "PM WRITE at addr: " << addr << endl;

    } 
}

// Pin calls this function every time a new rtn is executed
VOID Routine(RTN rtn, VOID *v)
{
    
    // Allocate a counter for this routine
    RTN_COUNT * rc = new RTN_COUNT;

    // The RTN goes away when the image is unloaded, so save it now
    // because we need it in the fini
    rc->_name = RTN_Name(rtn);
    rc->_image = StripPath(IMG_Name(SEC_Img(RTN_Sec(rtn))).c_str());
    rc->_address = RTN_Address(rtn);
    rc->_icount = 0;
    rc->_rtnCount = 0;

    // Add to list of routines
    rc->_next = RtnList;
    RtnList = rc;

    if(rc->_name[0] == '_') return;
    if(rc->_name.find("clock") != string::npos) return;
    if(rc->_name.find(".text") != string::npos) return;
    //if(rc->_name.find("@plt") != string::npos) return;
    if(rc->_name.find("sleep") != string::npos) return;
    if(rc->_name == "malloc") return;
    if(rc->_name == "cfree") return;
    if(rc->_name == "free") return;
    if(rc->_name == "calloc") return;
    RTN_Open(rtn);
            
    RTN_InsertCall(
            rtn, IPOINT_BEFORE, (AFUNPTR) print_rtn_name,
            IARG_PTR, rc,
            IARG_END);
   
    RTN_Close(rtn);
}

// Detect R/W to PMEM region - address starting from 0x10000000000 as set by PMEM_MMAP_HINT
// Also detect memcpy, memset, etc.

void Instruction(INS ins, void* v) {
    UINT32 memOperands = INS_MemoryOperandCount(ins);

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
    else if (INS_IsStandardMemop(ins))
    {

        for (UINT32 memOp = 0; memOp < memOperands; memOp++) {
            if (INS_MemoryOperandIsRead(ins,memOp)) {
                INS_InsertPredicatedCall(
                        ins, IPOINT_BEFORE, (AFUNPTR) record_pmread,
                        IARG_INST_PTR,
                        IARG_MEMORYOP_EA, memOp,
                        IARG_END);
            } else if (INS_MemoryOperandIsWritten(ins,memOp)) {
                INS_InsertPredicatedCall(
                        ins, IPOINT_BEFORE, (AFUNPTR) record_pmwrite,
                        IARG_INST_PTR,
                        IARG_MEMORYOP_EA, memOp,
                        IARG_END);
            }
        }

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

    //if (!fileName.empty()) { out = new std::ofstream(fileName.c_str());}

    nt_cnt = clwb_cnt = sfence_cnt = 0;

    RTN_AddInstrumentFunction(Routine,0);
    INS_AddInstrumentFunction(Instruction,0);
    PIN_AddFiniFunction(Fini, 0);

    cout <<  "===============================================" << endl;
    cout <<  "This application is instrumented by MyPinTool" << endl;
    if (!KnobOutputFile.Value().empty()) 
    {
        cout << "See file " << KnobOutputFile.Value() << " for analysis results" << endl;
    }
    cout <<  "===============================================" << endl;

    // Start the program, never returns
    PIN_StartProgram();

    return 0;
}

/* ===================================================================== */
/* eof */
/* ===================================================================== */
