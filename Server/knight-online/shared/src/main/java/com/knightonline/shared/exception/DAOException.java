package com.knightonline.shared.exception;

import javax.ejb.ApplicationException;

/**
 * @author Mamaorha
 *
 */
@ApplicationException(rollback = false)
public class DAOException extends Exception
{
	private static final long serialVersionUID = -8052911423770613085L;

	public DAOException(String message)
	{
		super(message);
	}

	public DAOException(Exception e)
	{
		super(e);
	}

	public DAOException(String message, Exception e)
	{
		super(message, e);
	}
}
