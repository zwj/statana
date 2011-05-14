#ifndef __MATCHER_GETER_H__
#define __MATCHER_GETER_H__
#include "activity.h"
#include <boost/shared_ptr.hpp>
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
	dyn_matcher_geter(const std::string& first,const std::string& second,const std::string& third);	
	dyn_matcher_geter(  const std::string& first,const std::string& second,
                        const std::string& third,const std::string& forth);	
	entity::matcher get_matcher(entity::log_item& item);		
private:
    str_vector _matcher_names;
};/*}}}*/

		
/// 
/// @brief  collect for unique dimension 
/// 

}}
#endif
