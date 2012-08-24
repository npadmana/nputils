#include "gtest/gtest.h"
#include "npTextFile.h"

TEST(InputTextFileTest, NoCommentNumLines) {
	InputTextFile t1("inputtextfile_nocomment.txt");;
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
