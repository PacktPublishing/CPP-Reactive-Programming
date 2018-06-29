//
//  transforms.h
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

// Transforms for streamulus grammar

#include "stream.h"
#include "strop.h"
#include "strop_func.h"
#include "strop_sliding_window.h"
#include "input_stream.h"
#include "strop_return_type.h"
#include "funcs.h"
#include "cpp14_utils.h"

#include <boost/type_traits.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits.hpp>


namespace streamulus
{
    
    using namespace boost;
        
    //////////////////////////// Utilities ////////////////////////////
    template<typename T>
    struct BaseType
    {
        static const bool value = false;
    };
    
    template<typename Type>
    struct BaseType<std::shared_ptr<Type> >
    {
        static const bool value = true;
        using type = Type;
    };
    
    template<typename Sig>
    struct MakeStropPtrType
    {  
        using type = std::shared_ptr<Strop<Sig> >;
    };
         
    //////////////////////////// Transforms ////////////////////////////
    
    struct AddStropToGraph : proto::callable  
    {
        template<class State, typename StropType>
        const StropType operator()(State engine, StropType strop)
        {
            if (!strop->GetEngine())
            {
                engine->AddVertexToGraph(strop);
                engine->AddSource(strop);
            }
            if (engine->IsVerbose()) {
                std::cout << "AddStropToGraph: " << strop << " returning " << strop->GetDescriptor() << std::endl;
            }
            return strop;
        }
    };

    
    struct generic_func : proto::callable
    {
        template<typename Sig> struct result;
        
        // Arity = 0
        template<typename State, typename FArg>
        struct result<generic_func(State,FArg)>
        {
            using F = remove_const_t<remove_reference_t<FArg>>;
            using R = typename std::result_of<F()>::type;
            using type = const std::shared_ptr<Strop<R()>>;
        };

        template<typename State, typename F>
        typename result<generic_func(State, const F&)>::type
        operator()(State engine, const F& f)
        {  
            if (engine->IsVerbose()) {
                std::cout << "generic_func" << std::endl;
            }

            using FuncStropType = Func<F>;

            std::shared_ptr<FuncStropType> funcStropPtr = std::make_shared<FuncStropType>(f);
            engine->AddVertexToGraph(funcStropPtr);
            engine->AddSource(funcStropPtr);
            return funcStropPtr;
        }        
        
        // Arity = 1
        template<typename State,
                typename FArg,
                typename Arg1Strop>
        struct result<generic_func(State,FArg,Arg1Strop)>
        {
            using F = remove_const_t<remove_reference_t<FArg>>;
            using Arg1Type = strop_return_type<Arg1Strop>;
            using  R = typename std::result_of<F(Arg1Type)>::type;
            using type = const std::shared_ptr<Strop<R(Arg1Type)>>;
        };
        
        template<typename State,
                typename F,
                typename Arg1Strop>
        typename result<generic_func(State,const F&,Arg1Strop)>::type
        operator()(State engine,
                   const F& f,
                   const Arg1Strop arg1)
        {  
            if (engine->IsVerbose()) {
                std::cout << "generic_func" << std::endl;
            }

            using FuncStropType = Func<F,strop_return_type<Arg1Strop>>;

            std::shared_ptr<FuncStropType> funcStropPtr = std::make_shared<FuncStropType>(f);
            
            typename BaseType<Arg1Strop>::type::OutputStreamPtr arg1Stream(arg1->MakeOutputStream());
            funcStropPtr->SetInputs(arg1Stream);
            
            engine->AddVertexToGraph(funcStropPtr);
            engine->AddEdgeToGraph(arg1, funcStropPtr, arg1Stream);
            return funcStropPtr;
        }

        // Arity = 2
        template<typename State, typename FArg, typename Arg1Strop, typename Arg2Strop>
        struct result<generic_func(State,FArg,Arg1Strop,Arg2Strop)>
        {
            using F = remove_const_t<remove_reference_t<FArg>>;
            using Arg1Type = strop_return_type<Arg1Strop>;
            using Arg2Type = strop_return_type<Arg2Strop>;
            using R = typename std::result_of<F(Arg1Type,Arg2Type)>::type;
            using type = const std::shared_ptr<Strop<R(Arg1Type,Arg2Type)>>;
        };
        
