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

	string m_error;

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
			m_error = "";
			return true;
		}
		else
		{
			m_error = mysql_error(&m_mysql);
			return false;
		}
	}

	bool executeSql(string sql)
	{
		keepConnection();

		if (mysql_query(&m_mysql, sql.c_str()))
		{
			m_error = mysql_error(&m_mysql);
			return false;
		}

		m_error = "";
		return true;
	}

	bool executeTransaction(deque<string>& sqlList)
	{
		keepConnection();

		mysql_autocommit(&m_mysql, 0);//自动提交关闭

		bool isSuccess = true;
		for (int i = 0; i < sqlList.size(); i++)
		{
			if (mysql_query(&m_mysql, sqlList[i].c_str()))
			{
				isSuccess = false;

				m_error = mysql_error(&m_mysql);

				mysql_rollback(&m_mysql);

				break;
			}
		}

		if (isSuccess)
		{
			m_error = "";
			mysql_commit(&m_mysql);
		}
		
		mysql_autocommit(&m_mysql, 1);//自动提交打开
		return isSuccess;
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

	string getError()
	{
		return m_error;
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
			m_error = mysql_error(&m_mysql);
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

		m_error = "";
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

	template<class T>
	bool insertObjectList(deque<T>& objectList)
	{
		keepConnection();

		string tableName;

		DbEntity* dbEntity = &objectList[0];
		dbEntity->bindMember();

		tableName = dbEntity->m_tableName;

		string keyStr = "(";
		for (auto& it : dbEntity->m_feildNameIntMap)
		{
			keyStr = keyStr + it.first + ",";
		}

		for (auto& it : dbEntity->m_feildNameDoubleMap)
		{
			keyStr = keyStr + it.first + ",";
		}

		for (auto& it : dbEntity->m_feildNameStringMap)
		{
			keyStr = keyStr + it.first + ",";
		}

		keyStr = keyStr.substr(0, keyStr.length() - 1) + ")";

		string valueStrList = " values ";
		for (int i = 0; i < objectList.size(); i++)
		{
			string valueStr = "(";
			T object = objectList[i];
			dbEntity = &object;
			dbEntity->bindMember();

			for (auto& it : dbEntity->m_feildNameIntMap)
			{
				valueStr = valueStr + std::to_string(*it.second) + ",";
			}

			for (auto& it : dbEntity->m_feildNameDoubleMap)
			{
				valueStr = valueStr + std::to_string(*it.second) + ",";
			}

			for (auto& it : dbEntity->m_feildNameStringMap)
			{
				valueStr = valueStr + "'" + *it.second + "'" + ",";
			}
			valueStr = valueStr.substr(0, valueStr.length() - 1) + ")";

			if (i == (objectList.size() - 1))
			{
				valueStrList = valueStrList + valueStr;
			}
			else
			{
				valueStrList = valueStrList + valueStr + ",";
			}
		}


		string sql = "insert into " + tableName + keyStr + valueStrList;

		//cout << sql << endl;

		return executeSql(sql);
	}
};

 