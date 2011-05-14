/*
  Name: exception_safe_machine assignment.h
  Author:  zuowenjian
  Description: 
  Date: 30-07-02 21:29
  Copyright: 
*/


#if !defined(AFX_EVALUATE_H__A7BC5B70_89DF_413F_8E00_18674FB691BE__INCLUDED_)
#define AFX_EVALUATE_H__A7BC5B70_89DF_413F_8E00_18674FB691BE__INCLUDED_


#include "action.h"
namespace improve
{
	template <typename T>
		class assignment : public action  
		{
		public:
			assignment(T& object, const T& new_value)
				:_object_ptr(&object),_old_value(object)
			{
				*_object_ptr=new_value;
  			}

  			assignment(const assignment& other)
				:_object_ptr(other._object_ptr),_old_value(other._old_value)  		
  			{
    		}

  			void 
			undo()
    		{
    			*_object_ptr=_old_value;
     		}		

		private:
			assignment& operator=(const assignment&);
			T* _object_ptr;
			const T  _old_value;
		};
}
#endif // !defined(AFX_assignment_H__A7BC5B70_89DF_413F_8E00_18674FB691BE__INCLUDED_)
