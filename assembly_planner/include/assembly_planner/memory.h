#pragma once
#include <iostream>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/assignment.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>

#include <boost/unordered_map.hpp>

namespace bnu=boost::numeric::ublas;


void eulerRPY2Mat(bnu::matrix<double> &T, double gamma, double beta, double alpha, double x, double y, double z);


class Object_Abstract
{
public:
    Object_Abstract();
    ~Object_Abstract();

    virtual void setId(std::string id);
    virtual std::string getId() const;

protected:

    std::string _id;

    bool operator < (const Object_Abstract& o) const {
        return this->_id < o.getId();
    }

};

class Memory
{
public:
	Memory();
	~Memory();

	void insertMemory(std::string memory, Object_Abstract* object);
	Object_Abstract* getMemory(std::string memory);

	boost::unordered_map<std::string, Object_Abstract*> memory_abstract;
	boost::unordered_map<std::string, Object_Abstract*> memory_factual;
private:


};

