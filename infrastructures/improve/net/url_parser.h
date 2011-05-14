#include <string> 
#include <boost/regex.hpp>
class url_info
{
public:
	std::string _protocal;
	std::string _username;
	std::string _password;
	std::string _hostname;
	int			_port;
	std::string	_path;
	std::string _filename;

	url_info()
	:_port(80)	
	{}
};
class url_parser
{
	boost::regex _url_pattern;
public:
	url_parser();
	url_info parse(const std::string& url);
};
