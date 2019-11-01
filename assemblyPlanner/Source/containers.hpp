#include <string>

enum class NodeType{AND, OR};

class NodeData{
public:
    double cost = 0;
    NodeType type;
    std::string name = "";
    bool marked = false;
    bool solved = false;;
    int leader_index = 0;
    bool solution = false;
};

typedef std::size_t EdgeData;
