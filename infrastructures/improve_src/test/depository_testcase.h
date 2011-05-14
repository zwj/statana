// channel_testcase.h: interface for the channel_testcase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHANNEL_TESTCASE_H__DFC6CE45_138C_41CC_92C4_513D7A088174__INCLUDED_)
#define AFX_CHANNEL_TESTCASE_H__DFC6CE45_138C_41CC_92C4_513D7A088174__INCLUDED_


#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/testcase.h>

class depository_testcase  : public CppUnit::TestCase
{
CPPUNIT_TEST_SUITE( depository_testcase );
CPPUNIT_TEST(test_depository);
CPPUNIT_TEST_SUITE_END();
public:
	void test_depository();
};

#endif // !defined(AFX_CHANNEL_TESTCASE_H__DFC6CE45_138C_41CC_92C4_513D7A088174__INCLUDED_)
