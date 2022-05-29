

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <mutex>
#include <sys/syscall.h>
#include "ocilib.hpp"
#include <boost/algorithm/string.hpp>
#include "OracleReader.h"
#include "MysqlWriter.h"
#include "PGWriter.h"
#include "OracleWriter.h"


#define gettid() syscall(SYS_gettid)


using namespace std;
using namespace ocilib;

string  get_objname(const string tname,const string pname)  {

   if (pname.length() > 0)
      return tname+":"+pname;
    return tname;
}


string replaceAll(string srcStr,const string s1,const string s2){
    int pos;
    pos = srcStr.find(s1);
    while(pos != -1){
        srcStr.replace(pos,s1.length(),s2);
        pos = srcStr.find(s1);
    }

    return srcStr;
}


OracleReader::OracleReader(DBTable *dbtable, const uint32_t threadID,void * dbWriter,const uint32_t commitNum,const int dbtype) :
    DBThread("YUNHUAN"),
    totalRows(0),
    dbtable(dbtable),
    threadID(threadID),
    commitNum(commitNum),
    dbtype(dbtype)
 {

    switch (dbtype)
    {
    case 1:
        oracleWriter = (OracleWriter *)dbWriter;
    case 2:
        mysqlWriter = (MysqlWriter *) dbWriter;
        break;
    case 3:
        pgWriter = (PGWriter *) dbWriter;
        break;
    
    default:
        break;
    }

    initOracleEnv();
    initTargetDBEnv();


}

OracleReader::~OracleReader() {

    //mysqlWriter->closeMysql();

}



void *OracleReader::run(void) {

 
   string tmpTabName,tmpPartName; 
   int pos;

    try {

        while (true) {  // non partitions --view

            dbtable->tabMutex.lock();

            if (dbtable->tables.empty()) {
                dbtable->tabMutex.unlock();
                break;
            }
            //fetch table 
            string tmpTabName = dbtable->tables[0];
            dbtable->tables.erase(dbtable->tables.begin());            
            cout <<"PID:" << getpid() << ", TID:" << gettid() << ", ThreadNO:" << threadID <<", table:" << tmpTabName << endl;
            dbtable->tabMutex.unlock();
            usleep(100);
            //objType 1 -view 0 -table 2-partition table
            // EMP, VIEW:VIEW_EMP
            pos = tmpTabName.find(":");
            string tmpStr = tmpTabName.substr(0,pos);

            if ( (pos > -1)  && (tmpStr.compare("VIEW") ==0) ) {//view type

                tmpTabName = tmpTabName.substr(pos + 1);
                getColumnInfo(tmpTabName,con,1);
            } else
            {
                getColumnInfo(tmpTabName,con,0);
            }
            
            databaseWrite(tmpTabName,"",con,st); 
           
        } 

        tmpTabName.clear();
        while (true) {  //partition tables

            dbtable->tabMutex.lock();

            if (dbtable->tabPartitions.empty()) {
                dbtable->tabMutex.unlock();
                break;
            }
            //fetch tableName   
            string tmp_TabPartName = dbtable->tabPartitions[0];
            dbtable->tabPartitions.erase(dbtable->tabPartitions.begin());            
            cout <<"PID:" << getpid() << ", TID:" << gettid() << ", ThreadNO:" << threadID <<", table:" << tmp_TabPartName << endl;
            dbtable->tabMutex.unlock();
            usleep(100);

            //EMP:P01
            int pos = tmp_TabPartName.find(":");
            //string tmpStr = 

            if ( pos > -1){
                tmpPartName = tmp_TabPartName.substr(pos + 1);
                tmpTabName  = tmp_TabPartName.substr(0,pos);
            }
        
            //cout << tmpTabName << ":"<< tmpPartName << endl;
            getColumnInfo(tmpTabName,con,0);
            databaseWrite(tmpTabName,tmpPartName,con,st); 
           
        } 

    }
    catch (std::exception e)
    {
        dbtable->tabMutex.unlock();
        cout << e.what() << endl;

    }
    
    return EXIT_SUCCESS;
}

