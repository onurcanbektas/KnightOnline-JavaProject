#ifndef ENUM_H
#define ENUM_H

#include<iostream>
#include <string>
#include <map>

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
	std::map<T, Enum *> values;

	Enum()
	{
	}

	Enum(std::string name, T value)
	{
		this->setName(name);
		this->value = value;
	}

	Enum * forValueT(T value)
	{
		if (values[value] == nullptr)
		{
			throw "invalid value enum value";
		}

		return values[value];
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
		if(name != other.name)
		{
			return false;
		}

		//we shouldn't reach here, i made this to test myself
		if (this != &other)
		{
			return false;
		}
		return true;
	}

	virtual bool Enum::operator !=(const Enum & other) const
	{
		return !(*this == other);
	}
};
#endif