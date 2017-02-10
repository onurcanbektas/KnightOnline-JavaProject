package com.knightonline.login.network.packet.handlers;

import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import javax.annotation.PostConstruct;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.login.server.LoginServer;
import com.knightonline.shared.data.SharedOpCodes;
import com.knightonline.shared.data.constants.ConfigurationConstants;
import com.knightonline.shared.network.client.KOClient;
import com.knightonline.shared.network.packet.Packet;
import com.knightonline.shared.network.packet.PacketHandlerBase;
import com.knightonline.shared.network.packet.PacketWriter;
import com.knightonline.shared.persistence.dao.IServerListDAO;
import com.knightonline.shared.persistence.entities.ServerList;
import com.knightonline.shared.utils.ApplicationPropertiesManager;

/**
 * @author Mamaorha
 *
 */
@Component
public class ServerListHandler extends PacketHandlerBase
{
	@Autowired
	protected ApplicationPropertiesManager applicationPropertiesManager;
	
	@Autowired
	protected PacketWriter packetWriter;

	@Autowired
	protected LoginServer loginServer;

	@Autowired
	protected IServerListDAO serverListDAO;

	private List<ServerList> serverList;
	private Map<String, KOClient> connections;
	
	@PostConstruct
	private void init()
	{
		connections = new ConcurrentHashMap<>();
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
			result.appendShort(getOnlineUsers(server.getServerIp()));
			result.appendShort((short) server.getFreeLimit());
			result.appendShort((short) server.getPremiumLimit());
		}

		packetWriter.sendPacket(result);
	}

	public void refreshServerList()
	{
		serverList = serverListDAO.getServerList();

		if (null == serverList)
		{
			throw new InternalError("Server list table cant be empty");
		}

		for (ServerList server : serverList)
		{
			System.out.println("Loaded server: " + server.toString());
		}
	}

	public short getOnlineUsers(String serverIp)
	{
		short online = 0;
		
		try
		{
			KOClient client = connections.get(serverIp);
			
			if(null == client)
			{
				client = KOClient.openConnection(serverIp, applicationPropertiesManager.getIntValue(ConfigurationConstants.GAME_SERVER_PORT, null));
				connections.put(serverIp, client);
			}
		
			Packet requestPacket = new Packet(SharedOpCodes.ONLINE_USERS, null);
			Packet responsePacket = client.sendSyncMessage(requestPacket);
			
			online = responsePacket.getShort();
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
		
		return online;
	}
}
