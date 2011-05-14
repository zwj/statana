#ifndef __DIMENSION_FACTORY_H__
#define __DIMENSION_FACTORY_H__
#include "dimension.h" 
#include "matcher.h"
#include "common.h"
namespace logs
{
namespace entity
{

/// 
/// @brief  create dimension factory;
/// 
class dimension_factory
{
public:		
	/// 
	/// @brief  create dimension by name, vlaue;
	/// 
	/// @param name 
	/// @param value 
	/// 
	/// @return  dimension shared pointer;
	/// 
	dimension::shared_ptr 		
	create_dimension(	const std::string& name, 
						const std::string& value);		

	dimension::shared_ptr 		
	create_default_dim(	const std::string& type, 
						const std::string& name);		
	matcher
	create_matcher(		const std::string& name, 
						const std::string& value);		
private:
	bool is_args(const std::string& name);
	bool is_addition(const std::string& name);
	bool include(	const std::string& str,
					const std::string& caption, 
				 	std::string& other);
    dimension::svector _define_dims;
};
		
}
}
#endif
