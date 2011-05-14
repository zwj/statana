#ifndef __UTILITY_H__
#define __UTILITY_H__
#include <test/test_base.h>
class utility_testcase : public test_base 
{
CPPUNIT_TEST_SUITE(utility_testcase); 
CPPUNIT_TEST(unify_test); 
CPPUNIT_TEST(split_test); 
CPPUNIT_TEST_SUITE_END(); 
public:
	void unify_test();
	void split_test();
	

};
#endif
