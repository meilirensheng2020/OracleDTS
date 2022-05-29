
#include <condition_variable>
#include <mutex>
#include <map>
#include <string>
#include <pthread.h>
#include "types.h"
#include "DBThread.h"
#include "DBTable.h"
#include "ocilib.hpp"
#include "MysqlWriter.h"
#include "PGWriter.h"
#include "OracleWriter.h"

#ifndef ORACLEREADER_H_
#define ORACLEREADER_H_

using namespace std;
using namespace ocilib;


    class OracleReader : public DBThread {

    private:
    	Connection con;
    	Statement st;
        map<int, string> columnMap;
        map<int, string> columnNames;
        uint64_t totalRows;
        void getColumnInfo(const string tabname,Connection con,const uint8_t objType);

    public:
        string tableName;
        DBTable *dbtable;
        uint32_t threadID;
        MysqlWriter *mysqlWriter;
        PGWriter *pgWriter;
        OracleWriter *oracleWriter;
        uint32_t commitNum;
        int  dbtype; // 1-oracle 2-mysql -3-postgresql 
        OracleReader(DBTable *dbtable, const uint32_t threadID,void  *dbWriter,const uint32_t commitNum,const int dbtype);
        ~OracleReader();
        void *run(void);
        int initOracleEnv();
        void initTargetDBEnv();
        void databaseWrite(const string tabname,const string partitionName,Connection con,Statement st);

    };

#endif
