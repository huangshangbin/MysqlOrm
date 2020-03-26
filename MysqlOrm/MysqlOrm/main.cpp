#include <iostream>

using namespace std;

#include "mysql.h"

#include "MysqlDb.hpp"

class User : public DbEntity
{
public:
	void bindMember()
	{
		bindTableName("user");

		bindFeild("name", &m_name);
		bindFeild("age", &m_age);
	}

public:
	string m_name;
	int m_age;

};

class Info : public DbEntity
{
public:
	Info() {  bindMember(); }
	Info(const Info& info)
	{
		bindMember();
		*this = info;
	}
	Info(Info&& info)
	{
		bindMember();
		*this = info;
	}

	void operator = (const Info& info)
	{
		m_id = info.m_id;
		m_time = info.m_time;
		m_age = info.m_age;
		m_grade = info.m_grade;
		m_math = info.m_math;
		m_remarks = info.m_remarks;
	}
	void operator =(Info&& info)
	{
		m_id = info.m_id;
		m_time = info.m_time;
		m_age = info.m_age;
		m_grade = info.m_grade;
		m_math = info.m_math;
		m_remarks = info.m_remarks;
	}

public:
	string m_id;
	string m_time;
	int m_age;
	double m_grade;
	double m_math;
	string m_remarks;

private:
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

	Info newInfo;
	newInfo.m_id = "11002";
	newInfo.m_time = "2020-3-26";
	newInfo.m_age = 10;
	newInfo.m_grade = 130.32;
	newInfo.m_math = 70.3;
	newInfo.m_remarks = "test";

	Info copyInfo;
	copyInfo = newInfo;
	cout << newInfo.m_feildNameStringMap["id"] << endl;
	cout << copyInfo.m_feildNameStringMap["id"] << endl;

	testDb.insertObject(copyInfo);

	int a;
	cin >> a;
	return 0;
}