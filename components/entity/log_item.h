#ifndef __LOG_ITEM_H__
#define __LOG_ITEM_H__
#include "common.h"
#include "dimension.h"
#include "dimension_def.h"
#include <iostream> 
#include "ip.h" 
#include "simple_dim.h"
#include "str_dimension.h"
#include "vector_dim.h"
#include "args_dimension.h"
#include "addition_dim.h"
#include "matcher.h" 
#include "entity_base.h" 
namespace logs
{
namespace entity
{

/// 
/// @brief  exception of data error;
/// 
class data_error : public improve::rt_exception
{
public:		
	data_error(const std::string& where)
	:improve::rt_exception(where)
	{}
};
/// 
/// @brief  describe a log item;
/// 
class log_item  : public entity_base
{
public: 	
	typedef boost::shared_ptr<log_item> shared_ptr ;	
	typedef std::vector<boost::shared_ptr<log_item> > vector;	
	log_item();	
	ip			get_ip();
	time		get_time();
    length      get_len();
	void		replace(const ip& new_ip);
	void		replace(const time& new_time);
	void		replace_len(int new_len);
	void		replace(const args_dimension& new_args);
    const dimension* item_dim(item_def::col item_col);
	/// 
	/// @brief get unique matcher allways success!
	///			eg: get_matcher("IP");
	/// 
	/// @param name 
	/// 
	matcher		get_matcher(const std::string& name);
	/// 
	/// @brief eg: get_matcher("ARG","ff");
	/// 
	/// @param frist 
	/// @param second 
	/// 
	matcher		get_matcher(const std::string& frist,
							const std::string& second);

	matcher		get_matcher(const std::string& frist,
							const std::string& second,
                            const std::string& thrid);

	matcher		get_matcher(const std::string& frist,
							const std::string& second,
                            const std::string& thrid,
                            const std::string& forth);

	matcher		get_matcher(const std::vector<std::string> svector);
private:

    bool is_args_name(const std::string& name);
    bool is_addi_name(const std::string& name);
	matcher		get_args_matcher(const std::string& name);		
    matcher     get_args_matcher(const std::string& father_name, const std::string& son_name);
	matcher		get_addi_matcher(const std::string& name);		
	ip					_ip;
	time				_time;
	str_dimension		_path;
	str_dimension		_file;
    str_dimension       _uri;
    str_dimension       _host;
	args_dimension		_args;
    status_code         _status;
	length				_len;
	vector_dimension    _ref;
    str_dimension       _agent;
	vector_dimension	_cookie;
    group_dimension::shared_ptr	_addition_sptr;

	friend std::ostream& operator<<(std::ostream& out, const log_item& item);
    friend class apache_analyser;
    friend class base_analyser;
    friend class log_item_testcase;
};
std::ostream& operator<<(std::ostream& out, const log_item& item);


}
}
#endif
