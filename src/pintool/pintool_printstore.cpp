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
#define PinDEBUG(output) PIN_MutexLock(&print_lock); \
                             output; \
                         PIN_MutexUnlock(&print_lock)

#define PM_ADDR_BASE 0x10000000000ULL
#define PM_ADDR_SIZE 0x10000000000ULL
#define PMEM_BASE ((uintptr_t)0x10000000000ull)
#define SHADOW_BASE ((uintptr_t)0x30000000000ull)
#define SP_DIFF (SHADOW_BASE - PMEM_BASE)
#define PAVISE_BASE ((uintptr_t)0x50000000000ull)
#define PLAY_BASE ((uintptr_t)0x60000000000ull)
#define PLAY_SHADOW_BASE ((uintptr_t)0x70000000000ull)
#define WITHIN_PMEM(a) ((uintptr_t)a >= PMEM_BASE && (uintptr_t)a < SHADOW_BASE)
#define WITHIN_SHADOW(a) ((uintptr_t)a >= SHADOW_BASE && (uintptr_t)a < PAVISE_BASE)
#define PMEM_OFFSET(a) ((uintptr_t)a - PMEM_BASE)
#define SHADOW_OFFSET(a) ((uintptr_t)a - SHADOW_BASE)
PIN_MUTEX print_lock;

bool in_roi = true;
bool record_memfunc = false;

std::ostream * out = &cerr;

// Struct that contains instruction info such as source code location and opcode mnemonic

struct ins_info {
    int line;
    string path;
    string mnemonic;
    string disassemble;
    bool visited;
};

// Map an instruction address to its source code location
// No lock needed because during analysis time it is read-only
std::unordered_map<ADDRINT, struct ins_info> ins_addr2info;

// Keep track of RTN's printed. Do not record duplicates
std::unordered_map<UINT32,bool> rtn_visited;

// Map routine addr to name
std::unordered_map<UINT32,string> rtn_id2name;

// Map routine addr to current inst addr calling the routine
std::unordered_map<UINT32,ADDRINT> rtn_id2ins;

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

// Get absolute path of a file
string get_abs_path(string rel_path) {
    char* abs_path = realpath(rel_path.c_str(), NULL);
    string p(abs_path);
    free(abs_path);
    return p;
}

// Check if function is a PM memory operation
bool check_memxxx(string name) {
    if(name.find("pmem") != string::npos){
        return false;
    }
    if(name.find("memcpy") != string::npos
            || name.find("memset") != string::npos
            || name.find("memmove") != string::npos){
        return true;
    }
    return false;
}

// Check if function is a PM memory operation
bool check_pmemxxx(string name) {
    if(name.find("pmem") == string::npos){
        return false;
    }
    if(name.find("memcpy") != string::npos
            || name.find("memset") != string::npos
            || name.find("memmove") != string::npos
            || name.find("persist") != string::npos
            || name.find("flush") != string::npos
            || name.find("drain") != string::npos
            ){
        return true;
    }
    return false;
}

/* ================================================================== */
// Functions to be inserted during analysis time (run-time)
/* ================================================================== */


/* ================================================================== */
// Functions to be inserted before routines
/* ================================================================== */

void roi_handler(string func_name) {

    if(func_name == "pavise_instrument_begin") {
        ASSERTX(!in_roi);
        in_roi = true;
        PinDEBUG(*out << "Begin RoI" << endl << endl;);
    }
    else if(func_name == "pavise_instrument_end") {
        ASSERTX(in_roi);
        in_roi = false;
        PinDEBUG(*out<< "End RoI" << endl << endl;);
    } 
}

