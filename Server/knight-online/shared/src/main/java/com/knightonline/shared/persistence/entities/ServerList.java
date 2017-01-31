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
@Table(name = "SERVER_LIST")
@Cache(usage = CacheConcurrencyStrategy.READ_WRITE)
@NamedQueries({ @NamedQuery(name = NamedQueriesConstants.GET_SERVER_LIST, query = "from ServerList") })
public class ServerList implements Serializable
{
	private static final long serialVersionUID = 8169583905131820106L;
	
	protected String serverIp;
	protected String lanIp;
	protected String serverName;
	protected int groupId;
	protected int premiumLimit;
	protected int freeLimit;

	@Id()
	@Column(name = "SERVER_IP", unique = true, nullable = false, length = 50)
	public String getServerIp()
	{
		return serverIp;
	}

	public void setServerIp(String serverIp)
	{
		this.serverIp = serverIp;
	}

	@Id()
	@Column(name = "LAN_IP", unique = true, nullable = false, length = 50)
	public String getLanIp()
	{
		return lanIp;
	}

	public void setLanIp(String lanIp)
	{
		this.lanIp = lanIp;
	}

	@Id()
	@Column(name = "SERVER_NAME", unique = true, nullable = false, length = 45)
	public String getServerName()
	{
		return serverName;
	}

	public void setServerName(String serverName)
	{
		this.serverName = serverName;
	}

	@Basic
	@Column(name = "GROUP_ID", unique = false, nullable = false)
	public int getGroupId()
	{
		return groupId;
	}

	public void setGroupId(int groupId)
	{
		this.groupId = groupId;
	}

	@Basic
	@Column(name = "PREMIUM_LIMIT", unique = false, nullable = false)
	public int getPremiumLimit()
	{
		return premiumLimit;
	}

	public void setPremiumLimit(int premiumLimit)
	{
		this.premiumLimit = premiumLimit;
	}

	@Basic
	@Column(name = "FREE_LIMIT", unique = false, nullable = false)
	public int getFreeLimit()
	{
		return freeLimit;
	}

	public void setFreeLimit(int freeLimit)
	{
		this.freeLimit = freeLimit;
	}

	@Override
	public int hashCode()
	{
		final int prime = 31;
		int result = 1;
		result = prime * result + freeLimit;
		result = prime * result + groupId;
		result = prime * result + ((lanIp == null) ? 0 : lanIp.hashCode());
		result = prime * result + premiumLimit;
		result = prime * result + ((serverIp == null) ? 0 : serverIp.hashCode());
		result = prime * result + ((serverName == null) ? 0 : serverName.hashCode());
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
		ServerList other = (ServerList) obj;
		if (freeLimit != other.freeLimit)
			return false;
		if (groupId != other.groupId)
			return false;
		if (lanIp == null)
		{
			if (other.lanIp != null)
				return false;
		}
		else if (!lanIp.equals(other.lanIp))
			return false;
		if (premiumLimit != other.premiumLimit)
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
		return true;
	}

	@Override
	public String toString()
	{
		return "ServerList [serverIp=" + serverIp + ", lanIp=" + lanIp + ", serverName=" + serverName + ", groupId=" + groupId + ", premiumLimit=" + premiumLimit + ", freeLimit=" + freeLimit + "]";
	}
}
