// ao_pattern_testcase.h: interface for the ao_pattern_testcase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AO_PATTERN_TESTCASE_H__BD69DB63_151E_4EC0_9539_D4C96F303721__INCLUDED_)
#define AFX_AO_PATTERN_TESTCASE_H__BD69DB63_151E_4EC0_9539_D4C96F303721__INCLUDED_


#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/testcase.h>
class ao_pattern_testcase : public CppUnit::TestCase
{
CPPUNIT_TEST_SUITE( ao_pattern_testcase );
//CPPUNIT_TEST(single_thread_test);
//CPPUNIT_TEST(muti_thread_test);
CPPUNIT_TEST_SUITE_END();

public:
	void single_thread_test();
  void muti_thread_test();
};
#endif // !defined(AFX_AO_PATTERN_TESTCASE_H__BD69DB63_151E_4EC0_9539_D4C96F303721__INCLUDED_)
