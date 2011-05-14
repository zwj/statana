#include <entity/dimension_factory.h>
#include "dim_factory_test.h" 
#include <entity/dimension.h>
CPPUNIT_TEST_SUITE_REGISTRATION(dimension_factory_testcase); 
using namespace std;
using namespace logs::entity;
dimension_factory_testcase::dimension_factory_testcase()
{
}
void
dimension_factory_testcase::create_defut()
{/*{{{*/
    dimension_factory factory;
    factory.create_default_dim("INT","test");
}/*}}}*/
