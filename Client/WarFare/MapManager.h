#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H
#include <map>

template<class T, class V>
class MapManager
{
private:
	std::map<T, V> values;
public:
	void put(T key, V value)
	{
		values[key] = value;
	}

	V get(T key)
	{
		return values[key];
	}

	~MapManager()
	{
		std::map<T, V>::iterator it;

		for (it = values.begin(); it != values.end(); it++)
		{
			delete it->second;
		}
	}
};
#endif