#include "SmartMysqlDbPtr.h"



SmartMysqlDbPtr::SmartMysqlDbPtr(MysqlDb* mysqlDb, MysqlDbPool* dbPool)
{
	m_mysqlDb = mysqlDb;
	m_dbPool = dbPool;
}


SmartMysqlDbPtr::~SmartMysqlDbPtr()
{
	if (m_dbPool != nullptr)
	{
		m_dbPool->releaseConnection(m_mysqlDb);
	}
	else
	{
		if (m_mysqlDb != nullptr)
		{
			delete m_mysqlDb;
			m_mysqlDb = nullptr;
		}
	}
}
