package com.knightonline.shared.helper;

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
		USERNAME, PASSWORD, CHARACTER_NAME
	}
	
	private Map<Validator, Pattern> patterns;
	
	@PostConstruct
	private void init()
	{
		patterns = new HashMap<>();
		
		patterns.put(Validator.USERNAME, Pattern.compile("[a-zA-Z0-9\\._\\-]+"));
		patterns.put(Validator.PASSWORD, Pattern.compile("[a-zA-Z0-9\\._\\-]+"));
		patterns.put(Validator.CHARACTER_NAME, Pattern.compile("[a-zA-Z0-9]+"));
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
