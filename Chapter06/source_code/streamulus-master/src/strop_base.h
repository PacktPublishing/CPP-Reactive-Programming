//
//  strop_base.h
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

namespace streamulus
{
    class Engine;
    
    class StropBase 
    {
    public:
        
        StropBase()
        {    
            Init("unnamed");
        }
        
        StropBase(const std::string& display_name)
        {   
            Init(display_name);
        }
        
    private:
        void Init(const std::string& display_name)
        {
            mEngine = nullptr;
            mVertexDescriptor = 0;
            mIsActive = false;
            mIsDeleted = false;
            mExternalReference = false;
            mDisplayName = display_name;
        }
        
    public:
        virtual ~StropBase()
        {
        }
        
        Graph::vertex_descriptor GetDescriptor() const
        {
            return mVertexDescriptor;
        }
        
        size_t GetTopSortIndex() const
        {
            return mTopSortIndex;
        }
        
        Engine* GetEngine()
        {
            return mEngine;
        }
        
        void SetGraph(Engine* engine, const Graph::vertex_descriptor& desc, size_t top_sort_index)
        {
            assert(mEngine==nullptr || engine==nullptr || engine == mEngine);
            mEngine = engine; 
            mVertexDescriptor = desc;
            mTopSortIndex = top_sort_index;
        }
        
        inline bool IsActive()
        {
            return mIsActive;
        }
        
        void SetIsActive(bool isActive)
        {
            mIsActive = isActive;
        }
        
        const std::string& DisplayName() const
        {
            return mDisplayName;
        }
        
        void SetDisplayName(const std::string& name)
        {
            mDisplayName = name;
        }

        virtual void Work()=0;
        
        friend std::ostream& operator<<(std::ostream& os, const StropBase& strop)
        {
            return os << strop.DisplayName();    
        }

        void MarkAsDeleted()
        {
            mIsDeleted = true;
        }
        
        bool IsDeleted()
        {
            return mIsDeleted;
        }
        
        void AddExternalReference()
        {
            assert(!mExternalReference);
            mExternalReference = true;
        }
        
        bool RemoveExternalReference()
        {
            assert(mExternalReference);
            if (!mExternalReference)
                return false;
            mExternalReference = false;
            return true;
        }
        
        bool HasExternalReference()
        {
            return mExternalReference;
        }
    protected:
        
        Engine* mEngine; // no ownership. Do not delete. 
        Graph::vertex_descriptor mVertexDescriptor;
        size_t mTopSortIndex;
        bool mIsActive;  // is it in the "active" queue
        bool mIsDeleted; // Was deleted from the graph by the user (to be garbage collected). 
        bool mExternalReference; // The user has a handle to this strop (so it can be used in future expressions).
        std::string mDisplayName;
    };
    
} // ns streamulus