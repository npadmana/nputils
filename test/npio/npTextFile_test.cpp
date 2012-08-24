#include "gtest/gtest.h"
#include "npTextFile.h"

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
