#include <string>
#include <cmath>

enum class NodeType{AND, OR};

class NodeData{
public:
    double cost = 0;
    NodeType type;

    std::string name = "";
    std::string worker = "";

    bool expanded = false;

    bool marked = false;
    bool solved = false;;
    bool solution = false;

    void reset();
};

void NodeData::reset(){
    // marked = false;
    if(type == NodeType::OR)
        cost = log2(name.length());

    solved = false;
    solution = false;
}

typedef std::size_t EdgeData;
