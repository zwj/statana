#ifndef ___UTILITY_H__
#define ___UTILITY_H__

#include <string>

class utility
{
public:		
	static std::string unify(std::string will);
    static bool split_pair(const std::string str, std::string& first, 
                            std::string& second,const std::string& separator);
    static unsigned int hexstr2int(const std::string& str);
};

#endif
