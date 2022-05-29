
#include <condition_variable>
#include <vector>
#include <mutex>
#include <string>

#ifndef DBTable_H_
#define DBTable_H_


using namespace std;


    class DBTable {
    
    private:
      vector<string>  tmpObjects;

    public:

        mutex tabMutex;
        vector<string> tables; // table or view name
        vector<string> tabPartitions; // partition name
        string db_constr;
        string db_user;
        string db_password;       
        string  tabStr;
        DBTable(const string constr, const string username, const string password,const string tabStr);
        void printTables();
        void initTables();
        ~DBTable();

    };

#endif
