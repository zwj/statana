
#ifndef __RULE_EXPRESS_TEST_H__
#define  __RULE_EXPRESS_TEST_H__
#include "cppunit/extensions/HelperMacros.h"
#include <cppunit/TestCase.h>
#include <improve/status.h>
enum TestStates{LIVED,SON,FATHER,GRANDFATHER,DEATH};
typedef improve::status_machine<TestStates> live_stats;

class status_machine_testcase : public CppUnit::TestCase
{
CPPUNIT_TEST_SUITE(status_machine_testcase );
CPPUNIT_TEST(SonToDeath);
CPPUNIT_TEST(FatherToDeath);
CPPUNIT_TEST(SuccessToNextState);
CPPUNIT_TEST(FailToNextState);
CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();
	void SuccessToNextState();
	void FailToNextState();
	void SonToDeath();
	void FatherToDeath();
	live_stats* _stats_ptr;
};
#endif
