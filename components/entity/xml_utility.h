#ifndef __XML_UTILITY_H__
#define __XML_UTILITY_H__

#include <string>
#include <boost/scoped_ptr.hpp>
#include <queue>

/*
typedef boost::scoped_ptr<char> ch_scptr;
class xml_utility
{
public:		
	static std::string xmlch2str(const XMLCh* xch_ptr);
	static xercesc::DOMString str2domstr(const std::string& str);
	static std::string domstr2str(DOMString domstr);
	static bool is_element(IDOM_Node* node ,const std::string& ele_name);
	static std::string attr_of(IDOM_Element* elm, const std::string& attr_name);			
	static std::string unify(std::string will);
};

class width_finder
{
public:		
	width_finder(IDOM_Node* root , IDOM_TreeWalker* walker)	;
	IDOM_Node* search(const std::string& tag);
private:	
	std::queue<IDOM_Node* > _queue;
	IDOM_Node* _root;
	IDOM_TreeWalker* _walker;
};
*/
#endif
