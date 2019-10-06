/*
 * Mutex.h
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef MUTEX_H
#define MUTEX_H

#include <thread>
#include <functional>
#include <memory>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <atomic>
#include <list>
#include <pthread.h>

#include "macro.h"
#include "noncopyable.h"
//#include "Fiber.h"

namespace JerryFish 
{

#ifdef __APPLE__
    typedef int pthread_spinlock_t;

    int pthread_spin_init(pthread_spinlock_t *lock, int pshared) {
        __asm__ __volatile__ ("" ::: "memory");
        *lock = 0;
        return 0;
    }

    int pthread_spin_destroy(pthread_spinlock_t *lock) {
        return 0;
    }

    int pthread_spin_lock(pthread_spinlock_t *lock) {
        while (1) {
            int i;
            for (i=0; i < 10000; i++) {
                if (__sync_bool_compare_and_swap(lock, 0, 1)) {
                    return 0;
                }
            }
            sched_yield();
        }
    }

    int pthread_spin_trylock(pthread_spinlock_t *lock) {
        if (__sync_bool_compare_and_swap(lock, 0, 1)) {
            return 0;
        }
        return EBUSY;
    }

    int pthread_spin_unlock(pthread_spinlock_t *lock) {
        __asm__ __volatile__ ("" ::: "memory");
        *lock = 0;
        return 0;
    }
#endif

    /**
     * @brief 信号量
     */
    class Semaphore : Noncopyable 
    {
        public:
            /**
             * @brief 构造函数
             * @param[in] count 信号量值的大小
             */
            Semaphore(uint32_t count = 0)
            {
#ifdef __APPLE__
                m_semaphore = sem_open(std::to_string(count).c_str(), O_CREAT|O_EXCL, S_IRWXU, count);
#else

                sem_init(m_semaphore, 0, count);
#endif

                if(m_semaphore == SEM_FAILED)
                {
                    throw std::logic_error("sem_init error");
                }
            }

            /**
             * @brief 析构函数
             */
            ~Semaphore()
            {
#ifdef __APPLE__
                sem_close(m_semaphore);
#else
                sem_destroy(m_semaphore);
#endif
            }

            /**
             * @brief 获取信号量
             */
            void wait()
            {
                if(sem_wait(m_semaphore)) {
                    throw std::logic_error("sem_wait error");
                }
            }

            /**
             * @brief 释放信号量
             */
            void notify()
            {
                if(sem_post(m_semaphore)) {
                    throw std::logic_error("sem_post error");
                }
            }        

        private:
            sem_t *m_semaphore;
    };

    /**
     * @brief 局部锁的模板实现
     */
    template<class T>
        struct ScopedLockImpl {
            public:
                /**
                 * @brief 构造函数
                 * @param[in] mutex Mutex
                 */
                ScopedLockImpl(T& mutex)
                    :m_mutex(mutex) {
                        m_mutex.lock();
                        m_locked = true;
                    }

                /**
                 * @brief 析构函数,自动释放锁
                 */
                ~ScopedLockImpl() {
                    unlock();
                }

                /**
                 * @brief 加锁
                 */
                void lock() {
                    if(!m_locked) {
                        m_mutex.lock();
                        m_locked = true;
                    }
                }

                /**
                 * @brief 解锁
                 */
                void unlock() {
                    if(m_locked) {
                        m_mutex.unlock();
                        m_locked = false;
                    }
                }
            private:
                /// mutex
                T& m_mutex;
                /// 是否已上锁
                bool m_locked;
        };

    /**
     * @brief 局部读锁模板实现
     */
    template<class T>
        struct ReadScopedLockImpl {
            public:
                /**
                 * @brief 构造函数
                 * @param[in] mutex 读写锁
                 */
                ReadScopedLockImpl(T& mutex)
                    :m_mutex(mutex) {
                        m_mutex.rdlock();
                        m_locked = true;
                    }

                /**
                 * @brief 析构函数,自动释放锁
                 */
                ~ReadScopedLockImpl() {
                    unlock();
                }

                /**
                 * @brief 上读锁
                 */
                void lock() {
                    if(!m_locked) {
                        m_mutex.rdlock();
                        m_locked = true;
                    }
                }

                /**
                 * @brief 释放锁
                 */
                void unlock() {
                    if(m_locked) {
                        m_mutex.unlock();
                        m_locked = false;
                    }
                }
            private:
                /// mutex
                T& m_mutex;
                /// 是否已上锁
                bool m_locked;
        };

    /**
     * @brief 局部写锁模板实现
     */
    template<class T>
        struct WriteScopedLockImpl {
            public:
                /**
                 * @brief 构造函数
                 * @param[in] mutex 读写锁
                 */
                WriteScopedLockImpl(T& mutex)
                    :m_mutex(mutex) {
                        m_mutex.wrlock();
                        m_locked = true;
                    }

                /**
                 * @brief 析构函数
                 */
                ~WriteScopedLockImpl() {
                    unlock();
                }

                /**
                 * @brief 上写锁
                 */
                void lock() {
                    if(!m_locked) {
                        m_mutex.wrlock();
                        m_locked = true;
                    }
                }

                /**
                 * @brief 解锁
                 */
                void unlock() {
                    if(m_locked) {
                        m_mutex.unlock();
                        m_locked = false;
                    }
                }
            private:
                /// Mutex
                T& m_mutex;
                /// 是否已上锁
                bool m_locked;
        };

    /**
     * @brief 互斥量
     */
    class Mutex : Noncopyable {
        public: 
            /// 局部锁
            typedef ScopedLockImpl<Mutex> Lock;

            /**
             * @brief 构造函数
             */
            Mutex() {
                pthread_mutex_init(&m_mutex, nullptr);
            }

            /**
             * @brief 析构函数
             */
            ~Mutex() {
                pthread_mutex_destroy(&m_mutex);
            }

            /**
             * @brief 加锁
             */
            void lock() {
                pthread_mutex_lock(&m_mutex);
            }

            /**
             * @brief 解锁
             */
            void unlock() {
                pthread_mutex_unlock(&m_mutex);
            }
        private:
            /// mutex
            pthread_mutex_t m_mutex;
    };

    /**
     * @brief 空锁(用于调试)
     */
    class NullMutex : Noncopyable{
        public:
            /// 局部锁
            typedef ScopedLockImpl<NullMutex> Lock;

            /**
             * @brief 构造函数
             */
            NullMutex() {}

            /**
             * @brief 析构函数
             */
            ~NullMutex() {}

            /**
             * @brief 加锁
             */
            void lock() {}

            /**
             * @brief 解锁
             */
            void unlock() {}
    };

    /**
     * @brief 读写互斥量
     */
    class RWMutex : Noncopyable{
        public:

            /// 局部读锁
            typedef ReadScopedLockImpl<RWMutex> ReadLock;

            /// 局部写锁
            typedef WriteScopedLockImpl<RWMutex> WriteLock;

            /**
             * @brief 构造函数
             */
            RWMutex() {
                pthread_rwlock_init(&m_lock, nullptr);
            }

            /**
             * @brief 析构函数
             */
            ~RWMutex() {
                pthread_rwlock_destroy(&m_lock);
            }

            /**
             * @brief 上读锁
             */
            void rdlock() {
                pthread_rwlock_rdlock(&m_lock);
            }

            /**
             * @brief 上写锁
             */
            void wrlock() {
                pthread_rwlock_wrlock(&m_lock);
            }

            /**
             * @brief 解锁
             */
            void unlock() {
                pthread_rwlock_unlock(&m_lock);
            }
        private:
            /// 读写锁
            pthread_rwlock_t m_lock;
    };

    /**
     * @brief 空读写锁(用于调试)
     */
    class NullRWMutex : Noncopyable {
        public:
            /// 局部读锁
            typedef ReadScopedLockImpl<NullMutex> ReadLock;
            /// 局部写锁
            typedef WriteScopedLockImpl<NullMutex> WriteLock;

            /**
             * @brief 构造函数
             */
            NullRWMutex() {}
            /**
             * @brief 析构函数
             */
            ~NullRWMutex() {}

            /**
             * @brief 上读锁
             */
            void rdlock() {}

            /**
             * @brief 上写锁
             */
            void wrlock() {}
            /**
             * @brief 解锁
             */
            void unlock() {}
    };

    /**
     * @brief 自旋锁
     */
    class Spinlock : Noncopyable {
        public:
            /// 局部锁
            typedef ScopedLockImpl<Spinlock> Lock;

            /**
             * @brief 构造函数
             */
            Spinlock() {
                pthread_spin_init(&m_mutex, 0);
            }

            /**
             * @brief 析构函数
             */
            ~Spinlock() {
                pthread_spin_destroy(&m_mutex);
            }

            /**
             * @brief 上锁
             */
            void lock() {
                pthread_spin_lock(&m_mutex);
            }

            /**
             * @brief 解锁
             */
            void unlock() {
                pthread_spin_unlock(&m_mutex);
            }
        private:
            /// 自旋锁
            pthread_spinlock_t m_mutex;
    };

    /**
     * @brief 原子锁
     */
    class CASLock : Noncopyable {
        public:
            /// 局部锁
            typedef ScopedLockImpl<CASLock> Lock;

            /**
             * @brief 构造函数
             */
            CASLock() {
                m_mutex.clear();
            }

            /**
             * @brief 析构函数
             */
            ~CASLock() {
            }

            /**
             * @brief 上锁
             */
            void lock() {
                while(std::atomic_flag_test_and_set_explicit(&m_mutex, std::memory_order_acquire));
            }

            /**
             * @brief 解锁
             */
            void unlock() {
                std::atomic_flag_clear_explicit(&m_mutex, std::memory_order_release);
            }
        private:
            /// 原子状态
            volatile std::atomic_flag m_mutex;
    };

    class Scheduler;

    //class FiberSemaphore : Noncopyable {
        //public:
            //typedef Spinlock MutexType;

            //FiberSemaphore(size_t initial_concurrency = 0){}

            //~FiberSemaphore()
            //{
                //JERRYFISH_ASSERT(m_waiters.empty());
            //}

            //bool tryWait();

            //void wait();

            //void notify();

            //size_t getConcurrency() const { return m_concurrency;}
            //void reset() { m_concurrency = 0;}
        //private:
            //MutexType m_mutex;
            //std::list<std::pair<Scheduler*, Fiber::ptr> > m_waiters;
            //size_t m_concurrency;
    //};
}

#endif /* !MUTEX_H */
