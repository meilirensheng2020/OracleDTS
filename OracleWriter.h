

#include <iostream>
#include <string>
#include "DBTable.h"
#include "ocilib.hpp"



#ifndef ORACLEWRITER_H_
#define ORACLEWRITER_H_

using namespace std;
using namespace ocilib;


    class OracleWriter {

    private:
    	Connection con;
    	Statement st;
        string conString;
		string charset;

    public:
        uint64_t succRowCount;
        uint64_t errorRowCount;
        uint32_t ThreadId;
        OracleWriter(const string conString,const string charset);
        ~OracleWriter();
        int runSQL(const string sql);
        int connectDB();
        void closeDB();

    };

#endif
