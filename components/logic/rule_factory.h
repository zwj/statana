#ifndef __RULE_FACTORY_H__
#define __RULE_FACTORY_H__
#include <string>
#include <entity/dimension_factory.h>
#include <entity/dimension_rule.h> 
#include "logic_base.h"
namespace logs
{
namespace logic
{

/// 
/// @brief  create rule from xml description;
/// 
class rule_factory : public logic_base
{
public:		
	rule_factory();	
	/// 
	/// @brief 
	/// 
	/// @param walker 
	/// 
	/// @return 
	/// 
	entity::rule::shared_ptr create_rule(const std::string& express);
    bool is_rule_express(const std::string& express);
	static bool have_rule(const std::string& express);
private:

	bool analyse_relation_exp(	const std::string& exp,
							std::string& left,
							std::string& symbol,
							std::string& right);

	bool analyse_compare_exp(	const std::string& exp,
							std::string& left,
							std::string& symbol,
							std::string& right);
    bool is_compare_exp(const std::string& express);
    bool is_relation_exp(const std::string& express);

    bool is_relation_symbol(const std::string& str);
    bool is_compare_symbol(const std::string& str);
	entity::rule::shared_ptr 
    create_relation_rule(const std::string& symbol, entity::rule::shared_ptr left, entity::rule::shared_ptr right); 
	entity::rule::shared_ptr 
    create_cmp_rule(const std::string& express); 
	entity::rule::shared_ptr 
    create_cmp_rule_impl(const std::string& left, const std::string& cmp_symbol, const std::string& right);
    std::string get_rule_type(std::string express);
	entity::dimension_factory _dim_factory;

    friend class rule_factory_testcase;
};

}
}
#endif
