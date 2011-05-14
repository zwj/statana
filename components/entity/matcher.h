#ifndef _MATCHER_H_
#define	_MATCHER_H_ 
#include <string>
#include <improve/utility.h>
#include <boost/shared_ptr.hpp>
#include <improve/dbc.h>
#include "dimension.h"
#include <vector>
namespace logs
{
namespace entity
{

typedef std::vector< const dimension* > dimension_vector;
/// 
/// @brief dimension matcher;
/// 
class matcher
{
public:		
	matcher(const dimension* dim);	
	matcher(const dimension* first,const dimension* second);	
    ~matcher();
	std::string name() const ;
	std::string value() const ;
	//void hold();
	/// 
	/// @brief merge other matcher to this;
	/// 
	/// @param other 
	/// 
	void merge(const matcher& other);
private:
    matcher();
    dimension_vector	_dim_array;		 
	friend bool operator==( const matcher& left, const matcher& right);
	friend bool operator<(const matcher& left, const matcher& right);
};

bool operator==( const matcher& left, const matcher& right);
bool operator<(  const matcher& left, const matcher& right);
bool operator!=( const matcher& left, const matcher& right);
bool operator<=(  const matcher& left, const matcher& right);
bool operator>(  const matcher& left, const matcher& right);
bool operator>=(  const matcher& left, const matcher& right);
}
}
#endif
