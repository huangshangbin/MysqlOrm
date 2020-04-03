#pragma once

#include <iostream>
#include <string>

using namespace std;


class DbInfo
{
public:
	string m_host;
	int m_port;
	string m_userName;
	string m_password;
	string m_dbName;
};

class DbBase
{
protected:
	DbInfo m_dbInfo;

public:
	void setDbInfo(DbInfo& dbInfo) { m_dbInfo = dbInfo;}
	DbInfo& getDbInfo () { return m_dbInfo;  }

public:
	virtual bool link() = 0;
	virtual bool executeSql(string sql) = 0;
	virtual void close() = 0;
};