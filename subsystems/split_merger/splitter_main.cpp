#include <unistd.h>
#include <math.h>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/scoped_ptr.hpp>
#include "splitter.h"
using namespace std;
using namespace boost;



//#ifndef SPLITTER_VERSION
//#   error "Error: macro SPLITTER_VERSION was not defined, check the bjam config file."
//#endif
#define SPLITTER_VERSION "1.5"


void printLogo()
{
    cout<<"(Programme: resulter_splitter, Version: "<<SPLITTER_VERSION<<")\n";
}

void printUsage()
{/*{{{*/
    printLogo();
    cout<<"Usage: result_splitter -f \"file1,file2[,...]\" [-m key|hash] [-w wipe|keyonly|nowipe] [-n split_count] [-t delimiter] [-k \"col1 [,col2 [...]]\"] [-l logconfig] [-o <out file>]\n"
        <<"  Options:\n"
        <<"  -f  ----  Specifies a comma-separated list as original input files to be splitted,\n"
        <<"              wildcards (*, ?) is allowed, eg: -f \"abc.txt,src*,file_?\".\n"
        <<"              ***NOTE***: the argument value should be double quoted if wildcards was used\n"
        <<"                          or space character was included.\n"
        <<"  -m  ----  Specifies the mode of splitter, \"*KEY*\" mode distinguish different lines by key name,\n"
        <<"              while \"*HASH*\" mode distinguish by hashed key value. HASH mode is the default.\n"
        <<"  -w  ----  Specifies \"*WIPE*\" if you want to wipe the selected keys (by -k option) from the result,\n"
        <<"              or \"*KEYONLY*\" to preserve only the selected keys and \"*NOWIPE*\" to preserve\n"
        <<"              the whole line as the output. NOWIPE is the default.\n"
        <<"  -n  ----  Specifies how many pieces you want to split the source files.\n"
        <<"              Accept a decimal number, eg: -n 5\n"
        <<"  -t  ----  Specified the delimiter character to split the fields, comma is the default.\n"
        <<"  -k  ----  Specifies the columns to be selected as final output key, in the form of\n"
        <<"              a comma-separated list contain column index beginning from 1, eg: -k \"1,3,4\"\n"
        <<"  -l  ----  Specifies a log4cpp configuration file.\n"
        <<"  -o  ----  Specifies the output merged file name.\n";
}/*}}}*/


void printInfo(const str_vector& srcfiles, bool run_keymode, KeyWipeMode wipemode)
{/*{{{*/
    printLogo();

    std::string key_mode_str = run_keymode ? "*KEY*" : "*HASH*";
    std::string wipe_mode_str;
    if (wipemode == KeyOnly)
       wipe_mode_str = "only the selected keys will be preserved in the output result.";
    else if (wipemode == WipeKey)
       wipe_mode_str = "only the columns didn't selected will be preserved in the output result.";
    else
       wipe_mode_str = "the whole line will be preserved in the output result.";

    std::string files_str;
    char fmtstr[1024];
    int ord_len = (int)log10f( (int)srcfiles.size() )+1;
    sprintf(fmtstr, "(%%%dd) ", ord_len);
    char buf[1024];
    for (size_t i=0 ; i<srcfiles.size() ; ++i)
    {
        files_str += "\n";
        sprintf(buf, fmtstr, i+1);
        files_str += line_begin + "   " + buf + srcfiles[i];
    }
    cout<<line_begin<<endl
        <<line_begin<<"The following files will be splitted in "<<key_mode_str<<" mode, and "<<wipe_mode_str<<endl
        <<line_begin<<files_str<<endl
        <<line_begin<<endl
        <<line_begin<<"(For more information about KEY|HASH and WIPE|KEYONLY|NOWIPE mode, run this programme with -? option.)\n"
        <<line_begin<<endl;
}/*}}}*/


int
main(int argc, char* argv[])
{/*{{{*/
    str_vector file_list;
    int_vector col_list;
    size_t split_count = 0;
    string out_name;
    string log_file("log.config");
    bool dolog = false;
    char delimiter = default_delimiter;
    const char* argspec = "f:m:w:n:t:k:l:o:?:";
    char ch = getopt(argc, argv, argspec);
    bool run_keymode = false;
    KeyWipeMode wipemode = NoWipe;
    if ( ch == -1 )
    {
        printUsage();
        return -1;
    }
    while (ch != -1)
    {
        switch (ch) 
        {
            case 'f': file_list = analyse_filelist(string(optarg)); break;
            case 'm': run_keymode = (strcasecmp(optarg, "key") == 0); break;
            case 'w':
                if (strcasecmp(optarg, "wipe") == 0)
                    wipemode = WipeKey;
                else if (strcasecmp(optarg, "keyonly") == 0)
                    wipemode = KeyOnly;
                else
                    wipemode = NoWipe;
                break;
            case 'n': split_count = lexical_cast<int>(string(optarg));  break;
            case 't': delimiter = optarg[0];            break;
            case 'k': col_list = analyse_collist(string(optarg));   break;
            case 'o': out_name = string(optarg);                    break;
            case 'l': log_file = string(optarg);    dolog = true;   break;
            case '?':
            default: printUsage(); return -1;
        }
        ch = getopt(argc, argv, argspec);
    }

    // check args
    if (out_name.empty()) out_name = "split/file";
    //if (dolog && !config_log(log_file)) return -1;
    //log4cpp::Category& logger(log4cpp::Category::getInstance(__FUNCTION__));		

    // do split

    try
    {
        // init core objs
        splitter        splt(col_list, delimiter, wipemode);
        src_fileset     src_set(file_list);
        scoped_ptr<fileset_writer> dest_writer;
//        fileset_writer* dest_writer;
        

        // select split fileset writer depending on the mode
        if (run_keymode)
            dest_writer.reset(  new keysplit_fileset_writer(out_name));
        else
            dest_writer.reset( new split_fileset_writer(out_name, split_count));
        if (dest_writer == NULL)
            throw runtime_error(string("Error: not enough memory to create dest file writer object!\n"));

        // print information
        str_vector srcfiles = src_set.get_expanded_file_list();
        printInfo(srcfiles, run_keymode, wipemode);

        splt.run(src_set, dest_writer);

        cout<<line_begin<<"Splitter process succeeded!"<<endl
            <<line_begin<<endl;
        return 0;
    }
    catch (exception& ex)
    {
        cout<<ex.what()<<endl;
        //if (dolog) logger.errorStream()<<"error:"<<ex.what();
    }
    catch(...)
    {
        cout<<"unknow error!"<<endl;
        //if (dolog) logger.errorStream()<<"unknow error!";
    }
    return -1;
}/*}}}*/


