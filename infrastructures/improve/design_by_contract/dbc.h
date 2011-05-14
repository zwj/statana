#ifndef ASSERT_EX_H_HEADER_INCLUDED_C3AC3A97
#define ASSERT_EX_H_HEADER_INCLUDED_C3AC3A97
#include <cassert>
#include <stdexcept>
#include <sstream>
namespace DBC
{
    /// 
    /// @brief  Assert of design by contract 
    /// it's have diffrent impl by compiler condition;  
    /// default is  throw exception;
    /// 

    template<class T>
        class validate
        {/*{{{*/
            public:

                /// 
                /// @brief  assert precondition ;
                /// 
                /// @param result 
                /// @param exp		discription; 
                /// @param function	function name; 
                /// @param file		file name; 
                /// @param line		line nu of file; 
                /// 
                static void require(bool result,const char* exp,
                        const char* function ,
                        const char* file,int line)
                {/*{{{*/
                    if(!result)
                    {
                        std::stringstream buf;
                        buf<<"require false! :["<<exp<<"] in <"<<function<<" file: "<<file<<" line:"<<line<<"> ";
                        //                throw logic_error(buf.str());
                        throw T(buf.str().c_str());
                    }

                }/*}}}*/
                /// 
                /// @brief	assert postcondition; 
                /// 
                /// @param result 
                /// @param exp 
                /// @param function 
                /// @param file 
                /// @param line 
                /// 
                static void ensure( bool result,const char* exp,
                        const char* function ,
                        const char* file,int line)
                {/*{{{*/
                    if(!result)
                    {
                        std::stringstream buf;
                        buf<<"ensure false! :["<<exp<<"] in <"<<function<<" file: "<<file<<" line:"<<line<<"> ";
                        throw T(buf.str().c_str());
                    }
                }/*}}}*/
        };
        /*}}}*/

    typedef validate<std::logic_error> assert;
}
#ifndef USE_EXCEPTION
#define  REQUIRE(x)         assert(x);
#define  REQUIRE_MSG(x,msg) assert(x);
#define  ENSURE(x)          assert(x);
#define  ENSURE_MSG(x,msg)  assert(x);
#define  FAIL_MSG(msg)      assert(false);
#define  UNEXPECTED(msg)    assert(false);
#else
#define  REQUIRE(x)         DBC::assert::require(x,#x, __FUNCTION__,__FILE__,__LINE__);
#define  ENSURE(x)          DBC::assert::ensure( x,#x, __FUNCTION__,__FILE__,__LINE__);
#define  REQUIRE_MSG(x,msg) DBC::assert::require(x,msg, __FUNCTION__,__FILE__,__LINE__);
#define  ENSURE_MSG(x,msg)  DBC::assert::ensure(x,msg, __FUNCTION__,__FILE__,__LINE__);
#define  FAIL_MSG(msg)      DBC::assert::ensure(false,msg, __FUNCTION__,__FILE__,__LINE__);
#define  UNEXPECTED(msg)    DBC::assert::ensure(false,msg, __FUNCTION__,__FILE__,__LINE__);
#endif


#define  I_REQUIRE(x)         self_validate::require(x,#x, __FUNCTION__,__FILE__,__LINE__);
#define  I_ENSURE(x)          self_validate::ensure( x,#x, __FUNCTION__,__FILE__,__LINE__);
#define  I_REQUIRE_MSG(x,msg) self_validate::require(x,msg, __FUNCTION__,__FILE__,__LINE__);
#define  I_ENSURE_MSG(x,msg)  self_validate::ensure(x,msg, __FUNCTION__,__FILE__,__LINE__);
#define  I_FAIL_MSG(msg)      self_validate::ensure(false,msg, __FUNCTION__,__FILE__,__LINE__);

#endif //ASSERT_EX_H_HEADER_INCLUDED_C3AC3A97
