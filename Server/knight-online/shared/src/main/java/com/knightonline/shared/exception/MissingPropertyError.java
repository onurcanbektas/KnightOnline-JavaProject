package com.knightonline.shared.exception;

/**
 * @author Mamaorha
 *
 */
public class MissingPropertyError extends InternalServerError
{	
	private static final long serialVersionUID = -5748564520785921368L;

	public MissingPropertyError(String propertyKey)
	{
		super(String.format("Missing property [{%s}]", propertyKey));
	}
}
