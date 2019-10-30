#ifndef VISITOR_HPP
#define VISITOR_HPP

#include <cstddef>
#include <iostream>

/// Visitors can be used to follow the indices of vertices and edges.
///
/// These indices change due to the insetion and removal of vertices and edges.
///
template<class S = std::size_t>
struct IdleGraphVisitor {
    typedef S size_type;

    IdleGraphVisitor() {}
    void insertVertex(const size_type a) const {}
    void insertVertices(const size_type a, const size_type n) const {}
    void eraseVertex(const size_type a) const {}
    void insertEdge(const size_type a) const {}
    void insertEdges(const size_type a) const {}
    void eraseEdge(const size_type a) const {}
};

/// Visitors can be used to follow the indices of vertices and edges.
///
/// These indices change due to the insetion and removal of vertices and edges.
///
template<class S = std::size_t>
struct VerboseGraphVisitor {
    typedef S size_type;

    VerboseGraphVisitor() {}
    void insertNode(const size_type a) const
        { std::cout << "inserting vertex " << a << std::endl; }
    void insertNodes(const size_type a, const size_t n) const
        { std::cout << "inserting " << n << " vertices, starting from index " << a << std::endl; }
    void eraseNode(const size_type a) const
        { std::cout << "removing vertex " << a << std::endl; }
    void insertEdge(const size_type a) const
        { std::cout << "inserting edge " << a << std::endl; }
    void insertEdges(const size_type a) const
        { std::cout << "inserting edge " << a << std::endl; }
    void eraseEdge(const size_type a) const
        { std::cout << "removing edge " << a << std::endl; }
};

#endif // #ifndef VISITOR_HPP