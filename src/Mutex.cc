/*
 * Mutex.cc
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "Mutex.h"
#include "macro.h"
//#include "Scheduler.h"

namespace JerryFish
{
    //bool FiberSemaphore::tryWait()
    //{
        //JERRYFISH_ASSERT(Scheduler::GetThis());
        //{
            //MutexType::Lock lock(m_mutex);
            //if(m_concurrency > 0u) {
                //--m_concurrency;
                //return true;
            //}
            //return false;
        //}
    //}

    //void FiberSemaphore::wait()
    //{
        //JERRYFISH_ASSERT(Scheduler::GetThis());
        //{
            //MutexType::Lock lock(m_mutex);
            //if(m_concurrency > 0u) {
                //--m_concurrency;
                //return;
            //}
            //m_waiters.push_back(std::make_pair(Scheduler::GetThis(), Fiber::GetThis()));
        //}
        //Fiber::YieldToHold();
    //}

    //void FiberSemaphore::notify()
    //{
        //MutexType::Lock lock(m_mutex);
        //if(!m_waiters.empty()) {
            //auto next = m_waiters.front();
            //m_waiters.pop_front();
            //next.first->schedule(next.second);
        //} else {
            //++m_concurrency;
        //}
    //}

}
