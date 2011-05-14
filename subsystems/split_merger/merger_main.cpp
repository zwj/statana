#include <unistd.h>
#include <math.h>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include "improve/utility/utility.h"
#include "improve/utility/exception_def.h"
#include "merger.h"
#include "log_def.h"
#include <boost/regex.hpp>

typedef  DBC::validate<improve::config_exception > self_validate;

using namespace std;

//#ifndef MERGER_VERSION
//#   error "Error: macro MERGER_VERSION was not defined, check the bjam config file."
//#endif

#define MERGER_VERSION "1.7"



void printLogo()
{
    cout<<"(Programme: resulter_merger, Version: "<<MERGER_VERSION<<")\n";
}

string useage()
{/*{{{*/
    string     useage_desc ="Usage: result_merger "
                       "-f \"split_file_name(allow wildcards)\" "
                       "[-t delimiter] "
                       "[-k \"col1 [,col2 [...]]\"] "
                       "[-K \"col1 [,col2 [...]]\"] "
                       "[-l <logconfig>] "
                       "[-m multi|single] "
                       "[-c \"count|sum|count qt ? | sum qt ?]  \" "
                       "[-o <out file>]\n"
        "  Options:\n"
        "  -f  ----  Specifies a comma-separated list as original input files to be merged,\n"
        "            wildcards (*, ?) is allowed, eg: -f \"abc.txt,src*,file_?\"\n"
        "            **NOTE***: the argument value should be double quoted if wildcards was used  or space character was included.\n"
        "  -t  ----  Specified the delimiter character to split the fields, comma is the default.\n"
        "  -k  ----  Specifies the columns to be selected as final output key, in the form of a comma-separated list contain column index beginning from 1, eg: -k \"1,3,4\"\n"
        "  -l  ----  Specifies a log4cpp configuration file.\n"
        "  -m  ----  The \"multi\" mode will merge every line from all input files into one result and  write to output file, this is the default behavior;  The \"single\" mode will try to merge the lines from one single input file into one result, and append all the successive results direct into the output file, but not to combine  the results from different files. This mode assumes two different input files do not contain identical keys, in which case the output file may have unmerged lines. This mode is commonly used cooperated with the \"result_splitter\" programme.\n"
        "  -c  ---- calculate mode , such as sum , count , sum qt 5 ,count qt 5 \n"
        "  -o  ----  Specifies the output merged file name.\n";
    return useage_desc;
}/*}}}*/


class merger_task
{/*{{{*/
    public:
        string _out_name;
        string _log_file;
        bool _run_single_mode ;
        ipair_analyzer::s_ptr       _anaer_ptr;
        ivalue_calculator::s_ptr _calculator_ptr;
        char _delimiter ;
        str_vector _file_list;
        void build_analyzer(bool case_siensitive,string keycol)
        {/*{{{*/
            if (keycol.size() ==0 )
            {
                _anaer_ptr.reset( new fullcols_analyzer(_delimiter,case_siensitive));
            }
            else
            {
                _anaer_ptr.reset(new defcols_analyzer(_delimiter,keycol,case_siensitive));
            }

        }/*}}}*/
        static merger_task init(int argc, char* argv[])
        {/*{{{*/
            merger_task task;
            task._out_name = "merged";
            task._run_single_mode = false;
            task._delimiter       =  default_delimiter;
            bool  case_siensitive = true;
            const char* argspec = "uf:t:k:o:l:m:c:?";
            char ch = getopt(argc, argv, argspec);
            I_ENSURE_MSG( ch != -1 , " input fail");
            task._anaer_ptr.reset( new fullcols_analyzer(task._delimiter));
            string keycol="";

            while (ch != -1)
            {/*{{{*/
                switch (ch)
                {
                    case 'u':
                        case_siensitive = false;
                        break;
                    case 'f': 
                        task._file_list = analyse_filelist(string(optarg)); 
                        break;
                    case 't': 
                        task._delimiter = optarg[0];            
                        break;
                    case 'k': 
                        keycol = optarg;
                        break;
                    case 'o': 
                        task._out_name = string(optarg);        
                        break;
                    case 'l': 
                        task._log_file = string(optarg);   
                        break;
                    case 'm': 
                        task._run_single_mode = (strcasecmp(optarg, "single") == 0); 
                        break;
                    case 'c': 
                        task._calculator_ptr = calculator_parser::parse(optarg);
                        break;
                    case '?':
                    default: 
                        stringstream info;
                        info << "-" << ch <<" " << optarg;
                        throw improve::config_exception(info.str().c_str());
                }
                ch = getopt(argc, argv, argspec);
            }/*}}}*/

            task.build_analyzer(case_siensitive,keycol);
            I_ENSURE_MSG(task._anaer_ptr.get() != NULL,"not set pair_anar");
            I_ENSURE_MSG(task._calculator_ptr.get() != NULL,"not set calculator");
            return task;

        }/*}}}*/

};/*}}}*/
int main(int argc, char* argv[])
{/*{{{*/
    ios_base::sync_with_stdio(true);

    merger_task task;/*{{{*/
    try { 

        task = merger_task::init(argc, argv) ;
        if (task._log_file.empty())
            BasicConfigurator::configure();
        else
        {
            PropertyConfigurator::configure(task._log_file);
        }
    } 
    catch(exception& e)
    {
        const char * err = e.what();
        std::cout << "Error:" << err <<"!"  << std::endl;
        std::cout<< useage();
        return -1;

    }/*}}}*/


    Category& logger = Category::getRoot();
    ofstream outs(task._out_name.c_str());
    try
    {

        split_merger merger(task._anaer_ptr.get(),task._calculator_ptr.get());
        src_fileset src_set(task._file_list);

        str_vector srcfiles = src_set.get_expanded_file_list();

        merger.run(src_set, outs);
        outs.close();
        logger.infoStream() <<"Merging process succeeded!" ;
        return 0;
    }
    catch (exception& ex)
    {
        logger.errorStream()<<"error:"<<ex.what();
    }
    catch(...)
    {
        logger.errorStream()<<"unknow error!";
    }
    outs.close();
    return -1;
}/*}}}*/


