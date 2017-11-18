
#include "Buffer.h"
#include "gtest\gtest.h"

namespace {
	const size_t enormous_size = SIZE_MAX;
	const size_t reasonable_size = 8;
}

TEST(Buffer, default_buffer_should_be_empty) 
{
	Buffer buffer;
	EXPECT_EQ(0, buffer.getSize());
}

TEST(Buffer, buffer_initialised_with_size_should_keep_it)
{
	Buffer buff(reasonable_size);
	EXPECT_EQ(reasonable_size, buff.getSize());
}

TEST(Buffer, should_throw_when_cannot_allocate)
{
	EXPECT_THROW(
		Buffer buff(enormous_size), 
		std::bad_alloc);
}

TEST(Buffer, should_not_throw_when_allocate)
{
	EXPECT_NO_THROW(Buffer buff(reasonable_size));
}
