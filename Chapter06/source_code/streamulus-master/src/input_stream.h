//
//  input_stream.h
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

#include "strop_data_source.h"

#include <boost/make_shared.hpp>

namespace streamulus
{
    // Convenience utilities for defining input streams    

    namespace detail {

        template<typename T>
        using input_stream_data_source_t = std::shared_ptr<DataSource<T>>;

        template<typename T>
        using input_stream_proto_expression_t = typename boost::proto::terminal<input_stream_data_source_t<T>>::type;
    }

    template<typename T>
    using InputStream = const detail::input_stream_proto_expression_t<T>;

    // Create a new stream
    template<typename T>
    InputStream<T> NewInputStream(const std::string& name, bool verbose)
    {
        detail::input_stream_proto_expression_t<T> expr = {std::make_shared<DataSource<T> >(name, verbose)};
        return expr;
    }

    // Add an input to the stream
    template<typename T>
    void InputStreamPut(InputStream<T> terminal, const T& value)
    {
        boost::proto::value(terminal)->Tick(value);
    }
    
        
} // ns streamulus
