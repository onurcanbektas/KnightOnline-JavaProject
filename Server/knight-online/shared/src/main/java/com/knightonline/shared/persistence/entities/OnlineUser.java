package com.knightonline.shared.persistence.entities;

import java.io.Serializable;

import javax.persistence.Basic;
import javax.persistence.Column;
import javax.persistence.Id;
import javax.persistence.Table;

import org.hibernate.annotations.Cache;
import org.hibernate.annotations.CacheConcurrencyStrategy;
import org.hibernate.annotations.NamedQueries;
import org.hibernate.annotations.NamedQuery;

import com.knightonline.shared.data.constants.NamedQueriesConstants;

/**
 * @author Mamaorha
 *
 */
@javax.persistence.Entity()
@Table(name = "ONLINE_USER")
@Cache(usage = CacheConcurrencyStrategy.READ_WRITE)
@NamedQueries({ @NamedQuery(name = NamedQueriesConstants.GET_ONLINE_USERS, query = "from OnlineUser") })
public class OnlineUser implements Serializable
{
	private static final long serialVersionUID = -7787439517324439647L;

	protected String username;
	protected String characterName;
	protected String serverName;
	protected String serverIp;
	protected String clientIp;

	@Id()
	@Column(name = "USERNAME", unique = true, nullable = false, length = 50)
	public String getUsername()
	{
		return username;
	}

	public void setUsername(String username)
	{
		this.username = username;
	}

	@Id()
	@Column(name = "CHARACTER_NAME", unique = true, nullable = false, length = 45)
	public String getCharacterName()
	{
		return characterName;
	}

	public void setCharacterName(String characterName)
	{
		this.characterName = characterName;
	}

	@Basic()
	@Column(name = "SERVER_NAME", unique = false, nullable = false, length = 45)
	public String getServerName()
	{
		return serverName;
	}

	public void setServerName(String serverName)
	{
		this.serverName = serverName;
	}

	@Basic()
	@Column(name = "SERVER_IP", unique = false, nullable = false, length = 50)
	public String getServerIp()
	{
		return serverIp;
	}

	public void setServerIp(String serverIp)
	{
		this.serverIp = serverIp;
	}

	@Basic()
	@Column(name = "CLIENT_IP", unique = false, nullable = false, length = 50)
	public String getClientIp()
	{
		return clientIp;
	}

	public void setClientIp(String clientIp)
	{
		this.clientIp = clientIp;
	}

	@Override
	public int hashCode()
	{
		final int prime = 31;
		int result = 1;
		result = prime * result + ((characterName == null) ? 0 : characterName.hashCode());
		result = prime * result + ((clientIp == null) ? 0 : clientIp.hashCode());
		result = prime * result + ((serverIp == null) ? 0 : serverIp.hashCode());
		result = prime * result + ((serverName == null) ? 0 : serverName.hashCode());
		result = prime * result + ((username == null) ? 0 : username.hashCode());
		return result;
	}

	@Override
	public boolean equals(Object obj)
	{
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		OnlineUser other = (OnlineUser) obj;
		if (characterName == null)
		{
			if (other.characterName != null)
				return false;
		}
		else if (!characterName.equals(other.characterName))
			return false;
		if (clientIp == null)
		{
			if (other.clientIp != null)
				return false;
		}
		else if (!clientIp.equals(other.clientIp))
			return false;
		if (serverIp == null)
		{
			if (other.serverIp != null)
				return false;
		}
		else if (!serverIp.equals(other.serverIp))
			return false;
		if (serverName == null)
		{
			if (other.serverName != null)
				return false;
		}
		else if (!serverName.equals(other.serverName))
			return false;
		if (username == null)
		{
			if (other.username != null)
				return false;
		}
		else if (!username.equals(other.username))
			return false;
		return true;
	}

	@Override
	public String toString()
	{
		return "OnlineUser [username=" + username + ", characterName=" + characterName + ", serverName=" + serverName + ", serverIp=" + serverIp + ", clientIp=" + clientIp + "]";
	}
}
