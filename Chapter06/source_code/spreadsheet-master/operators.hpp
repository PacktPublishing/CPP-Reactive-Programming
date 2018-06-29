//
//  operators.hpp
//  spreadsheet
//
//  Created by Irit Katriel on 01/08/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once


namespace spreadsheet {

    namespace bp = boost::proto;

    // is_cell temaplate for enabling/disabling operators
    template<typename T>
    struct is_cell : boost::mpl::false_
    {
    };
    
    template<typename T>
    struct is_cell<Cell<T> > : boost::mpl::true_
    {
    };
    

    ////////////////////////////////////////////////////////////////////////////////////////
    // operator overloads

#define UNARY_OP(OPERATOR, TAG) \
    template<typename T> \
    typename bp::result_of::make_expr<TAG, typename Cell<T>::expr_type>::type \
    OPERATOR(const Cell<T>& arg) \
    { \
        return bp::make_expr<TAG>(arg()); \
    } 
    
#define BINARY_OP(OPERATOR, TAG) \
    template<typename LhsT, typename RhsT> \
    typename boost::disable_if<typename is_cell<LhsT>::type, \
           typename bp::result_of::make_expr<TAG, \
                                typename bp::result_of::as_expr<LhsT>::type, \
                                typename Cell<RhsT>::expr_type>::type>::type \
    OPERATOR(const LhsT& lhs, const Cell<RhsT>& rhs) \
    { \
        return bp::make_expr<TAG>(bp::as_expr(lhs),rhs()); \
    } \
    \
    template<typename LhsT, typename RhsT> \
    typename boost::disable_if<typename is_cell<RhsT>::type, \
        typename bp::result_of::make_expr<TAG, \
                                typename Cell<LhsT>::expr_type, \
                                typename bp::result_of::as_expr<RhsT>::type>::type>::type \
    OPERATOR(const Cell<LhsT>& lhs, const RhsT& rhs) \
    { \
        return bp::make_expr<TAG>(lhs(),bp::as_expr(rhs)); \
    } \
    \
    template<typename LhsT, typename RhsT> \
    typename bp::result_of::make_expr<TAG, \
                                typename Cell<LhsT>::expr_type, \
                                typename Cell<RhsT>::expr_type>::type \
    OPERATOR(const Cell<LhsT>& lhs, const Cell<RhsT>& rhs) \
    { \
        return bp::make_expr<TAG>(lhs(),rhs()); \
    } 

    
    UNARY_OP(operator+, bp::tag::unary_plus);
    UNARY_OP(operator-, bp::tag::negate);
    UNARY_OP(operator~, bp::tag::complement);
    UNARY_OP(operator!, bp::tag::logical_not);

    
//    BINARY_OP(operator<<, bp::tag::shift_left);
    BINARY_OP(operator>>, bp::tag::shift_right);
    BINARY_OP(operator* , bp::tag::multiplies);
    BINARY_OP(operator/ , bp::tag::divides);
    BINARY_OP(operator% , bp::tag::modulus);
    BINARY_OP(operator+ , bp::tag::plus);
    BINARY_OP(operator- , bp::tag::minus);
    BINARY_OP(operator< , bp::tag::less);
    BINARY_OP(operator>  , bp::tag::greater);
    BINARY_OP(operator<= , bp::tag::less_equal);
    BINARY_OP(operator>= , bp::tag::greater_equal);
    BINARY_OP(operator== , bp::tag::equal_to);
    BINARY_OP(operator!= , bp::tag::not_equal_to);
    BINARY_OP(operator|| , bp::tag::logical_or);
    BINARY_OP(operator&& , bp::tag::logical_and);
    BINARY_OP(operator|  , bp::tag::bitwise_or);
    BINARY_OP(operator&  , bp::tag::bitwise_and);
    BINARY_OP(operator^  , bp::tag::bitwise_xor);
    
#undef UNARY_OP
#undef BINARY_OP
    
};