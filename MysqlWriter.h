
#include <stdio.h>
#include <stdlib.h>
#include <condition_variable>
#include <mutex>
#include <map>
#include <string>
#include "types.h"
#include "DBTable.h"
#include "/databackup/software_tools/mysql5.7/include/mysql.h"



#ifndef MYSQLWRITER_H_
#define MYSQLWRITER_H_

using namespace std;


    class MysqlWriter {

    private:
        MYSQL mysqlCon;
       //DBTable *dbtable;
        string conString;
		string charset;

    public:
        uint64_t succRowCount;
        uint64_t errorRowCount;
        uint32_t ThreadId;
        MysqlWriter(const string conString,const string charset);
        ~MysqlWriter();
        int runSQL(const string sql);
        int connectDB();
        void closeDB();

    };

#endif
