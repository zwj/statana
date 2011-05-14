// channel_testcase.cpp: implementation of the channel_testcase class.
//
//////////////////////////////////////////////////////////////////////

#include "depository_testcase.h"
#include "../depository.h"
#include "../service.h"
#include <utility>
#include <windows.h>
#include "../utility.h"
#include "test_support.h"
CPPUNIT_TEST_SUITE_REGISTRATION( depository_testcase );
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////
//13530137325
// support to unit test;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

using namespace std;
using namespace improve;

typedef depository<int> paper_depository;

class paper_reader : public worker , public class_mock_test<int>
{
public:
  paper_reader(paper_depository* where)
  :_newsstand_ptr(where)
  {}
  void log_exception(exception& e){ assert(false);};
  void do_task()
  {
    int hand;
    if(_newsstand_ptr->timed_get(hand,2000))
    {
      ++_s_original_value;
    }
    else
    {
      bool timed_get=false;
      CPPUNIT_ASSERT(timed_get);
    }
  }
private:
  paper_depository* _newsstand_ptr;
  
};
class paper_issuer :public worker
{
public:
  paper_issuer(paper_depository* where)
  :_newsstand_ptr(where)
  {}
  void log_exception(exception& e){assert(false);}
  void do_task()
  {
   
    int paper=create_serial_no();
    _newsstand_ptr->put(paper);

  }
  static int create_serial_no()
  {
    boost::mutex::scoped_lock lk(_s_mutex);
    return ++_s_serial_no;
  }
private:
  static int _s_serial_no;
  static boost::mutex _s_mutex;
  paper_depository* _newsstand_ptr;
  
};
int           paper_issuer::_s_serial_no=0;
boost::mutex  paper_issuer::_s_mutex;

void
depository_testcase::test_depository()
{
  paper_depository newsstand;
  int readers_num=10;
  int issures_num =2;
  int papers_num=100;
  paper_reader::expectation(papers_num);
  service1_manage<paper_issuer,paper_depository*> 
  issures_svrs(&newsstand,issures_num,papers_num/issures_num);

  service1_manage<paper_reader,paper_depository*> 
  readers_svrs(&newsstand,readers_num,papers_num/readers_num);

  readers_svrs.join_all();
  issures_svrs.join_all();
}

