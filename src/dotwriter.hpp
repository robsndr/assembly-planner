#pragma once

#include <fstream>

#include "node.hpp"
#include "types.hpp"

// Implements the writing of .dot files for a given graph
template <typename N>
struct DotWriter
{
    DotWriter(std::string name)
    {
        fs.open(name, std::fstream::out);
        fs << "digraph G {" << std::endl;
    }

    ~DotWriter()
    {
        fs.close();
    }

    void write(const std::unordered_map<std::size_t, Node<N>> & nodes)
    {
        for (auto const &x : nodes)
        {
            writeNodeId(x.second);
        }

        for (auto const &x : nodes)
        {
            writeNode(x.second);
        }
        fs << "}";
        fs.close();
    }

  private:

    void writeNode(const Node<N>& node)
    {
        fs << "  " << node.id_ << " -> "
        << "{";
        std::vector<Node<N>*> temp = node.getSuccessorNodes();
        for (std::size_t i = 0; i < node.numberOfSuccessors(); i++)
        {
            if (i != node.numberOfSuccessors() - 1)
            {
                fs << temp[i]->id_
                << ", ";
            }
            else
            {
                fs << temp[i]->id_;
            }
        }
        fs << "}" << std::endl;
    }

    void writeNodeId(const Node<N>& node)
    {
        fs << "  " << node.id << " [label=\""
            << node.data_.name << "\n"
            << "F: " << node.data_.f_score << std::endl
            << "H: " << node.data_.h_score
            << "\"];"
            << std::endl;
    }

    std::fstream fs;
};
