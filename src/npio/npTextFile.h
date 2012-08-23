/*
 * npTextFile.h
 *
 *  Created on: Aug 21, 2012
 *      Author: npadmana
 */

#ifndef NPTEXTFILE_H_
#define NPTEXTFILE_H_

#include <fstream>
#include <vector>
#include <string>

#include <boost/iostreams/filtering_stream.hpp>

/** Typedef for a line.
 *
 * A line is treated as a vector of strings
 */
typedef std::vector<std::string> OneLine;

/** Typedef for a vector of lines
 *
 */
typedef std::vector<OneLine> Lines;


/** A basic TextFile class for Input
 *
 * -- Handles character separated fields
 * -- Handles simple shell comments
 * -- Handles gzipped files
 *
 * Makes use of Boost.iostreams, Boost.string and Boost.Tokenizer
 */
class InputTextFile {

	/** Constructor
	 *
	 * @param fn (filename)
	 * @param commentchar -- character to be used for comments
	 * @param sepchar -- character to be used to separate fields
	 * @param quotechar -- character for quoted fields
	 * @param escapechar -- characted for escape characters
	 */
	InputTextFile(std::string fn, char commentchar='#', char sepchar=' ', char quotechar='\"', char escapechar='\\');

	/** Destructor
	 *
	 * Ensure that the file is closed
	 */
	~InputTextFile();

	/** Count the number of lines in the file
	 *
	 * Only non-empty files are returned.
	 *
	 * NOTE : This closes, opens, processes, closes and re-opens the file.
	 * Use only when the absolutely necessary (i.e. when you have no other
	 * processing necessary).
	 *
	 */
	int numLines();

	/** Read the file, and tokenize it into a vector of strings
	 *
	 * @param nlines [int] number of lines to read in (empty lines do not count). This is only attempted,
	 *   if EOF is reached, then fewer lines are actually read in.
	 *   if -1 [default], then read the entire file
	 *
	 * @returns Lines
	 *
	 * NOTE : The lines need not have the same length
	 *
	 */
	Lines read(int nlines=-1);


private :
	std::ifstream ff;
	boost::iostreams::filtering_istream ifs_;
	char commentchar_, quotechar_, escapechar_, sepchar_;
	std::string fn_;

	void open_(std::string fn);
	void close_();
};



#endif /* NPTEXTFILE_H_ */