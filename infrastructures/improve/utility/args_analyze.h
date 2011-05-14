#ifndef __ARGS_ANALYZE_H__
#define __ARGS_ANALYZE_H__
#include <string>
namespace  improve
{

class impl;
class args_analyzer
{
public:	
	args_analyzer(int argc, char ** args);
	~args_analyzer();
	bool getopt(char& opt, std::string& value);
private:
	impl* _impl_ptr;
};

}
#endif
