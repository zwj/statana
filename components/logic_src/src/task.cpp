#include <logic/common.h>/*{{{*/
#include "logic/task.h"
#include <boost/lexical_cast.hpp> 
#include <entity/xml_utility.h>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <fstream>
#include <logic/config_error.h>
#include <improve/time.h>/*}}}*/
using namespace boost;
using namespace std;
using namespace improve;
namespace logs
{
namespace logic
{

task::task(unsigned int task_id)
:_id(task_id),
_performance_logger(log4cpp::Category::getInstance("performance"))		
{
}
void
task::register_activity(activity::shared_ptr acty_ptr)
{/*{{{*/
	REQUIRE(acty_ptr); 	
	activity_pair pair;
	pair.in			= acty_ptr->get_inputer();
	pair.act		= acty_ptr;

	_activity_table.push_back(pair);
}/*}}}*/

void
task::execute()
{/*{{{*/
	performance_time task_timer;
	task_timer.start();
	activity_vector::iterator pos = _activity_table.begin();

	for(;pos!=_activity_table.end(); ++pos)
    {
        if(!pos->act->have_exed())
            execute_activity(*pos);
    }

    _data_board.output_results();
	task_timer.end();
	_performance_logger.infoStream()<<"task "<<_id<<" : "<< task_timer.duration_ms();
}/*}}}*/
	

void 
task::execute_activity(activity_pair pair)
{/*{{{*/
    if(pair.in != "")
    {
        activity_vector::iterator begin_pos = _activity_table.begin();
        activity_vector::iterator pos = begin_pos;
        activity_pair depended;
        while(pos != _activity_table.end())
        {
            if(pair.in == pos->act->get_out_filename())
            {
                depended = *pos;
                break;
            }
            ++pos;
        }

        if(!depended.act->have_exed())
            execute_activity(depended);
    }

    performance_time acty_timer;
    acty_timer.start();
    pair.act->execute(_data_board);
    acty_timer.end();
    _performance_logger.infoStream()<<
        pair.act->name()<<"."<<pair.act->get_out_filename()<<":"<<
        acty_timer.duration_ms();

}/*}}}*/

}
}


