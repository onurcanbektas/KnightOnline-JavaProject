package com.knightonline.shared.data.common;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

/**
 * @author Mamaorha
 *
 */
public class TwoSidesMap <K, V>
{
	protected Map<K, V> keys;
	protected Map<V, K> values;
	
	public TwoSidesMap()
	{
		keys = new ConcurrentHashMap<>();
		values = new ConcurrentHashMap<>();
	}
	
	public boolean put(K key, V value)
	{
		if(!keys.containsKey(key) && !values.containsKey(value))
		{
			keys.put(key, value);
			values.put(value, key);
			
			return true;
		}
		
		return false;
	}
	
	public V getByKey(K key)
	{
		return keys.get(key);
	}
	
	public K getByValue(V value)
	{
		return values.get(value);
	}
	
	public int size()
	{
		return keys.size();
	}
	
	public void removeByKey(K key)
	{
		if(keys.containsKey(key))
		{
			values.remove(keys.get(key));
			keys.remove(key);
		}
	}
	
	public void removeByValue(V value)
	{
		if(values.containsKey(value))
		{
			keys.remove(values.get(value));
			values.remove(value);
		}
	}
}
