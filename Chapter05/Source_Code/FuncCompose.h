#ifndef FUNC_COMPOSE_H
#define FUNC_COMPOSE_H
//------------ Compose.cpp
//------ g++ -std=c++ Compose.cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;
//---------- base case compile time recursion
//---------- stops here

template <typename F, typename G>
auto compose(F&& f, G&& g)
{
return [=](auto x) { return f(g(x)); };
}

template <typename F, typename... R>
auto compose(F&& f, R&&... r)
{
return [=](auto x) { return f(compose(r...)(x)); };
}


template <typename F, typename G>
auto operator | (F&& f, G&& g)
{
return compose(std::forward<F>(f), std::forward<G>(g));
}
template <typename F, typename... R>
auto operator | (F&& f, R&&... r)
{
return operator | (std::forward<F>(f), r...);
}

template <typename R, typename F>
R Map(R r , F&& fn)
{
	std::transform(
		std::begin(r), std::end(r), std::begin(r),
		std::forward<F>(fn));
	return r;
}




template <typename R, typename F>
R Filter( R r , F&& fn )
{
   R ret(r.size());
   
   auto first = std::begin(r), last = std::end(r) , result = std::begin(ret); 
   bool inserted = false;
   while (first!=last) {
    if (fn(*first)) { *result = *first; inserted = true; ++result; } 
    ++first;
   }

   if ( !inserted ) { ret.clear(); ret.resize(0);  }

#if 0
   auto it = std::copy_if (std::begin(r), std::end(r), std::begin(ret), std::forward<F>(fn) );

   size_t dist = std::distance(std::begin(ret),it);
   if ( dist <= 1 ) {
            cout << "Resizing the Array" << endl;
            ret.resize(0);  // shrink container to new size
   }

#endif

   return ret;

}
/*

template <typename F, typename R, typename T>
constexpr T foldl(F&& f, R&& r, T i)
{
return std::accumulate(
std::begin(r), std::end(r),
std::move(i),
std::forward<F>(f));
}
template <typename F, typename R, typename T>
constexpr T foldr(F&& f, R&& r, T i)
{
return std::accumulate(
std::rbegin(r), std::rend(r),
std::move(i),
std::forward<F>(f));
}

*/
#endif

