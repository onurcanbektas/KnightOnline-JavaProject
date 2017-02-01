package com.knightonline.shared.data.common;

import java.io.Serializable;

/**
 * @author Mamaorha
 *
 */
public class DynamicAttribute implements Serializable
{
	private static final long serialVersionUID = -8204189956662611722L;

	protected Object key;
	protected Object value;

	public DynamicAttribute()
	{
		
	}
	
	public DynamicAttribute(Object key, Object value)
	{
		this.key = key;
		this.value = value;
	}
	
	public Object getKey()
	{
		return key;
	}

	public void setKey(Object key)
	{
		this.key = key;
	}

	public Object getValue()
	{
		return value;
	}

	public void setValue(Object value)
	{
		this.value = value;
	}

	@Override
	public int hashCode()
	{
		final int prime = 31;
		int result = 1;
		result = prime * result + ((key == null) ? 0 : key.hashCode());
		result = prime * result + ((value == null) ? 0 : value.hashCode());
		return result;
	}

	@Override
	public boolean equals(Object obj)
	{
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		DynamicAttribute other = (DynamicAttribute) obj;
		if (key == null)
		{
			if (other.key != null)
				return false;
		}
		else if (!key.equals(other.key))
			return false;
		if (value == null)
		{
			if (other.value != null)
				return false;
		}
		else if (!value.equals(other.value))
			return false;
		return true;
	}

	@Override
	public String toString()
	{
		return "DynamicAttribute [key=" + key + ", value=" + value + "]";
	}

	
}
