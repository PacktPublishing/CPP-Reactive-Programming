//
// stream_test.cpp
//
// Streamulus Copyright (c) 2012 Irit Katriel. All rights reserved.
//
// This file is part of Streamulus.
//
// Streamulus is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Streamulus is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Streamulus.  If not, see <http://www.gnu.org/licenses/>.
//

#include "stream.h"

#define BOOST_TEST_MODULE stream
#include <boost/test/included/unit_test.hpp>

using namespace streamulus;

BOOST_AUTO_TEST_CASE(int_stream_append_one_value) {
    Stream<int> stream;

    // stream has no data before Append
    BOOST_CHECK(!stream.HasMore());
    BOOST_CHECK(!stream.IsValid());
    BOOST_CHECK_THROW(stream.Current(), std::invalid_argument);

    // stream has data and is valid after Append. Current sees the new data.
    stream.Append(1);
    BOOST_CHECK(stream.HasMore());
    BOOST_CHECK(stream.IsValid());
    BOOST_CHECK_EQUAL(1, stream.Current());

    // Current() consumed the 1, so no more data. But stream is valid and Current() still returns 1.
    BOOST_CHECK(!stream.HasMore());
    BOOST_CHECK(stream.IsValid());
    BOOST_CHECK_EQUAL(1, stream.Current());
}

BOOST_AUTO_TEST_CASE(int_stream_append_multiple_values) {
    Stream<int> stream;

    // Append multiple inputs
    stream.Append(2);
    stream.Append(3);
    BOOST_CHECK(stream.HasMore());
    BOOST_CHECK(stream.IsValid());
    BOOST_CHECK_EQUAL(2, stream.Current());

    // Current() consumed the 2, but we still have the 3 in the future.
    BOOST_CHECK(stream.HasMore());
    BOOST_CHECK(stream.IsValid());
    BOOST_CHECK_EQUAL(3, stream.Current());

    // Current() consumed the 3, so no more data.
    BOOST_CHECK(!stream.HasMore());
    BOOST_CHECK(stream.IsValid());
    BOOST_CHECK_EQUAL(3, stream.Current());
}


// repeats the tests above for integer, but on strings.
BOOST_AUTO_TEST_CASE(string_stream_test) {
    Stream<std::string> stream;
    // stream has no data before Append
    BOOST_CHECK(!stream.HasMore());
    BOOST_CHECK(!stream.IsValid());
    BOOST_CHECK_THROW(stream.Current(), std::invalid_argument);

    // stream has data and is valid after Append. Current sees the new data.
    stream.Append("1");
    BOOST_CHECK(stream.HasMore());
    BOOST_CHECK(stream.IsValid());
    BOOST_CHECK_EQUAL("1", stream.Current());

    // Current() consumed the 1, so no more data. But stream is valid and Current() still returns 1.
    BOOST_CHECK(!stream.HasMore());
    BOOST_CHECK(stream.IsValid());
    BOOST_CHECK_EQUAL("1", stream.Current());

    // Append multiple inputs
    stream.Append("2");
    stream.Append("3");
    BOOST_CHECK(stream.HasMore());
    BOOST_CHECK(stream.IsValid());
    BOOST_CHECK_EQUAL("2", stream.Current());

    // Current() consumed the 2, but we still have the 3 in the future.
    BOOST_CHECK(stream.HasMore());
    BOOST_CHECK(stream.IsValid());
    BOOST_CHECK_EQUAL("3", stream.Current());

    // Current() consumed the 3, so no more data.
    BOOST_CHECK(!stream.HasMore());
    BOOST_CHECK(stream.IsValid());
    BOOST_CHECK_EQUAL("3", stream.Current());
}