int OracleReader::initOracleEnv() {
    
    try{      

        con = Connection(dbtable->db_constr,dbtable->db_user,dbtable->db_password);
        
        con.SetFormat(FormatDate,"YYYY-MM-DD HH24:MI:SS");	
        con.SetFormat(FormatTimestamp,"YYYY-MM-DD HH24:MI:SS:FF3");	
        
        st  = Statement(con);

        st.SetFetchSize(200);
        st.SetPrefetchSize(200);       

    }
    catch (exception &ex)
    {
        cout << ex.what() << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void OracleReader::initTargetDBEnv(){


    switch (dbtype)
    {

    case 1:
        oracleWriter->connectDB();
        break;
    case 2:
   
        mysqlWriter->connectDB(); 
        break;
    case 3:
   
        pgWriter->connectDB();
        break;
    
    default:
        break;
    }
    

}


void OracleReader::getColumnInfo(const string tabname,Connection con,const uint8_t objType){

    try
    {
        // objType  0--TABLE 1--VIEW 2--PARTITION TABLE

        //  PARTITION TABLE-- EMP:P01
        //  NORMAL    TABLE-- EMP
        //  VIEW           -- VIEW:SCOTT.EMP

        if (objType==1) {

            TypeInfo table(con, tabname, TypeInfo::View);
            for (int i = 1, n = table.GetColumnCount(); i <= n; i++)
            {
                Column col = table.GetColumn(i);
                columnMap[i] =col.GetSQLType();
                columnNames[i] = col.GetName();
            }
        }
        if (objType==0 || objType==2) 
       
        {
            TypeInfo table(con, tabname, TypeInfo::Table);
            for (int i = 1, n = table.GetColumnCount(); i <= n; i++)
            {
                Column col = table.GetColumn(i);
                columnMap[i] =col.GetSQLType();
                columnNames[i] = col.GetName();
            }

        }

        // tableName = tmpTabname;
        
    }
    catch(const std::exception& e)
    {
        
        std::cerr  <<  "Table/View: " << tabname <<"," <<e.what() << endl;
        exit(1);
    }


}

void OracleReader::databaseWrite(const string tabname,const string partitionName,Connection con,Statement st){
    string sqltxt;
    vector <string> colList;

	try {

        //cout <<". . exporting table"<< setw(30) << setfill(' ') << right << tabname ;

        try
        {
            st.Execute("select current_scn from v$database");
            Resultset rsSCN = st.GetResultset();
            rsSCN++;
            string scn = rsSCN.Get<ostring>(1);
            cout << "- INFO: current scn is :" << scn << endl;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        


        string src_sqltxt;
        src_sqltxt="select tab$.* from " + tabname + " tab$ ";
        if (partitionName.length() > 0) 
            src_sqltxt="select tab$.* from " + tabname + " PARTITION ("  + partitionName + ") tab$ ";
        //cout << src_sqltxt << endl;
        st.Execute(src_sqltxt);
        Resultset rs = st.GetResultset();
        unsigned int numCols=rs.GetColumnCount();

        string sqlHeader = "insert into " + tabname + " values";

        while (rs++) {

            sqltxt.append("(");

            for (size_t i = 1; i<= numCols ;i++) {

                if (rs.IsColumnNull(i)) { //check null columns

                    sqltxt.append("NULL");

                } else
                {
                    string colTypeName = columnMap[i];
                    if (colTypeName.compare("NUMBER")==0){
                        sqltxt.append(rs.Get<ostring>(i));
                    }     
                    else {

                        string colName = columnNames[i];
                        if (colName.compare("GIS")==0) {

                            sqltxt.append("GeomFromText('POINT(");
                            string colValue = rs.Get<ostring>(i);
                            sqltxt.append(boost::algorithm::replace_all_copy(colValue,","," "));
                            sqltxt.append(")')");
                  
                        }  else {
                           
                            sqltxt.append("'");
                            string colValue = rs.Get<ostring>(i);
                            //colValue = replaceAll(colValue,"'","\'");
                            int pos = colValue.find("'");
                            if (pos != -1)
                            colValue = boost::algorithm::replace_all_copy(colValue,"'","''");
                            pos = colValue.find("\\");
                            if (pos != -1)
                                colValue =  boost::algorithm::replace_all_copy(colValue,"\\","\\\\");
                            sqltxt.append(colValue);
                            sqltxt.append("'");
                        }
                    }
                }
                
                sqltxt.append(",");                  

            }

            sqltxt.pop_back(); // delete last ','
            sqltxt.append(")");
            sqltxt.append(",");

            totalRows = rs.GetCurrentRow();

            switch (dbtype)
            {
            case 1:
                sqltxt.pop_back(); // delete last ','
                sqltxt.insert(0,sqlHeader);
                oracleWriter->runSQL(sqltxt);
                if (totalRows%commitNum==0) oracleWriter->runSQL("commit");
                sqltxt.clear();
                break;
            case 2:
                if (totalRows%commitNum==0) {
                    sqltxt.pop_back(); // delete last ','
                    sqltxt.insert(0,sqlHeader);
                    mysqlWriter->runSQL(sqltxt); //batch commit 
                    sqltxt.clear();
                }
                break;

            case 3:
                if (totalRows%commitNum==0) {
                    sqltxt.pop_back(); // delete last ','
                    sqltxt.insert(0,sqlHeader);
                    pgWriter->runSQL(sqltxt); //batch commit//batch commit
                    sqltxt.clear();
                }
              
                break;
            
            default:
                break;
            }
     

            if (totalRows%commitNum==0) {  // every commitNum rows per commit
                
                cout << "ThreadNO:" <<  threadID << ", table:" << get_objname(tabname,partitionName)  << ", " 
                 << totalRows << " rows processed  "<< endl;
            } 


        }

        if (dbtype==1) //oracle
            oracleWriter->runSQL("commit");

        if (sqltxt.length()>0) {

            sqltxt.pop_back(); // delete last ','
            sqltxt.insert(0,sqlHeader);
            switch (dbtype)
            {
            case 2: // mysql 
                mysqlWriter->runSQL(sqltxt);
                break;
            case 3:  //postgresql
                pgWriter->runSQL(sqltxt);
                break;
            
            default:
                break;
            }
            sqltxt.clear();
        }
                    
        cout << "ThreadNO:" <<  threadID << ", table:" << get_objname(tabname,partitionName) << ", " 
            << totalRows << " rows processed(Total) "<< endl;
        //mysqlWriter->errorRowCount << " rows failed." << endl;
        totalRows = 0;//reset totalRows to 0
        
	}
    
	catch (exception &ex)
	{
		cout << ex.what() << endl;
	}

}
