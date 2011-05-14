#ifndef __SPLITTER_H__
#define __SPLITTER_H__
#include <fstream>
#include <boost/scoped_ptr.hpp>
#include "shared.h"
#include "file_set.h"
#include "line_assembler.h"


enum KeyWipeMode
{
    NoWipe=0,
    WipeKey=1,
    KeyOnly=2
};


class splitter
{
public:
    splitter(const int_vector& col_list, char delimiter, KeyWipeMode wipemode);
    void run(src_fileset& src_set, boost::scoped_ptr<fileset_writer>& dest_writer);

private:
    void proc_file(const std::string& filename, insbuf_iter& ins_iter, std::ifstream& ins, boost::scoped_ptr<fileset_writer>& dest_writer);
    bool proc_line(insbuf_iter& ins_iter, boost::scoped_ptr<fileset_writer>& dest_writer);

private:
    line_assembler _line_assemb;
    KeyWipeMode _wipemode;
};



#endif
