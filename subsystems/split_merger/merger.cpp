#include "merger.h"
#include "log_def.h"
#include "line_assembler.h"
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include "improve/utility/utility.h"
#include "improve/utility/exception_def.h"
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

//#include <iostream>
using namespace std;
using namespace boost;
ivalue_calculator::s_ptr  calculator_parser::parse(const char * rawmode)
{/*{{{*/
    string mode= rawmode;
    trim_if(mode,is_any_of("\"\' ")); // phone == "423333444"

    if(strcasecmp(mode.c_str(), "count") == 0)
        return  ivalue_calculator::s_ptr(new  value_count());
    else if(strcasecmp(mode.c_str(), "sum") == 0)
        return  ivalue_calculator::s_ptr(new  value_sum());

    regex value_modle("([a-zA-Z]+) gt ([0-9]+)"); 
    cmatch what;
    if(regex_match(mode.c_str(), what, value_modle)) 
    {
        int base = lexical_cast<int>(what[2]);
        string submode= what[1];
        if(strcasecmp(submode.c_str(),"count")==0) 
            return ivalue_calculator::s_ptr(new  if_gt_value_count(base));
        if(strcasecmp(submode.c_str(),"sum")==0) 
            return ivalue_calculator::s_ptr(new  if_gt_value_sum(base));
    }
    stringstream err("unknow calcu mode:");
    err << rawmode ;
    throw improve::config_exception(err.str().c_str());
}/*}}}*/

split_merger::split_merger(ipair_analyzer* anaer_ptr, ivalue_calculator* calu_ptr)
:_calu_ptr(calu_ptr), _analyzer_ptr( anaer_ptr)
{/*{{{*/
}/*}}}*/

void split_merger::run(src_fileset& src_set, std::ofstream& outs)
{/*{{{*/
    run_multi(src_set,outs);
}/*}}}*/

void split_merger::run_single(src_fileset& src_set, std::ofstream& outs)
{/*{{{*/
    src_fileset::stream_pair_t* ptr = src_set.next_stream();
    while (ptr != NULL)
    {
        std::string& filename = ptr->first;
        ifstream* stream = ptr->second;


        insbuf_iter ins_iter(*stream);
        count_map_t result; // construct a file specific hash_map to store data from one single file
        proc_file(result,  stream, filename );
        output_result(result, outs);
        ptr = src_set.next_stream();
        result.clear();
    }
}/*}}}*/

void split_merger::run_multi(src_fileset& src_set, std::ofstream& outs)
{/*{{{*/
    count_map_t result; // construct all global hash_map to store data from all input files
    src_fileset::stream_pair_t* ptr = src_set.next_stream();
    while (ptr != NULL)
    {
        std::string& filename = ptr->first;
        ifstream* stream = ptr->second;

        insbuf_iter ins_iter(*stream);
        this->proc_file(result, stream, filename);
        ptr = src_set.next_stream();
    }
    this->output_result(result, outs);
    result.clear();
}/*}}}*/

void split_merger::proc_file(count_map_t& result, ifstream* ins_ptr, std::string& filename)
{/*{{{*/
    Category& data_logger = Category::getInstance("zlabs.data");
    data_logger.infoStream()<< "Processing file: "<<filename;
    int proc_count =0; 
    int err_count = 0;
    try
    {

        while (!ins_ptr->eof())
        {
            ++proc_count;
            if (!this->proc_line(result,  ins_ptr))
            {
                if(strlen(_analyzer_ptr->ana_content())>0)
                    data_logger.infoStream() << "failed ,at: " << proc_count << 
                        "line content:["<< _analyzer_ptr->ana_content() << "]";
                ++err_count;
                continue;
            }
        }
    }
    catch (std::exception e)
    {
        Category& root_logger = Category::getRoot();
        root_logger.errorStream()<< __FUNCTION__;
        root_logger.errorStream()<<e.what();
    }
    catch (...)
    {
        Category& root_logger = Category::getRoot();
        root_logger.errorStream() << "Error: Unknown error occurred!";
    }
    data_logger.infoStream() << "File:" << filename << "total lines: " << proc_count << "  *faild lines: " << err_count ;

    /*}}}*/
}

bool split_merger::proc_line(count_map_t& result, ifstream* ins_ptr )
{/*{{{*/

    stat_pair::sptr_vector   sptr_pairs;
    try
    {

        if (!_analyzer_ptr->ana_line(sptr_pairs,*ins_ptr))
            return false;

        BOOST_FOREACH(stat_pair::s_ptr pair_ptr, sptr_pairs)
        {
            count_map_t::iterator acc_pos = result.find(pair_ptr->key());
            if (acc_pos != result.end())
            {
                int v= _calu_ptr->calcu(pair_ptr->value(),acc_pos->second->value());

                acc_pos->second->set_value(v);
            }
            else
            {
                int v = _calu_ptr->calcu(pair_ptr->value(),0);
                pair_ptr->set_value(v);
                if(v > 0 )
                {
                    Category& data_logger = Category::getInstance("zlabs.data");
                    if(data_logger.isDebugEnabled())
                    {
                        data_logger.debugStream() <<"out:" << pair_ptr->key() << "-" << pair_ptr->value();
                    }
                    result[pair_ptr->key()] = pair_ptr;
                }
            }
        }
        return true;
    }
    catch (std::exception& e)
    {
        Category& data_logger = Category::getInstance("zlabs.data");
         _analyzer_ptr->ignore2end(*ins_ptr);
        data_logger.errorStream()<< __FUNCTION__;
        data_logger.errorStream()<<e.what();
        return false;

    }
    return false;
}/*}}}*/


void split_merger::output_result(count_map_t& result, std::ofstream& outs)
{/*{{{*/
    count_map_t::iterator pos = result.begin();
    for(; pos != result.end(); ++pos)
    {

        Category& data_logger = Category::getInstance("zlabs.data");
        if(data_logger.isDebugEnabled())
            data_logger.debugStream() <<  pos->second->value() << "," << pos->first;
        outs<< pos->second->value()  << ","  << pos->first <<endl;

    }
}/*}}}*/


