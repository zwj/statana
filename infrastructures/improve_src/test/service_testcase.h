// service_testcase.h: interface for the service_testcase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVICE_TESTCASE_H__6A8ACACC_FD32_4DB1_A358_BC4FB4CA7B3F__INCLUDED_)
#define AFX_SERVICE_TESTCASE_H__6A8ACACC_FD32_4DB1_A358_BC4FB4CA7B3F__INCLUDED_

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/testcase.h>

#include "../worker.h"
class config_user : public improve::worker
{
  public:
    void do_task();
};
class service_testcase  : public CppUnit::TestCase
{
CPPUNIT_TEST_SUITE( service_testcase );
//CPPUNIT_TEST(main);
//CPPUNIT_TEST(test_exception);
//CPPUNIT_TEST(timed_join);
//CPPUNIT_TEST(add_service_test);
CPPUNIT_TEST(dyn_services_test);
CPPUNIT_TEST_SUITE_END();
public:

  void main();
  void test_exception();
  void timed_join();
  void add_service_test();
  void dyn_services_test();
};

#endif // !defined(AFX_SERVICE_TESTCASE_H__6A8ACACC_FD32_4DB1_A358_BC4FB4CA7B3F__INCLUDED_)
