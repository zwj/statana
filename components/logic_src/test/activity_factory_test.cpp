#include "activity_factory_test.h"
#include <logic/activity_factory.h>
#include <logic/activity.h>
#include <logic/data_board.h>
#include <fstream>
using namespace std;
using namespace logs;
using namespace logs::logic;
using namespace logs::entity;
CPPUNIT_TEST_SUITE_REGISTRATION(activity_facotry_testcase); 

void 
activity_facotry_testcase::crt_collect_act()
{
    string str="@uv:        stat(@left,$IP); ";
    activity_factory  factory;
    activity::shared_ptr  act_ptr = factory.create_collect_acty(str,"/home/zwj/");
    CPPUNIT_ASSERT(act_ptr != NULL);
}

void 
activity_facotry_testcase::crt_div_act()
{
    string str="@left:div(@source,($IP > 192.168.0.110 && $IP < 192.168.0.120 ));";
    string str1="@stat2 :   div(@source,( $FILE == stat2.html)) ;";
    string str2="@stat2 :   div(@source,($FILE == stat2.html)) ;";
    activity_factory  factory;
    activity::shared_ptr  act_ptr = factory.create_classify_acty(str);
    activity::shared_ptr  act_ptr2 = factory.create_classify_acty(str2);
    activity::shared_ptr  act_ptr1 = factory.create_classify_acty(str1);
    CPPUNIT_ASSERT(act_ptr != NULL);
    CPPUNIT_ASSERT(act_ptr1 != NULL);
    CPPUNIT_ASSERT(act_ptr2 != NULL);

}

void 
activity_facotry_testcase::crt_load_act()
{
    string str="@souce:     load( $Date > 2005-06-07 )";
    activity_factory  factory;
    istream_sptr file_sptr(new  fstream("../test/activity_factory.cpp"));
    factory.set_load_param(file_sptr,10);
    activity::shared_ptr  act_ptr = factory.create_load_acty(str);
    CPPUNIT_ASSERT(act_ptr != NULL);


    str="@souce:     load()";
    act_ptr = factory.create_load_acty(str);
    CPPUNIT_ASSERT(act_ptr != NULL);
}
