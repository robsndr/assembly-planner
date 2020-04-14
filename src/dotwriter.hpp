#pragma once

#include <fstream>

#include "node.hpp"
#include "types.hpp"
#include "graph.hpp"

// Implements the writing of .dot files for a given graph
struct DotWriter
{    
    template <typename N>
    static void write(Graph<N,EdgeData>& graph, std::string path)
    {
        std::fstream fs;
        fs.open(path, std::fstream::out);
        fs << "digraph G {" << std::endl;
        for (auto x : graph.nodes())
        {
            writeNodeId(fs,x);
        }

        for (auto x : graph.nodes())
        {
            writeNode(fs, graph, x);
        }
        fs << "}";
        fs.close();
    }

  private:

    template <typename N>
    static void writeNode(std::fstream& fs, Graph<N,EdgeData>& graph, Node<N>* node)
    {
        fs << "  " << node->id << " -> " << "{";
        std::vector<Node<N>*> temp = graph.getSuccessorNodes(node->id);
        for (std::size_t i = 0; i < temp.size(); i++)
        {
            if (i != temp.size() - 1)
            {
                fs << temp[i]->id
                << ", ";
            }
            else
            {
                fs << temp[i]->id;
            }
        }
        fs << "}" << std::endl;
    }

    template <typename N>
    static void writeNodeId(std::fstream& fs, Node<N>* node)
    {
        fs << "  " << node->id;
        switch (node->data.type)
        {
            case NodeType::SUBASSEMBLY:
                fs << " [label=\""<< node->data.name
                   << "\" shape=\"rectangle\"";
                break;
            case NodeType::INTERASSEMBLY:
                fs << " [label=\""<< node->data.name 
                   << "\" shape=\"rectangle\" color=\"blue\"";
                break;
            case NodeType::INTERACTION:
                fs << " [label=\""<< node->data.name 
                   << " - " << node->data.assigned_agent 
                   << "\" color=\"blue\"";
                break;
            case NodeType::ACTION:
                fs << " [label=\""<< node->data.name 
                   << " - " << node->data.assigned_agent << "\"";
        }
        fs << "];" << std::endl;
    }
};
