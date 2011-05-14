#ifndef __DATA_BOARD_H__
#define	__DATA_BOARD_H__ 
#include "activity.h"
#include "input.h"
#include <string>
#include "logic_base.h"
namespace logs
{
namespace logic
{

/// 
/// @brief  data board
/// 
class data_board : public logic_base
{
public:		
	/// 
	/// @brief  get stat result by name
	/// 
	/// @param name 
	/// 
	/// @return  
	/// 
	outputer::shared_ptr 
	stat_outputer(const std::string& path,const std::string& name);

	/// 
	/// @brief get inputer by name
	/// 
	/// @param name 
	/// 
	/// @return 
	/// 
	inputer::shared_ptr 
	classify_inputer(const std::string& name);

	/// 
	/// @brief get outputer by name
	/// 
	/// @param name 
	/// 
	/// @return 
	/// 
	outputer::shared_ptr 
	classify_outputer(const std::string& path, const std::string& filename);

	/// 
	/// @brief clear the process data;
	/// 
	void clear_process();
	/// 
	/// @brief export the result data; 
	/// 
	void output_results();
    void regist_inputer(const std::string& name,logitem_container::shared_ptr in);
private:
	logitem_container::shared_ptr 
	get_logitem_container(const std::string& name);
	stat_result::name_map		 _results;
	logitem_container::name_map	 _process;
};
		
}

}

#endif