void memfunc_handler(UINT32 id, ADDRINT arg0, ADDRINT arg1, ADDRINT arg2) {

    //if(!in_roi || !record_memfunc) return;

    if( rtn_id2ins.find(id) == rtn_id2ins.end()) {
        //PinDEBUG(*out << "instruction not in rtn_id2ins, we should not reach here\n";);
        return;
    }

    ADDRINT ip = rtn_id2ins[id];
    if( ins_addr2info.find(ip) == ins_addr2info.end()) {
        PinDEBUG(*out << "instruction not in ins_addr2info, we should not reach here\n";);
        assert(0);
        return;
    }
    
    struct ins_info info = ins_addr2info[ip];
    if(rtn_id2name.find(id) == rtn_id2name.end()) {
        PinDEBUG(*out << "Addr not found in rtn_id2name, we should not reach here\n";);
        assert(0);
        return;
    }
    string func_name = rtn_id2name[id];

    if(info.line){
        if (WITHIN_PMEM(arg0)){
            PinDEBUG(*out << "mem call found, name = " << func_name << ", dst = "<< std::hex << arg0 << std::dec  << endl;);
            //PinDEBUG(*out << "INS Disassemble: " << info.disassemble << endl;);
            string path = get_abs_path(info.path);
            //PinDEBUG(*out << path << ":" << info.line << endl << endl;);
        }
    }

    //record_memfunc = false;
}
void pmemfunc_handler(UINT32 id, ADDRINT arg0, ADDRINT arg1, ADDRINT arg2, ADDRINT arg3) {
    return;
    if(!in_roi || !record_memfunc) return;

    if( rtn_id2ins.find(id) == rtn_id2ins.end()) {
        //PinDEBUG(*out << "instruction not in rtn_id2ins, we should not reach here\n";);
        return;
    }
    ADDRINT ip = rtn_id2ins[id];
    if( ins_addr2info.find(ip) == ins_addr2info.end()) {
        PinDEBUG(*out << "instruction not in ins_addr2info, we should not reach here\n";);
        assert(0);
        return;
    }
    
    struct ins_info info = ins_addr2info[ip];
    if(!info.visited) {
        if(rtn_id2name.find(id) == rtn_id2name.end()) {
            PinDEBUG(*out << "Addr not found in rtn_id2name, we should not reach here\n";);
            assert(0);
            return;
        }
        string func_name = rtn_id2name[id];

        if(info.line){
            if (WITHIN_PMEM(arg0)){
                PinDEBUG(*out << "pmem call found, name = " << func_name << ", dst = "<< std::hex << arg0 << std::dec  << endl;);
                PinDEBUG(*out << "INS Disassemble: " << info.disassemble << endl;);
                string path = get_abs_path(info.path);
                PinDEBUG(*out << path << ":" << info.line << endl << endl;);
                ins_addr2info[ip].visited = true;
            }
        }
    }

    record_memfunc = false;
}

/* ================================================================== */
// Functions to be inserted before instructions
/* ================================================================== */

void record_pmread (ADDRINT ip, void* addr) {
    return;
    if ( in_roi && WITHIN_PMEM(addr) ){

        struct ins_info info = ins_addr2info[ip];
        if(!info.visited) {
            PinDEBUG(*out << "PM READ at addr: " << std::hex << addr << std::dec << endl;);
            PinDEBUG(*out << "INS Mnemonic: " << info.mnemonic << endl;);
            PinDEBUG(*out << "INS Disassemble: " << info.disassemble << endl;);

            if(info.line){
                string path = get_abs_path(info.path);
                PinDEBUG(*out << path << ":" << info.line << endl << endl;);
            }
            else {
                PinDEBUG(*out << "line not available" << endl << endl;);
            }

            ins_addr2info[ip].visited = true;
        }
    } 
} 

void record_pmwrite (ADDRINT ip, void* addr, UINT32 size) {

    if (WITHIN_PMEM(addr) ){

        struct ins_info info = ins_addr2info[ip];
        PinDEBUG(*out << "PM WRITE at addr = " << std::hex << addr << std::dec << ", size = " <<
                size << endl;);
        //PinDEBUG(*out << "INS Mnemonic: " << info.mnemonic << endl;);
        //PinDEBUG(*out << "INS Disassemble: " << info.disassemble << endl;);

        if(info.line){
            string path = get_abs_path(info.path);
            //PinDEBUG(*out << path << ":" << info.line << endl << endl;);
        }
        else {
            //PinDEBUG(*out << "line not available" << endl << endl;);
        }
    } 
}

void record_memcall (ADDRINT ip, UINT32 id) {
    //return;
    if(!in_roi) return;

    struct ins_info info = ins_addr2info[ip];
    rtn_id2ins[id] = ip;

    if(info.line){
        record_memfunc = true;
    }

}


/* ================================================================== */
// Callback functions during instrumentation time (pre-run-time) 
/* ================================================================== */

// Detect annotations to Enable and Disable instrumentation,
// as well as detecting mmap()'s
VOID Routine(RTN rtn, VOID* v){

    if (!RTN_Valid(rtn))
    {
        PinDEBUG(*out << "RTN invalid" << endl;);
        return;
    }

    RTN_Open(rtn);

    string name = RTN_Name(rtn);
    //ADDRINT addr = RTN_Address(rtn);
    UINT32 id = RTN_Id(rtn);

    if(name.find("pavise_instrument") != string::npos) {
        RTN_InsertCall(
            rtn, IPOINT_BEFORE, (AFUNPTR) roi_handler,
            IARG_ADDRINT, RTN_Name(rtn).c_str(),
            IARG_END);
    }
    if(check_memxxx(name))
    {
        rtn_id2name[id] = name;
        RTN_InsertCall(
                rtn, IPOINT_BEFORE, (AFUNPTR) memfunc_handler,
                IARG_ADDRINT, id,
                IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
                IARG_FUNCARG_ENTRYPOINT_VALUE, 1,
                IARG_FUNCARG_ENTRYPOINT_VALUE, 2,
                IARG_END);
    }
    else if(check_pmemxxx(name))
    {
        rtn_id2name[id] = name;
        RTN_InsertCall(
                rtn, IPOINT_BEFORE, (AFUNPTR) pmemfunc_handler,
                IARG_ADDRINT, id,
                IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
                IARG_FUNCARG_ENTRYPOINT_VALUE, 1,
                IARG_FUNCARG_ENTRYPOINT_VALUE, 2,
                IARG_FUNCARG_ENTRYPOINT_VALUE, 3,
                IARG_END);
    }
    RTN_Close(rtn);
    
}



