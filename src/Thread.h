/*
 * Thread.h
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef THREAD_H
#define THREAD_H

#include "Mutex.h"
#include "Logger.h"
#include "Utils.h"

namespace JerryFish 
{
    /**
     * @brief 线程类
     */
    class Thread : Noncopyable 
    {
        public:
            /// 线程智能指针类型
            typedef std::shared_ptr<Thread> ptr;
            
            JerryFish::Logger::ptr getLogger()
            {
                static JerryFish::Logger::ptr g_logger = JERRYFISH_LOG_NAME("system");
                return g_logger;
            }

            static void *startThread(void* arg)
            {
                //Thread* thread = reinterpret_cast<Thread*>(arg);
                //thread->run(arg);
                reinterpret_cast<Thread*>(arg)->run(arg);
                return 0;
            }

            /**
             * @brief 构造函数
             * @param[in] cb 线程执行函数
             * @param[in] name 线程名称
             */
            Thread(std::function<void()> cb, const std::string& name) :m_cb(cb) ,m_name(name) 
            {
                std::cout << "start pthread_create" << std::endl;

                if(name.empty()) 
                {
                    m_name = "UNKNOW";
                }

                //int rt = pthread_create(&m_thread, nullptr, &Thread::run, this);
                int rt = pthread_create(&m_thread, nullptr, startThread, this);

                if(rt) 
                {
                    JERRYFISH_LOG_ERROR(getLogger()) << "pthread_create thread fail, rt=" << rt
                        << " name=" << name;
                    throw std::logic_error("pthread_create error");
                }

                m_semaphore.wait();
            }

            /**
             * @brief 析构函数
             */
            ~Thread() 
            {
                if(m_thread) 
                {
                    pthread_detach(m_thread);
                }
            }

            /**
             * @brief 线程ID
             */
            pid_t getId() const { return m_id;}

            /**
             * @brief 线程名称
             */
            const std::string& getName() const { return m_name;}

            /**
             * @brief 等待线程执行完成
             */
            void join() 
            {
                if(m_thread) 
                {
                    int rt = pthread_join(m_thread, nullptr);
                    if(rt) 
                    {
                        JERRYFISH_LOG_ERROR(getLogger()) << "pthread_join thread fail, rt=" << rt
                            << " name=" << m_name;
                        throw std::logic_error("pthread_join error");
                    }

                    m_thread = 0;
                }
            }

            /**
             * @brief 获取当前的线程指针
             */
            static Thread* GetThis()
            {
                static thread_local Thread* t_thread = nullptr;
                return t_thread;
            }

            /**
             * @brief 获取当前的线程名称
             */
            static std::string& GetName()
            {
                static thread_local std::string t_thread_name = "UNKNOW";
                return t_thread_name;
            }

            /**
             * @brief 设置当前线程名称
             * @param[in] name 线程名称
             */
            void SetName(const std::string& name) 
            {
                if(name.empty()) 
                {
                    return;
                }

                Thread* t_thread = GetThis();

                if(t_thread) 
                {
                    t_thread->m_name = name;
                }

                std::string& t_thread_name = GetName();
                t_thread_name = name;
            }

        private:
            /**
             * @brief 线程执行函数
             */
            void* run(void* arg) 
            {
                Thread* thread = (Thread*)arg;
                Thread* t_thread = GetThis();
                t_thread = thread;
                std::string& t_thread_name = GetName();
                t_thread_name = thread->m_name;
                thread->m_id = JerryFish::GetThreadId();
                //pthread_setname_np(pthread_self(), thread->m_name.substr(0, 15).c_str()); 
                pthread_setname_np(thread->m_name.substr(0, 15).c_str()); 
                std::function<void()> cb;
                cb.swap(thread->m_cb);

                thread->m_semaphore.notify();

                cb();
                return 0;
            }

        private:
            /// 线程id
            pid_t m_id = -1;
            /// 线程结构
            pthread_t m_thread = 0;
            /// 线程执行函数
            std::function<void()> m_cb;
            /// 线程名称
            std::string m_name;
            /// 信号量
            Semaphore m_semaphore;
    };
}

#endif /* !THREAD_H */
