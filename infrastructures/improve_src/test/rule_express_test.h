#ifndef __RULE_EXPRESS_TEST_H__
#define  __RULE_EXPRESS_TEST_H__
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestCase.h>
class rule_express_testcase : public CppUnit::TestCase
{
CPPUNIT_TEST_SUITE(rule_express_testcase);
CPPUNIT_TEST(or_test);
CPPUNIT_TEST(and_test);
CPPUNIT_TEST(not_test);
CPPUNIT_TEST_SUITE_END();
public:
	void or_test();
	void and_test();
	void not_test();
};
#endif
