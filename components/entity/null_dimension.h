#ifndef __NULL_DIMENSION_H__
#define __NULL_DIMENSION_H__
#include "dimension.h" 
namespace logs
{
namespace entity
{

/// 
/// @brief  define a null dimension;
/// 
class null_dimension : public dimension
{
public:
	std::string name() const ;
	std::string value() const;
    dimension* clone() const ;
protected:	
	null_dimension(std::string dim_name);
	bool equal(const dimension& other)const; 
	bool less(const dimension& other)const; 
private:
	std::string _name;
};

}
}
#endif
