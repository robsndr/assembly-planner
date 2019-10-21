#include "assembly_planner/astar_node.h"

namespace utilities_planning{

namespace search{

AStar_node::AStar_node()
{

}


AStar_node::~AStar_node()
{

}

double AStar_node::getPathCost() const{
    return this->g;
}

void AStar_node::setPathCost(double g){
    this->g=g;
}

double AStar_node::getTotalCost() const{
    return this->f;
}

void AStar_node::setTotalCost(double f){
    this->f=f;
}

double AStar_node::getHeuristicCost() const{
    return this->h;
}

void AStar_node::setHeuristicCost(double h){
    this->h=h;
}

AStar_node* AStar_node::getPredecessor(){
    return this->predecessor;
}

void AStar_node::setPredecessor(AStar_node *predecessor){
    this->predecessor=predecessor;
}

}
}
