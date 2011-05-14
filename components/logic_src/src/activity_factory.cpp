#include <logic/common.h>
#include <logic/activity_factory.h>
#include <entity/utility.h>
#include <boost/lexical_cast.hpp>
#include <logic/stat_activity.h>
#include <logic/load_activity.h>
#include <logic/div_activity.h>
#include <logic/config_error.h>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
using namespace boost;
using namespace std;
using namespace logs::entity;
namespace logs
{
namespace logic
{

const string activity_lable("activity");
const string type_lable("type");
const string input_lable("input");
const string output_lable("output");
const string unique_lable("unique");

const string load_acty_flag("LOAD");
const string classify_acty_flag("DIV");
const string collect_acty_flag("STAT");

activity_factory::activity_factory()
:logic_base(), _proc_count(0)
{}

activity_factory::ACT_TYPE
activity_factory::get_act_type( const string& exp )
{/*{{{*/
    const string load_pattern("@\\S+\\s*:\\s*([a-z]{3,5})\\s*\\(");
	static regex pattern (load_pattern);
	match_results<const char*> what;
	if(regex_search(exp.c_str(),what,pattern))
    { 
	    string type = 	utility::unify(string(what[1].first,what[1].second));
        if(load_acty_flag     == type) return activity_factory::LOAD;
        if(collect_acty_flag  == type) return activity_factory::STAT;
        if(classify_acty_flag == type) return activity_factory::DIV;
    }
    config_error error(_WHERE);
    error.stream()<<" activity type analyse error, exp:"<<exp;
    throw error;
}/*}}}*/
activity::shared_ptr
activity_factory::create_acty(const string& express , 
                              const string& params)
{/*{{{*/
    if (_logger.isDebugEnabled())
        _logger.debugStream() << "activity type :" << express;
    switch(get_act_type(express))
    {
        case LOAD:
            return create_load_acty(express);
        case STAT:
            return create_collect_acty(express,params);
        case DIV:
            return create_classify_acty(express);
    }
    config_error error(_WHERE);
    error.stream()<<"crate activity error, exp:"<<express;
    throw error;
}/*}}}*/
void 
activity_factory::set_analyser(log_analyser::shared_ptr anar)
{
    _analyser_sptr = anar;
}
void
activity_factory::set_load_param(istream_sptr src_stream,
                                 unsigned int proc_count)
{/*{{{*/
    REQUIRE(src_stream);
    REQUIRE(proc_count > 0);
    _src_stream = src_stream;
    _proc_count = proc_count;
    ENSURE(_proc_count == proc_count);
}/*}}}*/



bool 
activity_factory::analyse_load_exp( const string& exp, string& to, string& rule_exp)
{/*{{{*/
    const string load_pattern("(@\\S+)\\s*:\\s*load(.*)");
	static regex pattern (load_pattern);
	match_results<const char*> what;
	if(!regex_search(exp.c_str(),what,pattern)) return false;
	to = 	utility::unify(string(what[1].first,what[1].second));
	rule_exp= 	string(what[2].first,what[2].second);
    erase_all(rule_exp, string(")"));
    erase_all(rule_exp, string("("));
    erase_all(rule_exp, string(";"));
    return true;
}/*}}}*/

activity::shared_ptr
activity_factory::create_load_acty(const string& express )
{/*{{{*/

    REQUIRE(_src_stream);
    string to,from,rule_exp;
    if(!analyse_load_exp(express,to,rule_exp))
    {
		config_error error(_WHERE);
		error.stream()<<"load activity analyse error, exp:"<<express;
		throw error;
    }
   if (_logger.isDebugEnabled())
        _logger.debugStream() <<WATCH(to);

    load_activity* load_acty = new load_activity(_src_stream, 
                                                to,
												_proc_count,
                                                _analyser_sptr.get());
    activity::shared_ptr acty(load_acty);

    if(_rule_factory.is_rule_express(rule_exp))
    {
        rule::shared_ptr filter_rule = _rule_factory.create_rule(rule_exp);
        load_acty->register_rule(filter_rule);
    }
    return acty;
}/*}}}*/


bool 
activity_factory::analyse_div_exp( const string& exp, string& to, string& from,string& rule)
{/*{{{*/
    const string div_pattern("(@\\S+)\\s*:\\s*div\\((\\s*@\\S+)\\s*,(.+)\\)");
	static regex pattern (div_pattern);
	match_results<const char*> what;
	if(!regex_search(exp.c_str(),what,pattern)) return false;
	to = 	utility::unify(string(what[1].first,what[1].second));
	from = 	utility::unify(string(what[2].first,what[2].second));
	rule = 	(string(what[3].first,what[3].second));
    return true;
}/*}}}*/
activity::shared_ptr
activity_factory::create_classify_acty(const string& express )
{/*{{{*/

    string to,from,rule_exp;
    if(!analyse_div_exp(express,to,from,rule_exp))
	{
		config_error error(_WHERE);
		error.stream()<<"div activity analyse error, exp:"<<express;
		throw error;
	}

    if (_logger.isDebugEnabled())
    {
        _logger.debugStream() <<WATCH(from)<<";"<<WATCH(to);
    }
	rule::shared_ptr classify_rule = _rule_factory.create_rule(rule_exp);

    classify_activity* acty = new classify_activity( from,to);
    activity::shared_ptr acty_shptr(acty);
    acty->register_rule(classify_rule);
    if (_logger.isDebugEnabled())
        _logger.debugStream() << "classify rule:" << classify_rule->description();
    return acty_shptr;
}
/*}}}*/

bool 
activity_factory::analyse_unique_exp( const string& exp, string& to, string& from,string& var)
{/*{{{*/
    const string unique_pattern("(@\\S+)\\s*:\\s*stat\\((\\s*@\\S+)\\s*,(.+)\\)");
	static regex pattern (unique_pattern);
	match_results<const char*> what;
	if(!regex_search(exp.c_str(),what,pattern)) return false;
	to = 	utility::unify(string(what[1].first,what[1].second));
	from = 	utility::unify(string(what[2].first,what[2].second));
	var = 	string(what[3].first,what[3].second);
    return true;
}/*}}}*/

activity::shared_ptr
activity_factory::create_collect_acty(const string& express , const string& path)
{/*{{{*/


    string to,from,var;
    if(!analyse_unique_exp(express,to,from,var))
    {
		config_error error(_WHERE);
		error.stream()<<"stat activity analyse error, exp:"<<express;
		throw error;
    }
    if (_logger.isDebugEnabled())
        _logger.debugStream() <<WATCH(from)<<";"<<WATCH(to) ;

    stat_activity* acty = new stat_activity(from,path,to);
    activity::shared_ptr acty_shptr(acty);
    if(var.length() > 0)
    {
        trim(var);
        acty->unique_dimension(var);
    }
    return acty_shptr;
}/*}}}*/

}
}
