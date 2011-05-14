#include "task_test.h" 
#include <logic/task.h>
#include "logic_support.h"
#include <entity_src/test/entity_support.h>
#include <logic/div_activity.h>
#include <logic/load_activity.h>
#include <logic/stat_activity.h>
#include <iostream>

using namespace std;
using namespace boost;
using namespace logs;
using namespace logs::entity;
using namespace logs::logic;
CPPUNIT_TEST_SUITE_REGISTRATION(task_testcase); 
unsigned int 
task_testcase::get_count(const ip& which,const log_item::vector* logs)
{
	unsigned int count=0;
	log_item::vector::const_iterator pos = logs->begin();
	for(;pos!=logs->end(); ++pos)
	{
		log_item::shared_ptr cur_item = *pos;
        const dimension* cur= cur_item->item_dim(item_def::col_of(which.name()));
		if(*cur==which) count++;
	}
	return count;
}
void
task_testcase::usecase()
{
	support  test_support;	
	task test_task(1);
	ip ip142(192,168,140,142);
	unsigned int count142 = get_count(ip142,test_support.logitems());

	inputer::shared_ptr source(new test_inputer(test_support.logitems()));
	logitem_container::shared_ptr son_data(new logitem_container("son_data"));
	logitem_container::shared_ptr father_data(new logitem_container("father_data"));
	stat_result::shared_ptr ip_stat(new stat_result("./","ip_stat"));
	stat_result::shared_ptr ip_stat1(new stat_result("./","ip_stat1b"));

	classify_activity::shared_ptr 
		classify_setp2(new classify_activity("in","out"));
	ip two(192,168,140,143);
    matcher m = matcher(&two);
	rule::shared_ptr less_ip(new less_rule(m));
	classify_setp2->register_rule(less_ip);
	
	test_task.register_activity(classify_setp2);
						

	classify_activity::shared_ptr 
			classify_step1(new classify_activity("in","out"));
	ip one(192,168,140,141);
    matcher m1 = matcher(&one);
	rule::shared_ptr large_ip(new large_rule(m1));
	classify_step1->register_rule(large_ip);
	test_task.register_activity(classify_step1);
							
								
	stat_activity::shared_ptr 
	collect_it(new stat_activity("in","out","data"));
	collect_it->unique_dimension("$IP");
	test_task.register_activity(collect_it);
					
					

	stat_activity::shared_ptr 
	collect_it1(new stat_activity("in","out","data"));
	collect_it1->unique_dimension("$IP");
	test_task.register_activity(collect_it1);
	
	test_task.execute();
	cout<<"142:"<<count142<<endl;
	cout<<WATCH(ip_stat->size())<<endl;
	cout<<WATCH(ip_stat->sum_count())<<endl;
	cout<<WATCH(ip_stat1->size())<<endl;
	cout<<WATCH(ip_stat1->sum_count())<<endl;
	CPPUNIT_ASSERT(ip_stat->sum_count() == count142); 

}
