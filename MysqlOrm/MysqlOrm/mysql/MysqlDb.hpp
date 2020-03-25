#pragma once

#include "DbBase.hpp"

#include "DbEntity.hpp"

#include <deque>

#include "mysql.h"

#pragma  comment(lib, "libmysql.lib")

class MysqlDb : public DbBase
{
public:
	MysqlDb(DbInfo& dbInfo) { m_dbInfo = dbInfo; }
	~MysqlDb() { close(); }

private:
	MYSQL m_mysql;

//¸¸ÀàÐéº¯Êý
public:
	bool link()
	{
		mysql_init(&m_mysql);

		char value = 1;
		mysql_options(&m_mysql, MYSQL_OPT_RECONNECT, (char *)&value);

		if (mysql_real_connect(&m_mysql, m_dbInfo.m_host.c_str(), m_dbInfo.m_userName.c_str(), m_dbInfo.m_password.c_str(),
			m_dbInfo.m_dbName.c_str(), m_dbInfo.m_port, NULL, 0))
		{
			mysql_query(&m_mysql, "set names gbk");

			return true;
		}
		else
		{
			return false;
		}
	}

	bool executeSql(string sql)
	{
		keepConnection();

		if (mysql_query(&m_mysql, sql.c_str()))
		{
			return false;
		}

		return true;
	}

	void close()
	{
		mysql_close(&m_mysql);
	}

public:
	template<class T>
	void getObjectList(deque<T>& objectList, string sql)
	{
		objectList.clear();

		keepConnection();
		if (mysql_query(&m_mysql, sql.c_str()))
		{
			return;
		}

		MYSQL_RES* res = mysql_store_result(&m_mysql);
		int fieldCount = mysql_field_count(&m_mysql);
		MYSQL_FIELD* feilds = mysql_fetch_fields(res);

		map<string, int> feildNameIndexMap;
		for (int i = 0; i < fieldCount; i++)
		{
			feildNameIndexMap[feilds[i].name] = i;
		}

		MYSQL_ROW row;
		while (row = mysql_fetch_row(res))
		{
			T object;
			DbEntity* objectEntity = (DbEntity*)&object;

			for (auto& it : objectEntity->m_feildNameIntMap)
			{
				*it.second = atoi(row[feildNameIndexMap[it.first]]);
			}

			for (auto& it : objectEntity->m_feildNameDoubleMap)
			{
				*it.second = atof(row[feildNameIndexMap[it.first]]);
			}

			for (auto& it : objectEntity->m_feildNameStringMap)
			{
				*it.second = row[feildNameIndexMap[it.first]];
			}

			objectList.push_back(object);
		}

		mysql_free_result(res);
	}

private:
	void keepConnection()
	{
		mysql_ping(&m_mysql);
	}
};

 