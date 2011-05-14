#ifndef __FILESET_TEST_H__
#define __FILESET_TEST_H__
#include "../file_set.h"
#include<cppunit/extensions/HelperMacros.h> 
#include<cppunit/TestCase.h> 
#include<stdexcept>

class fileset_testcase : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE(fileset_testcase);
	CPPUNIT_TEST(split_fileset_writer_test);
	CPPUNIT_TEST(src_fileset_test);
	CPPUNIT_TEST(streambuf_iter_test);
	CPPUNIT_TEST_SUITE_END();
public: 
	void split_fileset_writer_test();
	void src_fileset_test();
    void streambuf_iter_test();

    void write_temp_split();
};
#endif
