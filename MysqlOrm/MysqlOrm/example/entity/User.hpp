#pragma once

#include <mysql/DbEntity.hpp>


class User : public DbEntity
{
public:
	string m_name;
	int m_age;

public:
	void bindMember()
	{
		bindTableName("user");

		bindFeild("name", &m_name);
		bindFeild("age", &m_age);
	}
};