

#include <string>
#include <pthread.h>
#include "types.h"

#ifndef THREAD_H_
#define THREAD_H_

using namespace std;



class Thread {
public:
        volatile bool shutdown;
        pthread_t pthread;
        string alias;

        static void *runStatic(void *context);

        virtual void *run(void) = 0;
        virtual void stop(void);
        uint64_t initialize(void);

        Thread(const string alias);
        virtual ~Thread();
    };


#endif
