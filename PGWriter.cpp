
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <vector>
#include "PGWriter.h"
#include "MysqlWriter.h"

using namespace std;

class MysqlWriter;

vector<string> split(const string &str,const string &pattern);

PGWriter::PGWriter(const string conString,const string charset) :
    conString(conString),
	charset(charset),
    succRowCount(0),
    errorRowCount(0),
    ThreadId(0)

 {
    //initOracleEnv();
}

PGWriter::~PGWriter() {

    closeDB(); 
}


int PGWriter::connectDB(){

    
    vector<string> dbinfo;
    char pgConnstr[1024];

    dbinfo = split(conString,":");

    //mysql:yansp:yansp:192.168.1.88:3306:yanspdb

    cout << dbinfo[0] <<"," << dbinfo[1] << endl;

    sprintf(pgConnstr,"user=%s password=%s  hostaddr=%s port=%s dbname=%s",dbinfo[1].c_str(),dbinfo[2].c_str(),dbinfo[3].c_str(),dbinfo[4].c_str(),dbinfo[5].c_str());
    pgConn = PQconnectdb(pgConnstr);
    ConnStatusType pgstatus = PQstatus(pgConn);

    cout  << pgConnstr<< endl;
    if (pgstatus == CONNECTION_OK)
    {
        //printf("Connect database success!\n");
        cout << " - INFO: Connect to PostgreSQL database " <<  PQdb(pgConn) << " success ";
        return 0;
    }
    else
    {
        //printf("Connect database fail:%s\n",PQerrorMessage(conn));
        cerr << " - ERROR: Connect to PostgreSQL database fail: " << PQerrorMessage(pgConn) ;
        return 1;
    }


}

void PGWriter::closeDB(){

    PQfinish(pgConn);  

}
    
int PGWriter::runSQL(const string sql){

    PGresult *res = PQexec(pgConn, sql.c_str()); 

    if (PQresultStatus(res) == PGRES_COMMAND_OK) {  

        PQclear(res); 
       // res = PQexec(pgConn, "commit"); 
        succRowCount++;
        return EXIT_SUCCESS;
            
    }  else
    {
        PQclear(res); 
        errorRowCount++;
        cerr << "ROW:" << succRowCount + errorRowCount << ",insert error :" << PQerrorMessage(pgConn) << ",sql:" << sql << endl;
        return EXIT_FAILURE;
        
    }
    
     
    
}
    