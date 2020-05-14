#pragma once

#include "DbBase.hpp"

#include "DbEntity.hpp"
#include "SqlString.hpp"

#include <deque>

#include "mysql.h"

#pragma  comment(lib, "libmysql.lib")

class MysqlDb : public DbBase
{
public:
	MysqlDb(DbInfo& dbInfo) { m_dbInfo = dbInfo; }
	~MysqlDb() { close(); }

private:
	MysqlDb(const MysqlDb& db) {}
	void operator = (const MysqlDb& db) {}

	MysqlDb(MysqlDb&& db) {}
	void operator = (MysqlDb&& db) {}

private:
	MYSQL m_mysql;

//父类虚函数
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

	MYSQL* getHandle()
	{
		keepConnection();

		return &m_mysql;
	}

	void keepConnection()
	{
		mysql_ping(&m_mysql);
	}

	void close()
	{
		mysql_close(&m_mysql);
	}

//增删改查
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
			objectEntity->bindMember();

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

	template<class T>
	bool insertObject(T& object)
	{
		keepConnection();

		DbEntity* dbEntity = &object;
		dbEntity->bindMember();

		string keyStr = "(";
		string valueStr = " values(";
		for (auto& it : dbEntity->m_feildNameIntMap)
		{
			keyStr = keyStr + it.first + ",";
			valueStr = valueStr + std::to_string(*it.second) + ",";
		}

		for (auto& it : dbEntity->m_feildNameDoubleMap)
		{
			keyStr = keyStr + it.first + ",";
			valueStr = valueStr + std::to_string(*it.second) + ",";
		}

		for (auto& it : dbEntity->m_feildNameStringMap)
		{
			keyStr = keyStr + it.first + ",";
			valueStr = valueStr + "'" + *it.second + "'" + ",";
		}

		keyStr = keyStr.substr(0, keyStr.length() - 1) + ")";
		valueStr = valueStr.substr(0, valueStr.length() - 1) + ");";

		string sql = "insert into " + dbEntity->m_tableName + keyStr + valueStr;
		
		//cout << sql << endl;

		return executeSql(sql);
	}

	
};

 