
#include "Buffer.h"
#include "gtest\gtest.h"

TEST(Buffer, DefaultBufferShouldBeEmpty ) {
	Buffer buffer;

	EXPECT_EQ(0, buffer.getSize());
}
