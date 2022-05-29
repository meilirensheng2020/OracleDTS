
/*
Rem $Header: oracleDTS.cpp  $
Rem
Rem oracleDTS.cpp
Rem
Rem Copyright (c) 2019, 2019, YunHuan. All rights reserved.  
Rem
Rem    NAME
Rem      oracleDTS.cpp - main program of oracleDTS
Rem
Rem    DESCRIPTION
Rem      The program oracleDTS used to
Rem      load RDBMS data to KAFKA、file.
Rem
Rem    NOTES
Rem      
Rem    MODIFIED   (YYYY-MM-DD)
Rem    yanshoupeng 2020-12-02 add postgresql support
Rem    yanshoupeng 2020-01-16 add cmd parameters
Rem    yanshoupeng 2019-12-05 created

*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <list>
#include <streambuf>
#include <pthread.h>
//#include "rapidjson/document.h"
#include <string.h>
#include "StreamConf.h"
#include "ocilib.hpp"
#include <boost/algorithm/string.hpp>
#include "OracleReader.h"
#include "DBTable.h"
#include "MysqlWriter.h"
#include "PGWriter.h"
#include "OracleWriter.h"

using namespace std;
using namespace ocilib;
//using namespace rapidjson;
//using namespace OracleDTS;



int main(int argc, char** argv)
{
	  
    list<OracleReader *> readers;
    
	// Output version information
	cout << "OracleDTS v1.0.0 (C) 2019-2021 by yanshoupeng, yanshoupeng@yunhuansoft.com" << endl;
    cout << endl;
    
       
    try {    

        // parameters
        StreamConf *streamConf = new StreamConf(argc,argv);

        string userid   = streamConf->getString("userid");
        string password = streamConf->getString("password");
        string server   = streamConf->getString("server");
        string tabstr   = streamConf->getString("tables");
        uint32_t threads   = streamConf->getInt("threads",1); 
        uint32_t commitNum   = streamConf->getInt("commitNum",1); 
        string target = streamConf->getString("target");

		string charset = streamConf->getString("charset");

        vector<string> targetInfo;
        boost::split(targetInfo, target,boost::is_any_of(":"), boost::token_compress_on);
        string targetDBtype = targetInfo[0];

        cout << "Max Threads:" << threads << ",commitNum " << commitNum << endl  << endl;

        //Environment::Initialize();
        Environment::Initialize(Environment::Threaded);

        DBTable *dbtable = new DBTable(server,userid,password,tabstr);


        
        if (targetDBtype.compare("oracle")==0) {  //oracle数据库
  
            for (int i=1;   i<=threads;  i++) {

                OracleWriter *oracleWriter = new OracleWriter(target,charset);
                OracleReader  *oraReader =  new OracleReader(dbtable,i,oracleWriter,commitNum,1);
    
                if (oraReader != nullptr) 
                    pthread_create(&oraReader->pthread, nullptr, &OracleReader::runStatic, (void*)oraReader);
                readers.push_back(oraReader);
                oraReader = nullptr;
             }
        }
        

        if (targetDBtype.compare("mysql")==0) {  //mysql数据库
  
            for (int i=1;   i<=threads;  i++) {

                MysqlWriter *mysqlWriter = new MysqlWriter(target,charset);
                OracleReader  *oraReader =  new OracleReader(dbtable,i,mysqlWriter,commitNum,2);
                //oraReader->dbtype=2;
    
                if (oraReader != nullptr) 
                    pthread_create(&oraReader->pthread, nullptr, &OracleReader::runStatic, (void*)oraReader);
                readers.push_back(oraReader);
                oraReader = nullptr;
             }
        }

        if (targetDBtype.compare("postgresql")==0) {  //pg数据库
  
            for (int i=1;   i<=threads;  i++) {

                PGWriter *pgWriter = new PGWriter(target,charset);
                OracleReader  *oraReader =  new OracleReader(dbtable,i,pgWriter,commitNum,3);
                //oraReader->dbtype=3;
                //oraReader->pgWriter = pgWriter;
                
                if (oraReader != nullptr) 
                    pthread_create(&oraReader->pthread, nullptr, &OracleReader::runStatic, (void*)oraReader);
                readers.push_back(oraReader);
                oraReader = nullptr;
             }
        }
        

        for (OracleReader * reader : readers) {
            pthread_join(reader->pthread, nullptr);
            delete reader;
        }


        if (dbtable != nullptr) {
                delete dbtable;
                dbtable = nullptr;
        }         
       
    }
    catch (exception &ex)
    {
        cout << ex.what() << endl;
    }

   Environment::Cleanup();
   return EXIT_SUCCESS;
}
