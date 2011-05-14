#include <boost/lexical_cast.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <ext/hash_map>
#include <algorithm>
#include <stdlib.h>
#include <glob.h>
#include "file_set.h"

using namespace std;
using namespace boost;


///////////////////////////////////////////////////////////////////////////
//

src_fileset::src_fileset(const str_vector& file_list)
{/*{{{*/
    REQUIRE_MSG(file_list.size() > 0, "Error: src file list is empty!");
    _expanded_file_list = this->expand_wildcards(file_list);
    REQUIRE_MSG(_expanded_file_list.size() > 0, "Error: wildcards-expanded file list is empty!");
    this->open_all_streams(_expanded_file_list);
}/*}}}*/

str_vector
src_fileset::expand_wildcards(const str_vector& file_list)
{/*{{{*/
    str_vector result;
    for (size_t i=0 ; i<file_list.size() ; ++i)
    {
        glob_t glob_result;
        glob(file_list[i].c_str(), 0, NULL, &glob_result);
        for (size_t j=0 ; j<glob_result.gl_pathc; ++j)
        {
            result.push_back(string(glob_result.gl_pathv[j]));
        }
        globfree(&glob_result);
    }
    return result;
}/*}}}*/

void
src_fileset::open_all_streams(const str_vector& expanded_file_list)
{/*{{{*/
    for (size_t i=0 ; i<expanded_file_list.size() ; ++i)
    {
        ifstream* ins = new ifstream(expanded_file_list[i].c_str());
        if ( !(*ins) )
        {
            char buf[1024];
            sprintf(buf,"Error: file \"%s\" does not exist!", expanded_file_list[i].c_str());
            FAIL_MSG(buf);
        }
        stream_pair_t ptr = stream_pair_t(expanded_file_list[i],ins);
        _files.push_back(ptr);
    }
    _cur_file = _files.begin();
}/*}}}*/

src_fileset::~src_fileset()
{/*{{{*/
    for (size_t i=0; i<_files.size() ; ++i)
    {
        ifstream* stream = _files[i].second;
        if (stream->is_open()) stream->close();
        delete stream;
    }
    _files.clear();
}/*}}}*/

src_fileset::stream_pair_t*
src_fileset::next_stream()
{/*{{{*/
    if (_cur_file == _files.end())
        return NULL;
    return &(*_cur_file++);
}/*}}}*/


///////////////////////////////////////////////////////////////////////////
//

filesystem::path
fileset_writer::get_parent_dir(const string& out_filename)
{/*{{{*/
    filesystem::path filepath(out_filename);
    filesystem::path dir = filepath.branch_path();
    if (dir.string() == "")
        dir = filesystem::path(".");
    return dir;
}/*}}}*/

void
fileset_writer::clear_split_files(const string& out_filename)
{/*{{{*/
    filesystem::path dir = this->get_parent_dir(out_filename);
    REQUIRE_MSG(!filesystem::equivalent(dir, filesystem::path(".")),
        "Error: current working directory cannot be the direct parent of the output directory!");
    filesystem::remove_all(dir);
}/*}}}*/

///////////////////////////////////////////////////////////////////////////
//

split_fileset_writer::split_fileset_writer(const string& out_filename, int split_count)
    : fileset_writer(out_filename)
{/*{{{*/
    REQUIRE_MSG(split_count > 0, "Error: split count must be a decimal number greater than 0 !");
    _split_count = split_count;
    this->clear_split_files(out_filename);
    this->create_split_files(out_filename);
}/*}}}*/

split_fileset_writer::~split_fileset_writer()
{/*{{{*/
    for (size_t i=0 ; i<_split_vec.size() ; ++i)
    {
        ofstream* outs = _split_vec[i];
        if (outs->is_open()) outs->close();
        delete outs;
    }
    _split_vec.clear();
}/*}}}*/

void
split_fileset_writer::write(const char* key, const char* line)
{/*{{{*/
    int line_len = strlen(line);
    size_t hash_val = this->gen_hash(key) % _split_count;

    ofstream* stream = _split_vec[hash_val];
    stream->write(line, line_len);
    if (line[line_len-1] != '\n')
        stream->write("\n", 1);
}/*}}}*/

void
split_fileset_writer::create_split_files(const string& out_filename)
{/*{{{*/
    filesystem::path dir = this->get_parent_dir(out_filename);
    if (!filesystem::exists(dir))
        filesystem::create_directories(dir);
    for (int i=0 ; i<_split_count ; ++i)
    {
        string name = out_filename + "_" + lexical_cast<string>(i) + ".split";
        _split_vec.push_back(new ofstream(name.c_str()));
    }
}/*}}}*/

size_t
split_fileset_writer::gen_hash(const char* key)
{
    return __gnu_cxx::__stl_hash_string(key);
}


///////////////////////////////////////////////////////////////////////////
//

keysplit_fileset_writer::keysplit_fileset_writer(const std::string& out_filename)
    : fileset_writer(out_filename)
{/*{{{*/
//    this->clear_split_files(out_filename);
    this->create_base_directory(out_filename);
}/*}}}*/

keysplit_fileset_writer::~keysplit_fileset_writer()
{/*{{{*/
    this->close_split_files();
}/*}}}*/

void
keysplit_fileset_writer::create_base_directory(const std::string& out_filename)
{/*{{{*/
    _out_path = filesystem::path(out_filename);
    if (filesystem::equivalent(_out_path, filesystem::path(".")))
        return;
    if (!filesystem::exists(_out_path))
        filesystem::create_directories(_out_path);
}/*}}}*/

void
keysplit_fileset_writer::set_current_filename(const std::string& filename)
{/*{{{*/
    fileset_writer::set_current_filename(filename);
    this->close_split_files();
}/*}}}*/

void
keysplit_fileset_writer::write(const char* key, const char* line)
{/*{{{*/
    if (strlen(key) == 0)
        return;

    int line_len = strlen(line);
    std::string key_str = this->normalizeKey(key);

    const char* first_sep = "/";
    const char* second_sep = "/";
    if (_out_filename[_out_filename.length()-1] == '/')
        first_sep = "";
    if (key_str[key_str.length()-1] == '/')
        first_sep = "";
    std::string full_filepath = _out_filename + first_sep + key_str + second_sep + _cur_filename;

    ofstream* stream = this->get_split_file_stream(full_filepath);
    stream->write(line, line_len);
    if (line[line_len-1] != '\n')
        stream->write("\n", 1);
}/*}}}*/

std::string
keysplit_fileset_writer::normalizeKey(const char* key)
{/*{{{*/
    std::string key_str(key);
    std::replace(key_str.begin(), key_str.end(), '/', '_');
    std::replace(key_str.begin(), key_str.end(), '\\', '_');
    return key_str;
}/*}}}*/

void
keysplit_fileset_writer::close_split_files()
{/*{{{*/
    stream_map_t::iterator iter = _split_map.begin();
    for (; iter!=_split_map.end() ; ++iter)
    {
        ofstream* stream = iter->second;
        if (stream->is_open()) stream->close();
        delete stream;
    }
    _split_map.clear();
}/*}}}*/


ofstream*
keysplit_fileset_writer::get_split_file_stream(const std::string& full_filepath)
{/*{{{*/
    stream_map_t::iterator iter = _split_map.find(full_filepath);
    if (iter == _split_map.end())
    {
        filesystem::path filepath(full_filepath);
        if (!filesystem::exists(filepath))
            filesystem::create_directories(filepath.branch_path());
        ofstream* stream = new ofstream(full_filepath.c_str());
        _split_map[full_filepath] = stream;
        return stream;
    }
    return iter->second;
}/*}}}*/



