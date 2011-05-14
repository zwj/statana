#include "splitter.h"
#include <boost/filesystem/exception.hpp>
//#include <stdexcept>
using namespace std;


splitter::splitter(const int_vector& col_list, char delimiter, KeyWipeMode wipemode)
    : _line_assemb(col_list, delimiter, false)
{
    _wipemode = wipemode;
}

void
splitter::run(src_fileset& src_set, boost::scoped_ptr<fileset_writer>& dest_writer)
{/*{{{*/
    src_fileset::stream_pair_t* ptr = src_set.next_stream();
    while (ptr != NULL)
    {
        const std::string& filename = ptr->first;
        ifstream* ins = ptr->second;

        cout<<line_begin<<"Processing file: "<<filename;
        cout.flush();

        insbuf_iter ins_iter(*ins);
        this->proc_file(filename, ins_iter, (*ins), dest_writer);
        ptr = src_set.next_stream();
    }
}/*}}}*/

void
splitter::proc_file(const std::string& filename, insbuf_iter& ins_iter, ifstream& ins, boost::scoped_ptr<fileset_writer>& dest_writer)
{/*{{{*/
    int cur_line_no = 0;
    dest_writer->set_current_filename(filename);

    int err_count = 0;
    while (true)
    {
        try
        {
            ++cur_line_no;
            if (cur_line_no % PROGRESS_REPR_COUNT == 0)
            {
                cout<<PROGRESS_CHAR;
                cout.flush();
            }

            if (!this->proc_line(ins_iter, dest_writer))
                break;
        }
        catch (input_error e)
        {
            ++err_count;
            while (*ins_iter++ != '\n') // skip to next line
                ;
            ++ins_iter;
            cout<<endl<<e.what()<<" - line no: "<<cur_line_no<<endl;
            cout.flush();
        }
        catch (boost::filesystem::filesystem_error e)
        {
            ++err_count;
            cout<<endl<<"Warning: "<<e.what()<<" - line no: "<<cur_line_no<<endl;
            cout.flush();
        }
        catch (exception e)
        {
            ++err_count;
            cout<<endl<<e.what()<<" - line no: "<<cur_line_no<<endl;
            cout.flush();
        }
        catch (...)
        {
            ++err_count;
            cout<<endl<<"Error: Unknown error occurred!"<<" - line no: "<<cur_line_no<<endl;
            cout.flush();
        }

        if (err_count > MAX_ERR_COUNT)
        {
            cout<<endl<<"Maximum error count exceeded, skip processing for current file"<<endl;
            cout.flush();
            break;
        }
    }

    if (err_count == 0)
        cout<<"[OK]"<<endl;
    else if (err_count <= MAX_ERR_COUNT)
        cout<<"[WARNING]"<<endl;
    else
        cout<<"[FAILED]"<<endl;
}/*}}}*/

bool
splitter::proc_line(insbuf_iter& ins_iter, boost::scoped_ptr<fileset_writer>& dest_writer)
{/*{{{*/
    if (!_line_assemb.proc_line(ins_iter))
        return false;

    const char* output_line;
    if (_wipemode == KeyOnly)
        output_line = _line_assemb.linebuf_key();
    else if (_wipemode == WipeKey)
        output_line = _line_assemb.linebuf_exclude_key();
    else
        output_line = _line_assemb.linebuf_org();

    dest_writer->write(_line_assemb.keybuf(), output_line);

    return true;
}/*}}}*/


