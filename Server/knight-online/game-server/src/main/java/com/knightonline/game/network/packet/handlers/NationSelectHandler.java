package com.knightonline.game.network.packet.handlers;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.game.server.GameServer;
import com.knightonline.shared.data.enums.NationEnum;
import com.knightonline.shared.network.KOServer;
import com.knightonline.shared.network.packet.Packet;
import com.knightonline.shared.network.packet.handlers.LoggedInHandler;

/**
 * @author Mamaorha
 *
 */
@Component
public class NationSelectHandler extends LoggedInHandler
{
	@Autowired
	protected GameServer gameServer;

	@Override
	protected KOServer getKOServer()
	{
		return gameServer.getServer();
	}

	@Override
	protected void handlePacketImpl(Packet requestPacket)
	{
		short nation = requestPacket.getByte();
		
		NationEnum nationEnum = NationEnum.forValue((int)nation);
		
		if(null == nationEnum)
		{
			System.out.println("Client sent an invalid nation");
			killConnection(requestPacket);
		}
		
		Packet result = new Packet(requestPacket.getOpcode(), requestPacket.getMessageInfo());
		result.appendInt8(nation);	
		
		packetWriter.sendPacket(result);
	}
}
