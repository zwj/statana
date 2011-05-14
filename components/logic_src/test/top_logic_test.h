#ifndef __TOP_LOGIC_H__
#define __TOP_LOGIC_H__
#include<cppunit/extensions/HelperMacros.h> 
#include<cppunit/TestCase.h> 
class top_logic_testcase : public CppUnit::TestCase 
{
	CPPUNIT_TEST_SUITE(top_logic_testcase); 	
	CPPUNIT_TEST(filter_test); 
	CPPUNIT_TEST(classify_test); 
	CPPUNIT_TEST(collect_test); 
	CPPUNIT_TEST(task_test); 
	CPPUNIT_TEST_SUITE_END(); 
public: 
	void filter_test();
	void classify_test();
	void collect_test();
	void task_test();
};
#endif
