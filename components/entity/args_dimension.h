#ifndef __ARGS_DIMENSION_H__
#define __ARGS_DIMENSION_H__
#include "common.h" 
#include "dimension.h" 
#include "dimension_def.h" 
#include "str_dimension.h" 
#include "group_dim.h" 
namespace logs
{
namespace entity
{
/// 
/// @brief description of url args
/// 
class args_dimension : public group_dimension 
{/*{{{*/
public:		
	args_dimension();
	args_dimension( const std::string& value);
	args_dimension( const std::string& arg_name,const std::string& arg_val);
	args_dimension( const args_dimension& other);
	std::string name() const;
	std::string value() const;
    void set_value(const std::string& name);
	bool have_son(const std::string& name) const;
	const dimension* son_dim(const std::string& name);
	std::string create_son_name(const std::string& name);
	const args_dimension& operator=(const args_dimension& other);
    dimension* clone() const ;
    static void add_defined_dim(dimension::shared_ptr dim);
    static bool get_defined_dim(const std::string& name, dimension::shared_ptr& dim);
    void add_son(dimension::shared_ptr dim);
    int count();
protected:
	bool equal(const dimension& other)const;
	bool less(const dimension& other)const ;
	bool in(const dimension& other) const;
	bool true_equal(const dimension& other)const;
    bool fetch_pair(const std::string& express ,  std::string& name,std::string& value);
private:
    void construct(const std::string& val);
	typedef std::map<std::string,dimension::shared_ptr> args_map;
	args_map _imp;
	log4cpp::Category& _data_err_logger;
    static dim_vector _s_def_dims;
};/*}}}*/
}
}
#endif
