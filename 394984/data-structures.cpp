#include "data-structures.hpp"
#include <sstream>
#include <iostream>

// Custom print function
template <typename... Args>
void dprint(Args&&... args) {
    std::ostringstream oss;
    oss << "[Thread " << std::this_thread::get_id() << "] ";
    (oss << ... << args); // Fold expression to handle multiple arguments
    std::cout << oss.str() << std::endl;
}

Transaction::Transaction(version gvc, unordered_set<void*>& seg_list_, bool is_ro_): rv{gvc}, seg_list{seg_list_}, is_ro{is_ro_} {}

MemoryRegion::MemoryRegion(size_t size_, size_t align_): size{size_}, align{align_}, locks{nullptr}, start{nullptr} {}

WriteOperation::WriteOperation(word* source_, word val_): source{source_}, val{val_} {}

VersionedWriteLock::VersionedWriteLock(): version_and_lock{0} {};

bool VersionedWriteLock::lock() {
    word expected, desired;
    // We hope the lock bit is not set
    expected = version_and_lock.load() & ~1;
    // We want the same timestamp but with the lock bit set
    desired = expected | 1;  

    if (version_and_lock.compare_exchange_weak(expected, desired)) {
        // Successfully took the lock
        // dprint("Locking succeeded");
        return true;
    }
    dprint("Locking failed");
    return false;
}


void VersionedWriteLock::unlock() {
    word current = version_and_lock.load();
    word new_value = (current & ~1u) + 2;  // Increment the version and clear the lock bit
    version_and_lock.store(new_value);
    // dprint("Unlocked");
}

word VersionedWriteLock::getVersion() {
    return version_and_lock.load() >> 1;
}

bool VersionedWriteLock::isLocked() {
    return version_and_lock.load() & 1;
}

void VersionedWriteLock::setVersion(version v) {
    word new_val = v << 1 | 1; // Set the version while keeping the lock locked
    version_and_lock.store(new_val);
}
