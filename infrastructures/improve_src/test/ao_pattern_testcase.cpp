// ao_pattern_testcase.cpp: implementation of the ao_pattern_testcase class.
//
//////////////////////////////////////////////////////////////////////
#include "ao_pattern_testcase.h"
#include <improve/patterns.h>

#include <windows.h>
#include <iostream>
#include <boost/ref.hpp>
using namespace std;

using namespace improve;
CPPUNIT_TEST_SUITE_REGISTRATION( ao_pattern_testcase );

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


class test_servant;
class test_add_req;



class test_servant
{
public:
  int add(const int x, const int y)
  {
    return x+y;
  }

};

class test_add_req : public param2_future1_request<test_servant,int,int,int>
{
public:
  test_add_req(const int x,const int y ,future_type::shared_ptr future_ref)
  :param2_future1_request<test_servant,int,int,int>(x,y,future_ref)
  {}
  void call(test_servant& svnt_ref)
  {
    _future_ptr->_value=svnt_ref.add(_param1,_param2);
    _future_ptr->have_set();
  }
};

class test_proxy
{
public:
  test_proxy()
  :_test_scheduler(2)
  {}
  int add(const int x, const int y)
  {

    int i;

    test_add_req::future_type::shared_ptr ret_value(new test_add_req::future_type(i));
    test_add_req::shared_ptr req_ptr(new test_add_req(x,y,ret_value));
    _test_scheduler.enqueue(req_ptr);
    ret_value->timed_wait(10000);
    return i;

  }
  task_scheduler0<test_servant> _test_scheduler;
};


class add_method_tester : public worker
{
public:
  void log_exception(exception& e){assert(false);}
  add_method_tester(test_proxy& proxy_ref)
  :_proxy_ref(proxy_ref)
  {}
  void do_task()
  {
    cout<<_proxy_ref.add(3,5)<<endl;
    //cout<<_proxy_ref.add(4,5)<<endl;
  }
  test_proxy& _proxy_ref;
};


void ao_pattern_testcase::single_thread_test()
{
  test_proxy one_proxy;
  add_method_tester cur_tester(one_proxy);
  for(int i=0; i<100; i++)
  {
    cur_tester.do_task();
  }

}

void ao_pattern_testcase::muti_thread_test()
{
  
  test_proxy one_proxy;
  Sleep(3000);
  service1_manage<add_method_tester,boost::reference_wrapper<test_proxy> > 
    test_svc(boost::ref(one_proxy),10);
  test_svc.timed_join_all(5000);
  /*
  Sleep(3000);
  test_svc.stop_all();
  add_method_tester cur_tester(one_proxy);
  cur_tester.do_task();
*/
}