        template<typename State, typename F, typename Arg1Strop, typename Arg2Strop>
        typename result<generic_func(State,const F&,Arg1Strop,Arg2Strop)>::type
        operator()(State engine, const F& f,const Arg1Strop arg1, const Arg2Strop arg2)
        {   
            if (engine->IsVerbose()) {
                std::cout << "generic_func" << std::endl;
            }

            using FuncStropType = Func<F, strop_return_type<Arg1Strop>, strop_return_type<Arg2Strop>>;
            std::shared_ptr<FuncStropType> funcStropPtr = std::make_shared<FuncStropType>(f);
            
            typename BaseType<Arg1Strop>::type::OutputStreamPtr arg1Stream(arg1->MakeOutputStream());
            typename BaseType<Arg2Strop>::type::OutputStreamPtr arg2Stream(arg2->MakeOutputStream());
            funcStropPtr->SetInputs(arg1Stream, arg2Stream);
            
            engine->AddVertexToGraph(funcStropPtr);
            engine->AddEdgeToGraph(arg1, funcStropPtr, arg1Stream);
            engine->AddEdgeToGraph(arg2, funcStropPtr, arg2Stream);
            return funcStropPtr;
        }

        // Arity = 3
        template<typename State,
                typename FArg,
                typename Arg1Strop,
                typename Arg2Strop,
                typename Arg3Strop>
        struct result<generic_func(State,FArg,Arg1Strop,Arg2Strop,Arg3Strop)>
        {
            using F = remove_const_t<remove_reference_t<FArg>>;
            using Arg1Type = strop_return_type<Arg1Strop>;
            using Arg2Type = strop_return_type<Arg2Strop>;
            using Arg3Type = strop_return_type<Arg3Strop>;
            using R = typename std::result_of<F(Arg1Type,Arg2Type,Arg3Type)>::type;
            using type = const std::shared_ptr<Strop<R(Arg1Type,Arg2Type,Arg3Type)> >;
        };
        
        template<typename State,
                typename F,
                typename Arg1Strop,
                typename Arg2Strop,
                typename Arg3Strop>
        typename result<generic_func(State,const F&,Arg1Strop,Arg2Strop,Arg3Strop)>::type
        operator()(State engine,
                   const F& f,
                   const Arg1Strop arg1, 
                   const Arg2Strop arg2, 
                   const Arg3Strop arg3)
        {   
            if (engine->IsVerbose()) {
                std::cout << "generic_func" << std::endl;
            }

            using FuncStropType = Func<F
            ,strop_return_type<Arg1Strop>
            ,strop_return_type<Arg2Strop>
            ,strop_return_type<Arg3Strop>
            >;

            std::shared_ptr<FuncStropType> funcStropPtr = std::make_shared<FuncStropType>(f);
            
            typename BaseType<Arg1Strop>::type::OutputStreamPtr arg1Stream(arg1->MakeOutputStream());
            typename BaseType<Arg2Strop>::type::OutputStreamPtr arg2Stream(arg2->MakeOutputStream());
            typename BaseType<Arg3Strop>::type::OutputStreamPtr arg3Stream(arg3->MakeOutputStream());
            funcStropPtr->SetInputs(arg1Stream, arg2Stream, arg3Stream);
            
            engine->AddVertexToGraph(funcStropPtr);
            engine->AddEdgeToGraph(arg1, funcStropPtr, arg1Stream);
            engine->AddEdgeToGraph(arg2, funcStropPtr, arg2Stream);
            engine->AddEdgeToGraph(arg3, funcStropPtr, arg3Stream);
            return funcStropPtr;
        }
        
        // Arity = 4
        template<typename State,
                typename FArg,
                typename Arg1Strop,
                typename Arg2Strop,
                typename Arg3Strop,
                typename Arg4Strop>
        struct result<generic_func(State,FArg,Arg1Strop,Arg2Strop,Arg3Strop,Arg4Strop)>
        {
            using F = remove_const_t<remove_reference_t<FArg>>;
            using Arg1Type = strop_return_type<Arg1Strop>;
            using Arg2Type = strop_return_type<Arg2Strop>;
            using Arg3Type = strop_return_type<Arg3Strop>;
            using Arg4Type = strop_return_type<Arg4Strop>;
            using R = typename std::result_of<F(Arg1Type,Arg2Type,Arg3Type,Arg4Type)>::type;
            using type = const std::shared_ptr<Strop<R(Arg1Type,Arg2Type,Arg3Type,Arg4Type)> >;
        };
        
