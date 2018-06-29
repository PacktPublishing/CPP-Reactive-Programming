//
//  sliding_window_library.h
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

#include "boost/tuple/tuple.hpp"
#include "boost/tuple/tuple_io.hpp"

namespace streamulus
{
    struct WindowCount
    {
    public:
        WindowCount() : mCount(0)
        {
        }
        
        template<typename Sig>
        struct value_type
        {
            using type = long;
        };
        
        template<typename T>
        void Insert(const T& value)
        {
            mCount++;
        }
        
        template<typename T>        
        void Remove(const T& value)
        {
            mCount--;
        }
        
        long Value() const
        {
            return mCount;
        }
        
    private:
        long mCount;
    };

    
    template<typename T>
    struct WindowSum
    {
    public:
        WindowSum() : mSum(0)
        {
        }
    
        template<typename Sig>
        struct value_type
        {
            using type = T;
        };

        void Insert(const T& value)
        {
            mSum += value;
        }
    
        void Remove(const T& value)
        {
            mSum -= value;
        }
    
        const T& Value() const
        {
            return mSum;
        }
    
    private:
        T mSum;
    };

    template<typename T>
    struct WindowAvg
    {
    public:
        WindowAvg() 
        {
        }

        using OutputType = boost::tuple<double,double,long>;
        
        template<typename Sig>
        struct value_type
        {
            using type = OutputType;
        };

        void Insert(const T& value)
        {
            mSum.Insert(value);
            mCount.Insert(value);
        }
        
        void Remove(const T& value)
        {
            mSum.Remove(value);
            mCount.Remove(value);
        }
        
        const OutputType Value() const
        {
            long count(mCount.Value());
            double sum(sum);
            double avg(count ? sum/count : 0);
            return boost::make_tuple(avg,sum,count);
        }
        
    private:
        WindowSum<T> mSum;
        WindowCount mCount;
    };
    
} // ns streamulus
