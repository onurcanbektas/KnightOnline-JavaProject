package com.knightonline.shared.data.common;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import com.knightonline.shared.exception.InternalServerError;
import com.knightonline.shared.persistence.dao.IOnlineUserDAO;
import com.knightonline.shared.utils.KOApplicationContext;

/**
 * @author Mamaorha
 *
 */
public class ConnectedMap
{
	private TwoSidesMap<String, Long> connectedChannels; //username to channelId and opposite
	private TwoSidesMap<String, String> usernameCharacter; //username to characterName and opposite
	private Map<String, ConnectedCharacterInfo> connectedUsers; //username to characterInfo
	
	public ConnectedMap()
	{
		this.connectedChannels = new TwoSidesMap<>();
		this.usernameCharacter = new TwoSidesMap<>();
		this.connectedUsers = new ConcurrentHashMap<>();
	}
	
	public void channelClosed(Long channelId)
	{
		String username = connectedChannels.getByValue(channelId);
		
		if(null != username)
		{
			connectedChannels.removeByValue(channelId);
			usernameCharacter.removeByKey(username);
			
			ConnectedCharacterInfo connectedCharacterInfo = connectedUsers.get(username);
			
			if(null != connectedCharacterInfo)
			{
				connectedCharacterInfo.save();
			}
			
			connectedUsers.remove(username);
			
			IOnlineUserDAO onlineUserDAO = (IOnlineUserDAO)KOApplicationContext.getInstance().getApplicationContext().getBean("onlineUserHibernateDAO");
			onlineUserDAO.deleteOnlineUser(username);
		}
	}
	
	public int getLoggedInClients()
	{
		return connectedChannels.size();
	}
	
	public int getOnlineCharacters()
	{
		return connectedUsers.size();
	}
	
	public boolean isConnectedAccount(String username)
	{
		return null != connectedChannels.getByKey(username);
	}
	
	public String getUsernameByChannel(Long channelId)
	{
		return connectedChannels.getByValue(channelId);
	}
	
	public Long getChannelByUsername(String username)
	{
		return connectedChannels.getByKey(username);
	}
	
	public String getUsernameByCharacter(String characterName)
	{
		return usernameCharacter.getByValue(characterName);
	}

	public void connectAccount(String username, Long channelId)
	{
		connectedChannels.put(username, channelId);
	}
	
	public void connectCharacter(Long channelId, ConnectedCharacterInfo connectedCharacterInfo)
	{
		String username = connectedChannels.getByValue(channelId);
		
		if(null == username)
		{
			throw new InternalServerError("channel id must be connected to active username");
		}
		
		usernameCharacter.put(username, connectedCharacterInfo.getCharacterData().getCharacterName());
		connectedUsers.put(username, connectedCharacterInfo);
	}
}
