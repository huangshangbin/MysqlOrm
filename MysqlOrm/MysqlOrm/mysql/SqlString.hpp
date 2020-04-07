#pragma once

#include <iostream>
#include <string>
#include <list>

using namespace std;

class SqlString
{
public:
	SqlString(string baseSql)
	{
		m_splitStrList = splitString(baseSql, "?");
		m_sql = m_splitStrList.front();
		m_splitStrList.pop_front();
	}
	

private:
	string m_sql;
	list<string> m_splitStrList;

public:
	void orderlyReplace(string value)
	{
		m_sql = m_sql + "'" + value + "'" + m_splitStrList.front();
		m_splitStrList.pop_front();
	}

	void orderlyReplace(int value)
	{
		m_sql = m_sql + std::to_string(value) + m_splitStrList.front();
		m_splitStrList.pop_front();
	}

	void orderlyReplace(double value)
	{
		m_sql = m_sql + std::to_string(value) + m_splitStrList.front();
		m_splitStrList.pop_front();
	}


	string toString()
	{
		return m_sql;
	}

private:
	list<string> splitString(string srcStr, string splitStr)
	{
		list<string> dataList;

		string str = srcStr;

		string::size_type pos1, pos2;
		pos2 = str.find(splitStr);
		pos1 = 0;
		while (string::npos != pos2)
		{
			dataList.push_back(str.substr(pos1, pos2 - pos1));

			pos1 = pos2 + splitStr.size();
			pos2 = str.find(splitStr, pos1);
		}
		dataList.push_back(str.substr(pos1));

		return std::move(dataList);
	}
};

