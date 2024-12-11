// External headers
#include <unordered_set>
#include <unordered_map>
#include <list>

// Internal headers
#include <tm.hpp>
#include "macros.hpp"

using namespace std;

using word = uintptr_t;

using version = uint64_t;

struct MemorySegment {
    /**
     * @attention may need to update to store ptr to actual location
     */
    MemorySegment* next;
    MemorySegment* prev;
    MemorySegment(): next{nullptr}, prev{nullptr} {};
};

struct MemoryRegion {
    MemorySegment* seg_list;
    size_t size;
    size_t align;
    void* start;
    MemoryRegion(size_t size, size_t align);
};

struct ReadOperation {
    // All of the locations we need to 
    word* target;
    word val;
    bool is_valid; 
    ReadOperation(word* target_, word val_, bool is_valid_);
};

struct WriteOperation {
    // All of the locations we need to 
    word* source;
    word val;
    WriteOperation(word* source_, word val_);
    // WriteOperation& operator=(WriteOperation&& other);
};

struct Transaction {
    version rv;
    unordered_map<word*, list<ReadOperation>> read_set;
    unordered_map<word*, WriteOperation> write_set;
    MemoryRegion* region;
    bool is_ro;
    Transaction(version gvc, MemoryRegion* region_, bool is_ro_);
};
