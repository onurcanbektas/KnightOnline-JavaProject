package com.knightonline.shared.persistence.dao.impl;

import java.util.List;

import javax.ejb.TransactionAttribute;
import javax.ejb.TransactionAttributeType;

import org.springframework.stereotype.Repository;

import com.knightonline.shared.data.constants.NamedQueriesConstants;
import com.knightonline.shared.persistence.dao.IOnlineUserDAO;
import com.knightonline.shared.persistence.entities.Account;
import com.knightonline.shared.persistence.entities.OnlineUser;

/**
 * @author Mamaorha
 *
 */
@Repository
@TransactionAttribute(value = TransactionAttributeType.REQUIRED)
public class OnlineUserHibernateDAO extends AbstractHibernateDAO<Account, Long> implements IOnlineUserDAO
{
	@Override
	@SuppressWarnings("unchecked")
	public List<OnlineUser> getOnlineUsers()
	{
		List<OnlineUser> list = (List<OnlineUser>) executeNamedQuery(NamedQueriesConstants.GET_ONLINE_USERS, true);
		return list;
	}
}
