#ifndef __TASK_MANAGE_TEST_H__
#define __TASK_MANAGE_TEST_H__ 
#include <entity/ip.h>
#include <entity/log_item.h>
#include <test/test_base.h> 
class task_manager_testcase : public test_base
{
	CPPUNIT_TEST_SUITE(task_manager_testcase); 	
	CPPUNIT_TEST(assign_task_test); 
	CPPUNIT_TEST(assign_union_task_test); 
	CPPUNIT_TEST(task_manager_get_file); 
	CPPUNIT_TEST(sec_name_test); 
	CPPUNIT_TEST_SUITE_END(); 
public: 
	void task_manager_test();
	void task_manager_get_file();
    void assign_union_task_test();
	void assign_task_test();
	void sec_name_test();
};
#endif
