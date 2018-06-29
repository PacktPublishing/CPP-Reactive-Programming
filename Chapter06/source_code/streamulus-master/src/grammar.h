//
//  grammar.h
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

#include "transforms.h"
#include "operators.h"

#include "composed_func.h"

#include <boost/proto/proto.hpp>

namespace streamulus
{
    struct smls_grammar;
    
    // ************************************* HELPER *************************************
    struct get_terminal_value
    : proto::when<proto::terminal<proto::_>, proto::_value>
    {
    };
        
    
    // ******************************* OPERATOR CATEGORIES ******************************
    
    struct UnaryOpCases
    {
        // The primary template matches nothing:
        template<typename Tag>
        struct case_
        : proto::not_<proto::_>
        {};
    };

    template<> struct UnaryOpCases::case_<proto::tag::unary_plus>  : proto::unary_plus <smls_grammar>  {};
    template<> struct UnaryOpCases::case_<proto::tag::negate>      : proto::negate <smls_grammar>      {};
    template<> struct UnaryOpCases::case_<proto::tag::dereference> : proto::dereference <smls_grammar> {};
    template<> struct UnaryOpCases::case_<proto::tag::complement>  : proto::complement <smls_grammar>  {};
    template<> struct UnaryOpCases::case_<proto::tag::logical_not> : proto::logical_not <smls_grammar> {};
    template<> struct UnaryOpCases::case_<proto::tag::pre_inc>     : proto::pre_inc <smls_grammar>     {};
    template<> struct UnaryOpCases::case_<proto::tag::pre_dec>     : proto::pre_dec <smls_grammar>     {};
    template<> struct UnaryOpCases::case_<proto::tag::post_inc>    : proto::post_inc <smls_grammar>    {};
    template<> struct UnaryOpCases::case_<proto::tag::post_dec>    : proto::post_dec <smls_grammar>    {};
                    
    struct BinaryOpCases
    {
        // The primary template matches nothing:
        template<typename Tag>
        struct case_
        : proto::not_<proto::_>
        {};
    };

