package com.knightonline.shared.utils;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.annotation.PostConstruct;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.shared.exception.InternalServerError;
import com.knightonline.shared.exception.MissingPropertyError;
import com.knightonline.shared.persistence.dao.IApplicationPropertiesDAO;
import com.knightonline.shared.persistence.entities.ApplicationProperties;

/**
 * @author Mamaorha
 *
 */
@Component
public class ApplicationPropertiesManager
{
	@Autowired
	protected IApplicationPropertiesDAO applicationPropertiesDAO;
	
	protected Map<String, String> properties;
	
	@PostConstruct
	private void init()
	{
		properties = new HashMap<>();
		
		try
		{
			List<ApplicationProperties> serverProperties = applicationPropertiesDAO.getProperties();
			
			for (ApplicationProperties applicationProperties : serverProperties)
			{
				properties.put(applicationProperties.getKey(), applicationProperties.getValue());
			}
		}
		
		catch (Exception e) 
		{
			throw new InternalServerError(e);
		}
	}
	
	public String getValue(String key, String defaultValue)
	{
		String res = properties.get(key);
		
		if(null == res)
		{
			res = defaultValue;
		}
		
		if(null == res)
		{
			throw new MissingPropertyError(key);	
		}
		
		return res;
	}
	
	public Integer getIntValue(String key, Integer defaultValue)
	{
		return Integer.valueOf(getValue(key, convertToString(defaultValue)));
	}
	
	private String convertToString(Object obj)
	{
		if(null == obj)
		{
			return null;
		}
		
		return String.valueOf(obj);
	}
}
