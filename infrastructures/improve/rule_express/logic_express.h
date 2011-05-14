#ifndef __LOGIC_EXPRESS_H__
#define __LOGIC_EXPRESS_H__
#include "rule_express.h"
#include "../utility/utility.h"
#include "../dbc.h"
#include <boost/shared_ptr.hpp>
namespace improve
{
/// 
/// @brief  rule of unitary; 
/// 
template< typename T > 
    class unitary_rule : public rule<T>
    {
    protected:
		typedef boost::shared_ptr< rule<T> > shared_ptr;
        //typedef rule<T>::shared_ptr shared_ptr;
        shared_ptr  _one_ptr;
    protected:
        unitary_rule(shared_ptr one)
        :_one_ptr(one)		
        {}
    };
template<typename T> 
class binary_rule : public rule<T>
{
protected:	
	typedef boost::shared_ptr< rule<T> > shared_ptr;
    shared_ptr  _first_ptr;			
    shared_ptr  _second_ptr;			
protected:	
	binary_rule(shared_ptr first, shared_ptr second)	
	:_first_ptr(first),_second_ptr(second)		
	{	
	}		
};

/// 
/// @brief  it's or relation of tow rules; 
/// 
template<typename T> 
class or_rule : public binary_rule<T>
{
public:		
	typedef boost::shared_ptr< rule<T> > shared_ptr;
	or_rule(shared_ptr first, shared_ptr second)
	:binary_rule<T>(first,second)		
	{}
	bool is_true(const T& obj) const 
	{
		return 	this->_first_ptr->is_true(obj) || 
				this->_second_ptr->is_true(obj);
	}
	std::string description() const 
	{
		return "("+this->_first_ptr->description() + ") or (" +this-> _second_ptr->description()+")" ;	
	}
		
};
/// 
/// @brief  it's and relation of tow rules;
/// 
template<typename T> 
class and_rule : public binary_rule<T>
{
public:		
	typedef boost::shared_ptr< rule<T> > shared_ptr;
	and_rule(shared_ptr first, shared_ptr second)
	:binary_rule<T>(first,second)		
	{}
	bool is_true(const T& obj) const 
	{
		return 	this->_first_ptr->is_true(obj) && 
				this->_second_ptr->is_true(obj);
	}
	std::string description() const 
	{
		return "("+this->_first_ptr->description() + ") and (" + this->_second_ptr->description() +")";	
	}
};


/// 
/// @brief not relation rule 
/// 
template<typename T>
class not_rule : public unitary_rule<T>
{
	public:	
	typedef boost::shared_ptr< rule<T> > shared_ptr;
	not_rule(shared_ptr not_it )
	:unitary_rule<T>(not_it)		
	{}
	bool is_true(const T& obj) const 
	{
		return 	!this->_one_ptr->is_true(obj); 
	}
	std::string description() const 
	{
		return "not (" +this->_one_ptr->description()+")";
	}
};

}
#endif
