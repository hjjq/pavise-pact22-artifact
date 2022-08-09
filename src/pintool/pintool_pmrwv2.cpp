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
#define SHADOW_ADDR_BASE 0x20000000000ULL
#define PAVISE_ADDR_BASE 0x30000000000ULL
#define PM_ADDR_SIZE 0x10000000000ULL

std::ostream * out = &cout;
unsigned nt_cnt, clwb_cnt, sfence_cnt;
uint64_t total_pmbytes;
uint64_t total_shadowbytes;
uint64_t total_pavisebytes;
uint64_t num_writes;
uint64_t total_movnt_bytes, total_clwb_bytes, clwb_nonpm;
bool begin_count;
std::unordered_map<UINT32, string> rtnid_name_map;
std::unordered_map<UINT32, uint64_t> rtn_write_size_map;

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

void rtn_incr_size (UINT32 id, uint64_t size){
    if(rtn_write_size_map.find(id) != rtn_write_size_map.end()){
        rtn_write_size_map[id] += size;
    }
    else{
        rtn_write_size_map.insert({id, size});
    }
}

void record_movnt(){
    //*out << "NTStore"<< endl;
    nt_cnt++;
}
void record_clwb(){
    //*out << "CLWB"<< endl;
    clwb_cnt++;
}
void record_sfence(){
    //*out << "========= SFENCE ========"<< endl;
    sfence_cnt++;
}

