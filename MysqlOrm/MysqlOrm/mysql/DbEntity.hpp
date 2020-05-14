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
	void bindTableName(string tableName) { m_tableName = keywordWrapper(tableName); }

	void bindFeild(string feildName, int* member) 
	{
		feildName = keywordWrapper(feildName);
		m_feildNameIntMap[feildName] = member; 
	}

	void bindFeild(string feildName, double* member) 
	{
		feildName = keywordWrapper(feildName);
		m_feildNameDoubleMap[feildName] = member; 
	}

	void bindFeild(string feildName, string* member)
	{
		feildName = keywordWrapper(feildName);
		m_feildNameStringMap[feildName] = member;
	}

public:
	virtual void bindMember() = 0;

private:
	string keywordWrapper(string feild)
	{
		if (feild == "order")
		{
			return "`order`";
		}

		return feild;
	}
};

