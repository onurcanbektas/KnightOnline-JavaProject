package com.knightonline.shared.utils;

import java.util.HashMap;
import java.util.Map;
import java.util.regex.Pattern;

import javax.annotation.PostConstruct;

import org.springframework.stereotype.Component;

/**
 * @author Mamaorha
 *
 */
@Component
public class RegexValidator
{
	public enum Validator
	{
		USERNAME, PASSWORD
	}
	
	private Map<Validator, Pattern> patterns;
	
	@PostConstruct
	private void init()
	{
		patterns = new HashMap<>();
		
		patterns.put(Validator.USERNAME, Pattern.compile("[a-zA-Z0-9\\._\\-]+"));
		patterns.put(Validator.PASSWORD, Pattern.compile("[a-zA-Z0-9\\._\\-]+"));
	}
	
	public boolean isValid(Validator validator, String value)
	{
		if(null == value)
		{
			return false;
		}
		return patterns.get(validator).matcher(value).matches();
	}
}
