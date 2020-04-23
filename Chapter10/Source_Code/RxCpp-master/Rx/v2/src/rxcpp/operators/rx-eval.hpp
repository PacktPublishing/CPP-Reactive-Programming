// Copyright (c) Microsoft Open Technologies, Inc. All rights reserved. See License.txt in the project root for license information.

#pragma once

/*! \file rx-eval.hpp

    \brief For each item from this observable use Selector to produce an item to emit from the new observable that is returned.

    \tparam Selector the type of the transforming function

    \param s the selector function

    \return  Observable that emits the items from the source observable, transformed by the specified function.

    \sample
    \snippet eval.cpp eval sample
    \snippet output.txt eval sample
*/

#if !defined(RXCPP_OPERATORS_RX_APPLYFUNC_HPP)
#define RXCPP_OPERATORS_RX_APPLYFUNC_HPP

#include "../rx-includes.hpp"

namespace rxcpp {

namespace operators {

namespace detail {

template<class... AN>
struct eval_invalid_arguments {};

template<class... AN>
struct eval_invalid : public rxo::operator_base<eval_invalid_arguments<AN...>> {
    using type = observable<eval_invalid_arguments<AN...>, eval_invalid<AN...>>;
};
template<class... AN>
using eval_invalid_t = typename eval_invalid<AN...>::type;

template<class T, class Selector>
struct eval
{
    typedef rxu::decay_t<T> source_value_type;
    typedef rxu::decay_t<Selector> select_type;
    typedef decltype((*(select_type*)nullptr)(*(source_value_type*)nullptr)) value_type;
    select_type selector;

    eval(select_type s)
        : selector(std::move(s))
    {
    }

    template<class Subscriber>
    struct eval_observer
    {
        typedef eval_observer<Subscriber> this_type;
        typedef decltype((*(select_type*)nullptr)(*(source_value_type*)nullptr)) value_type;
        typedef rxu::decay_t<Subscriber> dest_type;
        typedef observer<source_value_type, this_type> observer_type;
        dest_type dest;
        mutable select_type selector;

        eval_observer(dest_type d, select_type s)
            : dest(std::move(d))
            , selector(std::move(s))
        {
        }
        template<class Value>
        void on_next(Value&& v) const {
            auto selected = on_exception(
                [&](){
                    return this->selector(std::forward<Value>(v));},
                dest);
            if (selected.empty()) {
                return;
            }
            dest.on_next(std::move(selected.get()));
        }
        void on_error(std::exception_ptr e) const {
            dest.on_error(e);
        }
        void on_completed() const {
            dest.on_completed();
        }

        static subscriber<source_value_type, observer_type> make(dest_type d, select_type s) {
            auto cs = d.get_subscription();
            return make_subscriber<source_value_type>(std::move(cs), observer_type(this_type(std::move(d), std::move(s))));
        }
    };

    template<class Subscriber>
    auto operator()(Subscriber dest) const
        -> decltype(eval_observer<Subscriber>::make(std::move(dest), selector)) {
        return      eval_observer<Subscriber>::make(std::move(dest), selector);
    }
};

}

/*! @copydoc rx-eval.hpp
*/
template<class... AN>
auto eval(AN&&... an)
    -> operator_factory<eval_tag, AN...> {
    return operator_factory<eval_tag, AN...>(std::make_tuple(std::forward<AN>(an)...));
}



}

template<>
struct member_overload<eval_tag>
{
    template<class Observable, class Selector,
        class Enabled = rxu::enable_if_all_true_type_t<
            is_observable<Observable>>,
        class ResolvedSelector = rxu::decay_t<Selector>,
        class SourceValue = rxu::value_type_t<Observable>,
        class Eval = rxo::detail::eval<SourceValue, ResolvedSelector>,
        class Value = rxu::value_type_t<Eval>>
    static auto member(Observable&& o, Selector&& s)
        -> decltype(o.template lift<Value>(Eval(std::forward<Selector>(s)))) {
        return      o.template lift<Value>(Eval(std::forward<Selector>(s)));
    }

    template<class... AN>
    static operators::detail::eval_invalid_t<AN...> member(const AN...) {
        std::terminate();
        return {};
        static_assert(sizeof...(AN) == 10000, "eval takes Selector");
    }
};

}

#endif
