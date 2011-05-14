#ifndef __VCT_DIMENSION_H__
#define __VCT_DIMENSION_H__
#include "common.h"
#include "dimension.h" 
#include "dimension_def.h" 
#include "group_dim.h" 
namespace logs
{
namespace entity
{
    class vector_dimension : public group_dimension
    {/*{{{*/
    public:

        vector_dimension();
        vector_dimension(const std::string& basename);
        void set_basename(const std::string& basename);
        vector_dimension(const vector_dimension& other);
        bool have_son(const std::string& name) const ;
        const dimension* son_dim(const std::string& name) ;

        std::string name() const;
        std::string value() const;
        std::string create_son_name(const std::string& name);
        const vector_dimension& operator=(const vector_dimension& other);
        dimension* clone() const ;
        void add_son(dimension::shared_ptr dim);
        int  count();
    protected:	
        bool equal(const dimension& other)const;
        bool less(const dimension& other)const ;
        bool true_equal(const dimension& other) const;
        bool in(const dimension& other) const ;
    private:
        dim_vector  _dim_array;
        std::string _basename;
        
    };/*}}}*/

}
}
#endif
