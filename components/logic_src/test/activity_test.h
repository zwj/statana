#ifndef __ACTIVITY_TEST_H__
#define __ACTIVITY_TEST_H__
#include <test/test_base.h>
class activity_testcase : public test_base
{
	CPPUNIT_TEST_SUITE(activity_testcase); 
	CPPUNIT_TEST(load_acty_test); 
	CPPUNIT_TEST(collect_acty_test); 
	CPPUNIT_TEST_SUITE_END();
public :	
	void	load_acty_test();
	void	collect_acty_test();
};
#endif
