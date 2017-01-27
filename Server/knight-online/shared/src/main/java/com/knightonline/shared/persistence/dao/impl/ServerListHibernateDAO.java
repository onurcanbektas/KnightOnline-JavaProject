package com.knightonline.shared.persistence.dao.impl;

import java.util.List;

import javax.ejb.TransactionAttribute;
import javax.ejb.TransactionAttributeType;

import org.springframework.stereotype.Repository;

import com.knightonline.shared.data.constants.NamedQueriesConstants;
import com.knightonline.shared.persistence.dao.IServerListDAO;
import com.knightonline.shared.persistence.entities.ServerList;

/**
 * @author Mamaorha
 *
 */
@Repository
@TransactionAttribute(value = TransactionAttributeType.REQUIRED)
public class ServerListHibernateDAO extends AbstractHibernateDAO<ServerList, Long> implements IServerListDAO
{
	@Override
	@SuppressWarnings("unchecked")	
	public List<ServerList> getServerList()
	{
		List<ServerList> list = (List<ServerList>) executeNamedQuery(NamedQueriesConstants.GET_SERVER_LIST, true);
		return list;
	}
	
}
