#pragma once


#include <example/entity/Info.hpp>
#include <example/helper/SpaceTime.hpp>
#include <mysql/MysqlDb.hpp>
#include <mutex>
#include <thread>
#include <iostream>

MysqlDb* testDb;
mutex threadMutex;

using namespace std;

void threadFun1()
{
	for (int i = 0; i < 100000; i++)
	{
		lock_guard<mutex> lockGuard(threadMutex);

		Info newInfo;
		newInfo.m_id = std::to_string(i);
		newInfo.m_time = "2020-3-26";
		newInfo.m_age = 10;

		testDb->insertObject(newInfo);
	}
}

void threadFun2()
{
	for (int i = 100000; i < 200000; i++)
	{
		lock_guard<mutex> lockGuard(threadMutex);

		Info newInfo;
		newInfo.m_id = std::to_string(i);
		newInfo.m_time = "2020-3-26";
		newInfo.m_age = 10;

		testDb->insertObject(newInfo);
	}
}

void threadFun3()
{
	for (int i = 200000; i < 300000; i++)
	{
		lock_guard<mutex> lockGuard(threadMutex);

		Info newInfo;
		newInfo.m_id = std::to_string(i);
		newInfo.m_time = "2020-3-26";
		newInfo.m_age = 10;

		testDb->insertObject(newInfo);
	}
}

void threadFun4()
{
	for (int i = 300000; i < 400000; i++)
	{
		lock_guard<mutex> lockGuard(threadMutex);

		Info newInfo;
		newInfo.m_id = std::to_string(i);
		newInfo.m_time = "2020-3-26";
		newInfo.m_age = 10;

		testDb->insertObject(newInfo);
	}
}

void multiThreadInsertObject()
{
	SpaceTime spaceTime;
	spaceTime.recordStartTime();

	DbInfo dbInfo;
	dbInfo.m_host = "127.0.0.1";
	dbInfo.m_port = 3306;
	dbInfo.m_userName = "root";
	dbInfo.m_password = "123456";
	dbInfo.m_dbName = "test";

	testDb = new MysqlDb(dbInfo);
	testDb->link();

	thread thread1(threadFun1);
	thread thread2(threadFun2);
	thread thread3(threadFun3);
	thread thread4(threadFun4);

	thread1.join();
	thread2.join();
	thread3.join();
	thread4.join();


	spaceTime.recordEndTime();

	cout << "spaceTime = "<< spaceTime.getSpaceMilliSecond() <<endl;

	delete testDb;
}