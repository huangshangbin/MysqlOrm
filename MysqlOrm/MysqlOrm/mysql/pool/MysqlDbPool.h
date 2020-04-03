#pragma once

#include <iostream>
#include <queue>
#include <mutex>

#include <MysqlDb.hpp>
#include <pool/SmartMysqlDbPtr.h>

using namespace std;

class SmartMysqlDbPtr;

class MysqlDbPool
{
public:
	MysqlDbPool(int conSize, DbInfo dbInfo);
	~MysqlDbPool();

private:
	queue<MysqlDb*> m_mysqlDbQueue;
	mutex m_mutex;
	condition_variable m_conditionVar;

public:
	MysqlDb* waitGetConnection();
	SmartMysqlDbPtr waitGetSmartCon();

	void releaseConnection(MysqlDb* mysqlDb);

};



