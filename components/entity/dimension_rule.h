#ifndef __FILTER_RULE__
#define __FILTER_RULE__

#include "common.h"
#include "log_item.h"
#include <entity/log_item.h>
#include <improve/rule.h>

namespace logs
{
namespace entity
{

typedef improve::rule<log_item::shared_ptr> 		rule;
typedef improve::or_rule<log_item::shared_ptr> 		or_rule;
typedef improve::and_rule<log_item::shared_ptr> 	and_rule;
typedef improve::not_rule<log_item::shared_ptr> 	not_rule;

/// 
/// @brief  relation rule;
/// 
class relation_rule : public rule
{/*{{{*/
protected:		
	relation_rule(const matcher& the_matcher)
	:_std_matcher(the_matcher)		
	{
	}
	inline const matcher compared_matcher(const log_item::shared_ptr& obj) const 
	{
		return obj->get_matcher(_std_matcher.name());
	}
	const matcher _std_matcher;	
};
/*}}}*/

/// 
/// @brief  large rule
/// 
class large_rule :public relation_rule
{/*{{{*/
public: 		
	large_rule(const matcher& the_matcher)
	:relation_rule(the_matcher)
	{}
	bool is_true(const log_item::shared_ptr& obj) const 
	{
		return compared_matcher(obj) > _std_matcher;	
	}
	std::string description() const 
	{
		return _std_matcher.name() + " <large> " + _std_matcher.value();
	}
};/*}}}*/

/// 
/// @brief  large or equal rule;
/// 
class large_equal_rule :public relation_rule
{/*{{{*/
public: 		
	large_equal_rule (const matcher& the_matcher)
	:relation_rule(the_matcher)
	{}
	bool is_true(const log_item::shared_ptr& obj) const
	{
		return compared_matcher(obj) >= _std_matcher;	
	}
	std::string description() const 
	{
		return _std_matcher.name() + " <large equal> " + _std_matcher.value();
	}
};/*}}}*/

/// 
/// @brief  less rule ;
/// 
class less_rule :public relation_rule
{/*{{{*/
public: 		
	less_rule(const matcher& the_matcher)
	:relation_rule(the_matcher)
	{}
	bool is_true(const log_item::shared_ptr& obj) const 
	{
		REQUIRE(obj); 	
		return compared_matcher(obj) < _std_matcher;	
	}
	std::string description() const 
	{
		return _std_matcher.name() + " <less> " + _std_matcher.value();
	}
};/*}}}*/

/// 
/// @brief less or equal rule;
/// 
class less_equal_rule :public relation_rule
{/*{{{*/
public: 		
	less_equal_rule (const matcher& the_matcher)
	:relation_rule(the_matcher)
	{}
	bool is_true(const log_item::shared_ptr& obj) const 
	{
		REQUIRE(obj); 	
		return compared_matcher(obj) <= _std_matcher;	
	}
	std::string description() const 
	{
		return _std_matcher.name() + " <less equel> " + _std_matcher.value();
	}
};/*}}}*/

/// 
/// @brief equal rule; 
/// 
class equal_rule :public relation_rule
{/*{{{*/
public: 		
	equal_rule(const matcher& the_matcher)
	:relation_rule(the_matcher)
	{}
	bool is_true(const log_item::shared_ptr& obj) const 
	{
		REQUIRE(obj); 	
		return compared_matcher(obj) == _std_matcher;	
	}
	std::string description() const 
	{
		return _std_matcher.name() + " <equel> " + _std_matcher.value();
	}
};/*}}}*/

/// 
/// @brief unequal rule; 
/// 
class unequal_rule : public relation_rule
{/*{{{*/
public:
	unequal_rule(const matcher& the_matcher)
	:relation_rule(the_matcher)
	{}
	bool is_true(const log_item::shared_ptr& obj) const 
	{
		REQUIRE(obj); 	
		return !(compared_matcher(obj) == _std_matcher);
	}
	std::string description() const 
	{
		return _std_matcher.name() + " <nont equel> " + _std_matcher.value();
	}
				
};/*}}}*/

}
}
#endif
