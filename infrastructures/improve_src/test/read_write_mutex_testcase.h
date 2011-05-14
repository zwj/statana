// read_write_mutex_testcase.h: interface for the read_write_mutex_testcase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_READ_WRITE_MUTEX_TESTCASE_H__A338C6EA_223E_49B3_B023_8B1F2556523B__INCLUDED_)
#define AFX_READ_WRITE_MUTEX_TESTCASE_H__A338C6EA_223E_49B3_B023_8B1F2556523B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/testcase.h>
class read_write_mutex_testcase : public CppUnit::TestCase
{
CPPUNIT_TEST_SUITE( read_write_mutex_testcase );
CPPUNIT_TEST(mutil_reader_test);
CPPUNIT_TEST(mutil_writer_test);
CPPUNIT_TEST(mr_1w_test);
CPPUNIT_TEST(muti_thread_access);
CPPUNIT_TEST_SUITE_END();
public:
  void mutil_reader_test();
  void mutil_writer_test();
  void mr_1w_test();
  void muti_thread_access();
};

#endif // !defined(AFX_READ_WRITE_MUTEX_TESTCASE_H__A338C6EA_223E_49B3_B023_8B1F2556523B__INCLUDED_)
