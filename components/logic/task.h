#ifndef __TASK__H__
#define __TASK__H__
#include <improve/utility.h>
#include "activity.h" 
#include "stat_activity.h"
#include <entity/log_item.h>
#include <boost/shared_ptr.hpp> 
#include <stdexcept>
#include "activity_factory.h"
#include "logic_base.h"

namespace logs
{
namespace logic
{

/// 
/// @brief 
/// 
struct activity_pair
{
	std::string 	in; 
	activity::shared_ptr 	act;
};
/// 
/// @brief logs Stat task
/// 
class task : public logic_base
{
public: 		
	typedef boost::shared_ptr<task> shared_ptr ;	
	task(unsigned int task_id);	
	
	///
	/// @brief loaddata, classify , collect activity can register to task
	/// 
	void register_activity(	activity::shared_ptr acty_ptr);
							
	/// 
	/// @brief  execute task
	/// 
	void execute();

private:
	void execute_activity(activity_pair pair);
	typedef std::vector<activity_pair> activity_vector;
	unsigned int 		_id;
	activity_vector     _activity_table;	
	log4cpp::Category& 	_performance_logger;
    data_board          _data_board;
					
					
};

}
}
#endif
