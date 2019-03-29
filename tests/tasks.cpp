#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE test suite tasks
#include <boost/test/unit_test.hpp>

#include "runtime/Task.hpp"
#include "runtime/AsyncTask.hpp"
#include "runtime/LoopTask.hpp"

using namespace runtime;

class TaskClass : public TaskContext {
    public:
        bool done = false;
        void doTask(){
            BOOST_TEST_MESSAGE("TaskClass::doTask");
            this->done = true;
        }
};

BOOST_AUTO_TEST_CASE(task_test){

    bool runnedby = false;

    Task t([&runnedby](TaskContext * const ctx){
        runnedby = true;
    });

    BOOST_REQUIRE(!runnedby);

    t.run();

    BOOST_REQUIRE(runnedby);
}

BOOST_AUTO_TEST_CASE(task_test_with_context){

    TaskClass tc;

    Task t(&tc, [](TaskContext * const ctx){
        ((TaskClass *) ctx)->doTask();
    });

    BOOST_REQUIRE(!tc.done);

    t.run();

    BOOST_REQUIRE(tc.done);
}

BOOST_AUTO_TEST_CASE(async_task_test){

    bool runnedby = false;

    AsyncTask t([&runnedby](TaskContext * const ctx){
        runnedby = true;
    });

    BOOST_REQUIRE(!runnedby);

    t.run();
    t.join();

    BOOST_REQUIRE(runnedby);
}

BOOST_AUTO_TEST_CASE(async_task_count_test){

    int count = 0;

    AsyncTask t([&count](TaskContext * const ctx){
        while(count < 10){
            count++;
            std::this_thread::sleep_for(std::chrono::microseconds(1000));
        }
    });

    t.run();

    BOOST_TEST_MESSAGE("waiting for async task...");
    while(count < 10);

    t.join();
    BOOST_REQUIRE(count >= 10);
}

BOOST_AUTO_TEST_CASE(loop_task_count_test){

    int count = 0;

    LoopTask t([&count](TaskContext * const ctx){
        count++;
        std::this_thread::sleep_for(std::chrono::microseconds(1000));
    });

    t.run();

    BOOST_TEST_MESSAGE("waiting for loop task...");
    while(count < 10);

    t.stop(true);
    BOOST_REQUIRE(count >= 10);
}