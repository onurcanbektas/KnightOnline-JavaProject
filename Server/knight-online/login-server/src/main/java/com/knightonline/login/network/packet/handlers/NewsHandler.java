package com.knightonline.login.network.packet.handlers;

import javax.annotation.PostConstruct;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.shared.network.packet.IPacketHandler;
import com.knightonline.shared.network.packet.Packet;
import com.knightonline.shared.network.packet.PacketWriter;
import com.knightonline.shared.utils.ApplicationPropertiesManager;

/**
 * @author Mamaorha
 *
 */
@Component
public class NewsHandler implements IPacketHandler
{
	@Autowired
	protected PacketWriter packetWriter;

	@Autowired
	protected ApplicationPropertiesManager applicationPropertiesManager;
	
	private String news;
	
	@PostConstruct
	private void init()
	{
		refreshNews();
	}
	
	@Override
	public void handlePacket(Packet requestPacket)
	{
		Packet result = new Packet(requestPacket.getOpcode(), requestPacket.getMessageInfo());
		result.appendString("Login Notice");
		result.appendString(news);
		
		packetWriter.sendPacket(result);
	}
	
	public void refreshNews()
	{
		applicationPropertiesManager.refresh();
		
		news = applicationPropertiesManager.getValue("news", "<empty>");
		System.out.println("Loaded news: " + news);
	}
}
