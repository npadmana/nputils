#include "npTextFile.h"
#include <boost/algorithm/string/predicate.hpp>

// The includes below are copied as part of the shell_comments_input_filter case
#include <cassert>
#include <iostream>
#include <cstdio>    // EOF.
#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/detail/ios.hpp>  // BOOST_IOS.
#include <boost/iostreams/filter/stdio.hpp>
#include <boost/iostreams/operations.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>

// gzip decompressor
#include <boost/iostreams/filter/gzip.hpp>

namespace io = boost::iostreams;


/** This is copied directly from the Boost iostreams examples
 *
 */
class shell_comments_input_filter : public io::input_filter {
public:
    explicit shell_comments_input_filter(char comment_char = '#')
        : comment_char_(comment_char), skip_(false)
        { }

    template<typename Source>
    int get(Source& src)
    {
        int c;
        while (true) {
            if ((c = boost::iostreams::get(src)) == EOF || c == io::WOULD_BLOCK)
                break;
            skip_ = c == comment_char_ ?
                true :
                c == '\n' ?
                    false :
                    skip_;
            if (!skip_)
                break;
        }
        return c;
    }

    template<typename Source>
    void close(Source&) { skip_ = false; }
private:
    char comment_char_;
    bool skip_;
};



InputTextFile::InputTextFile(const std::string& fn, char commentchar, char sepchar,
		char quotechar, char escapechar, bool dropempty) :
		commentchar_(commentchar), fn_(fn), dropempty_(dropempty),
		tokfunc_(escapechar, sepchar, quotechar),
		tok_(std::string(),tokfunc_)
{
	open_(fn_);
}

InputTextFile::~InputTextFile() {
	close_();
}

int InputTextFile::numLines() {
	int retval = 0;
	OneLine tmp;

	close_();
	open_(fn_);

	for (std::string str; std::getline(ifs_, str);) {
		tmp = parseline_(str);
		if (tmp.size() > 0) retval++;
	}

	close_();
	open_(fn_);

	return retval;
}

Lines InputTextFile::read(int nlines) {
	Lines out;
	OneLine tmp;

	for (std::string str; (std::getline(ifs_, str)) && (nlines!=0); --nlines) {
		tmp = parseline_(str);
		if (tmp.size() > 0) out.push_back(tmp);
	}

	return out;
}

void InputTextFile::open_(const std::string& fn) {

	// If a gzipped file, push a decompressor on the stack
	// and open file in binary mode
	ifs_.reset();
	// Push the shell comment removal
	ifs_.push(shell_comments_input_filter(commentchar_));

	// Push decompressor if necessary
	if (boost::iends_with(fn, ".gz")) {
		ff.open(fn, std::ios_base::in | std::ios_base::binary);
		ifs_.push(io::gzip_decompressor());
	} else {
		ff.open(fn, std::ios_base::in);
	}
	// Check to see that the file is open
	if (!ff.is_open()) {
		throw "Unable to open file\n";
	}
	// Push the file handle
	ifs_.push(ff);


}

void InputTextFile::close_() {
	io::close(ifs_);
	ff.close();
}

OneLine InputTextFile::parseline_(const std::string& str) {
	OneLine ret;

	tok_.assign(str);
	for (auto ii=tok_.begin(); ii != tok_.end(); ++ii) {
		if ((ii->size() > 0) && dropempty_) {
			ret.push_back(*ii);
		}
	}

	return ret;
}
