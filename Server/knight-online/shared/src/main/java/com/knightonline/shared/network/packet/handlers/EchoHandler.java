package com.knightonline.shared.network.packet.handlers;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.shared.network.packet.IPacketHandler;
import com.knightonline.shared.network.packet.Packet;
import com.knightonline.shared.network.packet.PacketWriter;

/**
 * @author Mamaorha
 *
 */
@Component
public class EchoHandler implements IPacketHandler
{
	@Autowired
	protected PacketWriter packetWriter;

	@Override
	public void handlePacket(Packet requestPacket)
	{
		Packet result = new Packet(requestPacket.getOpcode(), requestPacket.getMessageInfo());
		result.appendData(requestPacket.getData());
		
		packetWriter.sendPacket(result);
	}
}
