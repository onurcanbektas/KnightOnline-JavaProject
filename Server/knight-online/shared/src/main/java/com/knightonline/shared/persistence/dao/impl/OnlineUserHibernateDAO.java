package com.knightonline.shared.persistence.dao.impl;

import java.util.List;

import javax.ejb.TransactionAttribute;
import javax.ejb.TransactionAttributeType;

import org.springframework.stereotype.Repository;

import com.knightonline.shared.data.common.DynamicAttribute;
import com.knightonline.shared.data.constants.NamedQueriesConstants;
import com.knightonline.shared.data.constants.StringConstants;
import com.knightonline.shared.persistence.dao.IOnlineUserDAO;
import com.knightonline.shared.persistence.entities.OnlineUser;

/**
 * @author Mamaorha
 *
 */
@Repository
@TransactionAttribute(value = TransactionAttributeType.REQUIRED)
public class OnlineUserHibernateDAO extends AbstractHibernateDAO<OnlineUser, Long> implements IOnlineUserDAO
{
	@Override
	public List<OnlineUser> getOnlineUsers()
	{
		return executeNamedQuery(NamedQueriesConstants.GET_ONLINE_USERS, true);
	}

	@Override
	public void deleteOnlineUsers(String serverIp)
	{
		deleteQuery(NamedQueriesConstants.DELETE_ONLINE_USERS, new DynamicAttribute(StringConstants.SERVER_IP, serverIp));
	}
	
	@Override
	public void deleteOnlineUser(String username)
	{
		deleteQuery(NamedQueriesConstants.DELETE_ONLINE_USER, new DynamicAttribute(StringConstants.USERNAME, username));
	}
}
