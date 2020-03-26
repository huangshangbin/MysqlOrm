#pragma once

#include<iostream>
#include<string>
#include <map>

using namespace std;

class DbEntity
{
public:
	DbEntity() { }


public:
    string m_tableName;

	map<string, int*> m_feildNameIntMap;
	map<string, double*> m_feildNameDoubleMap;
	map<string, string*> m_feildNameStringMap;

protected:
	void bindTableName(string tableName) { m_tableName = tableName; }

	void bindFeild(string feildName, int* member) { m_feildNameIntMap[feildName] = member; }
	void bindFeild(string feildName, double* member) { m_feildNameDoubleMap[feildName] = member; }
	void bindFeild(string feildName, string* member) { m_feildNameStringMap[feildName] = member; }

public:
	virtual void bindMember() = 0;
};

