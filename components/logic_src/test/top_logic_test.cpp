#include "top_logic_test.h" 
#include <improve/utility.h>
#include "logic/filter.h"
#include <entity/ip.h>
#include "logic/data_loader.h" 
#include "entity/activity.h" 
#include "entity/task.h" 
CPPUNIT_TEST_SUITE_REGISTRATION(top_logic_testcase ); 

void 
top_logic_testcase::filter_test()
{
	filter::shared_ptr  data_filter(new filter());
	dimension::shared_ptr one(new ip(192,168,0,1));
	dimension::shared_ptr two(new ip(192,168,0,100));
	rule::shared_ptr large_it(new large_rule(one));
	rule::shared_ptr  less_it(new less_rule(two));	
	data_filter->register_rule(rule::shared_ptr(new and_rule(large_it,less_it)));
	data_loader loader;
	loader.register_filter(data_filter);
	loader.load_data();

}
void 
top_logic_testcase::classify_test()
{
	data_loader loader;
	loader.load_data();
	const logitems_container*  logitems = loader.source();
	classify_activity test_activity;
	test_activity.input(logitems);
	
	dimension::shared_ptr one(new ip(192,168,0,1));
	rule::shared_ptr large_it(new large_rule(one));
	test_activity.classify(large_it);
	test_activity.classified_left();
	test_activity.classified_right();
	test_activity.output();
	
}
void 
top_logic_testcase::collect_test()
{
	data_loader loader;
	loader.load_data();
	const logitems_container*  logitems = loader.source();
	stat_activity test_activity;
	test_activity.input(logitems);
	test_activity.unique_dimension("ip");
	test_activity.collect();
	test_activity.output();
	
}

void
top_logic_testcase::task_test()
{
	data_loader loader;
	loader.load_data();
	classify_activity::shared_ptr test_classify(new classify_activity);
	stat_activity::shared_ptr test_collect(new stat_activity);
	dimension::shared_ptr one(new ip(192,168,0,1));
	rule::shared_ptr large_it(new large_rule(one));
	test_classify->classify(large_it);
	
	test_collect->unique_dimension("ip");
	task test_task;
	test_task.register_activity(test_classify);
	test_task.register_activity(test_collect);
	test_task.execute();

}
