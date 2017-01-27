package com.knightonline.login.network.packet.handlers;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.annotation.PostConstruct;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.login.server.LoginServer;
import com.knightonline.shared.data.enums.NationEnum;
import com.knightonline.shared.network.packet.IPacketHandler;
import com.knightonline.shared.network.packet.Packet;
import com.knightonline.shared.network.packet.PacketWriter;
import com.knightonline.shared.persistence.dao.IKingInfoDAO;
import com.knightonline.shared.persistence.dao.IServerListDAO;
import com.knightonline.shared.persistence.entities.KingInfo;
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
	
	@Autowired
	protected IKingInfoDAO kingInfoDAO;
	
	private List<ServerList> serverList;
	private Map<NationEnum, KingInfo> kingInfoMap;
	
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
			result.appendShort((short)getOnlineUsers());
			
			//current client doesn't handle king info....
			
			/*result.appendInt8(1);
			
			for (NationEnum nation : NationEnum.values())
			{
				if(nation == NationEnum.NO_NATION)
				{
					continue;
				}
				
				KingInfo kingInfo = kingInfoMap.get(nation);
				
				if(null == kingInfo)
				{
					result.appendString("");
					result.appendString("");
				}	
			}*/
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
	
		kingInfoMap = new HashMap<>();
		
		List<KingInfo> kingInfoList = kingInfoDAO.getKingInfo();
	
		for (KingInfo kingInfo : kingInfoList)
		{
			kingInfoMap.put(kingInfo.getNation(), kingInfo);
		}
	}

	public int getOnlineUsers()
	{
		//TODO ask ebenezer?
		return 50;
	}
}
