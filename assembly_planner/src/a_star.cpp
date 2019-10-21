#include "assembly_planner/a_star.h"

namespace algorithms{

namespace search{

AStar_node::AStar_node()
{

}


AStar_node::~AStar_node()
{

}

double AStar_node::getPathCost() const{
    return this->_g;
}

void AStar_node::setPathCost(double g){
    this->_g=g;
}

double AStar_node::getTotalCost() const{
    return this->_f;
}

void AStar_node::setTotalCost(double f){
    this->_f=f;
}

double AStar_node::getHeuristicCost() const{
    return this->_h;
}

void AStar_node::setHeuristicCost(double h){
    this->_h=h;
}

AStar_node* AStar_node::getPredecessor(){
    return this->_predecessor;
}

void AStar_node::setPredecessor(AStar_node *predecessor){
    this->_predecessor=predecessor;
}

AStar::AStar()
{

}

AStar::~AStar()
{

}

AStar_node *AStar::search(AStar_node *startNode, AStar_node *goalNode){


    this->_openMap.clear();
    this->_openList.clear();
    this->_closedList.clear();

    startNode->setPredecessor(NULL);
    startNode->setPathCost(0);
    startNode->setHeuristicCost(startNode->heuristic(goalNode));
    startNode->setTotalCost(startNode->getHeuristicCost()+startNode->getPathCost());

    boost::heap::fibonacci_heap<AStar_node*,boost::heap::compare<compare_openList> >::handle_type handle=this->_openList.push(startNode);
    this->_openMap.insert(std::pair<std::string,boost::heap::fibonacci_heap<AStar_node*,boost::heap::compare<compare_openList> >::handle_type >(startNode->getUniqueString(),handle));

    double tentative_g_score=0;
    AStar_node* currentNode;


//    int t_start=utilities::time::getMilliCount();


    while(!this->_openList.empty()){

        currentNode=this->_openList.top();

        // goal check
        if(currentNode->equalsNode(goalNode)){
//            this->writeReport(utilities::time::getMilliSpan(t_start));
            return currentNode;
        }
        this->_openList.pop();
        this->_openMap.erase(currentNode->getUniqueString());
        this->_closedList.insert(currentNode->getUniqueString());

        std::list<AStar_node*> neighbors = currentNode->getNeighbors();

        for(AStar_node* neighbor : neighbors){

            if(this->_closedList.find(neighbor->getUniqueString())!=this->_closedList.end()){
                continue;
            }
            neighbor->setPredecessor(currentNode);
            tentative_g_score=currentNode->getPathCost()+neighbor->getActionCost(currentNode);

            std::unordered_map<std::string,boost::heap::fibonacci_heap<AStar_node*,boost::heap::compare<compare_openList> >::handle_type>::iterator it;
            it=this->_openMap.find(neighbor->getUniqueString());
            if(it!=this->_openMap.end()){
                if(tentative_g_score>=(*(it->second))->getPathCost()){
                    continue;
                }
            }

            double f=tentative_g_score+neighbor->heuristic(goalNode);
            std::cout<<"f: "<<f<<std::endl;
            neighbor->heuristic(goalNode);
            neighbor->setPathCost(tentative_g_score);

            neighbor->setTotalCost(f);

            it=this->_openMap.find(neighbor->getUniqueString());
            if(it!=this->_openMap.end()){
                this->_openList.update(it->second,neighbor);
            }else{
                boost::heap::fibonacci_heap<AStar_node*,boost::heap::compare<compare_openList> >::handle_type handle=this->_openList.push(neighbor);
                this->_openMap.insert(std::pair<std::string,boost::heap::fibonacci_heap<AStar_node*,boost::heap::compare<compare_openList> >::handle_type >(neighbor->getUniqueString(),handle));
            }
        }
    }
//    this->writeReport(utilities::time::getMilliSpan(t_start));

    return NULL;
}

void AStar::writeReport(int t_elapsed){

    std::cout<<"-----------------------------"<<std::endl;
    std::cout<<"A star report"<<std::endl;
    std::cout<<"-----------------------------"<<std::endl;
    std::cout<<"Number of expanded nodes: "<<this->_closedList.size()<<std::endl;
    std::cout<<"Number of nodes still in open list: "<<this->_openList.size()<<std::endl;
    std::cout<<"Number of nodes in closed list: "<<this->_closedList.size()<<std::endl;
    std::cout<<"Time elapsed: "<<t_elapsed<<std::endl;
    std::cout<<"End of report."<<std::endl;
    std::cout<<"-----------------------------"<<std::endl;

}



}
}
