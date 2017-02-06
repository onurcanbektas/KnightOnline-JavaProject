package com.knightonline.game.network.packet.handlers;

import org.omg.CORBA.StringHolder;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.shared.data.LoginResultCodeEnum;
import com.knightonline.shared.network.KOServer;
import com.knightonline.shared.network.packet.Packet;
import com.knightonline.shared.network.packet.PacketHandlerBase;
import com.knightonline.shared.network.packet.PacketWriter;
import com.knightonline.shared.network.packet.handlers.BaseLogin;

/**
 * @author Mamaorha
 *
 */
@Component
public class GameLoginHandler extends PacketHandlerBase
{
	@Autowired
	protected PacketWriter packetWriter;

	@Autowired
	protected BaseLogin baseLogin;

	@Autowired
	protected KOServer koServer;
	
	@Override
	public void handlePacket(Packet requestPacket)
	{
		StringHolder username = new StringHolder();

		short resultCode = baseLogin.handlePacket(requestPacket, username);

		Packet result = new Packet(requestPacket.getOpcode(), requestPacket.getMessageInfo());

		if(resultCode == LoginResultCodeEnum.AUTH_SUCCESS)
		{
			if(koServer.isConnectedAccount(username.value))
			{
				resultCode = LoginResultCodeEnum.AUTH_IN_GAME;
				koServer.killAccount(username.value);
			}
			
			else
			{
				koServer.connectAccount(username.value, requestPacket.getMessageInfo().getChannelId());
			}
		}
		
		result.appendInt8(resultCode);
		packetWriter.sendPacket(result);
	}
}
