
#include "Buffer.h"
#include "gtest\gtest.h"
#include "gmock\gmock.h"
#include <array>

using namespace testing;

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

TEST(Buffer, should_write_data_from_smaller_container) 
{
	Buffer buff(3);
	buff << std::array<uint8_t, 2>{1, 2};

	auto ptr = buff.getData().get();
	EXPECT_EQ(1, ptr[0]);
	EXPECT_EQ(2, ptr[1]);
}

TEST(Buffer, should_overwrite_last_element_with_continuous_obj)
{
	Buffer buff(2);
	buff << std::array<uint8_t, 2>{1, 2}
		<< std::array<uint8_t, 2>{3, 4};

	auto ptr = buff.getData().get();
	EXPECT_EQ(3, ptr[0]);
	EXPECT_EQ(4, ptr[1]);
}

TEST(Buffer, should_write_with_offset)
{
	Buffer buff(4);
	auto arr = std::array<uint8_t, 2>{1, 2};
	buff.write(arr.data(), arr.size(), 2);

	auto ptr = buff.getData().get();
	EXPECT_EQ(1, ptr[2]);
	EXPECT_EQ(2, ptr[3]);
}

TEST(Buffer_writter, should_write_consecutive_streams)
{
	Buffer_writter buff(4);
	buff << std::array<uint8_t, 2>{1, 2}
		<< std::array<uint8_t, 2>{3, 4};

	auto ptr = buff.getData().get();
	EXPECT_EQ(1, ptr[0]);
	EXPECT_EQ(2, ptr[1]);
	EXPECT_EQ(3, ptr[2]);
	EXPECT_EQ(4, ptr[3]);
}

TEST(Buffer_writter, should_throw_error_on_overflow)
{
	Buffer_writter buffw(2);
	const auto data = std::array<uint8_t, 4>{1, 2, 3, 4};

	EXPECT_THROW(
		buffw << data,
		std::overflow_error
	);
}