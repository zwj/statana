#include <logic/common.h>
#include <iostream>
#include "logic/load_activity.h"
using namespace std;
using namespace log4cpp; 
using namespace logs::entity;
namespace logs
{
namespace logic
{

load_activity::load_activity(istream_sptr stream_in,	
							const string& out,
							unsigned int count,
                            const log_analyser* ana_ptr,
							const string& it_name
                            )
:activity("","",out,it_name),
_proc_count(count),
_stream_in(stream_in),
_analyser_ptr(ana_ptr)
{
    REQUIRE(stream_in);
}
void 
load_activity::load_item(unsigned int count, outputer::shared_ptr outer)
{/*{{{*/
	try
	{
		log_item::shared_ptr item_ptr(new log_item); 
        log_item& item=*item_ptr;
        _analyser_ptr->analyse(*_stream_in,item);
		if(_filter_rule_ptr && 
			! _filter_rule_ptr->is_true(item_ptr))
		{	
			if(_logger.isDebugEnabled())	
				_logger.debugStream()<<" filter it : "<< *item_ptr;	
		}
		else
		{
			outer->output(item_ptr);
		}
        if(_stream_in->bad()) 
            _stream_in->clear();
        if(_stream_in->fail())
            _stream_in->clear();
	}
	catch(data_error& e)
	{
		if(_stream_in->bad()) throw;
		if(_stream_in->eof()) return;
		_stream_in->clear();
		_stream_in->ignore(1024,'\n');
		if(_logger.isErrorEnabled())
			_logger.errorStream()<<e.what()<<"line:"<<count;
	}
	catch(exception& ex)
	{
		_stream_in->ignore(1024,'\n');
		_logger.errorStream()<<ex.what()<<"line:"<<count;
	}
}/*}}}*/

void
load_activity::execute(data_board& board)
{/*{{{*/
	_status_control.move_to(exed);

	outputer::shared_ptr outer = board.classify_outputer(_out_path, _out_filename);
	try
	{
        size_t count=1;
		for(; (!_stream_in->eof()) &&  (count<= _proc_count); ++count)
		{
			load_item(count,outer);	
		}
        if(_stream_in->fail())
			_logger.errorStream()<<" file stream error : "<< _stream_in->rdstate();
    	if(_logger.isDebugEnabled())	
			_logger.debugStream()<<" log item : "<< count <<" have loaded";	
	}
	catch(...)
	{
		_logger.errorStream()<<" unkonw error happend " ;	
	}
	outer->haved_output();
}/*}}}*/

void
load_activity::register_rule(rule::shared_ptr filter_rule)
{
	REQUIRE(filter_rule.get()); 		
	REQUIRE(_filter_rule_ptr.get()==0); 
	_filter_rule_ptr =  filter_rule;
}

}
}
