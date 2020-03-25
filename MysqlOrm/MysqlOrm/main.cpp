#include <iostream>

using namespace std;

#include "mysql.h"

#include "MysqlDb.hpp"

class User : public DbEntity
{
public:
	User()
	{
		bindTableName("user");
		
		bindFeild("name", &m_name);
		bindFeild("age", &m_age);
	}

public:
	string m_name;
	int m_age;

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

	deque<User> userList;
	testDb.getObjectList(userList, "select * from user");

	for (User& user : userList)
	{
		cout << user.m_name << " " << user.m_age << endl;
	}

	int a;
	cin >> a;
	return 0;
}