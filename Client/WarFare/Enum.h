#ifndef ENUM_H
#define ENUM_H

#include<iostream>
#include <map>

template<class T>
class Enum
{
private:
	char * name = 0;
	T value;

	Enum(const Enum & other);

	void setName(char * name)
	{
		if (this->name != 0)
		{
			delete[] this->name;
		}

		this->name = 0;

		if (name == 0)
		{
			return;
		}

		int len = strlen(name);
		this->name = new char[len + 1];

		for (int i = 0; i < len; i++)
		{
			this->name[i] = name[i];
		}

		this->name[len] = '\0';
	}

protected:
	std::map<T, Enum *> values;

	Enum()
	{
		name = 0;
	}

	Enum(char * name, T value)
	{
		this->setName(name);
		this->value = value;
	}

	Enum * forValueT(T value)
	{
		return values[value];
	}

public:
	virtual ~Enum()
	{
		if (name != 0)
		{
			delete[] name;
			this->name = 0;
		}
	}

	char * getName() const
	{
		return name;
	}

	T getValue() const
	{
		return value;
	}

	virtual bool Enum::operator ==(const Enum & other) const
	{
		int lenA = strlen(name);
		int lenB = strlen(other.name);

		if (lenA != lenB)
		{
			return false;
		}

		for (int i = 0; i < lenA; i++)
		{
			if (name[i] != other.name[i])
			{
				return false;
			}
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