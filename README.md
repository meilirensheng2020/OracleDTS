# OracleDTS
OracleDTS是一款全量或者部分的数据传输工具，主要将Oracle数据写入MySQL或者Postgresql。后续将会引入写文件和KAFKA。

主要命令行参数：

[yansp@localserver oracleDTS]$ ./OracleDTS 
OracleDTS v1.0.0 (C) 2019-2021 by yanshoupeng, yanshoupeng@yunhuansoft.com

Supported Options:
  -h [ --help ]         show help message
  
  --userid arg          userid for source database
  
  --password arg        password for source database
  
  --server arg          connection string for source database(tnsname)
  
  --charset arg         utf8|gbk
  
  --target arg          target database info
  
  --tables arg          tables to migrate
  
  --commitNum arg       number rows of every commit
  
  --threads arg         thread number
  
  
  
  
  
 使用方式：

[yansp@localserver oracleDTS]$ ./OracleDTS --userid=dbmgr --password="yanshoupeng" --server=192.168.1.88/yunhuandb  --tables="VIEW:VIEW_TT01,TT01" --threads=1  --commitNum=2000 --target="mysql:yansp:yansp:192.168.1.88:3306:yanspdb"
OracleDTS v1.0.0 (C) 2019-2021 by yanshoupeng, yanshoupeng@yunhuansoft.com

Max Threads:1,commitNum 2000


Thread :0, connect to mysql server successfully.

PID:14279, TID:14286, ThreadNO:1, table:VIEW:VIEW_TT01
- INFO: current scn is :379637370
ROW:         1,Insert error:1146:Table 'yanspdb.view_tt01' doesn't exist.
insert into VIEW_TT01 values(2024,'V$PROCESS_MEMORY')
ThreadNO:1, table:VIEW_TT01, 1 rows processed(Total) 
PID:14279, TID:14286, ThreadNO:1, table:TT01
- INFO: current scn is :379637371
ThreadNO:1, table:TT01, 2000 rows processed  
ThreadNO:1, table:TT01, 4000 rows processed  
ThreadNO:1, table:TT01, 6000 rows processed  
ThreadNO:1, table:TT01, 8000 rows processed  
ThreadNO:1, table:TT01, 10000 rows processed  
ThreadNO:1, table:TT01, 12000 rows processed  
ThreadNO:1, table:TT01, 14000 rows processed  
ThreadNO:1, table:TT01, 16000 rows processed  
ThreadNO:1, table:TT01, 18000 rows processed  
ThreadNO:1, table:TT01, 20000 rows processed  
ThreadNO:1, table:TT01, 22000 rows processed  
ThreadNO:1, table:TT01, 24000 rows processed  
ThreadNO:1, table:TT01, 26000 rows processed  
ThreadNO:1, table:TT01, 28000 rows processed  
ThreadNO:1, table:TT01, 30000 rows processed  
ThreadNO:1, table:TT01, 32000 rows processed  
ThreadNO:1, table:TT01, 34000 rows processed  
ThreadNO:1, table:TT01, 36000 rows processed  
ThreadNO:1, table:TT01, 38000 rows processed  
ThreadNO:1, table:TT01, 40000 rows processed  
ThreadNO:1, table:TT01, 42000 rows processed  
ThreadNO:1, table:TT01, 44000 rows processed  
ThreadNO:1, table:TT01, 46000 rows processed  
ThreadNO:1, table:TT01, 48000 rows processed  
ThreadNO:1, table:TT01, 50000 rows processed  
ThreadNO:1, table:TT01, 52000 rows processed  
ThreadNO:1, table:TT01, 54000 rows processed  
ThreadNO:1, table:TT01, 56000 rows processed  
ThreadNO:1, table:TT01, 58000 rows processed  
ThreadNO:1, table:TT01, 60000 rows processed  
ThreadNO:1, table:TT01, 62000 rows processed  
ThreadNO:1, table:TT01, 64000 rows processed  
ThreadNO:1, table:TT01, 66000 rows processed  
ThreadNO:1, table:TT01, 68000 rows processed  
ThreadNO:1, table:TT01, 70000 rows processed  
ThreadNO:1, table:TT01, 72000 rows processed  
ThreadNO:1, table:TT01, 74000 rows processed  
ThreadNO:1, table:TT01, 76000 rows processed  
ThreadNO:1, table:TT01, 78000 rows processed  
ThreadNO:1, table:TT01, 80000 rows processed  
ThreadNO:1, table:TT01, 82000 rows processed  
ThreadNO:1, table:TT01, 84000 rows processed  
ThreadNO:1, table:TT01, 86000 rows processed  
ThreadNO:1, table:TT01, 88000 rows processed  
ThreadNO:1, table:TT01, 90000 rows processed  
ThreadNO:1, table:TT01, 92000 rows processed  
ThreadNO:1, table:TT01, 94000 rows processed  
ThreadNO:1, table:TT01, 96000 rows processed  
ThreadNO:1, table:TT01, 98000 rows processed  
ThreadNO:1, table:TT01, 100000 rows processed  
ThreadNO:1, table:TT01, 102000 rows processed  
ThreadNO:1, table:TT01, 104000 rows processed  
ThreadNO:1, table:TT01, 106000 rows processed  
ThreadNO:1, table:TT01, 108000 rows processed  
ThreadNO:1, table:TT01, 110000 rows processed  
ThreadNO:1, table:TT01, 112000 rows processed  
ThreadNO:1, table:TT01, 114000 rows processed  
ThreadNO:1, table:TT01, 116000 rows processed  
ThreadNO:1, table:TT01, 118000 rows processed  
ThreadNO:1, table:TT01, 120000 rows processed  
ThreadNO:1, table:TT01, 122000 rows processed  
ThreadNO:1, table:TT01, 124000 rows processed  
ThreadNO:1, table:TT01, 126000 rows processed  
ThreadNO:1, table:TT01, 128000 rows processed  
ThreadNO:1, table:TT01, 130000 rows processed  
ThreadNO:1, table:TT01, 132000 rows processed  
ThreadNO:1, table:TT01, 134000 rows processed  
ThreadNO:1, table:TT01, 136000 rows processed  
ThreadNO:1, table:TT01, 138000 rows processed  
ThreadNO:1, table:TT01, 140000 rows processed  
ThreadNO:1, table:TT01, 142000 rows processed  
ThreadNO:1, table:TT01, 144000 rows processed  
ThreadNO:1, table:TT01, 146000 rows processed  
ThreadNO:1, table:TT01, 148000 rows processed  
ThreadNO:1, table:TT01, 150000 rows processed  
ThreadNO:1, table:TT01, 152000 rows processed  
ThreadNO:1, table:TT01, 154000 rows processed  
ThreadNO:1, table:TT01, 156000 rows processed  
ThreadNO:1, table:TT01, 158000 rows processed  
ThreadNO:1, table:TT01, 160000 rows processed  
ThreadNO:1, table:TT01, 162000 rows processed  
ThreadNO:1, table:TT01, 164000 rows processed  
ThreadNO:1, table:TT01, 166000 rows processed  
ThreadNO:1, table:TT01, 168000 rows processed  
ThreadNO:1, table:TT01, 170000 rows processed  
ThreadNO:1, table:TT01, 172000 rows processed  
ThreadNO:1, table:TT01, 174000 rows processed  
ThreadNO:1, table:TT01, 176000 rows processed  
ThreadNO:1, table:TT01, 178000 rows processed  
