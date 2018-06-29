//
// op_subscribe_values_fixture.h
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

#pragma once

#include "streamulus.h"

using namespace streamulus;

struct assert_eq {
    template<typename T1, typename T2>
    bool operator()(const T1 &v1, const T2 &v2) const {
        BOOST_CHECK_EQUAL(v1, v2);
        return (v1 == v2);
    }
};

struct ValuesFixture {
protected:
    int v1_ = 7;
    int v2_ = 2;
    int v1 = v1_;
    int v2 = v2_;
    bp::terminal<int>::type t1 = {v1};
    bp::terminal<int>::type t2 = {v2};
    bp::terminal<int *>::type p1 = {&v1};
    bp::terminal<int *>::type p2 = {&v2};

    template<typename Expr, typename ExpectedValueType>
    void RunTest(Expr &expr, ExpectedValueType expected) {
        Streamulus engine;
        engine.Subscribe(Streamify<assert_eq>(expr, expected));
        Reset();
    }

private:
    void Reset() {
        v1 = v1_;
        v2 = v2_;
    }
};
