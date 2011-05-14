#include <logic/common.h>
#include <improve/time.h>
#include <logic/task.h>
#include <logic/task_manager.h>
#include <iostream>
#include <boost/tokenizer.hpp> 
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <unistd.h>
#include <improve/utility/args_analyze.h>
#include <improve/exception/exception_handler.h>
#include <boost/filesystem/path.hpp>
#include <log4cpp/Category.hh> 
#include <boost/date_time/posix_time/posix_time.hpp>

#include "common.h"

using namespace std;
using namespace logs::logic;
using namespace logs::entity;
using namespace log4cpp;
using namespace improve;
using namespace boost;
using namespace boost::posix_time;


const char* STAT_WLOG_VERSION = "1.2.5-release";


str_vector 
analyse_filelist(const string& files)
{/*{{{*/
    try
    {
        str_vector file_list;
        string files_str = trim_copy_if(files, is_any_of("\"\'"));
        split(file_list, files, is_any_of(", \t\n"));
        for(size_t i=0 ; i<file_list.size() ; ++i)
        {
            trim(file_list[i]);
        }
        return file_list;
    }
    catch (std::exception& e)
    {
        throw runtime_error(string("analyse_filelist error! detail:")+e.what());
    }
}/*}}}*/

// I've add a comment here

bool analyse_param(int argc, char* argv[], str_vector& src_files, string& plan_file,
                    string& log_file, string& out_dir, bool& is_crash)
{/*{{{*/
	char opt;
	string value;
	args_analyzer analyzer(argc,argv);
	while (analyzer.getopt(opt,value))
	{
		switch (opt)
		{
        case 'f':
            {
                str_vector file_list = analyse_filelist(value);
                src_files.insert(src_files.begin(), file_list.begin(), file_list.end());
            }
            break;
		case 'p':
			plan_file = value;	
			break;
		case 'l':
			log_file = value;
			break;
		case 'o':
			out_dir = value;
			break;
		case 'c':
            is_crash = true;
			break;
		case 'v':
            cout << STAT_WLOG_VERSION << endl;
            break;
		default:
            cout<<"unknow option: \"-"<<opt<<"\""<<endl;
			return false;
		}
	}
    if(!plan_file.empty() && !log_file.empty() && !out_dir.empty())
    	return true;

    return false;
}
/*}}}*/


int
main(int ac, char* av[])
{/*{{{*/
	
	string log_file;
	string plan_file;
    str_vector src_files;
    string out_dir;
    bool is_crash = false;
	if(!analyse_param(ac,av,src_files,plan_file,log_file,out_dir,is_crash))
	{
		string useage("useage: stat_wlog -f \"src*.log\" -p stat.conf -l log.config [-c] [-v]");
		cout<<useage<<endl;
		return -1;
	}
	
	if(!config_log(log_file)) return -1;
	
    Category& root = Category::getRoot();
	Category& perf_logger= Category::getInstance("performance");
    perf_logger.infoStream()<<"_______________________start_____________________________";
	try
	{
		task_manager  manager(out_dir);
		performance_time work_timer;
		if(!manager.is_plan_ready(plan_file))
		{
			root.errorStream()<<"Stat plan config file is error :" << plan_file;
			return -1;
		}
		work_timer.start();
        manager.set_override_datasrc(src_files);
		manager.load_plan(plan_file, is_crash);
		while(manager.have_task())
		{
			task::shared_ptr one =manager.assign_task();
			one->execute();
		}
		work_timer.end();
		perf_logger.infoStream()<<"total time:"<< work_timer.duration_ms();
        return 0;
	}
    catch(improve::rt_exception &ie )
    {
        ie.stream()<<endl;
        cout<<ie.what()<<endl;
		root.errorStream()<<ie.what();
    }
	catch(std::exception& e)
	{
        cout<<e.what()<<endl;
		root.errorStream()<<e.what();
	}
	return -1;
}/*}}}*/


