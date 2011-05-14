#ifndef __LOAD_ACTIVITY_H__
#define __LOAD_ACTIVITY_H__
#include "activity.h" 
#include <entity/log_analyser.h>
namespace logs
{
namespace logic
{


/// 
/// @brief load data activity;
/// 
class load_activity : public activity 
{/*{{{*/
public: 
	load_activity(istream_sptr stream_in,
				const std::string& out,
				unsigned int count,
                const entity::log_analyser* ana_ptr,
				const std::string& it_name = "load_activity");
	void  execute(data_board& board);
	/// 
	/// @brief register filter rule;
	/// 
	/// @param filter_rule 
	/// 
	void register_rule(entity::rule::shared_ptr filter_rule);
private:
	void load_item(unsigned int count, outputer::shared_ptr outer);
	entity::rule::shared_ptr  	_filter_rule_ptr;
	unsigned int 	_proc_count;
	istream_sptr	_stream_in;
    const entity::log_analyser*  _analyser_ptr;

};/*}}}*/
}
}
#endif
