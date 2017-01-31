package com.knightonline.login.network.packet.handlers;

import java.util.List;

import javax.annotation.PostConstruct;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.login.server.LoginServer;
import com.knightonline.shared.network.packet.IPacketHandler;
import com.knightonline.shared.network.packet.Packet;
import com.knightonline.shared.network.packet.PacketWriter;
import com.knightonline.shared.persistence.dao.IServerListDAO;
import com.knightonline.shared.persistence.entities.ServerList;

/**
 * @author Mamaorha
 *
 */
@Component
public class ServerListHandler implements IPacketHandler
{
	@Autowired
	protected PacketWriter packetWriter;

	@Autowired
	protected LoginServer loginServer;
	
	@Autowired
	protected IServerListDAO serverListDAO;
	
	private List<ServerList> serverList;
	
	@PostConstruct
	private void init()
	{
		refreshServerList();
	}
	
	@Override
	public void handlePacket(Packet requestPacket)
	{
		Packet result = new Packet(requestPacket.getOpcode(), requestPacket.getMessageInfo());
		result.appendInt8(serverList.size());
		
		for (ServerList server : serverList)
		{
			result.appendString(server.getServerIp());
			result.appendString(server.getServerName());
			result.appendShort((short)getOnlineUsers(server.getPremiumLimit()));
			result.appendShort((short)server.getFreeLimit());
			result.appendShort((short)server.getPremiumLimit());
		}
		
		packetWriter.sendPacket(result);
	}
	
	public void refreshServerList()
	{
		serverList = serverListDAO.getServerList();
		
		if(null == serverList)
		{
			throw new InternalError("Server list table cant be empty");
		}
		
		for (ServerList server : serverList)
		{
			System.out.println("Loaded server: " + server.toString());
		}
	}

	public int getOnlineUsers(int max)
	{
		return (int) (Math.random() * max);
	}
}
