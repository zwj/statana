// singleton_testcase.cpp: implementation of the singleton_testcase class.
//
//////////////////////////////////////////////////////////////////////

#include "singleton_testcase.h"
#include <iostream>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

singleton_testcase::singleton_testcase()
{

}

singleton_testcase::~singleton_testcase()
{
  std::cout<<"destructor"<<std::endl;
}
