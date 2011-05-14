#ifndef __ACTIVITY_FACTORY_H__
#define __ACTIVITY_FACTORY_H__
#include "activity.h"
#include "input.h"
#include "data_board.h"
#include "rule_factory.h" 
#include "logic_base.h" 
#include "entity/log_analyser.h"
namespace logs
{
namespace logic
{
	
/// 
/// @brief  activity factory
/// 
class activity_factory : public logic_base
{
public:		
    enum ACT_TYPE{STAT,LOAD,DIV};
	activity_factory();	
	/// 
	/// @brief  create activity by xml description;
	/// 
	/// @param walker 
	/// 
	/// @return actvity shared pointer; 
	/// 
	activity::shared_ptr 
	create_acty( const std::string& express, const std::string& params);	
	void set_load_param(istream_sptr src_stream, 
                        unsigned int proc_count);
    void set_analyser(entity::log_analyser::shared_ptr anar);
private:

    ACT_TYPE get_act_type( const std::string& exp );
    bool analyse_div_exp(const std::string& express, std::string& to,std::string& from,std::string& rule);
    bool analyse_unique_exp( const std::string& exp, std::string& to, std::string& from,std::string& var);
    bool analyse_load_exp( const std::string& exp, std::string& to, std::string& rule_exp);
	activity::shared_ptr 
	create_collect_acty(const std::string& express, const std::string& path);
	activity::shared_ptr 
	create_load_acty(const std::string& express);
	activity::shared_ptr 
	create_classify_acty(const std::string& express);
	rule_factory 		_rule_factory;
	istream_sptr		_src_stream;
	unsigned int		_proc_count;
    entity::log_analyser::shared_ptr _analyser_sptr;
    friend class activity_facotry_testcase;
};
		
}

}
#endif
