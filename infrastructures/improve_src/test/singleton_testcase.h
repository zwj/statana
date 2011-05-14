// singleton_testcase.h: interface for the singleton_testcase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINGLETON_TESTCASE_H__BBA6A28D_B111_4A25_B7E1_21AB2288F4B3__INCLUDED_)
#define AFX_SINGLETON_TESTCASE_H__BBA6A28D_B111_4A25_B7E1_21AB2288F4B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../patterns/singleton.h"
class singleton_testcase  
{
  SINGLETON_IMPL(singleton_testcase);
public:
	singleton_testcase();
	virtual ~singleton_testcase();

};

#endif // !defined(AFX_SINGLETON_TESTCASE_H__BBA6A28D_B111_4A25_B7E1_21AB2288F4B3__INCLUDED_)
