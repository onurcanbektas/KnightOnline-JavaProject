package com.knightonline.shared.network.packet;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import org.springframework.stereotype.Component;

import com.knightonline.shared.utils.PacketUtils;

/**
 * @author Mamaorha
 *
 */
@Component
public class PacketWriter
{
	private static final int OPCODE_LEN = 2;

	public void sendPacket(Packet packet)
	{
		byte[] header = new byte[] { -86, 85 };
		byte[] data = packet.getData();
		byte[] opcodeBytes = new byte[] { (byte) packet.getOpcode() };
		byte[] tail = new byte[] { 85, -86 };

		if(data == null)
		{
			data = new byte[0];
		}
		
		int length = header.length + OPCODE_LEN + data.length + opcodeBytes.length + tail.length;
		short dataLen = (short) (data.length + 1);

		ByteBuffer byteBuffer = ByteBuffer.allocate(length);
		byteBuffer.order(ByteOrder.BIG_ENDIAN);
		byteBuffer.put(header);
		byteBuffer.put(PacketUtils.shortToByteArray(dataLen));
		byteBuffer.put(opcodeBytes);
		byteBuffer.put(data);
		byteBuffer.put(tail);

		packet.getMessageInfo().setResponse(byteBuffer.array());
		packet.getMessageInfo().getHandler().sendAsyncResponseMessage(packet.getMessageInfo());
	}
}
