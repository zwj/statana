#ifndef __IP_H__
#define __IP_H__
#include "common.h"
#include "dimension.h"
#include "simple_dim.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_ptr.hpp>

namespace logs
{
namespace entity
{
enum COL{IP=0,TIME,DURATION,LEN,PATH,FILE,ARG,ADDITION};
/// 
/// @brief  IP dimension;
/// 
class ip : public int_dimension
{
public :		
	typedef boost::shared_ptr<ip> shared_ptr ;	
	ip( unsigned char first,unsigned char second,
		unsigned char third,unsigned char forth	);	
	ip( unsigned int value);
	ip(); 
	ip( const std::string& str);
	std::string value() const;
    void set_value(const std::string& val);
	/// 
	/// @brief  eg: get '192' from '192.168.0.1 ' 
	/// 
	/// @return 
	/// 
	unsigned char first() const ;
	/// 
	/// @brief eg: get '168' from '192.168.0.1'
	/// 
	/// @return 
	/// 
	unsigned char second() const ;
	unsigned char third() const ;
	unsigned char forth() const;
    dimension* clone() const;
    ip& operator =(const ip& other);
protected: 	
private:
    void str_construct(const std::string& val);
	void construct(	unsigned char first, unsigned char second,
					unsigned char third, unsigned char forth);	
};

/// 
/// @brief  Time dimension
/// 
class time : public dimension
{
public:		
	typedef boost::shared_ptr<time> shared_ptr ;	
	time(time_t);
	time();
	time(boost::posix_time::ptime now_time);
	time(const std::string& time_str);
	std::string name() const;
	std::string value() const;
    dimension* clone() const;
    void set_value(const std::string& val);
    time& operator =(const time& other);
    static time now();
protected:
	bool equal(const dimension& other)const;
	bool less(const dimension& other)const ;
private:
	static std::string time_t2str(time_t v);
	boost::posix_time::ptime _imp;
};
			
/// 
/// @brief  length dimension
/// 
class length: public int_dimension 
{
public:		
	typedef boost::shared_ptr<length> shared_ptr ;
	length(unsigned int size);
	length();
    dimension* clone() const;
    length& operator =(const length& other);
protected:
};

/// 
/// @brief  http status code dimension
/// 
class status_code: public int_dimension 
{
public:		
	typedef boost::shared_ptr<status_code> shared_ptr ;
	status_code(unsigned int size);
	status_code();
    dimension* clone() const;
    status_code& operator =(const status_code& other);
};

}
}
#endif

