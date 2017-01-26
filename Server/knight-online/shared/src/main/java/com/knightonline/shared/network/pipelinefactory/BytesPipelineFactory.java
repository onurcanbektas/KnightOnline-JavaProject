package com.knightonline.shared.network.pipelinefactory;

import org.jboss.netty.channel.ChannelPipeline;
import org.jboss.netty.channel.ChannelPipelineFactory;
import org.jboss.netty.channel.Channels;

import com.knightonline.shared.network.messagehandler.BytesServerMessageHandler;

/**
 * @author Mamaorha
 *
 */
public class BytesPipelineFactory extends PipelineFactory implements ChannelPipelineFactory
{
	@Override
	public ChannelPipeline createPipeline(long channelId) throws Exception
	{
		BytesServerMessageHandler serverMessageHandler = new BytesServerMessageHandler(getConnectionStateReport(), getRequestQueue(), channelId);
		return Channels.pipeline(serverMessageHandler);
	}
}
