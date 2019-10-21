#ifndef ASTAR_NODE_H
#define ASTAR_NODE_H

#include <list>
#include <iostream>

namespace utilities_planning{

namespace search{

class AStar_node
{
public:
    AStar_node();
    ~AStar_node();

    virtual double heuristic(AStar_node* node)=0;
    virtual bool equalsNode(AStar_node* node)=0;
    virtual std::list<AStar_node*> getNeighbors()=0;
    virtual std::string getUniqueString() = 0;
    virtual bool goalTest(AStar_node* node) = 0;
    virtual double getActionCost(AStar_node* node) = 0;
    double getPathCost() const;
    double getTotalCost() const;
    double getHeuristicCost() const;
    void setHeuristicCost(double h);
    void setPathCost(double g);
    void setTotalCost(double f);
    AStar_node *getPredecessor();
    void setPredecessor(AStar_node *predecessor);

protected:

    double g;
    double h;
    double f;

    AStar_node* predecessor;

};

struct compare_openList
{
    bool operator()(const AStar_node* n1, const AStar_node* n2) const
    {
        return n1->getTotalCost() > n2->getTotalCost();
    }
};

}
}

#endif // ASTAR_NODE_H
