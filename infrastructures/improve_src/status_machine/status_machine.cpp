#include <improve/status.h>
#include <algorithm>
#include <improve/dbc.h>
namespace improve
{
		status_machine_imp::status_machine_imp()
		:_root_status(-1),_current_status(-1),_origin_status(-1)		
		{
		}
		void 
		status_machine_imp::add(int state)
		{
			add_son(_root_status,state);	
		}
		void 
		status_machine_imp::add_son(int parent,int son )
		{
			state_relation_map::iterator pos =_parent_sons_map.find(parent);
			if(pos==_parent_sons_map.end())
			{
				states_shared_ptr one(new state_vector());
				one->push_back(son);
				_parent_sons_map.insert(make_pair(parent,one));
			}
			else
			{
				states_shared_ptr one = (*pos).second;
				one->push_back(son);
			}
		}
		void 
		status_machine_imp::bidirec_moveable(int first ,int second)
		{
			moveable(first,second);
			moveable(second,first);
		}
		void 
		status_machine_imp::moveable(int from,int to)
		{
			REQUIRE(in_here(from));
			REQUIRE(in_here(to));
			status_direct left2right(from,to);
			_moveable_directs.push_back(left2right);	
		}
		void
		status_machine_imp::self_moveable(int status)
		{
			moveable(status,status);
		}
		void
		status_machine_imp::InitStatus(int status)
		{
			REQUIRE(in_here(status));	
			_current_status= status;
		}
		bool 
		status_machine_imp::is_parent_son_relation(int parent, int son)		
		{
				
			REQUIRE(in_here(parent));	
			REQUIRE(in_here(son));
			int parentid = son;
			do
			{
				parentid=parent_status(son);
				if(parent==parentid) return true;
				son = parentid;
			}while(parentid!=_root_status);
			return false;
		}
		bool
		status_machine_imp::can_move_to(int status)
		{
			REQUIRE(in_here(status));	
			direct_vector::iterator pos = _moveable_directs.begin();
			for(;pos!=_moveable_directs.end();++pos)
			{
				status_direct now = *pos;	
				if((now._from	== _current_status || 
					is_parent_son_relation(now._from ,_current_status) ) 
					&& now._to == status) return true;
			}
			return false;
		}
		void
		status_machine_imp::move_to(int status)
		{
			REQUIRE(can_move_to(status));	
            _origin_status = _current_status;
			_current_status = status;
		}
        void 
		status_machine_imp::undo()
        {
            _current_status = _origin_status;
        }
		bool
    	status_machine_imp::in_here(int status)
		{
			state_relation_map::iterator pos = _parent_sons_map.begin();
			for(;pos!=_parent_sons_map.end();++pos)
			{
				states_shared_ptr now = (*pos).second;
				if(std::find(now->begin(),now->end(),status) != now->end()) return true;
			}
			return false;
		}
		int 
		status_machine_imp::parent_status(int son)
		{
				
			REQUIRE(in_here(son));	
			state_relation_map::iterator pos = _parent_sons_map.begin();
			for(;pos!=_parent_sons_map.end();++pos)
			{
				states_shared_ptr now = (*pos).second;
				if(std::find(now->begin(),now->end(),son) != now->end()) return (*pos).first;
			}
			return _root_status;
		}
		int 
		status_machine_imp::current_status()
		{
			return _current_status;
		}
	
}
