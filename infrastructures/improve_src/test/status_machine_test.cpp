#include "status_machine_test.h"	
CPPUNIT_TEST_SUITE_REGISTRATION( status_machine_testcase );

		void status_machine_testcase::setUp()
		{
			_stats_ptr = new live_stats();	
			_stats_ptr->add(LIVED,DEATH);
			_stats_ptr->add_son(LIVED, SON,
								FATHER,GRANDFATHER);
			_stats_ptr->moveable(LIVED,DEATH);
			_stats_ptr->moveable(SON,FATHER,GRANDFATHER);
			_stats_ptr->init_status(SON);
		}
		void status_machine_testcase::tearDown()
		{
			delete _stats_ptr;
		}
		void 
		status_machine_testcase::SuccessToNextState()
		{
			_stats_ptr->move_to(FATHER);
			_stats_ptr->undo();
			_stats_ptr->move_to(FATHER);
			_stats_ptr->move_to(GRANDFATHER);
			_stats_ptr->move_to(DEATH);
		}
		void 
		status_machine_testcase::FailToNextState()
		{
			_stats_ptr->can_move_to(SON);	
			CPPUNIT_ASSERT(!_stats_ptr->can_move_to(GRANDFATHER));
			_stats_ptr->move_to(FATHER);
			CPPUNIT_ASSERT(!_stats_ptr->can_move_to(SON));
			_stats_ptr->move_to(GRANDFATHER);
			CPPUNIT_ASSERT(!_stats_ptr->can_move_to(SON));
			CPPUNIT_ASSERT(!_stats_ptr->can_move_to(FATHER));
			_stats_ptr->move_to(DEATH);
			CPPUNIT_ASSERT(!_stats_ptr->can_move_to(SON));
		}
		void 
		status_machine_testcase::SonToDeath()
		{
			_stats_ptr->move_to(DEATH);
		}
		void
		status_machine_testcase::FatherToDeath()
		{
			_stats_ptr->move_to(FATHER);
			_stats_ptr->move_to(DEATH);
		}


