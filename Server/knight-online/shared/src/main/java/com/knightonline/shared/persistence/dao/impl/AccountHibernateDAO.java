package com.knightonline.shared.persistence.dao.impl;

import java.util.List;

import javax.ejb.TransactionAttribute;
import javax.ejb.TransactionAttributeType;
import javax.persistence.ParameterMode;
import javax.persistence.StoredProcedureQuery;

import org.springframework.stereotype.Repository;

import com.knightonline.shared.data.constants.NamedQueriesConstants;
import com.knightonline.shared.persistence.dao.IAccountDAO;
import com.knightonline.shared.persistence.entities.Account;

/**
 * @author Mamaorha
 *
 */
@Repository
@TransactionAttribute(value = TransactionAttributeType.REQUIRED)
public class AccountHibernateDAO extends AbstractHibernateDAO<Account, Long> implements IAccountDAO
{
	@Override
	@SuppressWarnings("unchecked")
	public Account getAccountByUsername(String username)
	{
		List<Account> list = (List<Account>) executeNamedQuery(NamedQueriesConstants.GET_ACCOUNT_BY_USERNAME, new String[] { "username" }, new String[] { username }, true);

		if (null != list && !list.isEmpty())
		{
			return list.get(0);
		}

		return null;
	}

	@Override
	public short accountLoginProcedure(String username, String password)
	{
		StoredProcedureQuery createStoredProcedureQuery = getEntityManager().createStoredProcedureQuery("ACCOUNT_LOGIN");
		createStoredProcedureQuery.registerStoredProcedureParameter("i_username", String.class, ParameterMode.IN);
		createStoredProcedureQuery.registerStoredProcedureParameter("i_password", String.class, ParameterMode.IN);
		createStoredProcedureQuery.registerStoredProcedureParameter("o_res", Short.class, ParameterMode.OUT);

		createStoredProcedureQuery.setParameter("i_username", username);
		createStoredProcedureQuery.setParameter("i_password", password);

		createStoredProcedureQuery.execute();

		return (Short) createStoredProcedureQuery.getOutputParameterValue("o_res");
	}
}
