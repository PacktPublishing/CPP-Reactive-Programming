//
//  strop_stream_generator.h
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

#include "cpp14_utils.h"
#include "engine.h" 
#include "stream.h" 
#include "strop_base.h" 

#include <boost/make_shared.hpp>
#include <boost/optional.hpp>

namespace streamulus
{
    
    template<typename FR>
    class StropStreamProducer
    : public StropBase
    {
    public:

        // FR is the return type of the function that this strop computes.
        // R is the value that the strop publishes on its output stream (and is not optional).
        using R = remove_optional<FR>;

        using result_type = R;
        
        virtual ~StropStreamProducer() 
        {
        }

        void Output(const R& value) {
            if (!GetEngine())
                return;

            GetEngine()->template Output<R>(mVertexDescriptor, value);
            mCurrentValue = value;
        }

        void Output(const boost::optional<R>& value)
        {
            if(value) {
                Output(*value);
            }
        }

        using OutputStreamPtr = std::shared_ptr<Stream<R>>;

        OutputStreamPtr MakeOutputStream()
        {
            OutputStreamPtr stream = std::make_shared< Stream<R> >();
            if (mCurrentValue)
                stream->Append(*mCurrentValue);
            return stream;
        }
        
    private:
        boost::optional<R> mCurrentValue;
    };
    
    
} // ns streamulus