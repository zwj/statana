#ifndef __MERGER_H__
#define __MERGER_H__
//#include <boost/function.hpp>
//#include <boost/mem_fn.hpp>
//#include <boost/utility.hpp>
#include <ext/hash_map>
#include "file_set.h"
#include "line_assembler.h"
#include <fstream>
#include "merger_ex.h"
//#include "shared.h"
#include "log_def.h"
//#include <hash_map>


struct eqstr
{
    bool operator()(const char* s1, const char* s2) const
    {
        return strcmp(s1, s2) == 0;
    }
};


class split_merger
{

    public:
       typedef __gnu_cxx::hash_map<const char*, stat_pair::s_ptr, str_hash,eqstr> count_map_t;
        split_merger(ipair_analyzer* anaer_ptr, ivalue_calculator* calu_ptr);
        void run_single(src_fileset& src_set, std::ofstream& outs);
        void run_multi(src_fileset& src_set, std::ofstream& outs);
        void run(src_fileset& src_set, std::ofstream& outs);

    private:
        /*{{{*/
        void proc_file(count_map_t& result,  std::ifstream* ins_ptr, std::string& filename);
        bool proc_line(count_map_t& result,  std::ifstream* ins_ptr );
        void output_result(count_map_t& result, std::ofstream& outs);

        ivalue_calculator* _calu_ptr ;
        ipair_analyzer*    _analyzer_ptr;
        /*}}}*/

};


#endif
