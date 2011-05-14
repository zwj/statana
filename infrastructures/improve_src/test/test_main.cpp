//#include <improve/singleton.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
//#include "singleton_testcase.h"
//#include "../patterns/worker.h"
//using namespace improve;
int main( int argc, char* argv[] )
{
    
    {
       // singleton_testcase::instance();
        CppUnit::TextUi::TestRunner runner;
        CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
        runner.addTest( registry.makeTest() );
        runner.run();
       /* 
        exception_collector* collector_ptr=exception_collector::instance();
        for (int i=collector_ptr->size(); i>0 ; --i)
        {
            std::cout<<collector_ptr->top()<<std::endl;
            collector_ptr->pop();
            
        }
       */ 
    }
    
//    system("pause");
    return 0;
}