void print_rtn_name(RTN_COUNT* rc){
    //*out << "Calling " << rc->_name << endl;
    if(!begin_count)
        if(rc->_name.find("btree_map_insert") != string::npos) begin_count = 1;
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

void record_movnt_bytes (ADDRINT ip, void* addr, UINT32 size) {

    if(!begin_count) return;
    if ((uint64_t) addr >= PM_ADDR_BASE &&
            (uint64_t) addr < PM_ADDR_BASE + PM_ADDR_SIZE ){
        total_movnt_bytes += size;
        //*out << "PM WRITE at addr: " << addr << endl;
    } 
    else if ((uint64_t) addr >= SHADOW_ADDR_BASE &&
            (uint64_t) addr < SHADOW_ADDR_BASE + PM_ADDR_SIZE ){
        total_movnt_bytes += size;
    }
    else if ((uint64_t) addr >= PAVISE_ADDR_BASE &&
            (uint64_t) addr < PAVISE_ADDR_BASE + PM_ADDR_SIZE ){
        total_movnt_bytes += size;
    }
    else {
        assert(0);
        *out << "movnt at addr: " << addr << endl;
    }
}

void record_clwb_bytes (ADDRINT ip, void* addr, UINT32 size) {

    if(!begin_count) return;
    if ((uint64_t) addr >= PM_ADDR_BASE &&
            (uint64_t) addr < PM_ADDR_BASE + PM_ADDR_SIZE ){
        total_clwb_bytes += size;
        //*out << "PM WRITE at addr: " << addr << endl;
    } 
    else if ((uint64_t) addr >= SHADOW_ADDR_BASE &&
            (uint64_t) addr < SHADOW_ADDR_BASE + PM_ADDR_SIZE ){
        total_clwb_bytes += size;
    }
    else if ((uint64_t) addr >= PAVISE_ADDR_BASE &&
            (uint64_t) addr < PAVISE_ADDR_BASE + PM_ADDR_SIZE ){
        total_clwb_bytes += size;
    }
    else{
        clwb_nonpm += size;
    }
}

void record_pmread (ADDRINT ip, void* addr) {

    if ((uint64_t) addr >= PM_ADDR_BASE &&
            (uint64_t) addr < PM_ADDR_BASE + PM_ADDR_SIZE ){
        //*out << "PM READ at addr: " << addr << endl;
    } 
} 
void record_pmwrite (ADDRINT ip, void* addr, UINT32 size, UINT32 rtnid) {

    if(!begin_count) return;
    if ((uint64_t) addr >= PM_ADDR_BASE &&
            (uint64_t) addr < PM_ADDR_BASE + PM_ADDR_SIZE ){
        total_pmbytes += size;
        num_writes++;
        //*out << "PM WRITE at addr: " << addr << endl;
        rtn_incr_size(rtnid, size);
    } 
    if ((uint64_t) addr >= SHADOW_ADDR_BASE &&
            (uint64_t) addr < SHADOW_ADDR_BASE + PM_ADDR_SIZE ){
        total_shadowbytes += size;
        //*out << "PM WRITE at addr: " << addr << endl;
        rtn_incr_size(rtnid, size);
    } 
    if ((uint64_t) addr >= PAVISE_ADDR_BASE &&
            (uint64_t) addr < PAVISE_ADDR_BASE + PM_ADDR_SIZE ){
        total_pavisebytes += size;
        //*out << "PM WRITE at addr: " << addr << endl;
        rtn_incr_size(rtnid, size);
    } 
}

// Pin calls this function every time a new rtn is executed
VOID Routine(RTN rtn, VOID *v)
{
    UINT32 id = RTN_Id(rtn);
    rtnid_name_map.insert({id,RTN_Name(rtn)});
    //return;
    
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
    int movnt = 0, clwb = 0;
    string name = INS_Mnemonic(ins);
    if(name.find("MOVNT") != std::string::npos) {
        INS_InsertPredicatedCall(
                ins, IPOINT_BEFORE, (AFUNPTR) record_movnt,
                IARG_END);
        movnt = 1;
    }
    else if(name.find("CLWB") != std::string::npos) {
        INS_InsertPredicatedCall(
                ins, IPOINT_BEFORE, (AFUNPTR) record_clwb,
                IARG_END);
        clwb = 1;
    }
    else if(name.find("SFENCE") != std::string::npos) {
        INS_InsertPredicatedCall(
                ins, IPOINT_BEFORE, (AFUNPTR) record_sfence,
                IARG_END);
    }
    if (INS_IsStandardMemop(ins)){

        if(!INS_hasKnownMemorySize(ins))
            *out << "mem op has no memory size\n";
        for (UINT32 memOp = 0; memOp < memOperands; memOp++) {
            if (INS_MemoryOperandIsRead(ins,memOp)) {
                if(clwb){
                    INS_InsertPredicatedCall(
                            ins, IPOINT_BEFORE, (AFUNPTR) record_clwb_bytes,
                            IARG_INST_PTR,
                            IARG_MEMORYOP_EA, memOp,
                            IARG_MEMORYWRITE_SIZE,
                            IARG_END);
                }
                else{
                    INS_InsertPredicatedCall(
                            ins, IPOINT_BEFORE, (AFUNPTR) record_pmread,
                            IARG_INST_PTR,
                            IARG_MEMORYOP_EA, memOp,
                            IARG_END);
                }
            } else if (INS_MemoryOperandIsWritten(ins,memOp)) {
                if(movnt){
                    INS_InsertPredicatedCall(
                            ins, IPOINT_BEFORE, (AFUNPTR) record_movnt_bytes,
                            IARG_INST_PTR,
                            IARG_MEMORYOP_EA, memOp,
                            IARG_MEMORYWRITE_SIZE,
                            IARG_END);
                }
                else{
                    UINT32 rtn_id = RTN_Id(INS_Rtn(ins));
                    INS_InsertPredicatedCall(
                            ins, IPOINT_BEFORE, (AFUNPTR) record_pmwrite,
                            IARG_INST_PTR,
                            IARG_MEMORYOP_EA, memOp,
                            IARG_MEMORYWRITE_SIZE,
                            IARG_UINT32, rtn_id,
                            IARG_END);
                }
            }
        }

    }

}   

void Fini(INT32 code, VOID *v) {
    //*out << "MOVNT count = " << nt_cnt << ", clwb count = " << clwb_cnt << ", sfence count " << sfence_cnt << endl;
    *out << "Total NT-store bytes written = " << total_movnt_bytes << endl;
    *out << "Total PM bytes flushed = " << total_clwb_bytes << endl;
    *out << "Total non-PM bytes flushed = " << clwb_nonpm << endl;
    //*out << "Num writes = " << num_writes << ", Avg PM bytes written = " << total_pmbytes/num_writes << endl;
    *out << "Total Shadow bytes written = " << total_shadowbytes << endl;
    *out << "Total regular PM bytes written = " << total_pmbytes << endl;
    *out << "Total Pavise bytes written = " << total_pavisebytes << endl;
    *out << "Total non-NT bytes written = " << total_pavisebytes + total_pmbytes + total_shadowbytes << endl;
    *out << "Total sfences: " << sfence_cnt << endl;
    *out << endl;
    //*out << "Size of rtn_write_size_map = " << rtn_write_size_map.size() << endl;
    for(auto it = rtn_write_size_map.begin(); it != rtn_write_size_map.end(); it++){
        //*out << "RTN: " << rtnid_name_map[it->first] << " = " << it->second << endl;
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
