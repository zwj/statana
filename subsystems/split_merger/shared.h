#ifndef __SHARED_H__
#define __SHARED_H__

#include <string>
#include <iterator>
#include <iostream>
#include <ext/hash_map>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/pool/singleton_pool.hpp>
#include <log4cpp/Category.hh>


#define USE_EXCEPTION
#include "improve/dbc.h"
#include "../Stat/common.h"


// contants

const int MAX_COL_COUNT = 32;

const int MAX_COUNT_LEN = 256;
const int MAX_KEY_LEN   = 2048;
const int MAX_LINE_LEN  = MAX_COUNT_LEN + MAX_KEY_LEN + 1;

const char default_delimiter = ',';
const std::string line_begin = "---> ";

const char PROGRESS_CHAR = '.';
const int PROGRESS_REPR_COUNT = 1000000; // one dot representing 1000000 lines of progress


// typedef

typedef std::vector<std::string> str_vector;
typedef std::vector<size_t> int_vector;
typedef boost::tokenizer<boost::char_separator<char> > char_tokenizer;
typedef std::istreambuf_iterator<char> insbuf_iter;


// global function

str_vector analyse_filelist(const std::string& files);
int_vector analyse_collist(const std::string& cols);


// exceptions
const int MAX_ERR_COUNT = 60;

class input_error : public std::runtime_error
{
public:
    input_error(const std::string& msg) : std::runtime_error(msg) {}
    input_error(const input_error& other) : std::runtime_error(other) {}
};

class output_error : public std::runtime_error
{
public:
    output_error(const std::string& msg) : std::runtime_error(msg) {}
    output_error(const output_error& err) : std::runtime_error(err.what()) {}
};




// string key object

const int KEYBUF_SIZE1 = MAX_KEY_LEN;
const int KEYBUF_SIZE2 = MAX_KEY_LEN / 2;
const int KEYBUF_SIZE3 = MAX_KEY_LEN / 4;
const int KEYBUF_SIZE4 = MAX_KEY_LEN / 8;

/*
struct KeyPoolTag1 { };
typedef boost::singleton_pool<KeyPoolTag1, KEYBUF_SIZE1> key_pool1;
struct KeyPoolTag2 { };
typedef boost::singleton_pool<KeyPoolTag2, KEYBUF_SIZE2> key_pool2;

struct KeyPoolTag3 { };
typedef boost::singleton_pool<KeyPoolTag3, KEYBUF_SIZE3> key_pool3;
struct KeyPoolTag4 { };
typedef boost::singleton_pool<KeyPoolTag4, KEYBUF_SIZE4> key_pool4;

*/

class key_pool
{/*{{{*/
    public:
        key_pool()
            : _pool1(KEYBUF_SIZE1)
            , _pool2(KEYBUF_SIZE2)
            , _pool3(KEYBUF_SIZE3)
            , _pool4(KEYBUF_SIZE4)
        {
        }

        char* malloc(int len)
        {
            if (len < KEYBUF_SIZE4)
            {
                return (char*)_pool4.malloc();
            }
            else if ( len < KEYBUF_SIZE3)
            {
                return (char*)_pool3.malloc();
            }
            else if (len < KEYBUF_SIZE2)
            {
                return (char*)_pool2.malloc();
            }
            else if (len < KEYBUF_SIZE1)
            {
                return (char*)_pool1.malloc();
            }
            else
            {
                throw input_error("Warning: line exceeds the buffer length, will be ignored!");
            }
            // this should never be executed
            return NULL;
        }

    private:
        boost::pool<>   _pool1;
        boost::pool<>   _pool2;
        boost::pool<>   _pool3;
        boost::pool<>   _pool4;
};/*}}}*/


class str_key
{/*{{{*/
    private:
        const char* _value;

    public:
        str_key(const char* val)
            :_value(val)
            {
            }
        str_key(const str_key& right)
        {
            this->_value = right._value;
        }
        const str_key& operator=(const str_key& right)
        {
            this->_value = right._value;
            return (*this);
        }
        const char* str() const 
        {
            return _value;
        }

        inline friend bool operator==(const str_key &left, const str_key &right)
        {
            return (strcmp(left._value,right._value) == 0);
        }

};/*}}}*/


struct str_hash
{/*{{{*/
    size_t operator()(const str_key& key) const
    {
//        return stlp_priv::__stl_hash_string(key.str());
        return __gnu_cxx::__stl_hash_string(key.str());
    }
};/*}}}*/




#endif
