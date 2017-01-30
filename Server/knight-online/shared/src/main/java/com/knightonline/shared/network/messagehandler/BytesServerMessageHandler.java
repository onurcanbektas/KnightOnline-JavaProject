package com.knightonline.shared.network.messagehandler;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Queue;

import org.jboss.netty.buffer.BigEndianHeapChannelBuffer;
import org.jboss.netty.buffer.ChannelBuffer;
import org.jboss.netty.buffer.ChannelBuffers;
import org.jboss.netty.channel.ChannelHandlerContext;
import org.jboss.netty.channel.Channels;
import org.jboss.netty.channel.ExceptionEvent;
import org.jboss.netty.channel.MessageEvent;

import com.knightonline.shared.exception.ConnectivityException;
import com.knightonline.shared.network.common.IConnectionStateReport;
import com.knightonline.shared.network.common.MessageInfo;

/**
 * @author Mamaorha
 *
 */
public class BytesServerMessageHandler extends BaseServerMessageHandler
{
	private static final String TAIL_BUFFER = "TAIL_BUFFER";
	private static final int MIN_PACKET_SIZE = 7; //head=2, tail=2, length=2, opcode=1
	private static final int INT_SIZE = 4;
	private static final int SHORT_SIZE = 2;
	private static final int HEADER_PACKET_VALID_VALUE = 0x55AA;
	private static final String BAD_MESSAGE_TYPE = "Handler Type is [%s], message type should be 'byte[]'";

	public BytesServerMessageHandler(IConnectionStateReport connectionStateReport, Queue<MessageInfo> queue, long channelId)
	{
		super(connectionStateReport, queue, channelId);
	}

	@Override
	public void messageReceived(ChannelHandlerContext ctx, MessageEvent e) throws Exception
	{
		List<byte[]> FlatBufferMsg = getByteBuffer(ctx, e, getContext());

		for (byte[] FlatBufferMsg2 : FlatBufferMsg)
		{
			MessageInfo messageInfo = new MessageInfo(getChannelId(), getResponseHandler());
			messageInfo.setRequest(FlatBufferMsg2);

			getQueue().add(messageInfo);
		}
	}

	private List<byte[]> getByteBuffer(ChannelHandlerContext ctx, MessageEvent e, Map<String, Object> context) throws Exception
	{
		List<byte[]> flatBufferMsgs = new ArrayList<byte[]>();

		BigEndianHeapChannelBuffer msg = (BigEndianHeapChannelBuffer) e.getMessage();
		byte[] array = msg.array();

		ByteBuffer currBuff = null;

		int capacity = array.length;

		byte[] tailBuff = (byte[]) context.get(TAIL_BUFFER);

		if (tailBuff != null)
		{
			capacity += tailBuff.length;
			currBuff = ByteBuffer.allocate(capacity);
			currBuff.order(ByteOrder.LITTLE_ENDIAN);
			currBuff.put(tailBuff);
			context.remove(TAIL_BUFFER);
			tailBuff = null;
		}

		else
		{
			currBuff = ByteBuffer.allocate(capacity);
		}

		currBuff.order(ByteOrder.LITTLE_ENDIAN);
		currBuff.put(array);
		currBuff.rewind();

		msg.clear();

		int readableBytes = 0;

		while ((capacity - currBuff.position()) > 0)
		{
			readableBytes = (capacity - currBuff.position());

			if (readableBytes < MIN_PACKET_SIZE)
			{
				tailBuff = new byte[readableBytes];
				currBuff.get(tailBuff);
				context.put(TAIL_BUFFER, tailBuff);
				break;
			}

			int tempPosition = currBuff.position();

			byte[] headerByte = new byte[2];
			currBuff.get(headerByte);

			currBuff.position(tempPosition);

			short header = currBuff.getShort();

			if (header == 0 || header != HEADER_PACKET_VALID_VALUE)
			{
				continue;
			}

			short length = currBuff.getShort();

			readableBytes = (capacity - currBuff.position());

			//packet data length + the tail
			if (readableBytes < length + SHORT_SIZE)
			{
				tailBuff = new byte[readableBytes];
				currBuff.position(currBuff.position() - INT_SIZE); //go back before the length and header (4 bytes)
				currBuff.get(tailBuff);
				context.put(TAIL_BUFFER, tailBuff);
				break;
			}

			byte[] bytes = new byte[length];
			currBuff.get(bytes);

			byte[] tailByte = new byte[2];
			currBuff.get(tailByte);

			if (tailByte[0] != headerByte[1] || tailByte[1] != headerByte[0])
			{
				throw new Exception("Packet without tail");
			}

			flatBufferMsgs.add(bytes);
		}

		return flatBufferMsgs;
	}

	@Override
	public void writeRequested(ChannelHandlerContext chctx, MessageEvent e) throws Exception
	{
		Object message = e.getMessage();

		if (message instanceof byte[])
		{
			byte[] bytes = (byte[]) message;
			ChannelBuffer cBuff = ChannelBuffers.buffer(bytes.length);
			cBuff.writeBytes(bytes);
			Channels.write(chctx, e.getFuture(), cBuff);
		}

		else
		{
			throw new ConnectivityException(BAD_MESSAGE_TYPE);
		}
	}
	
	@Override
	public void exceptionCaught(ChannelHandlerContext ctx, ExceptionEvent e) throws Exception
	{
		
	}
}
