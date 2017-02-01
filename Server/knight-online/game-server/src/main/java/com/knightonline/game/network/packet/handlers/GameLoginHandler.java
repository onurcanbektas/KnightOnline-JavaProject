package com.knightonline.game.network.packet.handlers;

import org.omg.CORBA.StringHolder;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.game.server.GameServer;
import com.knightonline.shared.data.enums.LoginResultCodeEnum;
import com.knightonline.shared.network.packet.IPacketHandler;
import com.knightonline.shared.network.packet.Packet;
import com.knightonline.shared.network.packet.PacketWriter;
import com.knightonline.shared.network.packet.handlers.BaseLogin;

/**
 * @author Mamaorha
 *
 */
@Component
public class GameLoginHandler implements IPacketHandler
{
	@Autowired
	protected PacketWriter packetWriter;

	@Autowired
	protected BaseLogin baseLogin;

	@Autowired
	protected GameServer gameServer;
	
	@Override
	public void handlePacket(Packet requestPacket)
	{
		StringHolder username = new StringHolder();

		short resultCode = baseLogin.handlePacket(requestPacket, username);

		Packet result = new Packet(requestPacket.getOpcode(), requestPacket.getMessageInfo());

		if(resultCode == LoginResultCodeEnum.AUTH_SUCCESS)
		{
			if(gameServer.getServer().isConnectedAccount(username.value))
			{
				resultCode = LoginResultCodeEnum.AUTH_IN_GAME;
				gameServer.getServer().killAccount(username.value);
			}
			
			else
			{
				gameServer.getServer().connectAccount(username.value, requestPacket.getMessageInfo().getChannelId());
			}
		}
		
		result.appendInt8(resultCode);
		packetWriter.sendPacket(result);
	}
}
