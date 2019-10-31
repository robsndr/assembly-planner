#include <iostream>
#include <set>
#include <vector>
#include "graph.hpp"

/* Class representing the AOStar Search. Used as a functor - enables to store state.
**/
class AOStarSearch{
public:
    AOStarSearch(/* args */);
    ~AOStarSearch();

    bool operator()(Graph<> *, Node* );

private:
    
    std::vector< Node* > marked_;

};

AOStarSearch::AOStarSearch(/* args */)
{
}

AOStarSearch::~AOStarSearch()
{
} 



// template<typename edgeData, typename nodeData >
// bool AOStarSearch::getMinimalCostSuccessor()(Node<nodeData, edgeData> * start ){


//     return true;
// }


/* Functor which triggers the AO* search.
    @graph: graph object to perform the search on.
    @start: root node to begin the search at.
    \return: true if successfull.
**/
bool AOStarSearch::operator()(Graph<> * graph, Node * start ){

    Node * current_node = start;
    start->data_.marked = true;
    // Start node is terminating node and sole solution.
    if(start->numberOfSuccessors() == 0){
        start->data_.solved = true;
    }
    marked_.push_back(start);

    // If root has been solved - terminate.
    while(start->data_.solved == false){

        current_node = marked_.pop_back();

        while(current_node && current_node->data_.marked){
            if(current_node->numberOfSuccessors() == 0){
                start->data_.solved = true;
                return true;
            }

            int cost = INT_MAX;

            for(std::size_t i=0; i<current_node->numberOfSuccessors(); i++){
                std::vector<Node*> ans = current_node->getSuccessorNodes()[i]->getSuccessorNodes();

                int temp_cost=0;
                for(std::size_t j=0; j<ans.size(); j++){
                    Node* n= ans[j];
                    temp_cost+=n->data;
                }
                if(temp_cost<cost)
                {
                    min_ans=ans;
                    cost=temp_cost;
                }
            }
            vector<node*> min_ans_v=*min_ans;
            current_node=NULL;
            for(unsigned int j=0;j<min_ans_v.size();j++)
            {
                if(min_ans_v[j]->mark)
                {
                    current_node=min_ans_v[j];
                    break;
                }
            }

        }

        vector<node*> min_ans_v=*min_ans;
        for(unsigned int j=0;j<min_ans_v.size();j++)
        {
            node* n=min_ans_v[j];
            cout<<"Exploring :"<<n->data<<endl;
            int final_cost=INT_MAX;
            if(n->v.size()==0)
            {
                n->mark=true;
            }
            else{
            for(unsigned int i=0;i<n->v.size();i++)
            {
                vector<node*>*ans=(n->v)[i];
                vector<node*> ans_v=*ans;
                int temp_cost=0;
                for(unsigned int j=0;j<(ans_v.size());j++)
                {
                    node* n=ans_v[j];
                    temp_cost+=n->data;
                    temp_cost+=edge_cost;
                }
                if(temp_cost<final_cost)
                {
                    final_cost=temp_cost;
                }
            }
            n->data=final_cost;
            n->mark=true;
            }
            cout<<"Marked : "<<n->data<<endl;
        }

    for(int i=0;i<20;i++) cout<<"=";
    cout<<endl;
        while(!st.empty())
        {
            node* n=st.top();
            cout<<n->data<<" ";
            st.pop();
            int final_cost=INT_MAX;
            for(unsigned int i=0;i<n->v.size();i++)
            {
                vector<node*>*ans=(n->v)[i];
                vector<node*> ans_v=*ans;
                int temp_cost=0;
                for(unsigned int j=0;j<(ans_v.size());j++)
                {
                    node* n=ans_v[j];
                    temp_cost+=n->data;
                    temp_cost+=edge_cost;
                }
                if(temp_cost<final_cost)
                {
                    min_ans=ans;
                    final_cost=temp_cost;
                }
            }
            n->data=final_cost;
        }
        cout<<endl;
        current_node=root;

    }
}





    }

    return true;
}
