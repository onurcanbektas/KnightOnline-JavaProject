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
	public OnlineUser getOnlineUser(String username);
}
