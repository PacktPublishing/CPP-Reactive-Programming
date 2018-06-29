//
//  engine.h
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

#include "graph.h"
#include "strop_base.h"
#include "grammar.h"
#include "subscription.h"
#include "stream.h"

#include <boost/graph/graphviz.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/utility.hpp>

#include <iostream>

namespace streamulus
{
    
    class Engine : public boost::noncopyable
    {
    public:
        
        Engine()
            : mWorking(false)
            , mCurrentTime(0)
            , mVerbose(false)
            , mNumRemovedVertices(0)
        {
        };
        
        
        void AddVertexToGraph(const StropPtr& strop)
        {
            boost::add_vertex(strop, mGraph);
            GraphChanged();
        }
        
        void AddSource(const StropPtr& strop)
        {
            mSources.push_back(strop);
        }
                
        template<typename StreamPtrType>
        void AddEdgeToGraph(const StropPtr& source, 
                            const StropPtr& target,
                            const StreamPtrType& stream)
        {
            StreamPtr stream_ptr(stream);
            Graph::edge_descriptor desc = boost::add_edge(source->GetDescriptor(), target->GetDescriptor(), stream_ptr, mGraph).first;
            if (stream->IsValid())
                mSources.push_back(target);
            GraphChanged();
        }
        
        
        template<typename T>
        void Output(const BoostGraph::vertex_descriptor& source, const T& value)
        {
            BoostGraph::out_edge_iterator it, it_end;
            
            for (boost::tie(it,it_end) = boost::out_edges(source, mGraph); it != it_end; ++it)
            {
                BoostGraph::edge_descriptor edge(*it);
                
                // Put the data on the edge's stream 
                StreamPtr stream(mGraph[edge]);
                static_cast<Stream<T>*>(stream.get())->Append(value);
                
                ActivateVertex(boost::target(edge, mGraph));
            }
            Work();
        }

        
        void Work()
        {
            if (mWorking)
                return;
            mWorking = true;
            
            
            // std::cout << "Work called. mQueue.size() = " << mQueue.size() << std::endl;
            while (!mQueue.empty())
            {
                std::set<QueueEntry>::iterator it = mQueue.begin();
                mCurrentTime = std::max(mCurrentTime,it->mTime);
                
                StropPtr& strop(it->mStrop);
                strop->Work();
                strop->SetIsActive(false);
                mQueue.erase(it);
            }
            mWorking = false;
        }
        
        inline bool IsVerbose()
        {
            return mVerbose;
        }

        template<typename Expr>
        struct ExpressionResultType
        {
            // Get the type of the strop created by the expression
            using result_strop_type = typename std::result_of<smls_grammar(const Expr&, Engine*)>::type;
            // Extract the output type
            using type = strop_return_type<remove_reference_t<result_strop_type>>;
        };

        template<typename Expr>
        const typename Subscription<typename ExpressionResultType<Expr>::type>::type
        Subscribe(const Expr &expr)
        {
            using R = typename ExpressionResultType<Expr>::type;
                        
            if (IsVerbose())
                boost::proto::display_expr(expr);
            
            // Make sure the expression conforms to our grammar
            BOOST_MPL_ASSERT(( boost::proto::matches<Expr, smls_grammar> ));
            
            // add the expression to the graph
            using ResultStropType = typename Subscription<R>::strop_type;
            ResultStropType result = smls_grammar()(expr, this);
                     
            result->AddExternalReference();
            
            StartEngine();
            
            // using strop_type = const std::shared_ptr<StropStreamProducer<T> >;
            // const streamulus_expr<typename boost::proto::terminal<strop_type>::type>
            
            typename Subscription<R>::terminal_type terminal = {result};
            return terminal;
        }
        
        template<typename T>
        void UnSubscribe(const typename Subscription<T>::type& subscription)
        {
            using StropType = typename Subscription<T>::strop_type;
            const StropType& strop(boost::proto::value(subscription));
            strop->RemoveExternalReference();
            RemoveVertexFromGraph(strop);
            GarbageCollect();
        }
        
    private:
                
        void StartEngine()
        {
#if defined(WRITE_GRAPH)
            WriteGraph("TsGraph.vis");
#endif
            ActivateSources();
            Work();            
        }
        
        void ActivateVertex(const BoostGraph::vertex_descriptor& vertex)
        {
            ActivateVertex(mGraph[vertex]);
        }
        
        void ActivateVertex(StropPtr& strop)
        {
            assert(strop->GetEngine());
            
            if (strop->IsActive() || strop->IsDeleted())
                return;

            mQueue.insert(QueueEntry(mCurrentTime++, strop->GetTopSortIndex(), strop));
            strop->SetIsActive(true);
        }
        
        void ActivateSources()
        {
            if (IsVerbose())
                std::cout << "Activate sources: mSources.size() = " << mSources.size() << std::endl;
            for (std::vector<StropPtr>::iterator it = mSources.begin(); it != mSources.end(); ++it)
                ActivateVertex(*it);
            mSources.clear();
        }        

        bool IsRemoveable(const StropPtr& strop)
        {
            // A node is removeable if it does not have an external reference or an 
            // unremoved successor
            
            if (strop->HasExternalReference())
                return false;
            
            BoostGraph::out_edge_iterator it, it_end;
            
            for (boost::tie(it,it_end) = boost::out_edges(strop->GetDescriptor(), mGraph); it != it_end; ++it)
            {
                BoostGraph::edge_descriptor edge(*it);                
                if (! mGraph[boost::target(edge, mGraph)]->IsDeleted())
                    return false;
            }
            
            return true;
        }
        
