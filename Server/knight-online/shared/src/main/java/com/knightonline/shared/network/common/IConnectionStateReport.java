package com.knightonline.shared.network.common;

import org.jboss.netty.channel.ChannelHandlerContext;

/**
 * @author Mamaorha
 *
 */
public interface IConnectionStateReport extends IResponseHandler
{
	public void connected(long channelId, ChannelHandlerContext ctx);
	public void disconnected(long channelId, ChannelHandlerContext ctx);
}
