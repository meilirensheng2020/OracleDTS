

#include <iostream>
#include <string.h>
#include <unistd.h>
#include "OracleWriter.h"

using namespace std;
using namespace ocilib;

class MysqlWriter;

vector<string> split(const string &str,const string &pattern);

OracleWriter::OracleWriter(const string conString,const string charset) :
    conString(conString),
	charset(charset),
    succRowCount(0),
    errorRowCount(0),
    ThreadId(0)

 {
    //initOracleEnv();
}

OracleWriter::~OracleWriter() {

    //st.Execute("Commit"); 
}



int OracleWriter::connectDB(){

    vector<string> dbinfo;
    dbinfo = split(conString,":");

    //oracle:yansp:yansp:192.168.1.88:3306:yanspdb

    try{      

        string username  = dbinfo[1];
        string password  = dbinfo[2];
        string dbStr     = dbinfo[3]+":"+dbinfo[4]+"/"+dbinfo[5];

        cout << dbStr << endl;

        con = Connection(dbStr,username,password);
        
        con.SetFormat(FormatDate,"YYYY-MM-DD HH24:MI:SS");	
        con.SetFormat(FormatTimestamp,"YYYY-MM-DD HH24:MI:SS:FF3");	
        
        st  = Statement(con);

        cout << "Thread :" << ThreadId <<", connect to oracle server successfully." << endl;

    }
    catch (exception &ex)
    {
        cout << ex.what() << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;

}

void OracleWriter::closeDB(){

   // st.Execute("Commit"); 

}
    
int OracleWriter::runSQL(const string sql){
    try
    {
        st.Execute(sql);
        succRowCount++;
        return EXIT_SUCCESS;

    }
    catch(const std::exception& e)
    {
        //std::cerr << e.what() << '\n';
        errorRowCount++;
        cerr << "ROW:" << succRowCount + errorRowCount << ",Insert error:" << e.what() << endl;
        cerr << sql << endl;
        return EXIT_FAILURE;

    }
    
}
    