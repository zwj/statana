#ifndef __STR_DIMENSION_H__
#define __STR_DIMENSION_H__
#include "common.h"
#include "dimension.h" 
namespace logs
{
namespace entity
{
/// 
/// @brief  a dimension implement by string
/// 
class str_dimension : public dimension
{/*{{{*/
public:		
	typedef boost::shared_ptr<str_dimension> shared_ptr ;	
	str_dimension(const std::string& name, const std::string& value);
	str_dimension();
	std::string name() const ;
	std::string value() const;
    void set_value(const std::string& val);
    dimension* clone() const ;
	const str_dimension& operator=(const str_dimension& other);
    static unsigned int  cal_hash_code(const std::string& str);
protected:
	bool equal(const dimension& other)const;
	bool less(const dimension& other)const ;
private:
	str_dimension(const std::string& name, const std::string& value,unsigned int n_hash_code , unsigned int v_hash_code);
	std::string _imp;		
	std::string _name;
    unsigned int _n_hash_code;
    unsigned int _v_hash_code;
public:
    static str_dimension::shared_ptr SUM;
};/*}}}*/

class null_str_dimension : public str_dimension
{/*{{{*/

public:	
    static dimension::shared_ptr get(const std::string& the_name);
protected:
private:
	null_str_dimension(const std::string& the_name);	
    static dimension::smap _global_nullstrs;
};/*}}}*/

class empty_str_dimension : public str_dimension
{
public:	
	empty_str_dimension(const std::string& the_name);	
protected:
};

}
}
#endif
