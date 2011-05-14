#ifndef __ACTIVITY_H__
#define __ACTIVITY_H__
#include <entity/dimension.h>
#include <entity/dimension_rule.h> 
#include <entity/log_item.h> 
#include <string>
#include <map>
#include "input.h"
#include <improve/status.h>
#include "logic_base.h"
#include "data_board.h"
namespace logs
{
namespace logic
{

/// 
/// @brief  description of activity;
/// 
typedef boost::shared_ptr<std::istream> istream_sptr;
class activity : public logic_base
{/*{{{*/
public: 		
	enum stat{exed,unexed};	
	typedef boost::shared_ptr<activity> shared_ptr ;		
	activity(	const std::string&  in, const std::string& path, const std::string& filename, const std::string& it_name);
	activity(	const std::string&  in, const std::string& it_name);
	/// 
	/// @brief  execute this activity;
	/// 
	virtual void 	execute(data_board& board)=0;
	virtual ~activity(){}
	const std::string name() const ;
	bool have_exed();
	const std::string& get_inputer();
protected:
    std::string _in_lable;
    std::string _out_path;
    std::string _out_filename;
	improve::status_machine<stat> _status_control;
private:
	const std::string _name;
	const std::string& get_out_path();
	const std::string& get_out_filename();

    friend class task;
	
};/*}}}*/



}
}
#endif
