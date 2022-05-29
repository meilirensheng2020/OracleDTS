

#include <iostream>
#include "DBTable.h"
#include "ocilib.hpp"
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace ocilib;



DBTable::DBTable(const string constr, const string username, const string password,const string tabStr) :
    db_constr(constr),
    db_user(username),
    db_password(password),
    tabStr(tabStr) {
        boost::split(tmpObjects, tabStr,boost::is_any_of(","), boost::token_compress_on);
        initTables();
}

DBTable::~DBTable() {
}


void DBTable::printTables(){
    for(size_t i=0; i< tmpObjects.size();i++) {
        cout << tmpObjects[i] << endl;
    }
}

void DBTable::initTables(){
        
    try
    {
        //Environment::Initialize();

        Connection con(db_constr, db_user, db_password);
        Statement st(con);

        for(size_t i=0; i< tmpObjects.size();i++) {

            string tmpName = tmpObjects[i];
            string sqltxt="SELECT TABLE_NAME||':'||PARTITION_NAME  \
                    FROM ALL_TAB_PARTITIONS   \
                    WHERE TABLE_NAME='" + tmpName + "'";

            st.Execute(sqltxt);
            Resultset rs = st.GetResultset();
            if (rs++)
            {
                tabPartitions.push_back(rs.Get<ostring>(1));  //partition tables
                while(rs++) {
                    tabPartitions.push_back(rs.Get<ostring>(1));  //partition tables
                    //cout <<  rs.Get<ostring>(1) << endl;
                }
            }   else
            {
                tables.push_back(tmpName);
                //cout << tmpName << endl;
            }
            
            
   
        }


    }
    catch (std::exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }

   // Environment::Cleanup();
}