        void RemoveVertexFromGraph(const StropPtr& strop)
        {
            if (!IsRemoveable(strop))
                return;
            
            strop->MarkAsDeleted();
            mNumRemovedVertices++;
            BoostGraph::in_edge_iterator it, it_end;
            
            for (boost::tie(it,it_end) = boost::in_edges(strop->GetDescriptor(), mGraph); it != it_end; ++it)
            {
                BoostGraph::edge_descriptor edge(*it);
                const StropPtr& pred(mGraph[boost::source(edge, mGraph)]);
                RemoveVertexFromGraph(pred);
            }
        }
        
        void GarbageCollect()
        {
            if ( 2*mNumRemovedVertices < boost::num_vertices(mGraph) )
                return;
            
            DoGarbageCollection();
            mNumRemovedVertices=0;
        }

        void DoGarbageCollection()
        {
            Graph new_graph;
            
            std::map<BoostGraph::vertex_descriptor, BoostGraph::vertex_descriptor> descriptors_old2new;
            
            // copy live nodes
            BoostGraph::vertex_iterator vit, vit_end;            
            for (boost::tie(vit,vit_end) = boost::vertices(mGraph); vit != vit_end; ++vit)
            {
                const StropPtr& strop(mGraph[*vit]);
                if (! strop->IsDeleted())
                    descriptors_old2new[strop->GetDescriptor()] = boost::add_vertex(strop, new_graph);
            }
            
            // copy live edges
            BoostGraph::edge_iterator eit, eit_end;            
            for (boost::tie(eit,eit_end) = boost::edges(mGraph); eit != eit_end; ++eit)
            {
                BoostGraph::edge_descriptor edge(*eit);
                const StropPtr& source(mGraph[boost::source(edge, mGraph)]);
                const StropPtr& target(mGraph[boost::target(edge, mGraph)]);
                
                if (! (source->IsDeleted() || target->IsDeleted()))
                {
                    boost::add_edge(descriptors_old2new[source->GetDescriptor()], 
                                    descriptors_old2new[target->GetDescriptor()], 
                                    mGraph[*eit], 
                                    new_graph);                    
                }
            }
            
            mGraph.swap(new_graph);
            GraphChanged();
        }
        
        struct TopologicalSortVisitor : public boost::default_dfs_visitor
        {  // Reset descriptors and recompute topological order labels in the graph:
        public:
            TopologicalSortVisitor(Graph & g, Engine* engine)
            : mIndex(boost::num_vertices(g))
            , mGraph(g)
            , mEngine(engine)
            {
            }
            
            void finish_vertex(BoostGraph::vertex_descriptor u, const BoostGraph & g) 
            {
                assert( mIndex > 0);
                mGraph[u]->SetGraph(mEngine, u, mIndex--);
                
                if (mEngine->IsVerbose())
                {
                    std::cout << "Finish vertex " << u << "  " //  << mGraph[u]->GetName()
                              << "  TO=" << mGraph[u]->GetTopSortIndex() << std::endl;
                }
            }
        private:
            size_t mIndex;
            Graph& mGraph;
            Engine* mEngine;
        };
        
        void GraphChanged()
        {
            TopologicalSortVisitor vis(mGraph, this);
            boost::depth_first_search(mGraph, boost::visitor(vis));
        }


        using TimestampT = long;

        struct QueueEntry
        {
            QueueEntry(TimestampT time,
                       size_t top_sort_index, 
                       StropPtr& strop)
            : mTime(time)
            , mTopSortIndex(top_sort_index)
            , mStrop(strop)
            {
            }
            
            bool operator<(const QueueEntry& rhs ) const
            {
                // sort by (time, topological order)
                return (this->mTime<rhs.mTime) || 
                    (this->mTime==rhs.mTime && this->mTopSortIndex<rhs.mTopSortIndex);
            }
            
            TimestampT mTime;
            size_t     mTopSortIndex;
            StropPtr&  mStrop;
        };
    
        
        class VertexPropertyWriter
        {
        public:
            VertexPropertyWriter(Graph& g) : mGraph(g) {}
            
            void operator()(std::ostream& out, const Graph::vertex_descriptor& v) const
            {
                out << "[label=" << mGraph[v]->DisplayName() 
                    << "_Desc" << mGraph[v]->GetDescriptor() 
                    << "_TO" << mGraph[v]->GetTopSortIndex() << "]";
            }
            
        private:
            Graph& mGraph;
        };
        
        void WriteGraph(const std::string& filename)
        {
            if (IsVerbose())
                std::cout << "Writing graph to " << filename << std::endl;
            std::ofstream file;
            file.open (filename.c_str());
            boost::write_graphviz(file, mGraph, VertexPropertyWriter(mGraph));
            file.close();
        }

        Graph mGraph;
        std::set<QueueEntry> mQueue;
        bool mWorking;    
        TimestampT mCurrentTime;
        std::vector<StropPtr> mSources;
        bool mVerbose;
        unsigned int mNumRemovedVertices;

    };
        
    
} // ns streamulus
