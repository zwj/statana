#include <iostream>
//#include <fstream>
#include <strstream>
#include "fileset_test.h" 
#include "../file_set.h"
#include "../line_assembler.h"
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(fileset_testcase); 



void
fileset_testcase::write_temp_split()
{
    split_fileset_writer split_set("split_temp/split_file", 3);
    split_set.write("1", "haha");
}

void
fileset_testcase::split_fileset_writer_test()
{
    this->write_temp_split();
    ifstream ins("split_temp/split_file_1.split");
    CPPUNIT_ASSERT(ins);
    char buf[1024];
    ins.getline(buf, 1024);
    CPPUNIT_ASSERT(string(buf) == "haha");
}


void
fileset_testcase::streambuf_iter_test()
{
    string test_str("0123456789");
    istrstream ins(test_str.c_str());
    insbuf_iter iter(ins);
    char ch = *iter++;
    ch = *iter++;
    CPPUNIT_ASSERT(ins.peek() == '2');
    ins.ignore(100, '6');
    insbuf_iter iter2(ins);
    CPPUNIT_ASSERT((*iter2) == '7');
}


void
fileset_testcase::src_fileset_test()
{
    this->write_temp_split();

    str_vector file_list = analyse_filelist("split_temp/split_file*.split");
    src_fileset src_set(file_list);
//    CPPUNIT_ASSERT(src_set.file_exists());
    CPPUNIT_ASSERT(src_set.next_stream() != NULL);
    CPPUNIT_ASSERT(src_set.next_stream() != NULL);
    CPPUNIT_ASSERT(src_set.next_stream() != NULL);
    CPPUNIT_ASSERT(src_set.next_stream() == NULL);
}