    template<> struct BinaryOpCases::case_<proto::tag::shift_left>    : proto::shift_left    <smls_grammar,smls_grammar> {};
    template<> struct BinaryOpCases::case_<proto::tag::shift_right>   : proto::shift_right   <smls_grammar,smls_grammar> {};
    template<> struct BinaryOpCases::case_<proto::tag::multiplies>    : proto::multiplies    <smls_grammar,smls_grammar> {};
    template<> struct BinaryOpCases::case_<proto::tag::divides>       : proto::divides       <smls_grammar,smls_grammar> {};
    template<> struct BinaryOpCases::case_<proto::tag::modulus>       : proto::modulus       <smls_grammar,smls_grammar> {};
    template<> struct BinaryOpCases::case_<proto::tag::plus>          : proto::plus          <smls_grammar,smls_grammar> {};
    template<> struct BinaryOpCases::case_<proto::tag::minus>         : proto::minus         <smls_grammar,smls_grammar> {};
    template<> struct BinaryOpCases::case_<proto::tag::less>          : proto::less          <smls_grammar,smls_grammar> {};
    template<> struct BinaryOpCases::case_<proto::tag::greater>       : proto::greater       <smls_grammar,smls_grammar> {};
    template<> struct BinaryOpCases::case_<proto::tag::less_equal>    : proto::less_equal    <smls_grammar,smls_grammar> {};
    template<> struct BinaryOpCases::case_<proto::tag::greater_equal> : proto::greater_equal <smls_grammar,smls_grammar> {};
    template<> struct BinaryOpCases::case_<proto::tag::equal_to>      : proto::equal_to      <smls_grammar,smls_grammar> {};
    template<> struct BinaryOpCases::case_<proto::tag::not_equal_to>  : proto::not_equal_to  <smls_grammar,smls_grammar> {};
    template<> struct BinaryOpCases::case_<proto::tag::logical_or>    : proto::logical_or    <smls_grammar,smls_grammar> {};
    template<> struct BinaryOpCases::case_<proto::tag::logical_and>   : proto::logical_and   <smls_grammar,smls_grammar> {};
    template<> struct BinaryOpCases::case_<proto::tag::bitwise_or>    : proto::bitwise_or    <smls_grammar,smls_grammar> {};
    template<> struct BinaryOpCases::case_<proto::tag::bitwise_and>   : proto::bitwise_and   <smls_grammar,smls_grammar> {};
    template<> struct BinaryOpCases::case_<proto::tag::bitwise_xor>   : proto::bitwise_xor   <smls_grammar,smls_grammar> {};
    template<> struct BinaryOpCases::case_<proto::tag::subscript>     : proto::subscript     <smls_grammar,smls_grammar> {};    
    template<> struct BinaryOpCases::case_<proto::tag::assign>             : proto::assign             <smls_grammar,smls_grammar> {};
    template<> struct BinaryOpCases::case_<proto::tag::shift_left_assign>  : proto::shift_left_assign  <smls_grammar,smls_grammar> {};
    template<> struct BinaryOpCases::case_<proto::tag::shift_right_assign> : proto::shift_right_assign <smls_grammar,smls_grammar> {};
    template<> struct BinaryOpCases::case_<proto::tag::multiplies_assign>  : proto::multiplies_assign  <smls_grammar,smls_grammar> {};
    template<> struct BinaryOpCases::case_<proto::tag::divides_assign>     : proto::divides_assign     <smls_grammar,smls_grammar> {};
    template<> struct BinaryOpCases::case_<proto::tag::modulus_assign>     : proto::modulus_assign     <smls_grammar,smls_grammar> {};
    template<> struct BinaryOpCases::case_<proto::tag::plus_assign>        : proto::plus_assign        <smls_grammar,smls_grammar> {};
    template<> struct BinaryOpCases::case_<proto::tag::minus_assign>       : proto::minus_assign       <smls_grammar,smls_grammar> {};
    template<> struct BinaryOpCases::case_<proto::tag::bitwise_and_assign> : proto::bitwise_and_assign <smls_grammar,smls_grammar> {};
    template<> struct BinaryOpCases::case_<proto::tag::bitwise_or_assign>  : proto::bitwise_or_assign  <smls_grammar,smls_grammar> {};
    template<> struct BinaryOpCases::case_<proto::tag::bitwise_xor_assign> : proto::bitwise_xor_assign <smls_grammar,smls_grammar> {};

    struct UnsupportedOpCases
    {
        // The primary template matches nothing:
        template<typename Tag>
        struct case_
        : proto::not_<proto::_>
        {};
    };

    // Disabling the address-of operator. It is a special case in proto and is problematic to 
    // support. It is also a bit nonsensical for streams.
    template<> struct UnsupportedOpCases::case_<proto::tag::address_of>  : proto::address_of <smls_grammar>  {};
    template<> struct UnsupportedOpCases::case_<proto::tag::comma>       : proto::comma      <smls_grammar,smls_grammar> {};

    // ********************************* SLIDING WINDOWS *********************************

    struct window_ {};

    
    // ************************************** RULES **************************************
    
    // terminals
    struct strop_terminal_rule        : proto::terminal<std::shared_ptr<proto::_> > {};
    struct const_terminal_rule        : proto::terminal<proto::_>                     {};
    
    
    struct unary_op_rule       : proto::switch_<UnaryOpCases> {};
    struct binary_op_rule      : proto::switch_<BinaryOpCases> {};    
    struct ternary_op_rule     : proto::if_else_<smls_grammar,smls_grammar,smls_grammar> {};
    struct unsupported_op_rule : proto::switch_<UnsupportedOpCases> {};

