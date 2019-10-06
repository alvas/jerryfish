/*
 * test_thread.cc
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "../src/JerryFish.h"

JerryFish::Logger::ptr g_logger = JERRYFISH_LOG_ROOT();

int count = 0;
//JerryFish::RWMutex s_mutex;
JerryFish::Mutex s_mutex;

void fun1() {
    JERRYFISH_LOG_INFO(g_logger) << "name: " << JerryFish::Thread::GetName()
                             << " this.name: " << JerryFish::Thread::GetThis()->getName()
                             << " id: " << JerryFish::GetThreadId()
                             << " this.id: " << JerryFish::Thread::GetThis()->getId();

    for(int i = 0; i < 100000; ++i) {
        //JerryFish::RWMutex::WriteLock lock(s_mutex);
        JerryFish::Mutex::Lock lock(s_mutex);
        ++count;
    }
}

void fun2() {
    while(true) {
        JERRYFISH_LOG_INFO(g_logger) << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    }
}

void fun3() {
    while(true) {
        JERRYFISH_LOG_INFO(g_logger) << "========================================";
    }
}

int main(int argc, char** argv) {
    JERRYFISH_LOG_INFO(g_logger) << "thread test begin";
    YAML::Node root = YAML::LoadFile("../bin/conf/log2.yml");
    JerryFish::Config::LoadFromYaml(root);

    std::vector<JerryFish::Thread::ptr> thrs;
    std::cout << "start creating trhead" << std::endl;

    for(int i = 0; i < 1; ++i) {
        std::cout << "creating " << i << " thread" << std::endl;
        JerryFish::Thread::ptr thr(new JerryFish::Thread(&fun2, "name_" + std::to_string(i * 2)));
        //JerryFish::Thread::ptr thr2(new JerryFish::Thread(&fun3, "name_" + std::to_string(i * 2 + 1)));
        std::cout << "after creating " << i << " thread" << std::endl;
        thrs.push_back(thr);
        //thrs.push_back(thr2);
    }

    for(size_t i = 0; i < thrs.size(); ++i) {
        thrs[i]->join();
    }

    JERRYFISH_LOG_INFO(g_logger) << "thread test end";
    JERRYFISH_LOG_INFO(g_logger) << "count=" << count;

    return 0;
}
