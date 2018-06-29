//
//  compose_fn.h
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


// Take two functions F(x) and G(x), and create the function H(x)=F(G(x))
// Based on  
// (http://cpp-next.com/archive/2010/11/expressive-c-fun-with-function-composition/).


#pragma once

#include <boost/optional.hpp>

namespace streamulus 
{
    
    struct ArgType;
    
    template<typename Signature>
    class ComposedFunc;

    template<typename F, typename G>
    class ComposedFunc<F(G(ArgType))>
    {
    public:
        
        
        explicit ComposedFunc(const F& f_ = F(), const G& g_ = G())
            : f(f_)
            , g(g_)
        {
            std::cout << "Composed two functions!" << std::endl;
        }
    
        template<typename Signature>
        struct result;
    
        template<typename This, typename Arg>
        struct result<This(Arg)>
        {
            using G_result_type = typename std::result_of<G(Arg)>::type;
            using F_result_type = typename std::result_of<F(G_result_type)>::type;
            using type = F_result_type;
        };
            
        template<typename Arg>
        typename boost::optional<typename result<ComposedFunc<F(G(ArgType))>(Arg)>::type>
        operator()(const Arg& arg)
        {
            boost::optional<typename result<ComposedFunc<F(G(ArgType))>(Arg)>::G_result_type> g_res = g(arg);
            if (!g_res)
                return boost::none;
            boost::optional<typename result<ComposedFunc<F(G(ArgType))>(Arg)>::F_result_type> f_res = f(*g_res);
            if (!f_res)
                return boost::none;
            return *f_res;
        }
        
    private:
        F f;
        G g;
    };


} // ns streamulus

