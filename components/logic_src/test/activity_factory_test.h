#ifndef __ACTIVITY_FACTORY_TEST_H__
#define __ACTIVITY_FACTORY_TEST_H__
#include <test/test_base.h>
class activity_facotry_testcase : public test_base
{
	CPPUNIT_TEST_SUITE(activity_facotry_testcase); 
	CPPUNIT_TEST(crt_collect_act); 
	CPPUNIT_TEST(crt_div_act); 
	CPPUNIT_TEST(crt_load_act); 
	CPPUNIT_TEST_SUITE_END();
public :	
    void crt_collect_act();
    void crt_div_act();
    void crt_load_act();
};
#endif
