// service_testcase.cpp: implementation of the service_testcase class.
//
//////////////////////////////////////////////////////////////////////

#include "service_testcase.h"
#include <windows.h>
#include <iostream>
#include <improve/service.h>
#include <boost/ref.hpp>
#include <stdexcept>
#include <boost/thread.hpp>
#include <improve/utility.h>
#include <improve/depository.h>
#include <map>
#include <algorithm>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include "test_support.h"
using namespace std;

using namespace improve;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPPUNIT_TEST_SUITE_REGISTRATION( service_testcase );
class no_copy_data : private boost::noncopyable  
{
public:
    no_copy_data(string& data)
        :_data(data)
    {
        Sleep(2000);
    }
    no_copy_data(const no_copy_data& other)
        :_data(other._data)
    {
    }
    string _data;
};
class  service1_tester :public  worker
{
public:
    service1_tester(no_copy_data& name)
        :_name(name._data)
    {
        Sleep(2000);
    }
    void do_task()
    {
        Sleep(1000);
        cout<<_name<<endl;
    }
    void log_exception(exception& e){}
    string _name;
};

class  adder :public  worker , public class_mock_test<int>
{
public:
    adder(int add_num)
        :_add_num(add_num)
    {
    }
    void do_task()
    {
        boost::mutex::scoped_lock locker(_add_mutex);
        _s_original_value+=_add_num;
    }
    void log_exception(exception& e){}
    static boost::mutex _add_mutex;
    int _add_num;
};

boost::mutex adder::_add_mutex;

class exception_tester : public worker
{
public:
    exception_tester()
    {
        throw runtime_error("test exception");
    }
    void log_exception(exception& e){}
    void do_task()
    {
        bool no_excute=false;
        CPPUNIT_ASSERT(no_excute);
        
    }
};
class container_class_mock_test
{
public:
    static void expectation(vector<int>& value)
    {
        //_s_original_value=0;
        _s_expect_value=value;
    }
    static bool verify()
    {
        return std::equal(  _s_expect_value.begin(),
            _s_expect_value.end(),
            _s_original_value.begin());
        
    }
protected:
    static vector<int> _s_original_value;
    static vector<int> _s_expect_value;
};

vector<int> container_class_mock_test::_s_expect_value;
vector<int> container_class_mock_test::_s_original_value;

class executor;
class bean_factory : public worker , public container_class_mock_test
{
public:
    void log_exception(exception& e){assert(false);}
    bean_factory()
        :_times(0)
    {
    }
    void do_task()
    {
        
        export_bean();
        Sleep(_s_user_ms);
        log_workers_num();
        _s_bean_export.notify_all();
        ++_times;
    }
    void export_bean()
    {
        int i=_s_export_plan[_times];
        for(;i>0; --i )
        {
            _s_bean_box.put(_s_bean);
        }
        
    }
    void log_workers_num()
    {
        cout<<"worker:"<<_s_works_num<<endl;
        _s_original_value.push_back(_s_works_num);
    }
    
private:
    int _times;
    
    
public:
    static depository<int>      _s_bean_box;
    static vector<int>          _s_export_plan; 
    static int                  _s_bean;
    static int                  _s_user_ms;
    static boost::condition     _s_bean_export;  
    static int                  _s_works_num;
};

depository<int>  bean_factory::_s_bean_box;
vector<int>      bean_factory::_s_export_plan; 
int              bean_factory::_s_bean=1;
int              bean_factory::_s_user_ms=1000;
int              bean_factory::_s_works_num;
boost::condition bean_factory::_s_bean_export;



class bean_trader : public dyn_worker 
{
public:
    
    bean_trader(dyn_workers_manager_base* manager_ptr)
        :dyn_worker(manager_ptr)
    {
        ++bean_factory::_s_works_num;
    }
    ~bean_trader()
    {
        --bean_factory::_s_works_num;
    }
    void log_exception(exception& e){assert(false);}
    bool get_task_data()
    {
        unsigned int timeout_ms=1000;
        int value;
        return bean_factory::_s_bean_box.timed_get(value,timeout_ms);
        
    }
    void busy_do()
    {
        boost::mutex::scoped_lock lk(_mutex);
        bean_factory::_s_bean_export.wait(lk);
    }
    boost::mutex                _mutex;
};





void
service_testcase::main()
{
    adder::expectation(5);
    service1<adder,int> service1_test(1,5);
    service1_test.join();
    CPPUNIT_ASSERT(adder::verify());
    
    adder::expectation(25);
    service1_manage<adder,int >  
        service1s_test(1,5,5);       
    service1s_test.join_all();    
    CPPUNIT_ASSERT(adder::verify());
}
void
service_testcase::timed_join()
{
    
    
    
    no_copy_data zwj(string("zwj"));
    
    service1<service1_tester,boost::reference_wrapper<no_copy_data> > 
        service1_test(boost::ref(zwj));
    unsigned int wait_ms=2000;
    service1_test.timed_join(wait_ms);
}
void
config_user::do_task()
{     
    Sleep(500);
}

void 
service_testcase::test_exception()
{
    try
    {
        service<exception_tester> svc(1);
        bool exception_test=false;
        CPPUNIT_ASSERT(exception_test);
    }
    catch(exception& e)
    {
        cout<<e.what()<<endl;
    }
    
}
void service_testcase::add_service_test()
{
    service1_manage<adder,int >  
        service1s_test;        
    adder::expectation(50);
    service1s_test.add_service(2,5);
    service1s_test.add_service(4,5);
    service1s_test.add_service(3,5);
    service1s_test.add_service(1,5);
    service1s_test.add_service(0,5);
    service1s_test.join_all();
    CPPUNIT_ASSERT(adder::verify());
}
class bean_trader_manager : public dyn_workers_manager<bean_trader>
{
    void log(const char* name, int i)
    { 
        cout<< name <<" : " << i <<endl;
    }
};
void 
service_testcase::dyn_services_test()
{
    
    
    
    int plan[] = {10,7,5,20,17,14,7,30,26,20,5,0};
    int plan_len=sizeof(plan)/sizeof(plan[0]);
    vector<int> expect_workers;
    for(int i=0 ; i<plan_len; ++i )
    {
        bean_factory::_s_export_plan.push_back(plan[i]);
        expect_workers.push_back(plan[i]+5);
    }
    
    bean_factory::expectation(expect_workers);
    service<bean_trader_manager> dyn_svr;
    service<bean_factory>   bean_exporter(plan_len);
    
    
    
    bean_exporter.join();
    dyn_svr.timed_join(10);
    CPPUNIT_ASSERT(bean_factory::verify());
}