#ifndef __DIM_RULE_TESTCASE__
#define __DIM_RULE_TESTCASE__
#include <test/test_base.h> 
#include <entity/log_item.h>
class dimension_rule_testcase : public  test_base
{
CPPUNIT_TEST_SUITE(dimension_rule_testcase); 
CPPUNIT_TEST(rule_test); 
CPPUNIT_TEST_SUITE_END(); 
public:
	void rule_test();
	void rule_args_equal(logs::entity::log_item::shared_ptr item);
	void rule_args_not_equal(logs::entity::log_item::shared_ptr item);
	void rule_length_test(logs::entity::log_item::shared_ptr item);
};
#endif
