//
//  strop.h
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

#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/fusion/functional/invocation/invoke.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/transform.hpp>
#include <boost/fusion/include/make_vector.hpp>
#include <boost/proto/proto.hpp>


#include "strop_stream_producer.h" 

namespace streamulus {

    template<typename F> // F = function signature
    class Strop
            : public StropStreamProducer<typename boost::function_types::result_type<F>::type> {

    private:
        template<typename T>
        struct MakeStreamPtrType {
            using type = std::shared_ptr<Stream<T>>;
        };

        using R = typename boost::function_types::result_type<F>::type;
        using param_types =  boost::function_types::parameter_types<F>;
        using input_types = typename boost::mpl::transform<param_types, MakeStreamPtrType<boost::mpl::_1> >::type;

    public:

        virtual ~Strop() {
        }

        template<typename... Inputs>
        void SetInputs(Inputs... inputs) {
            mInputs = boost::fusion::make_vector(inputs...);
        }

        template<int I>
        Stream<typename boost::mpl::at_c<param_types, I>::type> *const Input() {
            return boost::fusion::at_c<I>(mInputs).get();
        }

        struct extract_input_value {
            template<typename Sig>
            struct result;

            template<typename This, typename T>
            struct result<This(std::shared_ptr<Stream<T>>)> {
                typedef T type;
            };

            template<typename T>
            T operator()(std::shared_ptr<Stream<T>> input) const {
                return input.get()->Current();
            }
        };

        R Invoke(std::function<F> &f) {
            return boost::fusion::invoke(
                    std::reference_wrapper<std::function<F>>(f),
                    boost::fusion::transform(mInputs, extract_input_value())
            );
        }

        /**
         * HasMore() returns true if any of the inputs has more data, false otherwise
         */
        struct has_more_fold_function {
            template<typename T>
            bool operator()(const bool &state, const T &t) const {
                return state || t->HasMore();
            }
        };

        bool HasMore() {
            return boost::fusion::fold(mInputs, false, has_more_fold_function());
        }

        /**
         * IsValid() returns true if all inputs are valid, false otherwise
         */
        struct is_valid_fold_function {
            template<typename T>
            bool operator()(const bool &state, const T &t) const {
                return state && t->IsValid();
            }
        };

        bool IsValid() {
            return NoInputs() || boost::fusion::fold(mInputs, true, is_valid_fold_function());
        }

        bool NoInputs() {
            return boost::fusion::empty(mInputs);
        }

    private:

        typename boost::fusion::result_of::as_vector<typename input_types::type>::type mInputs;

    };


} // ns streamulus