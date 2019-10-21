#pragma once

#include <vector>
#include <set>
#include <string>
#include <map>

#include "assembly_planner/memory.h"


struct Pose {
	Pose();
	Pose(std::vector<std::vector<double> > T);
	~Pose();

	std::vector<double> getPosition();
	std::vector<std::vector<double> > getRotation();
	std::vector<std::vector<double> > getPose();

	std::vector<std::vector<double> > T;


};

template<class T> class Object_Set : public Object_Abstract {
public:
    Object_Set();
    ~Object_Set();

    std::set<T> _set;

};

class Object_CostMap : public Object_Abstract {
public:
    Object_CostMap(){}
    ~Object_CostMap(){}

	std::map<std::pair<std::string, std::string>, double> _costmap;
};

template<class T>class Object_Map : public Object_Abstract {
public:
    Object_Map();
    ~Object_Map();

    std::map<std::string, T> _map;
};

template <>
class Object_Map<std::set<std::string> >{

};

class Object_Type_Worker : public Object_Abstract {
public:
	Object_Type_Worker();
	~Object_Type_Worker();
};

class Object_Type_Action : public Object_Abstract {
public:
    Object_Type_Action(std::map<std::string,double> cost);
    ~Object_Type_Action();

    double getCost(std::string type_worker);

private:
    std::map<std::string, double> _cost;
};

class Object_Phys : public Object_Abstract
{
public:
	Object_Phys();
	~Object_Phys();

    bnu::matrix<double> _T;


};

class Object_Assembly : public Object_Phys {
public:
    Object_Assembly();
    Object_Assembly(std::set<std::string> parts,bnu::matrix<double> T,bnu::matrix<double> T_assembly){
        this->_T=T;
        this->_T_assembly=T_assembly;
        this->_parts=parts;
    }

    ~Object_Assembly();

    bnu::matrix<double> getPose_Assembly();

    std::string getAssemblyName() const {
        std::string assembly;
        for (std::string part : this->_parts) {
            assembly.append(part);
            assembly.append("_");
        }
        assembly.pop_back();
        return assembly;
    }

    const std::set<std::string>* getPartNames() {
        return &this->_parts;
    }

    bool operator < (const Object_Assembly& a) const {
        return this->getAssemblyName()<a.getAssemblyName();
    }

private:

    bnu::matrix<double> _T_assembly;
    std::set<std::string> _parts;
};

class Object_Worker : public Object_Phys {
public:
	Object_Worker() {}
    Object_Worker(std::string id,double x1,double x2,double y1,double y2) {
        this->_id=id;
        this->_workspace_x1=x1;
        this->_workspace_x2=x2;
        this->_workspace_y1=y1;
        this->_workspace_y2=y2;
    }
	~Object_Worker() {}

	bool isReachable(Object_Assembly* assembly) {
        if (assembly->_T(0,3)<this->_workspace_x1 || assembly->_T(0,3)>this->_workspace_x2) {
			return false;
		}
        if (assembly->_T(1,3)<this->_workspace_y1 || assembly->_T(1,3)>this->_workspace_y2) {
			return false;
		}
		return true;
	}

	bool operator < (const Object_Worker& w) const {
		return this->_id<w.getId();
	}

    void setSkills(std::set<std::string> skills){
        this->_skills=skills;
    }

    bool isCapable(std::set<std::string> skills){
        for(std::string s : skills){
            if(this->_skills.find(s)!=this->_skills.end()){
                return false;
            }
        }
        return true;
    }

private:

	double _workspace_x1;
	double _workspace_x2;
	double _workspace_y1;
	double _workspace_y2;

    std::set<std::string> _skills;

};




