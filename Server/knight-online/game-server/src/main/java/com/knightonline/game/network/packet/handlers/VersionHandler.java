package com.knightonline.game.network.packet.handlers;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.game.server.GameServer;
import com.knightonline.shared.network.packet.Packet;
import com.knightonline.shared.network.packet.PacketHandlerBase;
import com.knightonline.shared.network.packet.PacketWriter;

/**
 * @author Mamaorha
 *
 */
@Component
public class VersionHandler extends PacketHandlerBase
{
	@Autowired
	protected PacketWriter packetWriter;

	@Autowired
	protected GameServer gameServer;

	@Override
	public void handlePacket(Packet requestPacket)
	{
		Packet result = new Packet(requestPacket.getOpcode(), requestPacket.getMessageInfo());
		result.appendString(gameServer.getVersion());	
		
		packetWriter.sendPacket(result);
	}
}
