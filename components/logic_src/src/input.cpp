#include <logic/common.h>
#include <logic/input.h>
#include <algorithm>
#include <numeric>
#include <logic/config_error.h>
#include <algorithm>
#include <queue>
#include <fstream>
#include <iostream>
#include <boost/algorithm/string.hpp>



using namespace std;
using namespace boost;
using namespace logs::entity;
namespace logs
{
namespace logic
{

load_inputer::load_inputer(const string& the_name)
:_name(the_name)
{}
string 
load_inputer::name()
{
	return _name;
}

inputer::iterator 
load_inputer::begin()
{
	config_error error(_WHERE);
	error.stream()<<"load inputer can't call begin, config error!"<<endl;
	throw error;
}

inputer::iterator 
load_inputer::end()
{
	config_error error(_WHERE);
	error.stream()<<"load inputer can't call end, config error!"<<endl;
	throw error;
}

bool
load_inputer::ready()
{
	return true;
}

logitem_container::logitem_container(const string& the_name)
:
_name(the_name)
{/*{{{*/
	_status_ctrl.add(unoutput,outputed);
	_status_ctrl.bidirec_moveable(unoutput,outputed);
	_status_ctrl.self_moveable(outputed);
	_status_ctrl.init_status(unoutput);
	_vector.reserve(10000);
}/*}}}*/

string 
logitem_container::name()
{
	return _name;
}
inputer::iterator 
logitem_container::begin()
{
	return _vector.begin();
}

inputer::iterator 
logitem_container::end()
{
	return _vector.end();
}

bool
logitem_container::ready()
{
	return _status_ctrl.current_status() == outputed;
}
void 
logitem_container::output(const matcher& the_matcher,
					log_item::shared_ptr item )
{
	REQUIRE(false); 
}
void 
logitem_container::output(const log_item::shared_ptr item )
{
	_vector.push_back(item);
}
void
logitem_container::haved_output()
{
	_status_ctrl.move_to(outputed);
}
void
stat_result::output(const matcher& the_matcher,log_item::shared_ptr item )
{/*{{{*/
	matcher_count_map::iterator pos = _stat_map.find(the_matcher);
	if(pos!=_stat_map.end())
	{
		++((*pos).second);
	}
	else
	{
		_stat_map.insert(make_pair(the_matcher,1));
	}
}/*}}}*/

stat_result::stat_result(const std::string& the_path,const std::string& the_name)
:_path(the_path),_name(the_name)
{
}
void 
stat_result::output(const log_item::shared_ptr item )
{
	REQUIRE(false); 
}
unsigned int 
stat_result::size()
{
	return _stat_map.size();
}
unsigned int sum(unsigned int init,matcher_count value)
{
	return init+= value.second;
}
unsigned int 
stat_result::sum_count()
{
	return accumulate(_stat_map.begin(),_stat_map.end(),0,sum);
}
string
stat_result::name()
{
	return _name;
}
	
class count_less
{/*{{{*/
	public:		
	bool 
	operator()(const matcher_count left, const matcher_count& right) const
	{
		return left.second < right.second;
	}
};/*}}}*/

typedef priority_queue<matcher_count,vector<matcher_count>,count_less > count_pqueue;	


string 
stat_result::result_name()
{/*{{{*/
	string name= _path+"/"+_name+".stat";
    erase_all(name, string("@"));
    erase_all(name, string("$"));
    erase_all(name, string("%"));
    return name;
}/*}}}*/
void
stat_result::haved_output()
{}
void 
stat_result::save()
{/*{{{*/
	string name= result_name(); 	
    ofstream stat_file(name.c_str(),ios::app);	
	matcher_count_map::iterator pos = _stat_map.begin();
    int count =0 ;
	for(;pos!=_stat_map.end();++pos,++count)
	{
		matcher_count count = *pos;
		stat_file<<count.second<<SEPARATOR<<count.first.value()<<endl;	
	}
    if (_logger.isDebugEnabled())
        _logger.debugStream() << "stat item output:"<<count;

}/*}}}*/

stat_result:: ~stat_result()
{
}
}
}
