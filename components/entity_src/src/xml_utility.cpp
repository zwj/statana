#include <entity/common.h>
#include <entity/xml_utility.h>
#include <algorithm>
using namespace std; 
using namespace improve;
std::string 
xml_utility::xmlch2str(const XMLCh* xch_ptr)
{
	DOMString domstr(xch_ptr);
	ch_scptr ch_ptr(domstr.transcode());
	return string(ch_ptr.get());
}
DOMString 
xml_utility::str2domstr(const std::string& str)
{
		DOMString domstr(str.c_str());
		return domstr;
}
bool
xml_utility::is_element(IDOM_Node* node,const string&  elm_name)
{
	if(node==0) return false;	
	string 	lable(xml_utility::xmlch2str(node->getNodeName()));
	return 	lable == elm_name && 
			node->getNodeType() == IDOM_Node::ELEMENT_NODE;
}
std::string 
xml_utility::attr_of(IDOM_Element* elm, const string& attr_name)			
{
	REQUIRE(elm); 	
	DOMString  attr= xml_utility::str2domstr(attr_name);
	return xml_utility::xmlch2str(elm->getAttribute(attr.rawBuffer()));
}	
std::string
xml_utility::unify(std::string will)
{
	string trimed = str_utility::trim(will);	
	return str_utility::upcase(trimed); 	
}
width_finder::width_finder(IDOM_Node* root,
							IDOM_TreeWalker* walker)
:_root(root),_walker(walker)
{
	REQUIRE(root); 	
	REQUIRE(walker); 
	_walker->setCurrentNode(_root);
}
IDOM_Node*
width_finder::search(const string& tag)
{
	REQUIRE(_root); 	
	REQUIRE(_walker); 
	IDOM_Node* node =_walker->getCurrentNode();
    while(node)
	{
		if(xml_utility::is_element(node,tag))	
		{
			return node;
		}
		_queue.push(node);
	
		node = _walker->nextSibling();
	}
	while(!_queue.empty())
	{
	
		IDOM_Node* node = _queue.front();	
		_queue.pop();
		_walker->setCurrentNode(node);
		if(_walker->firstChild())
		{
			IDOM_Node* found = search(tag);	
			if(found)
				return found;
		}
	
	    return (IDOM_Node* )0; 
	}
	throw improve::no_realization(__FUNCTION__); 
}
