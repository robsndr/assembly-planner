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

    void write(const std::vector<Node<N>*> & nodes)
    {
        for (auto const &x : nodes)
        {
            writeNodeId(x);
        }

        for (auto const &x : nodes)
        {
            writeNode(x);
        }
        fs << "}";
        fs.close();
    }

  private:

    void writeNode(const Node<N>& node)
    {
        fs << "  " << node.id << " -> "
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
