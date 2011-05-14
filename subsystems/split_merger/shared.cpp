#include <boost/lexical_cast.hpp>
#include "shared.h"
using namespace std;
using namespace boost;


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


int_vector
analyse_collist(const string& cols)
{/*{{{*/
    try
    {
        str_vector col_str_list;
        string cols_str = trim_copy_if(cols, is_any_of("\"\'"));
        split(col_str_list, cols_str, is_any_of(", \t\tn"));
        int_vector col_list;
        for (size_t i=0 ; i<col_str_list.size() ; ++i)
        {
            string col_str = trim_copy(col_str_list[i]);
            if (col_str.empty())
                continue;
            size_t col = lexical_cast<int>(col_str);
            if (col < 1)
                throw runtime_error(string("\"The specified column must be greater than 0\""));
            col_list.push_back(col);
        }
        return col_list;
    }
    catch (std::exception& e)
    {
        throw runtime_error(string("analyse_collist error! detail:")+e.what());
    }
}/*}}}*/



