#ifndef __TASK_TEST_H__
#define __TASK_TEST_H__
#include <entity/ip.h>
#include <entity/log_item.h>
#include <test/test_base.h> 
class task_testcase : public test_base
{
	CPPUNIT_TEST_SUITE(task_testcase); 	
	CPPUNIT_TEST(usecase); 	
	CPPUNIT_TEST_SUITE_END(); 
public: 
	void usecase();
	void assign_task_test();
	unsigned int get_count(const logs::entity::ip& which, const logs::entity::log_item::vector* logs);
};
#endif
