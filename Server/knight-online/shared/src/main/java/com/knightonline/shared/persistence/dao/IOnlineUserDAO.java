package com.knightonline.shared.persistence.dao;

import java.util.List;

import com.knightonline.shared.persistence.entities.OnlineUser;

/**
 * @author Mamaorha
 *
 */
public interface IOnlineUserDAO extends IGenericDAO<OnlineUser, Long>
{
	public List<OnlineUser> getOnlineUsers();
	public void deleteOnlineUsers(String serverIp);
	public void deleteOnlineUser(String username);
}
