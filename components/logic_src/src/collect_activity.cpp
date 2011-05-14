#include <logic/common.h>
#include <logic/collect_activity.h>
#include <boost/tokenizer.hpp>
#include <entity/xml_utility.h>
#include <algorithm>
#include <queue>
using namespace std;
using namespace log4cpp; 
using namespace boost;
using namespace logs::entity;
namespace logs
{
namespace logic
{

collect_activity::collect_activity(	const string& in, 
									const string& out,
									const string& it_name )
:activity(in,out,it_name),
_logger(Category::getInstance(__FUNCTION__))
{}


str_vector 
collect_activity::unique_args(const string& str)
{/*{{{*/
	REQUIRE(str.length()>0); 	
	typedef tokenizer<char_separator<char> > ch_tokenizer;
	char_separator<char> sep("+");
	ch_tokenizer tokens(str, sep);
	ch_tokenizer::iterator pos = tokens.begin();
	str_vector args;
	for(;pos!= tokens.end();++pos)
	{
		args.push_back(*pos);	
	}
	return args;
		
}/*}}}*/

void 
collect_activity::unique_dimension(const std::string& dimension_name)
{/*{{{*/
	
   	str_vector args= unique_args(dimension_name); 
	if(args.size()==1)
	{
		string unifyed_arg = args[0];
		if(_logger.isDebugEnabled())
			_logger.debugStream()<<WATCH(unifyed_arg);
		_matcher_geter_ptr.reset(new static_matcher_geter(unifyed_arg));
	}
	else if( args.size() == 2 ) 
	{
			
		string unifyed_first  = args[0];
		string unifyed_second = args[1];
		if(_logger.isDebugEnabled())
		{
			_logger.debugStream()<<WATCH(unifyed_first);
			_logger.debugStream()<<WATCH(unifyed_second);
		}
		_matcher_geter_ptr.reset(new dyn_matcher_geter(unifyed_first,unifyed_second));
	}
	else
	{
		throw runtime_error(__FUNCTION__);
	}
}/*}}}*/

void
collect_activity::execute(data_board& board)
{/*{{{*/
	REQUIRE(_out_lable);
	REQUIRE(_in_lable); 

	outputer::shared_ptr out= board.classify_outputer(_out_lable);
	inputer::shared_ptr in = board.classify_inputer(_in_lable);
	inputer::iterator pos = in->begin();
	for(; pos!= in->end(); ++pos)
	{
		log_item::shared_ptr item = *pos;
		if(_matcher_geter_ptr)
		{
			matcher will_matcher = _matcher_geter_ptr->get_matcher(*item);
			in->output(will_matcher,item);	
		}
		else
		{
			matcher sum_matcher(dimension::shared_ptr(new str_dimension("SUM","SUM")));
			out->output(sum_matcher,item);
		}
	}
	out->haved_output();
	_status_control.move_to(exed);
}/*}}}*/


static_matcher_geter::static_matcher_geter(const string& name)	
:_name(name)		
{}	
matcher 
static_matcher_geter::get_matcher(log_item& item)		
{
	return item.get_matcher(_name);	
}		

dyn_matcher_geter::dyn_matcher_geter(const string& first, 
									 const string& second)	
:_first(first),_second(second)
{}	
matcher 
dyn_matcher_geter::get_matcher(log_item& item)		
{
	return item.get_matcher(_first,_second);	
}		

}
}
