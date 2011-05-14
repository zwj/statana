
#ifndef __RULE_FACTORY_TEST_H__
#define __RULE_FACTORY_TEST_H__
#include "test/test_base.h"
class rule_factory_testcase : public test_base
{
CPPUNIT_TEST_SUITE(rule_factory_testcase); 
CPPUNIT_TEST(analyse_test); 
CPPUNIT_TEST(config_error_test); 
CPPUNIT_TEST(symbol_test); 
CPPUNIT_TEST(rule_test); 
CPPUNIT_TEST_SUITE_END(); 
public:
	void analyse_test();
	void config_error_test();
    void symbol_test();
    void rule_test();
};
#endif
