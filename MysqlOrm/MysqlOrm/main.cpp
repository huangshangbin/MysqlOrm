#include <iostream>

using namespace std;

#include "mysql.h"

#include "MysqlDb.hpp"
#include "SqlString.hpp"

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


int main()
{
	SqlString sql("select * from user where id between ? and ?");
	sql.orderlyReplace("002");
	sql.orderlyReplace("004");
	cout << sql.toString() << endl;

	int a;
	cin >> a;
	return 0;
}