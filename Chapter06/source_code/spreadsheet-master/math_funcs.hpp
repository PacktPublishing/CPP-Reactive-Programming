//
//  functions.hpp
//  spreadsheet
//
//  Created by Irit Katriel on 05/08/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "streamulus.h"

#include <boost/type_traits.hpp>
#include <boost/utility/enable_if.hpp>

namespace spreadsheet {

namespace bm = boost::mpl;
    
    
template<typename F, typename ARG>
struct StreamifyType
{
    typedef typename bp::result_of::make_expr<bp::tag::function, F, ARG const &>::type type;
};
    
struct ABS_FUNC
{
    template<class Sig> struct result;

    template<class This,typename A> 
    struct result<This(A)>
    {
        typedef A type; 
    };
        
    template<typename A>
    typename boost::enable_if<boost::is_integral<A>, typename result<ABS_FUNC(A)>::type>::type
    operator()(const A& value) const
    { 
        return std::abs(value); 
    }

    template<typename A>
    typename boost::enable_if<boost::is_floating_point<A>, typename result<ABS_FUNC(A)>::type>::type
    operator()(const A& value) const
    { 
        return std::fabs(value); 
    }
};

template<double FUNC(double)>
struct DOUBLE_TO_DOUBLE_FUNC
{
    template<class Sig> struct result;
    
    template<class This,typename A> 
    struct result<This(A)>
    {
        typedef A type; 
    };
  
    template<typename A>
    typename result<DOUBLE_TO_DOUBLE_FUNC<FUNC>(A)>::type
    operator()(const A& value) const
    { 
        return FUNC(value); 
    }
};
    
#define LAZY_FUNC1(NAME, FUNC) \
template<typename A> \
typename StreamifyType<FUNC,A>::type \
NAME(const A& arg) \
{ \
    return streamulus::Streamify<FUNC>(arg); \
}
    
LAZY_FUNC1(ABS,   ABS_FUNC);
LAZY_FUNC1(ACOS,  DOUBLE_TO_DOUBLE_FUNC<std::acos >);
LAZY_FUNC1(ASIN,  DOUBLE_TO_DOUBLE_FUNC<std::asin >);
LAZY_FUNC1(ATAN,  DOUBLE_TO_DOUBLE_FUNC<std::atan >);
LAZY_FUNC1(CEIL,  DOUBLE_TO_DOUBLE_FUNC<std::ceil >);
LAZY_FUNC1(COS,   DOUBLE_TO_DOUBLE_FUNC<std::cos  >);
LAZY_FUNC1(COSH,  DOUBLE_TO_DOUBLE_FUNC<std::cosh >);
LAZY_FUNC1(EXP,   DOUBLE_TO_DOUBLE_FUNC<std::exp  >);
LAZY_FUNC1(FLOOR, DOUBLE_TO_DOUBLE_FUNC<std::floor>);
LAZY_FUNC1(LOG,   DOUBLE_TO_DOUBLE_FUNC<std::log  >);
LAZY_FUNC1(LOG10, DOUBLE_TO_DOUBLE_FUNC<std::log10>);
LAZY_FUNC1(SIN,   DOUBLE_TO_DOUBLE_FUNC<std::sin  >);
LAZY_FUNC1(SINH,  DOUBLE_TO_DOUBLE_FUNC<std::sinh >);
LAZY_FUNC1(SQRT,  DOUBLE_TO_DOUBLE_FUNC<std::sqrt >);
LAZY_FUNC1(TAN,   DOUBLE_TO_DOUBLE_FUNC<std::tan  >);
LAZY_FUNC1(TANH,  DOUBLE_TO_DOUBLE_FUNC<std::tanh >);

#undef LAZY_FUNC1

} // ns spreadsheet