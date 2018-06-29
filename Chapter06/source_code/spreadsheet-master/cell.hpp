//
//  cell.hpp
//  spreadsheet
//
//  Created by Irit Katriel on 01/08/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "streamulus.h"

namespace spreadsheet {
    
    template<typename T>
    class Cell
    {
    public:
        
        using expr_type = streamulus::InputStream<T>;

        Cell()
            : mOutStream(streamulus::NewInputStream<T>("unnamed", false))
        {
            SetEngine(nullptr, "");
        }
        
        Cell(streamulus::Streamulus* streamulus_engine, const std::string& name)
            : mOutStream(streamulus::NewInputStream<T>(name.c_str(), false))
        {
            SetEngine(streamulus_engine, name);
        }
        
        void SetEngine(streamulus::Streamulus* streamulus_engine, const std::string& name)
        {
            mStreamulusEngine = streamulus_engine;
            mName = name;
        }
                
        expr_type operator()() const
        {
            return mOutStream;
        }
                
        T Value()
        {
            assert(IsValid());
            return *mValue;
        }
        
        template<typename Expr>
        void Set(const Expr& expr)
        {
            using streamulus::Streamify;
            
            if (mSubscription)
            {
                mStreamulusEngine->UnSubscribe<T>(*mSubscription);
                mSubscription = boost::none;
            }
            
            SetCellValue set_cell_value(this);
            mSubscription = mStreamulusEngine->Subscribe(Streamify(set_cell_value)(expr)); 
        }

    private:
                
        bool IsValid()
        {
            return mSubscription && mValue;
        }
        
        void SetValue(const T& value)
        {
            mValue = value;
            InputStreamPut(mOutStream,value);
        }
        
        
        struct SetCellValue
        {
        public:
            SetCellValue(Cell* cell)
                :mCell(cell)
            {
            }
            
            template<class Sig> 
            struct result 
            {
                typedef T type; 
            };
            
            typename result<SetCellValue>::type
            operator()(const T& value)
            {
                // std::cout << mCell->mName << " <-- " << value << std::endl;
                mCell->SetValue(value);
                return value;
            }
            
        private:
            Cell* mCell;
        };
        

        streamulus::Streamulus* mStreamulusEngine; // No ownership. Do not delete.
        std::string mName;
        boost::optional<typename streamulus::Subscription<T>::type> mSubscription;
        expr_type mOutStream;
        boost::optional<T> mValue;
    };
    
} // ns spreadsheet


