package com.knightonline.shared.exception;

/**
 * @author Mamaorha
 *
 */
public class InternalServerError extends RuntimeException
{
	private static final long serialVersionUID = -6812227476975608510L;
	
	public InternalServerError(String message)
	{
		super(message);
	}

	public InternalServerError(Exception e)
	{
		super(e);
	}
	
	public InternalServerError(String message, Exception e)
	{
		super(message, e);
	}
}
