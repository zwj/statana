#include "rule_express_test.h"
#include <improve/rule_express/rule_express.h>
#include <improve/rule_express/logic_express.h>
using namespace std; 
using namespace boost; 

//using namespace improve;
typedef improve::rule<int> int_rule;
typedef improve::or_rule<int> int_or_rule;
typedef improve::and_rule<int> int_and_rule;
typedef improve::not_rule<int> int_not_rule;
CPPUNIT_TEST_SUITE_REGISTRATION( rule_express_testcase );

class true_rule : public int_rule
{
public:		
    
	bool is_true(const int& obj) const
	{
		return true;	
	}
	string description() const
	{
		return "true";	
	}
	
};

class false_rule : public int_rule
{
public:		
	bool is_true(const int& obj) const
	{
		return false;	
	}
	string description() const 
	{
		return "false";	
	}
};

void
rule_express_testcase::or_test()
{
	int_or_rule one(int_rule::shared_ptr(new false_rule),
					int_rule::shared_ptr(new true_rule));	
	int v=1;
	CPPUNIT_ASSERT(one.is_true(v));		
	
	int_or_rule two(int_rule::shared_ptr(new true_rule()),
					int_rule::shared_ptr(new true_rule()));	
	CPPUNIT_ASSERT(two.is_true(v));		

	int_or_rule three(int_rule::shared_ptr(new true_rule()),
					int_rule::shared_ptr(new false_rule()));	
	CPPUNIT_ASSERT(three.is_true(v));		
	
	int_or_rule four(int_rule::shared_ptr(new false_rule()),
					int_rule::shared_ptr(new false_rule()));	
	CPPUNIT_ASSERT(!four.is_true(v));		
	
}
void
rule_express_testcase::and_test()
{
	int_and_rule one(int_rule::shared_ptr(new false_rule()),
					int_rule::shared_ptr(new true_rule()));	
	int v=1;
	CPPUNIT_ASSERT(!one.is_true(v));		
	
	int_and_rule two(int_rule::shared_ptr(new true_rule()),
					int_rule::shared_ptr(new true_rule()));	
	CPPUNIT_ASSERT(two.is_true(v));		

	int_and_rule three(int_rule::shared_ptr(new true_rule()),
					int_rule::shared_ptr(new false_rule()));	
	CPPUNIT_ASSERT(!three.is_true(v));		
	
	int_and_rule four(int_rule::shared_ptr(new false_rule()),
					int_rule::shared_ptr(new false_rule()));	
	CPPUNIT_ASSERT(!four.is_true(v));		
}
void
rule_express_testcase::not_test()
{
    int_rule::shared_ptr x(new false_rule());
	int_not_rule one(x);
	int v=1;
	CPPUNIT_ASSERT(one.is_true(v));		
	
	int_not_rule two(int_rule::shared_ptr(new true_rule()));
	CPPUNIT_ASSERT(!two.is_true(v));		

}
