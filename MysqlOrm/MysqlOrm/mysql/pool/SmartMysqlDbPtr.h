#pragma once

#include <mysql/pool/MysqlDbPool.h>

class MysqlDbPool;

class SmartMysqlDbPtr
{
private:
	MysqlDb* m_mysqlDb;
	MysqlDbPool* m_dbPool;

public:
	SmartMysqlDbPtr(MysqlDb* mysqlDb, MysqlDbPool* dbPool);
	~SmartMysqlDbPtr();

	SmartMysqlDbPtr(SmartMysqlDbPtr&& smartMysqlDb)
	{
		*this = std::move(smartMysqlDb);
	}
	void operator = (SmartMysqlDbPtr&& smartMysqlDb)
	{
		if (&smartMysqlDb == this)
		{
			return;
		}

		m_mysqlDb = smartMysqlDb.m_mysqlDb;
		m_dbPool = smartMysqlDb.m_dbPool;

		smartMysqlDb.m_mysqlDb = nullptr;
		smartMysqlDb.m_dbPool = nullptr;
	}

private:
	SmartMysqlDbPtr(const SmartMysqlDbPtr& smartMysqlDb) {}
	void operator = (const SmartMysqlDbPtr& smartMysqlDb) {}



public:
	MysqlDb& operator * ()
	{
		return *m_mysqlDb;
	}

	MysqlDb* operator -> ()
	{
		return m_mysqlDb;
	}
};