#include <logic/common.h>
#include <logic/rule_factory.h>
#include <entity/utility.h>
#include <boost/regex.hpp>
#include <logic/config_error.h>
#include <iostream>
#include <boost/tokenizer.hpp>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>

using namespace  boost;
using namespace std; 
using namespace logs::entity;
namespace logs
{
namespace logic
{

const string rule_lable("rule");
const string type_lable("type");
const string express_lable("exp");
namespace rule_symbol
{
	const string equal("==");
	const string unequal("<>");
	const string large(">");
	const string less("<");
	const string large_equal(">=");
	const string less_equal("<=");
}

namespace rule_type
{
	const string AND("&&");
	const string OR("||");
	const string NOT("!");
}

rule_factory::rule_factory()
{}

bool
rule_factory::is_rule_express(const string& express)
{
    return is_relation_exp(express) || is_compare_exp(express);
}
rule::shared_ptr
rule_factory::create_rule(const string& express)
{/*{{{*/
	using namespace rule_type;	
    string symbol, left,right;
    if(is_relation_exp(express))
    {
        if(analyse_relation_exp(express,left,symbol,right))
        {
            rule::shared_ptr left_exp = create_rule(left);
            rule::shared_ptr right_exp = create_rule(right); 
            return create_relation_rule(symbol,left_exp,right_exp);
        }
	    throw runtime_error("relation express analyse failed! exp:"+express);
    }
    else if( is_compare_exp(express))
    {
        return create_cmp_rule(express);
    }
   throw runtime_error("unknow express  exp:"+express);
}/*}}}*/

rule::shared_ptr 
rule_factory::create_cmp_rule(const string& express)
{/*{{{*/
	string left="";
    string symbol="";
    string right="";	
	if(!analyse_compare_exp(express,left,symbol,right))
    {
	    throw runtime_error("compare express analyse failed! exp:"+express);
    }
    return create_cmp_rule_impl(left,symbol,right);
}/*}}}*/

bool 
rule_factory::have_rule(const string& express)
{/*{{{*/
    assert(false);
    return false;
}/*}}}*/


const string relation_symbo("[&|]{2}");
const string compare_symbo("[<>=]{1,2}");
bool
rule_factory::is_relation_exp(const string& str)
{/*{{{*/

	static regex pattern (relation_symbo);
	match_results<const char*> what;
	if(!regex_search(str.c_str(),what,pattern))
    {
        return false;
    }
    return is_relation_symbol(string(what[0].first,what[0].second));
}/*}}}*/


bool
rule_factory::is_compare_exp(const string& str)
{/*{{{*/

	static regex pattern (compare_symbo);
	match_results<const char*> what;
	if(!regex_search(str.c_str(),what,pattern)) return false;
    return is_compare_symbol(string(what[0].first,what[0].second));
}/*}}}*/
bool
rule_factory::is_relation_symbol(const string& str)
{/*{{{*/

    return rule_type::AND == str ||
            rule_type::OR == str ||
            rule_type::NOT == str;
}/*}}}*/

bool
rule_factory::is_compare_symbol(const string& str)
{/*{{{*/

    return rule_symbol::equal == str ||
            rule_symbol::unequal == str ||
            rule_symbol::large == str ||
            rule_symbol::less == str ||
            rule_symbol::large_equal == str ||
            rule_symbol::less_equal == str ;
}/*}}}*/
//const string relation_pattern("\\s*([^&|!]+)\\s*\\s*([&|]{2})\\s*\\s*([^&!]+)");
const string relation_pattern("\\s*([^!]+)\\s*\\s*([&|]{2})\\s*\\s*([^!]+)");
bool 
rule_factory::analyse_relation_exp(	const string& exp, string& left, string& symbol, string& right)
{/*{{{*/

	static regex pattern (relation_pattern);
	match_results<const char*> what;

	if(!regex_search(exp.c_str(),what,pattern)) return false;
	left= 			string(what[1].first,what[1].second);
	trim(left);
	symbol = 	utility::unify(string(what[2].first,what[2].second));
	right =		 	string(what[3].first,what[3].second);
	trim(right);

    erase_all(left, string("("));
    erase_all(left, string(")"));
    erase_all(right, string("("));
    erase_all(right, string(")"));
    if(is_relation_symbol(symbol))  return true; 
    return false;
}/*}}}*/

rule::shared_ptr 
rule_factory::create_relation_rule(const string& symbol, rule::shared_ptr left, rule::shared_ptr right)
{/*{{{*/
    if(rule_type::AND == symbol)
    {
        return  rule::shared_ptr(new and_rule(left,right));
    }
    else if(rule_type::OR == symbol) 
    {
        return rule::shared_ptr(new or_rule(left,right));
    }
    else if (rule_type::NOT == symbol) 
    {
        return rule::shared_ptr( new not_rule(right));
    }
    throw runtime_error("relation symbol erorr! symbol:"+symbol);

}/*}}}*/
const string cmp_pattern("([^=<>\\(]+)([=<>]{1,2})([^=<>\\)]+)");

bool
rule_factory::analyse_compare_exp(	const string& exp, string& left, string& cmp_symbol, string& right)
{/*{{{*/


	static regex pattern (cmp_pattern);
	match_results<const char*> what;
	if(!regex_search(exp.c_str(),what,pattern)) return false;
	left= 			string(what[1].first,what[1].second);
    trim(left);
	cmp_symbol = 	utility::unify(string(what[2].first,what[2].second));
	right =		 	string(what[3].first,what[3].second);
    trim(right);
    return true;
	if(_logger.isDebugEnabled())
	{
		_logger.debugStream()<<"exp "<<exp;	
		_logger.debugStream()<<left<<" " << cmp_symbol<<" " << right ;
	}
    erase_all(left, string("("));
    erase_all(left, string(")"));
    erase_all(right, string("("));
    erase_all(right, string(")"));
}
rule::shared_ptr /*}}}*/

rule_factory::create_cmp_rule_impl(const string& left, const string& cmp_symbol, const string& right)
{/*{{{*/
	using namespace rule_symbol;	
	config_error error(_WHERE);
    try
    {
        matcher dim = _dim_factory.create_matcher(left,right);
        if(_logger.isDebugEnabled())
            _logger.debugStream()<<WATCH(dim.name())<<" : " << dim.value()
                <<" "<<WATCH(cmp_symbol)<<" "<< left;

        if(equal==cmp_symbol)
        {
            return rule::shared_ptr(new equal_rule(dim));	
        }
        else if(unequal== cmp_symbol)
        {
            return rule::shared_ptr( new unequal_rule(dim));	
        }	
        else if(large== cmp_symbol)
        {
            //因为 $var > 1 
            //那 dim(1) 将要小于 $var;	
            return rule::shared_ptr(new less_rule(dim)); 
        }
        else if(less== cmp_symbol)
        {

            //因为 $var < 1 
            //那 dim(1) 将要大于 $var;	
            return rule::shared_ptr(new large_rule(dim));	
        }
        else if(large_equal == cmp_symbol)
        {
            //同上理
            return rule::shared_ptr(new less_equal_rule(dim)); 	
        }
        else if(less_equal == cmp_symbol)
        {
            //同上理
            return rule::shared_ptr(new large_equal_rule(dim)); 	
        }
    }
    catch(std::exception &e )
    {
	    error.stream()<<" create rule failed: "<<left << cmp_symbol << right;
	    error.stream()<<" \n detail :"<<e.what();
        throw error;
    }
	error.stream()<<" rule type no support :" <<left << cmp_symbol << right;
	throw error;/*}}}*/
}

}
}
