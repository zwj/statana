#include "task_manager_test.h" 
#include <logic/task_manager.h>
#include "logic_support.h"
#include <entity_src/test/entity_support.h>
#include <iostream>
using namespace std;
using namespace logs;
using namespace logs::entity;
using namespace logs::logic;
CPPUNIT_TEST_SUITE_REGISTRATION(task_manager_testcase); 
void
task_manager_testcase::task_manager_test()
{
	task_manager  manager(string("stat_1"));
	manager.load_plan("../config/stat_test.conf");
	task::shared_ptr one =manager.assign_task();
	one->execute();
}
void
task_manager_testcase::task_manager_get_file()
{
	task_manager  manager(string("stat_2"));
	manager.get_src_files("/usr/include/*.cpp");
	CPPUNIT_ASSERT(manager._files.size()>0); 

	task_manager  manager1(string("stat_21"));
	manager.get_src_files("/usr/include/*.h");
	CPPUNIT_ASSERT(manager._files.size()>1); 
	
}
void
task_manager_testcase::assign_task_test()
{
	task_manager  manager(string("stat_3"));
	manager.load_plan("../config/stat_test.conf");
	while(manager.have_task())
	{
		task::shared_ptr one =manager.assign_task();
		one->execute();
	}
}


void
task_manager_testcase::assign_union_task_test()
{
	task_manager  manager(string("stat_4"));
	manager.load_plan("../config/stat-union.conf");
	while(manager.have_task())
	{
		task::shared_ptr one =manager.assign_task();
		one->execute();
	} 
}

void
task_manager_testcase::sec_name_test()
{
	task_manager  manager(string("stat_5"));
    string sec_name = manager.get_section_name("source:../data/:*.log ;");
    CPPUNIT_ASSERT_EQUAL(sec_name ,string("SOURCE"));

    sec_name = manager.get_section_name("task: 5000;");
    CPPUNIT_ASSERT_EQUAL(sec_name ,string("TASK"));

    sec_name = manager.get_section_name("@source:     load(*.log(5000), ($Date < 2005-06-07) ; ");
    CPPUNIT_ASSERT_EQUAL(sec_name ,string("@SOURCE"));
}
