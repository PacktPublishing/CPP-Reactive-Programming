//
//  strop_func.h
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

#pragma once


#include <sstream>

#include <boost/optional.hpp>

#include "strop.h"

namespace streamulus {
    template<class F, typename... Args>
    class Func : public Strop<typename std::result_of<F(Args...)>::type(Args...)> {
    public:

        using R = typename std::result_of<F(Args...)>::type;
        using StropType = Strop<R(Args...)>;

        Func(const std::function<R(Args...)> &f)
                : mFunction(f), mInputExists(false) {
            std::stringstream ss;
            ss << "Func_" << "F";
            Strop<R(Args...)>::SetDisplayName(ss.str());
        }

        R ApplyFunction() {
            return Strop<R(Args...)>::Invoke(mFunction);
        }


        virtual void Work() {
            mInputExists |= StropType::IsValid();
            if (mInputExists) {
                if (StropType::NoInputs()) {
                    // apply function once for strop without inputs
                    StropStreamProducer<R>::Output(ApplyFunction());
                } else {
                    while (StropType::HasMore()) {
                        // apply function as long as there are more inputsfor strop without inputs
                        StropStreamProducer<R>::Output(ApplyFunction());
                    }
                }
            }
        }

    private:
        bool mInputExists;
        std::function<R(Args...)> mFunction;
    };


} // ns streamulus
