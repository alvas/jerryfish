/*
 * test1.cpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

//#include "test1.h"
#include "../unittest-cpp/UnitTest++/UnitTest++.h"

#include <unistd.h> 
#include <iostream>
#include <thread>

#include "../process.hpp"
#include "../task.hpp"
#include "../task_timer.hpp"
#include "../buffer.hpp"
#include "../client2.hpp"
#include "../server2.hpp"

using namespace JerryFish;

static std::string file(".test/test1.json");
static std::string file2("test/test2.json");
static std::string file3("test/test3.json");
static std::string task("test/task.json");
static std::string invalid_task("invalid_task.json");

//static void init(JerryFish::Server& s)
//{
    //pid_t pid = fork();

    //if(pid < 0) [> error <]
    //{
        //exit(1); 
    //}
    //else if (pid == 0) [> child <]
    //{
        //s.Run();
        //exit(0);
    //}
    //else [> parrent <]
    //{
        //sleep(3);
    //}
//}

SUITE(TEST1)
{
    // test different combinations of task
    TEST(TASK)
    {
        std::string err;
        std::string str1;
        json11::Json json1 = json11::Json::parse(str1, err);

        JerryFish::Client c;

        // test all kinds of valid task json
        c.ReadTasks(task);

        for (auto o: c.GetTasks())
        {
            json1 = json11::Json::parse(o, err);
            auto task1 = Task(json1);
            REQUIRE CHECK(task1.IsValid());
        }

        c.ClearTasks();

        // test all kinds of invalid task json
        c.ReadTasks(invalid_task);

        for (auto o: c.GetTasks())
        {
            json1 = json11::Json::parse(o, err);
            auto task1 = Task(json1);
            REQUIRE CHECK(!task1.IsValid());
        }
    }

    TEST(BUFFER)
    {
        std::string err;
        std::string str1 = R"({"name": "aa", "category": "hight", "bufferLife": 300, "dropRate": 0.5})";
        auto json1 = json11::Json::parse(str1, err);
        std::vector<Task*> tasks;
        const int BUFFER_SIZE = 15;
        const int TASK_NUMBER = 20;
        Buffer* buffer = new Buffer(BUFFER_SIZE);

        for (int i = 0; i < TASK_NUMBER; ++i)
        {
            Task* o = new Task(json1);
            tasks.push_back(o);
            buffer->Add(o);

            if (i < BUFFER_SIZE)
            {
                CHECK_EQUAL(buffer->Size(), i + 1);
                CHECK_EQUAL(buffer->AvailableShiftSize(), BUFFER_SIZE - i - 1);
                CHECK_EQUAL(buffer->IsFull(), i == BUFFER_SIZE - 1 ? true : false);
            }
            else
            {
                CHECK_EQUAL(buffer->Size(), BUFFER_SIZE);
                CHECK_EQUAL(buffer->AvailableShiftSize(), 0);
                CHECK_EQUAL(buffer->IsFull(), true);
            }
        }

        Task* extraTask = new Task(json1);
        buffer->Add(extraTask);
        REQUIRE CHECK(!buffer->HasTask(extraTask));
        delete extraTask;

        for (int i = 0; i < TASK_NUMBER; ++i)
        {
            Task* o = tasks[i];
            REQUIRE CHECK(i < BUFFER_SIZE && buffer->HasTask(o) || i >= BUFFER_SIZE && !buffer->HasTask(o));
            buffer->Remove(o);
            delete o;
        }

        CHECK_EQUAL(buffer->Size(), 0);
        CHECK_EQUAL(buffer->AvailableShiftSize(), BUFFER_SIZE);
        REQUIRE CHECK(!buffer->IsFull());

        JerryFish::Client c;
        c.ReadTasks(invalid_task);

        for (auto o: c.GetTasks())
        {
            json1 = json11::Json::parse(o, err);
            auto task = new Task(json1);
            buffer->Add(task);
            REQUIRE CHECK(buffer->Size() == 0);
            delete task;
        }

        delete buffer;
    }

    TEST(PROCESS_1)
    {
    }

    class TestFixture
    {
        public:
            TestFixture() {}
            ~TestFixture() { s.Shutdown(); }
            JerryFish::Server s;
    };

    TEST_FIXTURE(TestFixture, PROCESS_1)
    {
        Process* process = Process::GetInstance();
        REQUIRE CHECK(process != nullptr);
        std::string err;

        auto str = R"({"name": "aa", "category": "low", "bufferLife": 300, "dropRate": 0.77})";
        auto json = json11::Json::parse(str, err);
        auto task = new Task(json);

        process->Add(task);
        REQUIRE CHECK(process->Size() == 1);
        process->Delete(task);
        REQUIRE CHECK(process->Size() == 0);

        // adding the task which has been deleted
        process->Add(task);
        REQUIRE CHECK(process->Size() == 0);

        task = new Task(json);
        process->Add(task);
        REQUIRE CHECK(process->Size() == 1);
        process->Deliver(task);
        REQUIRE CHECK(process->Size() == 0);

        Process::Destroy();
    }

    TEST_FIXTURE(TestFixture, PROCESS_2)
    {
        std::string err;
        auto process = Process::GetInstance();
        REQUIRE CHECK(process != nullptr);
        JerryFish::Client c;

        c.ReadTasks(file);
        auto tasks = c.GetTasks();
        int num = tasks.size();

        for (int i = 0; i < num; ++i)
        {
            auto json = json11::Json::parse(tasks[i], err);
            auto task = new JerryFish::Task(json);
            process->Add(task);

            if (i < process->GetCapacity())
            {
                CHECK_EQUAL(process->Size(), i + 1);
            }
            else
            {
                CHECK_EQUAL(process->Size(), process->GetCapacity());
            }
        }

        c.ClearTasks();
        Process::Destroy();
    }

    TEST_FIXTURE(TestFixture, PROCESS_3)
    {
        std::string err;
        auto process = Process::GetInstance();
        REQUIRE CHECK(process != nullptr);
        JerryFish::Client c;

        c.ReadTasks(invalid_task);
        auto tasks = c.GetTasks();

        for (auto o: c.GetTasks())
        {
            auto json = json11::Json::parse(o, err);
            auto task = new JerryFish::Task(json);
            process->Add(task);
            REQUIRE CHECK(process->Size() == 0);
            delete task;
        }

        c.ClearTasks();
        Process::Destroy();
    }

    // no task timeout, no overflow buffer task shift
    TEST_FIXTURE(TestFixture, PROCESS_4)
    {
        std::string err;
        auto process = Process::GetInstance();
        REQUIRE CHECK(process != nullptr);

        JerryFish::Client c;
        c.ReadTasks(file2);
        int idx = 0;
        auto tasks = c.GetTasks();

        for (auto o: tasks)
        {
            auto json = json11::Json::parse(o, err);
            auto task = new JerryFish::Task(json);
            process->Add(task);

            if (idx == 14)
            {
                REQUIRE CHECK(process->Size() == 15);
            }
            else if (idx == 29)
            {
                REQUIRE CHECK(process->Size() == 30);
            }
            else if (idx == 44)
            {
                REQUIRE CHECK(process->Size() == 45);
            }
            else if (idx == 64)
            {
                REQUIRE CHECK(process->Size() == 65);
            }
            else if (idx == 68)
            {
                REQUIRE CHECK(process->Size() == 65);
            }

            idx++;
        }

        c.ClearTasks();
        Process::Destroy();
    }

    TEST_FIXTURE(TestFixture, PROCESS_5)
    {
        std::string err;
        auto process = Process::GetInstance();
        REQUIRE CHECK(process != nullptr);
        JerryFish::Client c;

        std::thread cleanUpInvalidTask(JerryFish::TaskTimer(), 1);
        cleanUpInvalidTask.detach();

        c.ReadTasks(file3);
        auto tasks = c.GetTasks();

        for (auto o: tasks)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            auto json = json11::Json::parse(o, err);
            auto task = new JerryFish::Task(json);
            process->Add(task);
        }

        REQUIRE CHECK(process->Size() == 63);
        c.ClearTasks();
        Process::Destroy();
    }
}

