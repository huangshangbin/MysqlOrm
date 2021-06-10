#pragma once

#include <example/entity/Info.hpp>
#include <mysql/MysqlDb.hpp>

//insert
void insertData()
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
	newInfo.m_time = "2020-3-26";
	newInfo.m_age = 10;
	testDb.insertObject(newInfo);

	cout << testDb.getError() << endl;
}


//transaction
void insertUseTransaction()
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
	newInfo.m_time = "2020-3-26";
	newInfo.m_age = 10;
	testDb.insertObject(newInfo);

	deque<string> sqlList;
	sqlList.push_back("insert into info values('11008', '2020-3-26', 10, 100, 70, 'no');");
	sqlList.push_back("insert into info values('11009', '2020-3-26', 10, 100, 70, 'no');");
	testDb.executeTransaction(sqlList);

	cout << testDb.getError() << endl;
}