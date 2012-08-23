#include "npTextFile.h"


InputTextFile::InputTextFile(std::string fn, char commentchar, char sepchar,
		char quotechar, char escapechar) :
		commentchar_(commentchar), sepchar_(sepchar), quotechar_(quotechar),
		escapechar_(escapechar)
{
	open_(fn);
}

InputTextFile::~InputTextFile() {
	ff.close();
}

int InputTextFile::numLines() {
	int retval = 0;

	return retval;
}

Lines InputTextFile::read(int nlines) {
	Lines out;

	return out;
}

void InputTextFile::open_(std::string fn) {
}

void InputTextFile::close_() {
	ff.close();
}
