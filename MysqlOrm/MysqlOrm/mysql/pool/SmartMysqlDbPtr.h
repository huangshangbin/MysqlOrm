#pragma once

#include <mysql/pool/MysqlDbPool.h>

class MysqlDbPool;

class SmartMysqlDbPtr
{
private:
	MysqlDb* m_mysqlDb = nullptr;
	MysqlDbPool* m_dbPool = nullptr;

public:
	SmartMysqlDbPtr(MysqlDb* mysqlDb, MysqlDbPool* dbPool);
	~SmartMysqlDbPtr();

	SmartMysqlDbPtr(SmartMysqlDbPtr&& smartMysqlDb)
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
	
	//允许移动赋值会造成自身链接不能释放。可调用析构函数解决，对于mysql链接来说复杂度太强
	void operator = (SmartMysqlDbPtr&& smartMysqlDb) {}

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