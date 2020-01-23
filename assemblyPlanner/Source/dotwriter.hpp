#pragma once

#include <fstream>
#include "node.hpp"

class DotWriter
{
private:
    std::fstream fs;
    void writeNode(Node *);
    void writeNodeId(Node *);

public:
    DotWriter(std::string name);
    ~DotWriter();

    void write(std::unordered_map<std::size_t, Node *> &);
};

DotWriter::DotWriter(std::string name)
{
    fs.open(name, std::fstream::out);

    fs << "digraph G {" << std::endl;
}

DotWriter::~DotWriter()
{
    fs.close();
}

void DotWriter::write(std::unordered_map<std::size_t, Node *> &nodes)
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

void DotWriter::writeNode(Node *node)
{
    fs << "  " << node->id_ << " -> "
       << "{";
    std::vector<Node *> temp = node->getSuccessorNodes();
    for (std::size_t i = 0; i < node->numberOfSuccessors(); i++)
    {
        if (i != node->numberOfSuccessors() - 1)
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

void DotWriter::writeNodeId(Node *node)
{
    fs << "  " << node->id_
       << " [label=\""
       << node->data_.name << "\n"
       << "F: " << node->data_.f_score << std::endl
       << "H: " << node->data_.h_score
       << "\"];"
       << std::endl;
}
