package com.knightonline.shared.exception;

/**
 * @author Mamaorha
 *
 */
public class ConnectivityException extends InternalError
{
	private static final long serialVersionUID = -4168257170683453534L;

	public ConnectivityException(String message)
	{
		super(message);
	}
}