    struct window_rule       : proto::function<proto::terminal<window_>, proto::terminal<int>, smls_grammar> {};
            
    struct func_terminal     : proto::and_<proto::terminal<proto::_>,proto::not_<proto::terminal<window_> > > {};
    struct function_0_rule   : proto::function<func_terminal> {};
    struct function_1_rule   : proto::function<func_terminal,smls_grammar> {};
    struct function_2_rule   : proto::function<func_terminal,smls_grammar,smls_grammar> {};
    struct function_3_rule   : proto::function<func_terminal,smls_grammar,smls_grammar,smls_grammar> {};
    struct function_4_rule   : proto::function<func_terminal,smls_grammar,smls_grammar,smls_grammar,smls_grammar> {};
    struct function_5_rule   : proto::function<func_terminal,smls_grammar,smls_grammar,smls_grammar,smls_grammar,smls_grammar> {};
    
    
    // ************************************** GRAMMAR **************************************
    
    struct smls_grammar;
    
    struct smls_grammar_cases
    {
        
        // The primary template matches nothing:
        template<typename TAG>
        struct case_
        : proto::or_<
             proto::when<unary_op_rule,
                        generic_func(proto::_state,
                                     functor_of<TAG>(),
                                     smls_grammar(proto::_child))>

           , proto::when<binary_op_rule,
                        generic_func(proto::_state,
                                     functor_of<TAG>(),
                                     smls_grammar(proto::_left),
                                     smls_grammar(proto::_right))>
           , proto::when<ternary_op_rule,
                        generic_func(proto::_state,
                                     functor_of<TAG>(),
                                     smls_grammar(proto::_child0),
                                     smls_grammar(proto::_child1),
                                     smls_grammar(proto::_child2))>
         >
        {};
    };
            
    // Terminal expressions 
    template<>
    struct smls_grammar_cases::case_<proto::tag::terminal>
        : proto::when<proto::_, HandleTerminal(proto::_state, proto::_value)>
    {};
    
    
    // ////////////////////////////// Function expressions //////////////////////////////
    
    
    template<>
    struct smls_grammar_cases::case_<proto::tag::function>
    : proto::or_< 
        proto::when<window_rule,
                    SlidingWindow(proto::_state, get_terminal_value(proto::_child1), smls_grammar(proto::_child2))
        >
        , proto::when<function_0_rule, 
                    generic_func(proto::_state, get_terminal_value(proto::_child0))
        >
        , proto::when<function_1_rule, 
                    generic_func(proto::_state,
                                 get_terminal_value(proto::_child0),
                                 smls_grammar(proto::_child1))
        >
        , proto::when<function_2_rule, 
                    generic_func(proto::_state,
                                 get_terminal_value(proto::_child0),
                                 smls_grammar(proto::_child1),
                                 smls_grammar(proto::_child2))
        >
        , proto::when<function_3_rule,
                    generic_func(proto::_state,
                                 get_terminal_value(proto::_child0),
                                 smls_grammar(proto::_child1),
                                 smls_grammar(proto::_child2),
                                 smls_grammar(proto::_child3))
        >
        , proto::when<function_4_rule, 
                      generic_func(proto::_state,
                                   get_terminal_value(proto::_child0),
                                   smls_grammar(proto::_child1),
                                   smls_grammar(proto::_child2),
                                   smls_grammar(proto::_child3),
                                   smls_grammar(proto::_child4))
        >
        , proto::when<function_5_rule,
                      generic_func(proto::_state,
                                   get_terminal_value(proto::_child0),
                                   smls_grammar(proto::_child1),
                                   smls_grammar(proto::_child2),
                                   smls_grammar(proto::_child3),
                                   smls_grammar(proto::_child4),
                                   smls_grammar(proto::_child5))
        >
    >
    {};
    
    
    // The grammar
    struct smls_grammar
    : proto::switch_<smls_grammar_cases>
    {};
    
    
} // ns streamulus