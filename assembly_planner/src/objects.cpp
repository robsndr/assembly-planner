#include "assembly_planner/objects.h"

Pose::Pose() {

}

Pose::Pose(std::vector<std::vector<double> > T) {
	this->T = T;
}

Pose::~Pose() {

}

std::vector<std::vector<double> > Pose::getPose() {
	return this->T;
}

std::vector<std::vector<double> > Pose::getRotation() {
	std::vector<std::vector<double> > R(3);

	for (unsigned i = 0; i<3; i++) {
		R[i].resize(3);
		for (unsigned j = 0; j<3; j++) {
			R[i][j] = T[i][j];
		}
	}
	return R;
}

std::vector<double> Pose::getPosition() {
	std::vector<double> x(3);
	x[0] = this->T[0][3];
	x[1] = this->T[1][3];
	x[2] = this->T[2][3];
	return x;
}

Object_Phys::Object_Phys()
{

}

Object_Phys::~Object_Phys()
{

}

Object_Assembly::Object_Assembly() {

    this->_T=bnu::identity_matrix<double>(4);
    this->_T_assembly=bnu::identity_matrix<double>(4);

}

Object_Assembly::~Object_Assembly() {

}

bnu::matrix<double> Object_Assembly::getPose_Assembly()
{
    return this->_T_assembly;
}

double Object_Type_Action::getCost(std::string type_worker){
    return 0;
}

template<>
Object_Map<Object_Assembly>::Object_Map(){

}

template<>
Object_Map<Object_Worker>::Object_Map(){

}

template<>
Object_Set<std::string>::Object_Set(){

}
