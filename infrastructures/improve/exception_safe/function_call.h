// function_call.h: interface for the function_call class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(EXCEPTION_SAFE_FUNCTION_CALL)
#define EXCEPTION_SAFE_FUNCTION_CALL

#include "action.h"
#include <boost/function.hpp>
namespace improve
{
	template<typename FUN_T>
		class function_call : public action  
		{
		public:
			function_call(const FUN_T undo_fun)
 			:_undo_fun(undo_fun)
			{}

			virtual 
			~function_call()
			{}

  			void
   			undo()
   			{
   				_undo_fun();
      		}
 		protected:
 			const boost::function0<void> _undo_fun;

		};

	template<typename FUN_T, typename PARAM>
		class function_call_one : public action  
		{
		public:
			function_call_one(const FUN_T undo_fun,PARAM param)
 			:_undo_fun(undo_fun),_param(param)
			{}

			virtual 
			~function_call_one()
			{}

  			void
   			undo()
   			{
   				_undo_fun(_param);
      		}
 		protected:
 			const boost::function1<void,PARAM> _undo_fun;
 			PARAM _param;
		};

	template<typename FUN_T, typename PARAM1,typename PARAM2>
 		class function_call_two :public action
 		{
 			public:
 				function_call_two(const FUN_T undo_fun,PARAM1 param1,PARAM2 param2)
 				:_undo_fun(undo_fun),_param1(param1),_param2(param2)
 		 		{}

				virtual
				~function_call_two()
				{}

   				void
   				undo()
   				{
   					_undo_fun(_param1,_param2);
      			}
 			protected:
 				boost::function2<void ,PARAM1,PARAM2> _undo_fun;
 				PARAM1 _param1;
				PARAM2 _param2;
  		};



	template<typename FUN_T, typename PARAM1,typename PARAM2,typename PARAM3>
 		class function_call_three :public action
 		{
 			public:
 				function_call_three(const FUN_T undo_fun,
									PARAM1 param1,
									PARAM2 param2, 
									PARAM3 param3)
 				:_undo_fun(undo_fun),_param1(param1),_param2(param2),_param3(param3)
 		 		{}

				virtual
				~function_call_three()
				{}

   				void
   				undo()
   				{
   					_undo_fun(_param1,_param2,_param3);
      			}
 			protected:
 				boost::function3<void ,PARAM1,PARAM2,PARAM3> _undo_fun;
 				PARAM1 _param1;
				PARAM2 _param2;
				PARAM3 _param3;
  		};


	template<typename FUN_T, typename PARAM1,typename PARAM2,
			typename PARAM3,typename PARAM4>
 		class function_call_four :public action
 		{
 			public:
 				function_call_four(const FUN_T undo_fun,
									PARAM1& param1,
									PARAM2& param2, 
									PARAM3& param3,
									PARAM4& param4)
 				:_undo_fun(undo_fun),_param1(param1),_param2(param2)
				 ,_param3(param3),_param4(param4)
 		 		{}

				virtual
				~function_call_four()
				{}

   				void
   				undo()
   				{
   					_undo_fun(_param1,_param2,_param3,_param4);
      			}
 			protected:
 				boost::function4<void ,PARAM1,PARAM2,PARAM3,PARAM4> _undo_fun;
 				PARAM1 _param1;
				PARAM2 _param2;
				PARAM3 _param3;
				PARAM4 _param4;
  		};
}
#endif // !defined(EXCEPTION_SAFE_FUNCTION_CALL)
