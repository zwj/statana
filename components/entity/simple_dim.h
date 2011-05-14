#ifndef __SAM_H__
#define __SAM_H__
#include "common.h"
#include "dimension.h"

namespace logs
{
namespace entity
{
/// 
/// @brief dimension by unsigned int 
/// 
class int_dimension : public dimension
{/*{{{*/
public :		
	std::string name() const;
	std::string value() const;
    dimension* clone() const ;
    void set_value(const std::string& val);
    void set_by_hexstr(const std::string& val);
    virtual ~int_dimension(); 
	int_dimension(const std::string& name);	
	int_dimension(const std::string& name,unsigned int value);	
protected:		
	bool equal(const dimension& other)const;
	bool less(const dimension& other)const ;
	unsigned int _imp;		
    std::string _name;		
	friend std::istream& operator>>(std::istream& ins,int_dimension& obj);	

};/*}}}*/

std::istream& operator>>(std::istream& ins,int_dimension& obj);	
}
}
#endif

