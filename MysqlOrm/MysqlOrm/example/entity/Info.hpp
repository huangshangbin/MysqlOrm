#pragma once

#include <mysql/DbEntity.hpp>


class Info : public DbEntity
{
public:
	string m_id;
	string m_time;
	int m_age;
	double m_grade;
	double m_math;
	string m_remarks;

public:
	void bindMember()
	{
		bindTableName("info");

		bindFeild("id", &m_id);
		bindFeild("time", &m_time);
		bindFeild("age", &m_age);
		bindFeild("grade", &m_grade);
		bindFeild("math", &m_math);
		bindFeild("remarks", &m_remarks);
	}
};