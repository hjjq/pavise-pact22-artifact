#pragma once

#include <iostream>
#include <mutex>
#include <manager.hpp>

class Box {
  public:

    void ResetMutex() {
        // We want to ensure mutex is locked before we try to unlock it.
        manager_lock_.try_lock();
        manager_lock_.unlock();
    }

    void SetManager(persistent_ptr<Manager> manager_ptr) {
        assert(manager_ptr_ == nullptr);
        manager_ptr_ = manager_ptr;
    }

    // Can modify the pointer itself which doesn't affect us,
    // but cannot modify the underlying manager.
    const persistent_ptr<Manager> GetManager() {
        return manager_ptr_;
    }

    persistent_ptr<Manager> LockAndReturnManager() {
        manager_lock_.lock();
        return manager_ptr_;
    }

    void UpdateManagerAndUnlock(persistent_ptr<Manager> new_manager) {
        manager_ptr_ = new_manager;
        manager_lock_.unlock();
    }
    //Manager_t *acquire_read_lock()

  private:
    // int readers_;
    // bool lock_held_;
    std::mutex manager_lock_;
    persistent_ptr<Manager> manager_ptr_;
};
