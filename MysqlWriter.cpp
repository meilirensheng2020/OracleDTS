

#include <iostream>
#include <string.h>
#include <unistd.h>
#include "MysqlWriter.h"

using namespace std;


MysqlWriter::MysqlWriter(const string conString,const string charset) :
    conString(conString),
	charset(charset),
    succRowCount(0),
    errorRowCount(0),
    ThreadId(0)

 {
    //initOracleEnv();
}

MysqlWriter::~MysqlWriter() {

    closeDB(); 
}


vector<string> split(const string &str,const string &pattern)
{
    //const char* convert to char*
    char * strc = new char[strlen(str.c_str())+1];
    strcpy(strc, str.c_str());
    vector<string> resultVec;
    char* tmpStr = strtok(strc, pattern.c_str());
    while (tmpStr != NULL)
    {
        resultVec.push_back(string(tmpStr));
        tmpStr = strtok(NULL, pattern.c_str());
    }

    delete[] strc;

    return resultVec;
}


int MysqlWriter::connectDB(){

    int ret;

    vector<string> dbinfo;
    dbinfo = split(conString,":");

    cout << dbinfo[0] <<"," << dbinfo[1] << endl;

    mysql_init(&mysqlCon);

    if(mysql_real_connect(&mysqlCon,dbinfo[3].c_str(),dbinfo[1].c_str(),dbinfo[2].c_str(),dbinfo[5].c_str(),stoi(dbinfo[4]),NULL,CLIENT_FOUND_ROWS)){

            cout << "Thread :" << ThreadId <<", connect to mysql server successfully." << endl;
            ret=mysql_query(&mysqlCon,"set autocommit 0");

			string cmd = "set names utf8";
			if (charset.size() > 0 )
				cmd = "set names " + charset; 
            if (charset.size() > 0 )
              ret = mysql_set_character_set(&mysqlCon, charset.c_str());
           // ret=mysql_query(&mysqlCon,cmd.c_str());
            cout << "charset=" << charset << ",ret = " << ret << endl;
    }
        
    else {
           
        fprintf(stderr,"Connection mysql server failed: %s\n",mysql_error(&mysqlCon));
    }

    return ret;

}

void MysqlWriter::closeDB(){

    mysql_close(&mysqlCon);

}
    
int MysqlWriter::runSQL(const string sql){
    int ret;
    ret=mysql_query(&mysqlCon,sql.c_str());
    
    if(!ret){

      mysql_query(&mysqlCon,"commit");
      succRowCount++;
      return EXIT_SUCCESS;
    }	
    else{
        errorRowCount++;
        fprintf(stderr,"ROW:%10lu,Insert error:%d:%s.\n",succRowCount + errorRowCount,mysql_errno(&mysqlCon),mysql_error(&mysqlCon));
        cerr << sql << endl;
        return EXIT_FAILURE;
    }
    
}
    