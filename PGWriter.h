
#include <condition_variable>
#include <mutex>
#include <map>
#include <string>
#include "types.h"
#include <libpq-fe.h>  

#ifndef PGWRITER_H_
#define PGWRITER_H_

using namespace std;


    class PGWriter {

    private:
        PGconn *pgConn;
        string conString;
		string charset;

    public:
        uint64_t succRowCount;
        uint64_t errorRowCount;
        uint32_t ThreadId;
        PGWriter(const string conString,const string charset);
        ~PGWriter();
        int runSQL(const string sql);
        int connectDB();
        void closeDB();

    };

#endif
