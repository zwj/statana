/**
* @file 
*
* 本.h文件定义scope_guard接口。
*
* @author zuowenjian zuowj@sina.com
*/


#if !defined(AFX_SCOPE_GUARD_H__C3E48CE7_A264_48A3_8FB9_1DFEEB07CB13__INCLUDED_)
#define AFX_SCOPE_GUARD_H__C3E48CE7_A264_48A3_8FB9_1DFEEB07CB13__INCLUDED_



#include "assignment.h"
#include "function_call.h"
#include <vector>
#include <boost/shared_ptr.hpp>
namespace improve
{
  /**
    * @brief scope_guard 的說明
    *
    * 
    * 
    *
    */
	class scope_guard  
	{
	  public:
		scope_guard()
		:_is_unsafe(true)
		{	}		
		virtual ~scope_guard()
		{
			try
			{
				std::vector<action::shared_ptr>::iterator it;
				if(_is_unsafe)
				{
	 				for(it=_action_ptrs.begin();it!=_action_ptrs.end();++it)		
	 				{
	 					action::shared_ptr action_ptr=*it;
						action_ptr->undo();
	  				}
				}
			}
			catch(...)
			{}
		}		

		/**
			* @brief obj_assign()
			* Requires: 
			* Effects: 对象赋值。
			* Throws:  
			* Danger:
			* 
			*/
		template < typename T>
  			void 
			obj_assign(T& object, const T& new_value)
  			{
					action::shared_ptr action_ptr(new assignment<T>(object,new_value));
  				_action_ptrs.push_back(action_ptr);  		
    		}

		/**
			* @brief obj_assign()
			* Requires: 
			* Effects: 注册undo的操作。
			* Throws:  
			* Danger:
			* 
			*/
		template <typename FUN_T>
			void
			register_todo(const FUN_T undo_function)
			{
  				action::shared_ptr action_ptr(
					new function_call<FUN_T>(undo_function));
  				_action_ptrs.push_back(action_ptr);  		
				
			}

		template <typename FUN_T,typename PARAM>
			void
			register_todo(const FUN_T undo_function,PARAM param)
			{
  				action::shared_ptr action_ptr(
					new function_call_one<FUN_T,PARAM>(undo_function,param));
  				_action_ptrs.push_back(action_ptr);  		
				
			}

		template<typename FUN_T, typename PARAM1,typename PARAM2>
			void
			register_todo(const FUN_T undo_function,PARAM1 param1,PARAM2 param2)
			{
  				action::shared_ptr action_ptr(
					new function_call_two<FUN_T,PARAM1,PARAM2>(undo_function,param1,param2));
  				_action_ptrs.push_back(action_ptr);  		
			}

		template<typename FUN_T, typename PARAM1,typename PARAM2,typename PARAM3>
			void
			register_todo(const FUN_T undo_function,
						PARAM1 param1,
						PARAM2 param2,
						PARAM3 param3)
			{
  				action::shared_ptr action_ptr(
					new function_call_three<FUN_T,PARAM1,PARAM2,PARAM3>(
						undo_function,param1,param2,param3)
					);
  				_action_ptrs.push_back(action_ptr);  		
			}

		template<typename FUN_T, 
				typename PARAM1,
				typename PARAM2,
				typename PARAM3,
				typename PARAM4>
			void
			register_todo(const FUN_T undo_function,
						PARAM1 param1,
						PARAM2 param2,
						PARAM3 param3,
						PARAM3 param4)
			{
  				action::shared_ptr action_ptr(
					new function_call_four<FUN_T,PARAM1,PARAM2,PARAM3,PARAM4>(
						undo_function,param1,param2,param3,param4)
					);
  				_action_ptrs.push_back(action_ptr);  		
			}

		/**
			* @brief dismiss()
			* Requires: 
			* Effects: 撤消所有的undo操作。
			* Throws:  
			* Danger:
			* 
			*/
		void dismiss()
		{
			_is_unsafe=false;
		}		

	  private:
 			std::vector<action::shared_ptr> _action_ptrs;		
 			bool _is_unsafe;
	};
	
	template < typename T> 
	  class ref_holder
	  {		
	     public:
	       	ref_holder(T& ref)
	       	:_ref(ref)
	       	{}
			ref_holder(const ref_holder& other)
				:_ref(other._ref)
			{
			}
			operator T&() const
			{
				return _ref;
			}
        private:
			ref_holder& operator=(ref_holder& other);
			T& _ref;	       	
      };
      
	template < typename T >
		inline ref_holder<T>
		by_ref(T& t)
		{
			return ref_holder<T>(t);
 		}
}

#endif // !defined(AFX_SCOPE_GUARD_H__C3E48CE7_A264_48A3_8FB9_1DFEEB07CB13__INCLUDED_)