        template<typename State,
                typename F,
                typename Arg1Strop,
                typename Arg2Strop,
                typename Arg3Strop,
                typename Arg4Strop>
        typename result<generic_func(State,const F&,Arg1Strop,Arg2Strop,Arg3Strop,Arg4Strop)>::type
        operator()(State engine,
                   const F& f,
                   const Arg1Strop arg1, 
                   const Arg2Strop arg2, 
                   const Arg3Strop arg3, 
                   const Arg4Strop arg4)
        {   
            if (engine->IsVerbose()) {
                std::cout << "generic_func" << std::endl;
            }

            using FuncStropType = Func<F
            ,strop_return_type<Arg1Strop>
            ,strop_return_type<Arg2Strop>
            ,strop_return_type<Arg3Strop>
            ,strop_return_type<Arg4Strop>
            >;

            std::shared_ptr<FuncStropType> funcStropPtr = std::make_shared<FuncStropType>(f);
            
            typename BaseType<Arg1Strop>::type::OutputStreamPtr arg1Stream(arg1->MakeOutputStream());
            typename BaseType<Arg2Strop>::type::OutputStreamPtr arg2Stream(arg2->MakeOutputStream());
            typename BaseType<Arg3Strop>::type::OutputStreamPtr arg3Stream(arg3->MakeOutputStream());
            typename BaseType<Arg4Strop>::type::OutputStreamPtr arg4Stream(arg4->MakeOutputStream());
            funcStropPtr->SetInputs(arg1Stream, arg2Stream, arg3Stream, arg4Stream);
            
            engine->AddVertexToGraph(funcStropPtr);
            engine->AddEdgeToGraph(arg1, funcStropPtr, arg1Stream);
            engine->AddEdgeToGraph(arg2, funcStropPtr, arg2Stream);
            engine->AddEdgeToGraph(arg3, funcStropPtr, arg3Stream);
            engine->AddEdgeToGraph(arg4, funcStropPtr, arg4Stream);
            return funcStropPtr;
        }
        
        // Arity = 5
        template<typename State,
                typename FArg,
                typename Arg1Strop,
                typename Arg2Strop,
                typename Arg3Strop,
                typename Arg4Strop,
                typename Arg5Strop>
        struct result<generic_func(State,FArg,Arg1Strop,Arg2Strop,Arg3Strop,Arg4Strop,Arg5Strop)>
        {
            using F = remove_const_t<remove_reference_t<FArg>>;
            using Arg1Type = strop_return_type<Arg1Strop>;
            using Arg2Type = strop_return_type<Arg2Strop>;
            using Arg3Type = strop_return_type<Arg3Strop>;
            using Arg4Type = strop_return_type<Arg4Strop>;
            using Arg5Type = strop_return_type<Arg5Strop>;
            using R = typename std::result_of<F(Arg1Type,Arg2Type,Arg3Type,Arg4Type,Arg5Type)>::type;

            using type = const std::shared_ptr<Strop<R(Arg1Type,Arg2Type,Arg3Type,Arg4Type,Arg5Type)>>;
        };
        
