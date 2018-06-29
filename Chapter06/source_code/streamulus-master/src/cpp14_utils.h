//
//  engine.h
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

#include <boost/optional.hpp>
#include <type_traits>

namespace streamulus {

    /**
     * emulate missing c++14 shortcuts
     */
    template<class T>
    using remove_reference_t = typename std::remove_reference<T>::type;

    template<class T>
    using remove_pointer_t = typename std::remove_pointer<T>::type;

    template<class T>
    using remove_const_t = typename std::remove_const<T>::type;



    /**
     * remove-optional (where should this live?)
     */

    namespace detail {
            template<typename T>
            struct remove_optional_impl {
                using type = T;
            };

            template<typename T>
            struct remove_optional_impl<boost::optional<T>> {
                using type = T;
            };
    }

    template<typename T>
    using remove_optional = typename detail::remove_optional_impl<remove_reference_t<remove_const_t<T>>>::type;

}
