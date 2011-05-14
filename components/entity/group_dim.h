#ifndef __GRP_DIMENSION_H__
#define __GRP_DIMENSION_H__
#include "common.h"
#include "dimension.h" 
namespace logs
{
namespace entity
{
/// 
/// @brief group dimension
/// 
class group_dimension: public dimension
{
public:		
    typedef boost::shared_ptr<group_dimension>  shared_ptr;
	/// 
	/// @brief whether have son dimension which name is 'name'
	/// 
	/// @param name  
	/// 
	/// @return  ture of false
	/// 
	virtual bool have_son(const std::string& name) const =0;
	/// 
	/// @brief get the son dimension  by son dimension name
	/// 
	/// @param name 
	/// 
	/// @return  pointer of son dimension;
	/// 
	virtual const dimension* son_dim(const std::string& name) =0;
    virtual void add_son(dimension::shared_ptr dim)=0;
    virtual std::string create_son_name(const std::string& name) =0 ;
    virtual int  count()=0;
};

}
}
#endif
