#pragma once

#include <map>

class Object {
public:
	Object() {

	}

	Object(std::string id) {
		this->_id = id;
		this->_posX = 0;
		this->_posY = 0;
		this->_posZ = 0;
		this->_rpyA = 0;
		this->_rpyB = 0;
		this->_rpyC = 0;
	}

	std::string _id;

	double _posX;
	double _posY;
	double _posZ;
	double _rpyA;
	double _rpyB;
	double _rpyC;

};


class Database
{
public:
	Database();
	~Database();


	std::map<std::string,Object> _objects;

};

