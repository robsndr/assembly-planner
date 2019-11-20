#include "node.hpp"

namespace helper{

    bool isLeafNode(Node* temp){
        if(temp->numberOfSuccessors())
            return true;
        else
            return false;
    }

}
 