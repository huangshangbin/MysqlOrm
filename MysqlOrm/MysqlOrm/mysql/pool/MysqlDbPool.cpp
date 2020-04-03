#include "MysqlDbPool.h"



MysqlDbPool::MysqlDbPool(int conSize, DbInfo dbInfo)
{
	lock_guard<mutex> lockGuard(m_mutex);

	for (int i = 0; i < conSize; i++)
	{
		MysqlDb* dbCon = new MysqlDb(dbInfo);
		dbCon->link();

		m_mysqlDbQueue.push(dbCon);
	}
}


MysqlDbPool::~MysqlDbPool()
{
	lock_guard<mutex> lockGuard(m_mutex);

	while (m_mysqlDbQueue.size() > 0)
	{
		MysqlDb* dbCon = m_mysqlDbQueue.front();
		m_mysqlDbQueue.pop();

		if (dbCon != nullptr)
		{
			delete dbCon;
			dbCon = nullptr;
		}
	}
}

MysqlDb* MysqlDbPool::waitGetConnection()
{
	unique_lock<mutex> uniqueLock(m_mutex);
	m_conditionVar.wait(uniqueLock, [this] {
		if (m_mysqlDbQueue.empty())
		{
			return false;
		}
		else
		{
			return true;
		}
	});

	MysqlDb* mysqlDb = m_mysqlDbQueue.front();
	m_mysqlDbQueue.pop();

// 	SmartMysqlDbPtr smartMysqlDb(mysqlDb, this);
// 
// 	return std::move(smartMysqlDb);

	return mysqlDb;
}

SmartMysqlDbPtr MysqlDbPool::waitGetSmartCon()
{
	unique_lock<mutex> uniqueLock(m_mutex);
	m_conditionVar.wait(uniqueLock, [this] {
		if (m_mysqlDbQueue.empty())
		{
			return false;
		}
		else
		{
			return true;
		}
	});

	MysqlDb* mysqlDb = m_mysqlDbQueue.front();
	m_mysqlDbQueue.pop();

	SmartMysqlDbPtr smartMysqlDb(mysqlDb, this);
	
	return std::move(smartMysqlDb);
}

void MysqlDbPool::releaseConnection(MysqlDb* mysqlDb)
{
	unique_lock<mutex> uniqueLock(m_mutex);

	m_mysqlDbQueue.push(mysqlDb);

	m_conditionVar.notify_one();
}