package com.knightonline.shared.network.client;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import com.knightonline.shared.network.messagehandler.BytesServerMessageHandler;
import com.knightonline.shared.network.packet.Packet;
import com.knightonline.shared.network.packet.PacketWriter;
import com.knightonline.shared.utils.KOApplicationContext;
import com.knightonline.shared.utils.PacketUtils;

/**
 * @author Mamaorha
 *
 */
public class KOClient
{
	private Socket socket;
	private OutputStream os;
	private InputStream is;

	private KOClient(String serverIP, int port) throws UnknownHostException, IOException
	{
		socket = new Socket(serverIP, port);
		os = socket.getOutputStream();
		is = socket.getInputStream();
	}

	public static KOClient openConnection(String serverIP, int port) throws UnknownHostException, IOException
	{
		return new KOClient(serverIP, port);
	}

	public void closeConnection()
	{
		if (null != socket)
		{
			try
			{
				socket.close();
			}

			catch (IOException e)
			{
				e.printStackTrace();
			}

			socket = null;
		}
	}

	public Packet sendSyncMessage(Packet reqPacket) throws Exception
	{
		try
		{
			if (null == socket)
			{
				throw new Exception("Connection is closed");
			}

			writePacket(reqPacket);

			// read server response
			byte[] tempB = new byte[4];
			is.read(tempB, 0, tempB.length);

			ByteBuffer currBuff = ByteBuffer.allocate(tempB.length);
			currBuff.order(ByteOrder.LITTLE_ENDIAN);
			currBuff.put(tempB);
			currBuff.rewind();

			int tempPosition = currBuff.position();

			byte[] headerByte = new byte[2];
			currBuff.get(headerByte);

			currBuff.position(tempPosition);

			short header = currBuff.getShort();

			if (header == 0 || header != BytesServerMessageHandler.HEADER_PACKET_VALID_VALUE)
			{
				throw new Exception("Packet without valid header");
			}

			short length = currBuff.getShort();

			tempB = new byte[length + 2];
			is.read(tempB, 0, tempB.length);

			currBuff = ByteBuffer.allocate(tempB.length);
			currBuff.order(ByteOrder.LITTLE_ENDIAN);
			currBuff.put(tempB);
			currBuff.rewind();

			byte[] opcodeByte = new byte[1];
			currBuff.get(opcodeByte);

			short opcode = PacketUtils.byteArrayToShort(new byte[] { opcodeByte[0] }, 0);

			byte[] data = new byte[length - 1];
			currBuff.get(data);

			byte[] tailByte = new byte[2];
			currBuff.get(tailByte);

			if (tailByte[0] != headerByte[1] || tailByte[1] != headerByte[0])
			{
				throw new Exception("Packet without valid tail");
			}

			Packet packet = new Packet(opcode, null);
			packet.setData(data);

			return packet;
		}
		
		catch (SocketException e)
		{
			socket = new Socket(socket.getInetAddress().toString(), socket.getPort());
			os = socket.getOutputStream();
			is = socket.getInputStream();
			
			return sendSyncMessage(reqPacket);
		}
	}

	private void writePacket(Packet packet) throws IOException
	{
		PacketWriter packetWriter = (PacketWriter) KOApplicationContext.getInstance().getApplicationContext().getBean("packetWriter");

		os.write(packetWriter.buildPacketData(packet));
		os.flush();
	}
}
