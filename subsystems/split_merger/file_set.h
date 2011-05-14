#ifndef __FILE_SET_H__
#define __FILE_SET_H__

#include <vector>
#include <fstream>
#include <boost/filesystem/path.hpp>
#include <map>
//#include <boost/filesystem/operations.hpp>
#include "shared.h"
//#include "line_assembler.h"


class src_fileset
{
    /*{{{*/
    public:
        typedef std::pair<std::string,std::ifstream*> stream_pair_t;
        typedef std::vector<stream_pair_t> stream_vector_t;

    public:
        src_fileset(const str_vector& file_list);
        ~src_fileset();
        stream_pair_t* next_stream();
        str_vector get_expanded_file_list() { return _expanded_file_list; }

    private:
        str_vector expand_wildcards(const str_vector& file_list);
        void open_all_streams(const str_vector& expanded_file_list);

    private:
        stream_vector_t _files;
        stream_vector_t::iterator _cur_file;
        str_vector _expanded_file_list;
        /*}}}*/
};


class fileset_writer
{/*{{{*/
    public:
        virtual ~fileset_writer() {}

        virtual void write(const char* key, const char* line) = 0;
        virtual void set_current_filename(const std::string& filename) { _cur_filename = filename; } 

    protected:
        fileset_writer(const std::string out_filename) {_out_filename = out_filename;}
        void clear_split_files(const std::string& base_name);
        boost::filesystem::path get_parent_dir(const std::string& base_name);

    protected:
        std::string _cur_filename;
        std::string _out_filename;
};/*}}}*/


class split_fileset_writer : public fileset_writer
{/*{{{*/
    public:
        split_fileset_writer(const std::string& base_name, int split_count);
        virtual ~split_fileset_writer();
        void write(const char* key, const char* line);

    private:
        size_t gen_hash(const char* key);
        void create_split_files(const std::string& base_name);

    private:
        typedef std::vector<std::ofstream*> stream_vec_t;
        stream_vec_t _split_vec;
        int _split_count;
};/*}}}*/


class keysplit_fileset_writer : public fileset_writer
{/*{{{*/
    public:
        keysplit_fileset_writer(const std::string& base_name);
        virtual ~keysplit_fileset_writer();

        void set_current_filename(const std::string& filename);
        void write(const char* key, const char* line);

    private:
        void create_base_directory(const std::string& base_name);
        std::ofstream* get_split_file_stream(const std::string& full_filepath);
        void close_split_files();
        std::string normalizeKey(const char* key);

    private:
        typedef std::map<std::string, std::ofstream*> stream_map_t;
        stream_map_t _split_map;
        boost::filesystem::path _out_path;
};/*}}}*/


#endif

