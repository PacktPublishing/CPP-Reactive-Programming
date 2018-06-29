//
// reference.h
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

#include <type_traits> // std::reference_wrapper 

namespace streamulus
{    
  
    // streamulus::reference
    // if you attempt to Streamify(FunctionObject) something that is noncopyable, 
    // then compilation will fail since the copy constructor is used to move the function
    // object during expression transformation.
    // 
    // To circumvent this, one may wrap the object to be streamified using this reference 
    // streamulus::Streamify( streamulus::reference<Type>(instance) )(operand)
    //
    // This allows objects which are noncopyable, particularly those with 
    // some internal state which might be used in other places to be streamified directly.
    // Note that this works only with the streamify variant which takes an instance of an object
    // and not the templated type version, for obvious reasons.

    template <typename F>
    struct reference {
      // output type is same as wrapped object output type
      template <class Sig> struct result { using type = typename F::template result<F>::type; };

      // construct using a supplied object, initialize our reference wrapper
      inline reference<F>(F &internal) : mInternal(internal) {      
//        std::cout << "streamulus_wrapper(F)" << std::endl;
      }

      // copy constructor passes the reference to the new copy
      inline reference<F>(const reference<F> &other) : mInternal(other.mInternal) {
//        std::cout << "streamulus_wrapper(streamulus_wrapper(F))" << std::endl;
      }; 

      // we must overload the () operator and call the wrapped object () operator
      // we'll use a templated parameter pack and let the compiler decide if the user
      // is doing it right, since we don't know what arguments the wrapped () operater takes
      template <typename... Args>
      inline typename F::template result<F>::type operator()(Args... args) { 
        return mInternal.get().operator()(args...); 
      }

    private:
      const std::reference_wrapper<F> mInternal;
    };
    
} // ns streamulus
