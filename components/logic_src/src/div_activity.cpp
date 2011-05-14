#include <logic/common.h>
#include <iostream>
#include "logic/div_activity.h"
using namespace std;
using namespace log4cpp; 
using namespace logs::entity;
namespace logs
{
namespace logic
{


classify_activity::classify_activity(const std::string& in , const std::string& out,
        const std::string& it_name)
:activity(in,"",out,it_name)
{}

void
classify_activity::register_rule(rule::shared_ptr classify_rule)
{
	REQUIRE(classify_rule.get()); 		
	REQUIRE(_classify_rule_ptr.get()==0); 
	_classify_rule_ptr =  classify_rule;
}

void 
classify_activity::execute(data_board& board)
{/*{{{*/
	outputer::shared_ptr out= board.classify_outputer(_out_path, _out_filename);
	inputer::shared_ptr in = board.classify_inputer(_in_lable);
	inputer::iterator pos = in->begin();
    int count=0 ;
	for(; pos!= in->end(); ++pos)
	{
		log_item::shared_ptr item = *pos;
		if(_classify_rule_ptr->is_true(item))
        {
            ++count;
			out->output(item);
        }
	}
    
    if (_logger.isDebugEnabled())
        _logger.debugStream() << "div logitem count:"<<count;
	out->haved_output();
	_status_control.move_to(exed);
}/*}}}*/

}
}
