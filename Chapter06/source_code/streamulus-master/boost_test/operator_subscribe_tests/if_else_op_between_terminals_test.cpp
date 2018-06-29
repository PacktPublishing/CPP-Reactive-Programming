//
// if_else_ops_between_terminals_test.cpp
//
// Streamulus Copyright (c) 2015 Irit Katriel. All rights reserved.
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

#include "streamulus.h"

#define BOOST_TEST_MODULE operator_subscribe
#include <boost/test/included/unit_test.hpp>

#include "op_subscribe_values_fixture.h"

BOOST_FIXTURE_TEST_SUITE(operator_subscribe_test, ValuesFixture)

    BOOST_AUTO_TEST_CASE(if_else_operator_between_terminals) {
        // TODO: make this work
        /*
        RunTest(true ? t1 : t2, true ? v1 : v2);
        RunTest(true ? t2 : t1, true ? v2 : v1);

        RunTest(false ? t1 : t2, false ? v1 : v2);
        RunTest(false ? t2 : t1, false ? v2 : v1);
         */
    }

BOOST_AUTO_TEST_SUITE_END()
