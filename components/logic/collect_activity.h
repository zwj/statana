#ifndef __COLLECT_ACTIVITY_H__
#define __COLLECT_ACTIVITY_H__
#include "activity.h"
#include <map>
#include <boost/shared_ptr.hpp>
#include <log4cpp/Category.hh>
namespace logs
{
namespace logic
{

typedef std::vector<std::string> str_vector;
/// 
/// @brief  matcher geter
/// 
class matcher_geter
{/*{{{*/
public: 		
	boost::shared_ptr<matcher_geter> shared_ptr ;	
	virtual entity::matcher get_matcher(entity::log_item& item)=0;	
	virtual ~matcher_geter(){};
};/*}}}*/

/// 
/// @brief 
/// 
class static_matcher_geter : public matcher_geter
{/*{{{*/
public:		
	static_matcher_geter(const std::string& name);	
	entity::matcher get_matcher(entity::log_item& item);		
private:
	const std::string _name;
};/*}}}*/

/// 
/// @brief 
/// 
class dyn_matcher_geter : public matcher_geter
{/*{{{*/
public:		
	dyn_matcher_geter(const std::string& first,const std::string& second);	
	entity::matcher get_matcher(entity::log_item& item);		
private:
	const std::string _first;
	const std::string _second;
};/*}}}*/
		
/// 
/// @brief  collect for unique dimension 
/// 
class collect_activity : public activity 
{/*{{{*/
public: 		
	collect_activity(	const std::string&  	in, 
						const std::string&      out,
						const std::string& it_name = "collect_activity");

	typedef boost::shared_ptr<collect_activity> shared_ptr ;		
	void unique_dimension(const std::string& dimension_name);
	void execute();
private:
	str_vector 
	unique_args(const string& str);
	boost::shared_ptr<matcher_geter> _matcher_geter_ptr;
	log4cpp::Category& _logger;
	friend class activity_testcase;
};/*}}}*/


}}
#endif
