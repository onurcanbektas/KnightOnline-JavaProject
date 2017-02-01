#ifndef ENUM_H
#define ENUM_H

#include<iostream>
#include <string>

template<class T>
class Enum
{
private:
	std::string name;
	T value;

	Enum(const Enum & other);

	void setName(std::string name)
	{
		this->name = name;
	}

protected:
	Enum(std::string name, T value)
	{
		this->setName(name);
		this->value = value;
	}

public:
	std::string getName() const
	{
		return name;
	}

	T getValue() const
	{
		return value;
	}

	virtual bool Enum::operator ==(const Enum & other) const
	{
		return this == &other;
	}

	virtual bool Enum::operator !=(const Enum & other) const
	{
		return this != &other;
	}
};
#endif