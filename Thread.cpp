

#include <iostream>
#include "Thread.h"

using namespace std;



    Thread::Thread(const string alias) :
        shutdown(false),
        pthread(0),
        alias(alias)
             {
    }

    Thread::~Thread() {
    }

    void *Thread::runStatic(void *context){
        void *ret = ((Thread *) context)->run();
        return ret;
    }

    void Thread::stop(void) {
        shutdown = true;
    }

