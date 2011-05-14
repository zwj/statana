#ifndef __TASK_MANAGE_H__
#define __TASK_MANAGE_H__
#include <improve/utility.h>
#include "task.h" 
#include <boost/shared_ptr.hpp> 
#include <stdexcept>
#include "activity_factory.h"
#include "input.h"
#include "data_board.h"
#include <map>
#include "logic_base.h"
#include <entity/dimension_factory.h>
typedef std::map<std::string,std::string> strstrmap;
namespace logs
{
namespace logic
{

/// 
/// @brief  manager task; 
/// 
class task_manager : public logic_base
{
public:		
	task_manager(const std::string& out_dir);		
    ~task_manager();
	/// 
	/// @brief  the plan config file is ready?
	/// 
	/// @param plan_name 
	/// 
	/// @return 
	/// 
	bool is_plan_ready(const std::string& plan_name);
    /// 
	/// @brief  set override data source path pattern from command line
	/// 
	void set_override_datasrc(const str_vector& src_files);
	/// 
	/// @brief  load plan from confige file;
	/// 
	void load_plan(const std::string& plan_name, bool is_crash = false);
	/// 
	/// @brief  have no assigned task ?
	/// 
	/// @return  true or flase
	/// 
	bool have_task();
	/// 
	/// @brief  assign a task ;
	/// 
	/// @return  
	/// 
	task::shared_ptr  assign_task();
private:

    bool recover_from_crash();
    std::string get_sec_express(const std::string& sec_name);
    void setting_argsvar(const std::string& express);
    void setting_logvar(entity::log_analyser::shared_ptr analyser, const std::string& express);

    entity::log_analyser::shared_ptr
    create_analyser(const std::string& analyser, const std::string& format);
    std::string get_argsvar_express(const std::string& express);
    std::string get_logvar_express(const std::string& express);
	typedef std::vector<std::string> source_vector;
	typedef std::vector<std::string> string_vector;
	
    void get_task_setting(const std::string& express);
    void init_plan();

    std::string get_section_name(const std::string& line);
    void strip_comment(std::string& line);
	void load_datasource();
	task::shared_ptr create_task();

    std::string get_sec_value(const std::string& sec_name,const std::string& express);
	void proc_next_file();
	void get_src_files(	const std::string& path_pattern );
	void get_directs_files(	const std::string& path_str ,
							const std::string& regex_str);
	unsigned int create_task_id();
    std::string trans2regex(std::string& express);

	unsigned int 		_begin_pos;
	unsigned int 		_proc_count;
	istream_sptr		_istream;

	activity_factory	_acty_factory;
    entity::dimension_factory   _dim_factory;

    str_vector          _src_file_patterns;
	source_vector		_files;
	unsigned int 		_proc_pos;
    unsigned int        _file_pos;
    bool                _is_crash;
	unsigned int 		_current_task_id;
    std::string         _out_path;
    strstrmap           _plan_items;
	friend class task_manager_testcase;
	
};
}
}
#endif
