package com.knightonline.shared.network.common;

import java.io.Serializable;

/**
 * @author Mamaorha
 *
 */
public class MessageInfo
{
	protected long channelId;
	protected Serializable request;
	protected Serializable response;
	protected IResponseHandler handler;

	public MessageInfo(long channelId, IResponseHandler handler)
	{
		this.channelId = channelId;
		this.handler = handler;
	}

	public long getChannelId()
	{
		return channelId;
	}

	public void setChannelId(long channelId)
	{
		this.channelId = channelId;
	}

	public Serializable getRequest()
	{
		return request;
	}

	public void setRequest(Serializable request)
	{
		this.request = request;
	}

	public Serializable getResponse()
	{
		return response;
	}

	public void setResponse(Serializable response)
	{
		this.response = response;
	}

	public IResponseHandler getHandler()
	{
		return handler;
	}

	public void setHandler(IResponseHandler handler)
	{
		this.handler = handler;
	}

	@Override
	public int hashCode()
	{
		final int prime = 31;
		int result = 1;
		result = prime * result + (int) (channelId ^ (channelId >>> 32));
		result = prime * result + ((handler == null) ? 0 : handler.hashCode());
		result = prime * result + ((request == null) ? 0 : request.hashCode());
		result = prime * result + ((response == null) ? 0 : response.hashCode());
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
		MessageInfo other = (MessageInfo) obj;
		if (channelId != other.channelId)
			return false;
		if (handler == null)
		{
			if (other.handler != null)
				return false;
		}
		else if (!handler.equals(other.handler))
			return false;
		if (request == null)
		{
			if (other.request != null)
				return false;
		}
		else if (!request.equals(other.request))
			return false;
		if (response == null)
		{
			if (other.response != null)
				return false;
		}
		else if (!response.equals(other.response))
			return false;
		return true;
	}

	@Override
	public String toString()
	{
		return "MessageInfo [channelId=" + channelId + ", request=" + request + ", response=" + response + ", handler=" + handler + "]";
	}
}
