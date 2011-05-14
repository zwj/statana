#ifndef  __STATUS_MACHINE__
#define   __STATUS_MACHINE__
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>
namespace improve
{
	class status_machine_imp
	{
		struct status_direct
		{
			status_direct(int from, int to)
			{
				_from	=from;
				_to		=to;
			}
			int _from;
			int _to;
		};
		public:
			status_machine_imp();
			void add(int state);
			void add_son(int parent,int state );
			void bidirec_moveable(int first ,int second);
			void moveable(int frist,int second);
			void self_moveable(int status);
			void InitStatus(int status);
			bool can_move_to(int status);
			void move_to(int status);
    	    void undo();
			int current_status();
		private:
			typedef std::vector<int> state_vector;
			typedef std::vector<status_direct> direct_vector;
			typedef boost::shared_ptr<state_vector>   states_shared_ptr;
			typedef std::map<int,states_shared_ptr> state_relation_map;	
			
			bool in_here(int status);
			int parent_status(int subStatus);
			bool is_parent_son_relation(int parent, int son);		
		
			direct_vector 				_moveable_directs	;
			state_relation_map			 _parent_sons_map	;	
		
			int		  _root_status		;
			int		  _current_status	;
        	int       _origin_status      ;
	};
	
	/// 
	/// @brief  status machine
	/// 
	template<typename T>	
	class status_machine
	{
	public:	
		status_machine()
		{
		}

		/// 
		/// @brief  add status
		/// 
		/// @param status 
		/// 
		void add(T status)
		{
			_imp.add(status);	
		}
		void add(T first,T second )
		{
			add(first);
			add(second);
		}
		void add(T first,T second, T third)
		{
			add(first);	
			add(second,third);
		}
		void add(T first,T second, T third,T forth)
		{
			add(first,second);	
			add(third,forth);
		}
		/// 
		/// @brief  add son status
		/// 
		/// @param parent 
		/// @param son 
		/// 
		void add_son(T parent,T son )
		{
			_imp.add_son(parent,son);	
		}
		void add_son(T parent,T son1 ,T son2)
		{
			add_son(parent,son1);	
			add_son(parent,son2);	
		}
		void add_son(T parent,T son1 ,T son2,T son3)
		{
			add_son(parent,son1);	
			add_son(parent,son2,son3);	
		}
		/// 
		/// @brief  set tow status bidirectional moveable;
		/// 
		/// @param first 
		/// @param second 
		/// 
		void bidirec_moveable(T first ,T second)
		{
			_imp.bidirec_moveable(first,second);	
		}
		
		/// 
		/// @brief set moveable from status to status
		/// 
		/// @param from 
		/// @param to 
		/// 
		void moveable(T from , T to)
		{
			_imp.moveable(from,to);	
		}
		void moveable(T first, T second, T third)
		{
			moveable(first,second);
			moveable(second,third);
		}
		void moveable(T first , T second,T third,T forth)
		{
			movealbe(first,second);
			moveable(second,third,forth);
		}
		
		void self_moveable(T status)
		{
			_imp.self_moveable(status);	
		}
		void init_status(T status)
		{
			_imp.InitStatus(status);	
		}
		/// 
		/// @brief  can move to param status from current status?
		/// 
		/// @param status 
		/// 
		/// @return 
		/// 
		bool can_move_to(T status)
		{
			return _imp.can_move_to(status);	
		}
		/// 
		/// @brief  move current status to param status
		/// 
		/// @param status 
		/// 
		void move_to(T status)
		{
			_imp.move_to(status);	
		}

		/// 
		/// @brief unmove
		/// 
        void undo()
        {
			_imp.undo();	
        }
		T current_status()
		{
			return (T)_imp.current_status();	
		}
	private:
		status_machine_imp _imp;

	};
}
#endif
