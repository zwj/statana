#include <entity/common.h>
#include <entity/dimension_factory.h>
#include <entity/ip.h>
#include <entity/str_dimension.h>
#include <entity/args_dimension.h>
#include <boost/regex.hpp>
#include <entity/utility.h>
#include <boost/lexical_cast.hpp>
#include <entity/null_dimension.h>
using namespace std;
using namespace boost;
using namespace improve;
namespace logs
{
namespace entity
{
namespace dim_type
{
    const string INT("INT");
    const string STR("STRING");
    const string IP("IP");
    const string TIME("TIME");
}
dimension::shared_ptr 
dimension_factory::create_dimension(
				const string& name,
				const string& value )
{/*{{{*/
	string unify_name = name;	
	dimension::shared_ptr null_dim =null_str_dimension::get(unify_name);
	string unify_value = value;
	if(null_dim->name() == unify_value)
	{
		return null_dim;
	}

	dimension::shared_ptr empty_dim(new empty_str_dimension(unify_name));
	if(empty_dim->name() == unify_value)
	{
		return empty_dim;
	}
	if(item_def::name_of(item_def::IP)== unify_name)
	{
		return  dimension::shared_ptr(new ip(value));	
	}
	else if(item_def::name_of(item_def::TIME) == unify_name)
	{
		return dimension::shared_ptr(new time(value));	
	}
	else if(item_def::name_of(item_def::LEN) == unify_name)
	{
		return dimension::shared_ptr(new length(lexical_cast<unsigned int>(value)));	
	}
	else if(item_def::name_of(item_def::STATUS) == unify_name)
	{
		return dimension::shared_ptr(new status_code(lexical_cast<unsigned int>(value)));	
	}
	else if(is_args(unify_name))
	{
		return dimension::shared_ptr( new str_dimension(unify_name,value));	
	}
	else if(is_addition(unify_name))
	{
		return dimension::shared_ptr( new str_dimension(unify_name,value));	
	}
	else
	{
		return dimension::shared_ptr( new str_dimension(unify_name,value));	
	}
}/*}}}*/

dimension::shared_ptr 		
dimension_factory::create_default_dim(	const std::string& type, 
						const std::string& name)		
{
	string unify_type= utility::unify(type);	
    if(unify_type== dim_type::INT) 
    {
        return dimension::shared_ptr(new int_dimension(name,0));
    }
    else if(unify_type== dim_type::STR)
    {
        return dimension::shared_ptr(new str_dimension(name,""));
    }
    else if(unify_type== dim_type::IP)
    {
        return dimension::shared_ptr(new ip());
    }
    else if(unify_type== dim_type::TIME)
    {
        return dimension::shared_ptr(new time());
    }
    throw new runtime_error("unknow type:"+type);

}
matcher
dimension_factory::create_matcher(
				const string& name,
				const string& value )
{/*{{{*/
    dimension::shared_ptr dim=create_dimension(name,value);
    _define_dims.push_back(dim);
	return matcher(dim.get());
}/*}}}*/

bool
dimension_factory::include(	const string& str,
							const string& caption, 
				 			string& other)
{/*{{{*/
	regex pattern("\\s*(\\S*)\\.\\s*(\\S*)\\s*");	
	match_results<const char*> what;
	if(regex_search(str.c_str(),what,pattern))
	{
		string unifyed(utility::unify(string(what[1].first,what[1].second)));		
		if(caption != unifyed) return false;
		other = string(what[2].first,what[2].second);
		return true;
	}		
	return false;
}/*}}}*/

bool
dimension_factory::is_args(const string& name)
{
	return name.find(item_def::name_of(item_def::ARG))!=string::npos;		
}
bool
dimension_factory::is_addition(const string& name)
{
	return name.find(item_def::name_of(item_def::ADDI))!=string::npos;		
}
				
}
}
