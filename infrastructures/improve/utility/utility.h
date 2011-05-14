#ifndef _UTILITY_H_
#define _UTILITY_H_
#include <cassert>
#include <string>
#include <stdexcept>
#include <string>
#include <sstream>
#include "../dbc.h"
#define  WATCH(x) #x<<":"<<x
namespace improve
{
 	class dumb
  	{
  	};
	/// 
	/// @brief  logic exception for warning forget implement 
	/// 
  	class no_realization : public std::logic_error
  	{
  	public:
  		no_realization()
  		:std::logic_error("no_realization")
  		{
			REQUIRE(false); 	
		}
  		no_realization(const char* msg)
  		:std::logic_error(merge(msg," no_realization!"))
  		{
			REQUIRE(false); 	
		}
	private:		
		static const char* merge(const char* first, const char* second)
		{
            std::string merged = std::string(first) + std::string(second);
			return merged.c_str();
		}
  	};

	#define _WHERE __FUNCTION__
	//static ostringstream error_stream();
	class rt_exception : public std::runtime_error
	{
	public:		
		rt_exception(const std::string& description, const std::string& where);	
		rt_exception(const std::ostringstream& sout, const std::string& where); 
		rt_exception(const rt_exception& other);
		rt_exception(const std::string& where); 
        std::ostringstream& stream();
		const char* what() const throw();
        virtual ~rt_exception() throw();
	private:
        std::ostringstream _sout;
	};
	/// 
	/// @brief  string utiltiy tools 
	/// 
	class str_utility
  	{
	public:		  
		/// 
		/// @brief  remove left and right space;
		/// 
		/// @param need 
		/// 
		/// @return 
		/// 
		static	std::string trim(std::string& need, char ch);	
		static	std::string upcase(std::string& need);
		static	std::string lowcase(std::string& need);
  	};
}

//之所以需要MAKE_NAME和CONCATENATE两个宏定义，是因为如果只有一个
//"MAKE_NAME( S1, S2 ) S1##S2"定义，则形式参数S1和S2不会用实际参数替换
#define MAKE_NAME( S1, S2 ) CONCATENATE( S1, S2 )
#define CONCATENATE( S1, S2 ) S1##S2

#define REGISTER_SUIT( NAME ) extern "C" { improve::dumb NAME;}

#endif
