#include "assembly_planner/memory.h"


void eulerRPY2Mat(bnu::matrix<double> &T, double gamma, double beta, double alpha, double x, double y, double z){

    gamma*=boost::math::constants::pi<double>()/180.0;
    beta*=boost::math::constants::pi<double>()/180.0;
    alpha*=boost::math::constants::pi<double>()/180.0;

    bnu::matrix<double> R_z=bnu::identity_matrix<double>(3);
    bnu::matrix<double> R_y=bnu::identity_matrix<double>(3);
    bnu::matrix<double> R_x=bnu::identity_matrix<double>(3);

    T=bnu::matrix<double>(4,4);

    bnu::matrix<double> R(3,3);

    if(gamma!=0){
        R_z<<=cos(gamma),-sin(gamma),0,
                sin(gamma),cos(gamma),0,
                0,0,1;
    }
    if(beta!=0){
        R_y<<=cos(beta),0,sin(beta),
                0,1,0,
                -sin(beta),0,cos(beta);
    }
    if(alpha!=0){
        R_x<<=1,0,0,
                0,cos(alpha),-sin(alpha),
                0,sin(alpha),cos(alpha);
    }
    bnu::matrix<double> tmp=bnu::prod(R_y,R_x);
    R=bnu::prod(R_z,tmp);

    T<<=R(0,0),R(0,1),R(0,2),x,
            R(1,0),R(1,1),R(1,2),y,
            R(2,0),R(2,1),R(2,2),z,
            0,0,0,1;


}

Object_Abstract::Object_Abstract()
{
}


Object_Abstract::~Object_Abstract()
{
}

void Object_Abstract::setId(std::string id) {
    this->_id = id;
}

std::string Object_Abstract::getId() const
{
    return this->_id;
}

Memory::Memory()
{

}

Memory::~Memory()
{

}

void Memory::insertMemory(std::string memory, Object_Abstract *object) {
    this->memory_factual.insert(std::pair<std::string, Object_Abstract*>(memory, object));
}

Object_Abstract* Memory::getMemory(std::string memory) {
    return this->memory_factual[memory];
}
