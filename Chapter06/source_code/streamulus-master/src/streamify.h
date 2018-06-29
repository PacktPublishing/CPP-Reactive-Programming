//
//  streamify.h
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

#include <boost/proto/proto.hpp>

namespace streamulus
{    
    
    namespace bp = boost::proto;
    
    // Streamify template - turns a function object into a streamulus function.
    //
    // There are two variants for Streamify - one that gets a function object
    // and copies it, and the other which gets the type of the function as a 
    // template parameter and constructs an object of this type. 
    //
    // Both variants are useful - the first is more general because the function
    // object does not need to be constructible without parameters. The second
    // can be invoked more concisely when the function is constructible in this way.
    //
    
    
    // From object: Copies the function object into a terminal.
    template <typename F>
    typename bp::terminal<F>::type  const
    Streamify(F const& f)
    {   
        typename bp::terminal<F>::type that = {f};
        return that;           
    }

    // From type: Constructs the function object    
    // We have one Streamify function for each arity of F. 
    template <typename F, typename A1>
    typename bp::result_of::make_expr<
    bp::tag::function  // Tag type
    , F                          // First child (by value)
    , A1 const &                 // Second child (by reference)
    >::type const
    Streamify(const A1& a1)
    {    
        return bp::make_expr<bp::tag::function>(
                                                                   F()                // First child (by value)
                                                                  , boost::ref(a1)   // Second child (by reference)
                                                                  );    
    } 
    
    template <typename F, typename A1, typename A2>
    typename bp::result_of::make_expr<
    bp::tag::function  // Tag type
    , F                          // First child (by value)
    , A1 const &                 // Second child (by reference)
    , A2 const &                 // Third child (by reference)
    >::type const
    Streamify(const A1& a1, const A2& a2)
    {    
        return bp::make_expr<bp::tag::function>(
                                                             F()                // First child (by value)
                                                            , boost::ref(a1)   // Second child (by reference)
                                                            , boost::ref(a2)   // Third child (by reference)
                                                            );    
    }
    
    template <typename F, typename A1, typename A2, typename A3>
    typename bp::result_of::make_expr<
    bp::tag::function  // Tag type
    , F                          // First child (by value)
    , A1 const &                 // Second child (by reference)
    , A2 const &                 // Third child (by reference)
    , A3 const &                 // Fourth child (by reference)
    >::type const
    Streamify(const A1& a1, const A2& a2, const A3& a3)
    {    
        return bp::make_expr<bp::tag::function>(
                                                            F()                // First child (by value)
                                                            , boost::ref(a1)   // Second child (by reference)
                                                            , boost::ref(a2)   // Third child (by reference)
                                                            , boost::ref(a3)   // Fourth child (by reference)
                                                            );    
    }
    
    template <typename F, typename A1, typename A2, typename A3, typename A4>
    typename bp::result_of::make_expr<
    bp::tag::function  // Tag type
    , F                          // First child (by value)
    , A1 const &                 // Second child (by reference)
    , A2 const &                 // Third child (by reference)
    , A3 const &                 // Fourth child (by reference)
    , A4 const &                 // Fifth child (by reference)
    >::type const
    Streamify(const A1& a1, const A2& a2, const A3& a3, const A4& a4)
    {    
        return bp::make_expr<bp::tag::function>(
                                                            F()                // First child (by value)
                                                            , boost::ref(a1)   // Second child (by reference)
                                                            , boost::ref(a2)   // Third child (by reference)
                                                            , boost::ref(a3)   // Fourth child (by reference)
                                                            , boost::ref(a4)   // Fifth child (by reference)
                                                            );    
    }
    
    template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5>
    typename bp::result_of::make_expr<
    bp::tag::function  // Tag type
    , F                          // First child (by value)
    , A1 const &                 // Second child (by reference)
    , A2 const &                 // Third child (by reference)
    , A3 const &                 // Fourth child (by reference)
    , A4 const &                 // Fifth child (by reference)
    , A5 const &                 // Sixth child (by reference)
    >::type const
    Streamify(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5)
    {    
        return bp::make_expr<bp::tag::function>(
                                                            F()                // First child (by value)
                                                            , boost::ref(a1)   // Second child (by reference)
                                                            , boost::ref(a2)   // Third child (by reference)
                                                            , boost::ref(a3)   // Fourth child (by reference)
                                                            , boost::ref(a4)   // Fifth child (by reference)
                                                            , boost::ref(a5)   // Sixth child (by reference)
                                                            );    
    }
    
} // ns streamulus