// Detect R/W to PMEM region - address starting from 0x10000000000 as set by PMEM_MMAP_HINT
// Also detect memcpy, memset, etc.

void Instruction(INS ins, void* v) {

    UINT32 memOperands = INS_MemoryOperandCount(ins);
    //*out << "Instruction addr: " << INS_Address(ins) << endl;

    if (INS_IsStandardMemop(ins))
    {
    
        for (UINT32 memOp = 0; memOp < memOperands; memOp++) {
            if (INS_MemoryOperandIsRead(ins,memOp)) {

                // Get info of an instruction and insert it to global map
                int line;
                string path;
                PIN_GetSourceLocation(INS_Address(ins), NULL, &line, &path);
                string mnemonic = INS_Mnemonic(ins);
                string disassemble = INS_Disassemble(ins);
                struct ins_info info = {line, path, mnemonic, disassemble, false};
                ins_addr2info.insert({INS_Address(ins), info});

                INS_InsertPredicatedCall(
                        ins, IPOINT_BEFORE, (AFUNPTR) record_pmread,
                        IARG_INST_PTR,
                        IARG_MEMORYOP_EA, memOp,
                        IARG_END);
            } else if (INS_MemoryOperandIsWritten(ins,memOp)) {

                // Get info of an instruction and insert it to global map
                int line;
                string path;
                PIN_GetSourceLocation(INS_Address(ins), NULL, &line, &path);
                string mnemonic = INS_Mnemonic(ins);
                string disassemble = INS_Disassemble(ins);
                struct ins_info info = {line, path, mnemonic, disassemble, false};
                ins_addr2info.insert({INS_Address(ins), info});

                if(INS_hasKnownMemorySize(ins)){
                    INS_InsertPredicatedCall(
                            ins, IPOINT_BEFORE, (AFUNPTR) record_pmwrite,
                            IARG_INST_PTR,
                            IARG_MEMORYOP_EA, memOp,
                            IARG_MEMORYWRITE_SIZE,
                            IARG_END);
                }
                else{
                    INS_InsertPredicatedCall(
                            ins, IPOINT_BEFORE, (AFUNPTR) record_pmwrite,
                            IARG_INST_PTR,
                            IARG_MEMORYOP_EA, memOp,
                            IARG_UINT32, 0,
                            IARG_END);
                }
            }
        }

    }
    if(INS_IsCall(ins) && INS_IsDirectControlFlow(ins)){
        ADDRINT callee_addr = INS_DirectControlFlowTargetAddress(ins);
        string callee_name = RTN_FindNameByAddress(callee_addr);

        if(check_memxxx(callee_name) || check_pmemxxx(callee_name))
        {
            RTN calleeRTN = RTN_FindByAddress(callee_addr);
            // Get info of an instruction and insert it to global map
            int line;
            string path;
            PIN_GetSourceLocation(INS_Address(ins), NULL, &line, &path);
            string mnemonic = INS_Mnemonic(ins);
            string disassemble = INS_Disassemble(ins);
            struct ins_info info = {line, path, mnemonic, disassemble, false};
            ins_addr2info.insert({INS_Address(ins), info});

            //RTN caller_func = INS_Rtn(ins);
            UINT32 callee_id = RTN_Id(calleeRTN);

            /*if(check_pmemxxx(callee_name)){
                if(check_memxxx(name)){
                    *out << "Parent is pmem: " << callee_name << ", child is mem: " <<
                        name << endl;
                }
                else{
                    *out << "Parent is pmem: " << callee_name << ", child is pmem: " <<
                        name << endl;
                }
                return;
            }*/

            INS_InsertPredicatedCall(
                    ins, IPOINT_BEFORE, (AFUNPTR) record_memcall,
                    IARG_INST_PTR,
                    IARG_ADDRINT, callee_id,
                    IARG_END);

        }
    }
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

    PIN_MutexInit(&print_lock);

    string fileName = KnobOutputFile.Value();

    if (!fileName.empty()) { out = new std::ofstream(fileName.c_str());}

    RTN_AddInstrumentFunction(Routine, 0);

    INS_AddInstrumentFunction(Instruction,0);

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
