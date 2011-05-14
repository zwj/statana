#ifndef __LOG_ITEM_TEST_H__
#define __LOG_ITEM_TEST_H__
#include <test/test_base.h>
class log_item_testcase : public test_base 
{
CPPUNIT_TEST_SUITE(log_item_testcase);
CPPUNIT_TEST(constrcut_test); 
CPPUNIT_TEST(input_usecase);
CPPUNIT_TEST(multi_constrcut); 
CPPUNIT_TEST(unique_test); 
CPPUNIT_TEST(ctype_test); 
CPPUNIT_TEST(performace_test); 
CPPUNIT_TEST(apache_constrcut_test); 
CPPUNIT_TEST_SUITE_END(); 
public:
	void constrcut_test();
	void apache_constrcut_test();
	void input_usecase();
	void multi_constrcut();
	void unique_test();
	void ctype_test();
	void performace_test();
};
#endif
