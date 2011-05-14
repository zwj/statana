#include <logic/common.h>
#include <logic/data_board.h>
#include <boost/algorithm/string.hpp>
using namespace boost;
using namespace std;
using namespace improve;
namespace logs
{
namespace logic
{

outputer::shared_ptr 
data_board::stat_outputer(const string& path, const string& name)
{/*{{{*/
	stat_result::name_map::iterator pos = _results.find(name);
	if(pos == _results.end())
	{
        string file_name = name;
        to_lower(file_name);
		stat_result::shared_ptr stat_ptr(new stat_result(path,file_name));
		 _results.insert(make_pair(name,stat_ptr));
		 return shared_dynamic_cast<outputer>(stat_ptr);
	}
	return shared_dynamic_cast<outputer>((*pos).second);
}/*}}}*/

logitem_container::shared_ptr 
data_board::get_logitem_container(const string& name)
{/*{{{*/
	logitem_container::name_map::iterator pos = _process.find(name);
	if(pos == _process.end())
	{
		logitem_container::shared_ptr container_ptr(new logitem_container(name));
		_process.insert(make_pair(name,container_ptr));
		return container_ptr;
	}
	return (*pos).second;
}/*}}}*/

outputer::shared_ptr 
data_board::classify_outputer(const string& path, const string& name)
{/*{{{*/
	try
	{
		logitem_container::shared_ptr container_ptr = get_logitem_container(name);
		return shared_dynamic_cast<outputer>(container_ptr);
	}
	catch(std::exception& e)
	{
		rt_exception rt_error(_WHERE);
		rt_error.stream()<<e.what()<< WATCH(name);
		throw rt_error;
	}
}/*}}}*/

inputer::shared_ptr 
data_board::classify_inputer(const string& name)
{/*{{{*/
	try
	{
		logitem_container::shared_ptr container_ptr = get_logitem_container(name);
		return shared_dynamic_cast<inputer>(container_ptr);
	}
	catch(std::exception& e)
	{
		rt_exception rt_error(_WHERE);
		rt_error.stream()<<e.what()<< WATCH(name);
		throw rt_error;
	}
}/*}}}*/


void 
data_board::regist_inputer(const std::string& name,logitem_container::shared_ptr in)
{
	logitem_container::name_map::iterator pos = _process.find(name);
	if(pos == _process.end())
	{
		_process.insert(make_pair(name,in));
        return ;
	}
    ENSURE(false);
}

void
data_board::clear_process()
{/*{{{*/
    _process.clear();
}/*}}}*/
void 
save_it(stat_result::name_sptr_pair& pair)
{/*{{{*/
	pair.second->save();
}/*}}}*/
void 
data_board::output_results()
{/*{{{*/
	for_each(_results.begin(),_results.end(),save_it);
}/*}}}*/
}
}
