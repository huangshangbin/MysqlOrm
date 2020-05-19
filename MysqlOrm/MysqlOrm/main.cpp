#include <iostream>

using namespace std;

#include "mysql.h"

#include <mysql/MysqlDb.hpp>
#include <mysql/SqlString.hpp>

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


#include <mysql/pool/MysqlDbPool.h>


int main()
{
	DbInfo dbInfo;
	dbInfo.m_host = "127.0.0.1";
	dbInfo.m_port = 3306;
	dbInfo.m_userName = "root";
	dbInfo.m_password = "123456";
	dbInfo.m_dbName = "test";

	MysqlDb testDb(dbInfo);
	testDb.link();

	Info newInfo;
	newInfo.m_id = "11003";
	newInfo.m_age = 10;
	//testDb.insertObject(newInfo);

	deque<string> sqlList;
	sqlList.push_back("insert into info values('11008', '2020-3-26', 10, 100, 70, 'no');");
	sqlList.push_back("insert into info values('11009', '2020-3-26', 10, 100, 70, no);");
	testDb.executeTransaction(sqlList);

	cout << testDb.getError() << endl;

	int a;
	cin >> a;
	return 0;
}