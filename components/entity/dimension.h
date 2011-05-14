#ifndef _DIMENSION_H 
#define _DIMENSION_H 
#include "common.h"
#include "entity_base.h"
#include <vector>
#include <map>
namespace logs
{
namespace entity
{

/// 
/// @brief abstractor  of logitem's domain
/// 
class dimension : public entity_base
{
public:	
	typedef boost::shared_ptr<dimension> shared_ptr; 	
    typedef std::vector< boost::shared_ptr<dimension> > svector;
    typedef std::map<std::string, boost::shared_ptr<dimension> > smap;

	virtual std::string name() const = 0;
	/// 
	/// @brief colne this dimension;
	/// 
	/// @return pointer of new dimension; 
	/// 
	virtual dimension* clone() const = 0;
	virtual std::string value() const = 0;
	virtual ~dimension(){};
    virtual void set_value(const std::string& val)
	{throw improve::no_realization(__FUNCTION__); }
	
protected: 	
	virtual bool equal(const dimension& other)const 
	{throw improve::no_realization(__FUNCTION__); }
	virtual bool less(const dimension& other)const 
	{throw improve::no_realization(__FUNCTION__); }
	friend bool operator==(const dimension& left , const dimension& right);
	friend bool operator<(const dimension& left , const dimension& right);
	
};
    
bool operator==(const dimension& left , const dimension& right); 
bool operator!=(const dimension& left , const dimension& right); 
bool operator<(const dimension& left , const dimension& right);
bool operator<=(const dimension& left , const dimension& right);
bool operator>(const dimension& left , const dimension& right);
bool operator>=(const dimension& left , const dimension& right);
/// 
/// @brief log item define;
/// 
class item_def
{
		
public:
	enum col{IP=0,TIME,PATH,FILE,URI,ARG,STATUS,LEN,REF,AGENT,HOST,COOKIE,ADDI};	
	static bool 		have_col(const std::string& name);
	static std::string 	name_of(col item_col);
	static col			col_of(const std::string&  name);
private:	
	static item_def* instance();
	typedef std::map<const std::string ,col>	str_col_map; 
	static const std::string  NAME[]; 
	str_col_map  name_col_map;
	static item_def* _imp_ptr;
	item_def();
	
};
}
}
#endif
