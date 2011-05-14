#include "activity_test.h"
#include "entity_src/test/entity_support.h" 
#include <entity/apache_anar.h>
#include <logic/activity.h>
#include <logic/div_activity.h>
#include <logic/load_activity.h>
#include <logic/stat_activity.h>
#include <fstream>
#include "../../entity_src/test/entity_support.h"
#include "logic_support.h"
#include <logic/input.h>
#include <iostream>
using namespace std;
using namespace logs;
using namespace logs::logic;
using namespace logs::entity;
CPPUNIT_TEST_SUITE_REGISTRATION(activity_testcase); 
void
activity_testcase::load_acty_test()
{
	logitem_container*  	data_ptr = new logitem_container("inout");
	outputer::shared_ptr  data_container(data_ptr);
	istream_sptr src( new  ifstream("../data/access.log"));
    apache_analyser anar("%h,%l,%u,%t,%r,%>s,%b,%{Referer}i");
	load_activity load(src,"load_test",5,&anar,"test");
    data_board board;
	load.execute(board);
	inputer* items_ptr = data_ptr;
	unsigned int count=0;
	for(inputer::iterator pos = items_ptr->begin();
			pos != items_ptr->end(); ++pos)
	{
		++count;
	}
	cout<<WATCH(count)<<endl;
	CPPUNIT_ASSERT(count==5); 
	
}
void 
activity_testcase::collect_acty_test()
{
		
	support  test_support;	
	unsigned int total_size = test_support.logitems()->size();
    for(int i =0 ; i<100000; ++i)
    {
        data_board board;
        logitem_container::shared_ptr my_inputer(new test_inputer(test_support.logitems()));	
        board.regist_inputer("source",my_inputer);
        stat_result* result_ptr = new stat_result("./","ipStat");
        outputer::shared_ptr my_outputer(result_ptr);	
        stat_activity
                collect_it("source","stat_test","data");
        collect_it.unique_dimension("$IP+$TIME+$LEN");
        collect_it.execute(board);
    	cout<<WATCH(result_ptr->sum_count())<<endl;
    	cout<<WATCH(result_ptr->size())<<endl;
    	cout<<WATCH(total_size)<<endl;
    	CPPUNIT_ASSERT(result_ptr->size() < total_size); 
    	CPPUNIT_ASSERT(result_ptr->sum_count() == total_size); 
    }
}

