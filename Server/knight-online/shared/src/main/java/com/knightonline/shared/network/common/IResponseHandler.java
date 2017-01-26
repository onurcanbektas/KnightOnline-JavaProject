package com.knightonline.shared.network.common;

import com.knightonline.shared.exception.ConnectivityException;

/**
 * @author Mamaorha
 *
 */
public interface IResponseHandler
{
	public void sendAsyncResponseMessage(MessageInfo messageInfo) throws ConnectivityException;
}
