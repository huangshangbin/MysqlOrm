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



int main()
{
	DbInfo dbInfo;
	dbInfo.m_dbType = DbType::MYSQL_DB;
	dbInfo.m_host = "127.0.0.1";
	dbInfo.m_port = 3306;
	dbInfo.m_userName = "root";
	dbInfo.m_password = "123456";
	dbInfo.m_dbName = "test";

	MysqlDb testDb(dbInfo);
	testDb.link();

	Info info;
	info.m_id = "11003";
	info.m_time = "2020-3-26";
	info.m_age = 20;
	info.m_grade = 120.1;
	info.m_math = 34.2;
	info.m_remarks = "new design";
	testDb.insertObject(info);

	int a;
	cin >> a;
	return 0;
}