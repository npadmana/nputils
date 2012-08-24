#include "gtest/gtest.h"
#include "npTextFile.h"

bool CompareLines(const Lines& l1, const Lines& l2) {

	// Check to see that the two have the same length
	if (l1.size() != l2.size()) return false;

	for (int ii=0; ii < l1.size(); ++ii) {
		// Check to see that the i'th line element has the same number of elts
		if (l1[ii].size() != l2[ii].size()) return false;

		// Now do a detailed comparison
		for (int jj=0; jj < l1[ii].size(); ++jj)
			if (l1[ii][jj].compare(l2[ii][jj]) != 0) return false;
	}


	return true;
}



TEST(InputTextFileTest, NoCommentNumLines) {
	InputTextFile t1("inputtextfile_nocomment.txt");
	EXPECT_EQ(4, t1.numLines());
}

TEST(InputTextFileTest, CommentNumLines) {
	InputTextFile t1("inputtextfile_comment.txt");
	EXPECT_EQ(4, t1.numLines());
}

TEST(InputTextFileTest, GzipNumLines) {
	InputTextFile t1("inputtextfile_commentgzip.txt.gz");
	EXPECT_EQ(4, t1.numLines());
}

// Now make sure we believe that the no comment case is read in correctly
// since we will use this to compare against all others
TEST(InputTextFileTest, NoCommentRead) {
	InputTextFile t1("inputtextfile_nocomment.txt");
	Lines val = t1.read();
	EXPECT_EQ(4, val.size());
	for (int ii=0; ii<3; ++ii) EXPECT_EQ(3, val[ii].size());
	EXPECT_EQ(4, val[3].size());

	EXPECT_EQ(0, std::string("This is a test").compare(val[1][0]));
	EXPECT_EQ(0, std::string("3.218181").compare(val[1][1]));
	EXPECT_EQ(0, std::string("Hello").compare(val[2][1]));
}


TEST(InputTextFileTest, CommentRead) {
	InputTextFile t1("inputtextfile_nocomment.txt");
	InputTextFile t2("inputtextfile_comment.txt");

	Lines l1 = t1.read();
	Lines l2 = t2.read();

	EXPECT_TRUE(CompareLines(l1, l2));
}

TEST(InputTextFileTest, GzipRead) {
	InputTextFile t1("inputtextfile_nocomment.txt");
	InputTextFile t2("inputtextfile_commentgzip.txt.gz");

	Lines l1 = t1.read();
	Lines l2 = t2.read();

	EXPECT_TRUE(CompareLines(l1, l2));
}







