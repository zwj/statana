#ifndef __ARGS_ANALYZE_TEST_H__
#define __ARGS_ANALYZE_TEST_H__
#include<cppunit/extensions/HelperMacros.h> 
#include<cppunit/TestCase.h>
class args_analyze_testcase : public CppUnit::TestCase 
{
	CPPUNIT_TEST_SUITE(args_analyze_testcase); 
	CPPUNIT_TEST(short_args_test); 
	CPPUNIT_TEST_SUITE_END(); 
public:
	void short_args_test();
};
#endif
