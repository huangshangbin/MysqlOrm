#include <iostream>

using namespace std;

#include "mysql.h"

#include "MysqlDb.hpp"

class User : public DbEntity
{
public:
	string m_name;
	int m_age;

public:
	void bindMember()
	{
		bindTableName("user");

		bindFeild("name", &m_name);
		bindFeild("age", &m_age);
	}
};

class Info : public DbEntity
{
public:
	string m_id;
	string m_time;
	int m_age;
	double m_grade;
	double m_math;
	string m_remarks;

public:
	void bindMember()
	{
		bindTableName("info");

		bindFeild("id", &m_id);
		bindFeild("time", &m_time);
		bindFeild("age", &m_age);
		bindFeild("grade", &m_grade);
		bindFeild("math", &m_math);
		bindFeild("remarks", &m_remarks);
	}
};


#include <pool/MysqlDbPool.h>



MysqlDbPool* mysqlDbPool;

void dbPoolTestFun(int threadId)
{
	int count = 0;
	while (count < 100)
	{
		//MysqlDb* mysqlDb = mysqlDbPool->waitGetConnection();
		SmartMysqlDbPtr mysqlDb = mysqlDbPool->waitGetSmartCon();

		cout << "线程" << threadId << " 获得使用权" << endl;

		User user;
		user.m_name = std::to_string(threadId * count);
		mysqlDb->insertObject(user);

		Sleep(10);

		//mysqlDbPool->releaseConnection(mysqlDb);


		count++;
	}
}


int main()
{
	DbInfo dbInfo;
	dbInfo.m_host = "127.0.0.1";
	dbInfo.m_port = 3306;
	dbInfo.m_userName = "root";
	dbInfo.m_password = "123456";
	dbInfo.m_dbName = "test";

	mysqlDbPool = new MysqlDbPool(5, dbInfo);

	thread testThread1(dbPoolTestFun, 1);
	thread testThread2(dbPoolTestFun, 2);
	thread testThread3(dbPoolTestFun, 3);
	thread testThread4(dbPoolTestFun, 4);
	thread testThread5(dbPoolTestFun, 5);
	thread testThread6(dbPoolTestFun, 6);
	thread testThread7(dbPoolTestFun, 7);

	testThread1.join();
	testThread2.join();
	testThread3.join();
	testThread4.join();
	testThread5.join();
	testThread6.join();
	testThread7.join();


	delete mysqlDbPool;

	int a;
	cin >> a;
	return 0;
}