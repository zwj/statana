#ifndef __ADDI_DIMENSION_H__
#define __ADDI_DIMENSION_H__
#include "dimension.h" 
#include "dimension_def.h" 
#include "str_dimension.h" 
#include "group_dim.h" 
namespace logs
{
namespace entity
{
/// 
/// @brief define  of log addition  
/// 
class addition_dimension : public group_dimension 
{/*{{{*/
public:		
    typedef boost::shared_ptr< addition_dimension > shared_ptr;
	enum COL{E1=0,E2,E3,E4,E5,E6,E7,E8,E9,Ea,Eb,Ec,Ed,Ee,Ef,Eg,Eh,EM,Em,En,Eo,Ep,ER,Er,Es,Et,Ew,Ex,EZ,Ez};	
	addition_dimension(COL index,const std::string& vaule);	
	addition_dimension(const std::string& name, const std::string& value);
	addition_dimension();
	addition_dimension(const addition_dimension& other);
	std::string name() const;
	std::string value() const;
	void add_dimension(COL index, const std::string& value);
	bool have_son(const std::string& name) const;
	const dimension* son_dim(const std::string& name);
	std::string create_son_name(const std::string& name);
	static bool parse(const std::string& name_value,
							COL& col, std::string& value);
	const addition_dimension& operator=(const addition_dimension& other);
    dimension* clone() const ;
    void add_son(dimension::shared_ptr dim);
    int  count(){return _dim_array.size();}
protected:	
	bool equal(const dimension& other)const;
	bool less(const dimension& other)const ;
	bool true_equal(const dimension& other) const;
	bool in(const dimension& other) const ;
private:
	dim_vector 	_dim_array;	
};/*}}}*/

class addition_init
{/*{{{*/
public:		
	static char COL_TABLE[255];
	addition_init();
	static addition_dimension::COL index(char c);
	static addition_dimension::COL index(const std::string& name);
	static bool is_define(char c);
	static bool is_define(const std::string& name);
	static bool index_c(const std::string& name,char& ch);
};/*}}}*/

}
}
#endif
