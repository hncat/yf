#include <iostream>
#include <thread>
#include <unistd.h>
#include <list>
#include <functional>

#include "mutex.h"

static yf::Semaphore s;
static yf::MutexLock mutex;
static yf::AtomicLock atomic;
static yf::SpinLock spin;
static yf::ReadWriteLock rwlock;
static yf::CondinitVar cond;
static uint32_t count = 0;
bool isCount(const uint32_t& num) {
    return num < 10;
}

int main() {
    std::thread t1([]{
        yf::MutexLock::Lock lock(mutex);
        // while (count < 10) {
        // // while (true) {
        //     // yf::MutexLock::Lock lock(mutex);
        //     // yf::AtomicLock::Lock lock(atomic);
        //     // yf::SpinLock::Lock lock(spin);
        //     // yf::ReadWriteLock::ReadLock lock(rwlock);
        //     // yf::ReadWriteLock::WriteLock lock(rwlock);
        //     std::cout << "wait begin";
        //     sleep(1);
        //     // s.wait();
        //     std::cout << "----wait end\n";
        //     cond.wait(lock);
        // }
        // cond.wait(lock, []{
        //     std::cout << "wait\n";
        //     return count < 10;
        // });
        // s.wait([]{
        //     std::cout << "wait\n";
        //     return count < 10;
        // });
        cond.wait(lock, std::bind(isCount, 10));
        // s.wait(isCount, count);
        std::cout << "t1 over~\n";
    });
    for (int i = 0; i < 10; ++i) {
        ++count;
        usleep(1000 * 100);
    }
    // while (true) {
    //     // yf::MutexLock::Lock lock(mutex);
    //     // yf::AtomicLock::Lock lock(atomic);
    //     // yf::SpinLock::Lock lock(spin);
    //     // yf::ReadWriteLock::ReadLock lock(rwlock);
    //     // yf::ReadWriteLock::WriteLock lock(rwlock);
    //     std::cout << "<<<main begin>>>";
    //     sleep(1);
    //     std::cout << "<<<main end>>>\n";
    // }
    std::cout << count << " post\n";
    sleep(1);
    // s.post();
    // cond.notify_one();
    cond.notify_all();
    t1.join();
    return 0;
}
