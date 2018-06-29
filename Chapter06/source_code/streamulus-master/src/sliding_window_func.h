//
//  sliding_window_func.h
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

#include <boost/optional.hpp>

#include "strop_sliding_window.h"

namespace streamulus
{
    template<typename F>
    class WindowFunc 
    {
    public:

        WindowFunc()
        : mFunction(F())
        {
        }
        
        WindowFunc(const F& f)
            : mFunction(f)
        {
        }

        template<typename WA>
        boost::optional<typename F::template value_type<F(typename WindowBaseType<WA>::type)>::type>
        operator()(const WA& window_update) 
        { 
            if (window_update.first == DATA_OUT)
            {
                mFunction.Remove(window_update.second);
                return boost::none;
            }
            mFunction.Insert(window_update.second);
            return mFunction.Value();
        }
        
    private:
        F mFunction;
    };
    
} // ns streamulus
