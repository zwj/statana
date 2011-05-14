 // read_write_mutex_testcase.cpp: implementation of the read_write_mutex_testcase class.
//
//////////////////////////////////////////////////////////////////////

#include "read_write_mutex_testcase.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPPUNIT_TEST_SUITE_REGISTRATION( read_write_mutex_testcase );
// muti_readwrite.cpp : Defines the entry point for the console application.
//


#include <boost/shared_ptr.hpp>
#include <string>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <windows.h>
#include "../patterns/singleton.h"
#include "../patterns/read_write_mutex.h"
#include "../time/performance_time.h"
using namespace std;
using namespace boost;
using namespace improve;

class config
{

  SINGLETON_IMPL(config);
public:
	string name()
	{
		read_write_mutex::scoped_readlock lock(_mutex);
		Sleep(_s_use_ms);		
		return _name;
	}
	void debug_on()
	{
		read_write_mutex::scoped_writelock lock(_mutex);
		Sleep(_s_use_ms);		
		_state="DEBUG";
	}
	void debug_off()
	{
		read_write_mutex::scoped_writelock lock(_mutex);
		Sleep(_s_use_ms);		
		_state="NDEBUG";
	}
	bool is_debug()
	{
		read_write_mutex::scoped_readlock lock(_mutex);
		Sleep(_s_use_ms);		
		return _state=="DEBUG";
	}
private:
	config():_name("config"){}
	string _name;
	string _state;
	static read_write_mutex _mutex;
public:
  static int _s_use_ms;
};

read_write_mutex config::_mutex;
int config::_s_use_ms=100;


mutex g_io_mutex;
int g_call_times=10;
void read()
{
  config* cfg_ptr=config::instance(); 
  for(int i=0; i< g_call_times; ++i)
  {
	  cfg_ptr->is_debug();
  }
  
}

void write()
{
  config* cfg_ptr=config::instance(); 
  for(int i=0; i< g_call_times; ++i)
  {
    cfg_ptr->debug_on();
  }
}

void read_write_mutex_testcase::mutil_reader_test()
{
	thread_group threads;
  performance_time action;
  action.start();
  int thread_num=50;
	for(int i=0; i<thread_num;i++)
	{
		threads.create_thread(&read);
	}
	threads.join_all();
  action.end();
  int min_ms= g_call_times *config::_s_use_ms;
  int max_ms=thread_num*g_call_times * config::_s_use_ms;
  CPPUNIT_ASSERT(action.duration_ms()<=max_ms);
  CPPUNIT_ASSERT(action.duration_ms()>=min_ms);
  cout<< "use :"<<action.duration_ms() <<" ms "<<endl;
}


void read_write_mutex_testcase::mutil_writer_test()
{
	thread_group threads;
	config* ptr=config::instance();
  performance_time action;
  int thread_num=3;
  action.start();
	for(int i=0; i<thread_num;i++)
	{
		threads.create_thread(&write);
	}
	threads.join_all();
  action.end();
  int min_ms= thread_num*g_call_times *config::_s_use_ms;
  int max_ms=(2+thread_num*g_call_times) * config::_s_use_ms;
  CPPUNIT_ASSERT(action.duration_ms()<=max_ms);
  CPPUNIT_ASSERT(action.duration_ms()>=min_ms);

  cout<< "use :"<<action.duration_ms() <<" ms "<<endl;
}

void read_write_mutex_testcase::mr_1w_test()
{
  thread_group threads;
  config* ptr=config::instance();
  int thread_num=3;
  performance_time action;
  action.start();
  threads.create_thread(&write);
  for(int i=0; i<thread_num;i++)
  {
    threads.create_thread(&read);
  }
  threads.join_all();
  action.end();
  int min_ms= g_call_times *config::_s_use_ms;
  int max_ms=(1+thread_num)*g_call_times * config::_s_use_ms;
  CPPUNIT_ASSERT(action.duration_ms()<=max_ms);
  CPPUNIT_ASSERT(action.duration_ms()>=min_ms);
  cout<<" use :"<<action.duration_ms() <<" ms "<<endl;
}

void read_write_mutex_testcase::muti_thread_access()
{
	thread_group threads;
	config* ptr=config::instance();
  performance_time action;
  action.start();
	for(int i=0; i<5;i++)
	{
		threads.create_thread(&read);
	}
	
	for(i=0; i<2;i++)
	{
		threads.create_thread(&write);
	}
	for(i=0; i<5;i++)
	{
		threads.create_thread(&read);
	}
	threads.join_all();
  action.end();
  int min_ms= (2*g_call_times +1) *config::_s_use_ms;
  int max_ms=(2+10)*g_call_times * config::_s_use_ms;
  CPPUNIT_ASSERT(action.duration_ms()<=max_ms);
  CPPUNIT_ASSERT(action.duration_ms()>=min_ms);
  cout<<" use :"<<action.duration_ms() <<" ms "<<endl;
}




