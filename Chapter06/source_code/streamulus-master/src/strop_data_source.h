//
//  strop_data_source.h
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

#include <iostream>

#include "strop_stream_producer.h"

namespace streamulus
{    
    
    template<typename R>
    class DataSource : public StropStreamProducer<R>  
    {
    public:
        
        DataSource(const std::string& name, bool verbose)
        : mIsValid(false)
        , mIsVerbose(verbose)
        {
            StropStreamProducer<R>::SetDisplayName(name);
        }
                
        virtual void Work()
        {
            // Return the last tick's value. 
            if (mIsValid)
                StropStreamProducer<R>::Output(mLastValue);
        }
            
        inline bool IsVerbose()
        {
            return mIsVerbose;
        }
        
        void Tick(const R& value)
        {
            if (IsVerbose())
                std::cout << "-------------   " 
                          << StropStreamProducer<R>::DisplayName() << " <-- " 
                          << value << "   -------------" << std::endl;
            StropStreamProducer<R>::Output(value); 
            mLastValue = value;
            mIsValid = true;
        }
                
    private:
        R           mLastValue;
        bool        mIsValid;
        bool        mIsVerbose;
    };
    
} // ns streamulus