        template<typename State,
                typename F,
                typename Arg1Strop,
                typename Arg2Strop,
                typename Arg3Strop,
                typename Arg4Strop,
                typename Arg5Strop>
        typename result<generic_func(State,F&,Arg1Strop,Arg2Strop,Arg3Strop,Arg4Strop,Arg5Strop)>::type
        operator()(State engine,
                   const F& f,
                   const Arg1Strop arg1, 
                   const Arg2Strop arg2, 
                   const Arg3Strop arg3, 
                   const Arg4Strop arg4, 
                   const Arg5Strop arg5)
        {   
            if (engine->IsVerbose()) {
                std::cout << "generic_func" << std::endl;
            }

            using FuncStropType = Func<F
                        ,strop_return_type<Arg1Strop>
                        ,strop_return_type<Arg2Strop>
                        ,strop_return_type<Arg3Strop>
                        ,strop_return_type<Arg4Strop>
                        ,strop_return_type<Arg5Strop>
                        >;

            std::shared_ptr<FuncStropType> funcStropPtr = std::make_shared<FuncStropType>(f);
            
            typename BaseType<Arg1Strop>::type::OutputStreamPtr arg1Stream(arg1->MakeOutputStream());
            typename BaseType<Arg2Strop>::type::OutputStreamPtr arg2Stream(arg2->MakeOutputStream());
            typename BaseType<Arg3Strop>::type::OutputStreamPtr arg3Stream(arg3->MakeOutputStream());
            typename BaseType<Arg4Strop>::type::OutputStreamPtr arg4Stream(arg4->MakeOutputStream());
            typename BaseType<Arg5Strop>::type::OutputStreamPtr arg5Stream(arg5->MakeOutputStream());
            funcStropPtr->SetInputs(arg1Stream, arg2Stream, arg3Stream, arg4Stream, arg5Stream);
            
            engine->AddVertexToGraph(funcStropPtr);
            engine->AddEdgeToGraph(arg1, funcStropPtr, arg1Stream);
            engine->AddEdgeToGraph(arg2, funcStropPtr, arg2Stream);
            engine->AddEdgeToGraph(arg3, funcStropPtr, arg3Stream);
            engine->AddEdgeToGraph(arg4, funcStropPtr, arg4Stream);
            engine->AddEdgeToGraph(arg5, funcStropPtr, arg5Stream);
            return funcStropPtr;
        }

    };
      

    struct HandleTerminal : proto::callable  
    {
        template<class Sig> struct result;
        
        // Case 1: terminal is a shared_ptr to a strop 
        template<class State, class This, class StropType>
        struct result<This(State, const std::shared_ptr<StropType>&)>
        {
            using StropPtrType = const std::shared_ptr<StropType>&;
            using type = typename std::result_of<AddStropToGraph(State, StropPtrType)>::type;
        };
        
        template<class State, class This, class StropType>
        struct result<This(State, std::shared_ptr<StropType>)>
        : result<This(State, const std::shared_ptr<StropType>&)>
        {};
        
        template<class State, typename StropType>
        typename boost::enable_if<BaseType<StropType>, typename result<HandleTerminal(State, StropType)>::type>::type
        operator()(State engine, StropType strop)
        {
            return AddStropToGraph()(engine, strop);
        }
        
        // Final case: Terminal is any other constant
        template<class State, class This, class ConstType>
        struct result<This(State, ConstType)>
        {
            using type = const std::shared_ptr<
                    Strop<typename std::result_of<ConstFunc<ConstType>()>::type()>
            >;
        };
        
        template<class State, typename ConstType>
        typename boost::disable_if<BaseType<ConstType>, typename result<HandleTerminal(State, ConstType)>::type>::type
        operator()(State engine, ConstType const_value)
        {
            return generic_func()(engine,ConstFunc<ConstType>(const_value));
        }
         
    };

    
    struct SlidingWindow : proto::callable
    {
        template<typename Sig> struct result;
        
        template<typename State, typename ArgStrop>
        struct result<SlidingWindow(State,const int&,ArgStrop)>
        {
            using input_type = strop_return_type<ArgStrop>;
            using result_type = WindowUpdateType<input_type>;
            using type = const std::shared_ptr<StropStreamProducer<result_type> >;
        };
        
        template<typename State, typename ArgStrop>
        typename result<SlidingWindow(State,const int&,ArgStrop)>::type
        operator()(State engine, const int& size, const ArgStrop arg)
        {
            using WindowStropType = Window<strop_return_type<ArgStrop>>;
            std::shared_ptr<WindowStropType> strop = std::make_shared<WindowStropType>(size);

            typename BaseType<ArgStrop>::type::OutputStreamPtr argStream(arg->MakeOutputStream());
            strop->SetInputs(argStream);
            
            engine->AddVertexToGraph(strop);
            engine->AddEdgeToGraph(arg, strop, argStream);
            return strop;
        }        
    };
    
        
    
    
} // ns streamulus
