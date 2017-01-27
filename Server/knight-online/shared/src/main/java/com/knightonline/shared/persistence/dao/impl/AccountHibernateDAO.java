package com.knightonline.shared.persistence.dao.impl;

import javax.ejb.TransactionAttribute;
import javax.ejb.TransactionAttributeType;
import javax.persistence.ParameterMode;
import javax.persistence.StoredProcedureQuery;

import org.springframework.stereotype.Repository;

import com.knightonline.shared.data.common.DynamicAttribute;
import com.knightonline.shared.data.constants.NamedQueriesConstants;
import com.knightonline.shared.data.constants.StringConstants;
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
	public Account getAccountByUsername(String username)
	{
		return executeNamedQuerySingleResult(NamedQueriesConstants.GET_ACCOUNT_BY_USERNAME, true, new DynamicAttribute(StringConstants.USERNAME, username));
	}

	@Override
	public short accountLoginProcedure(String username, String password)
	{
		StoredProcedureQuery createStoredProcedureQuery = getEntityManager().createStoredProcedureQuery("ACCOUNT_LOGIN");
		createStoredProcedureQuery.registerStoredProcedureParameter(StringConstants.USERNAME, String.class, ParameterMode.IN);
		createStoredProcedureQuery.registerStoredProcedureParameter(StringConstants.PASSWORD, String.class, ParameterMode.IN);
		createStoredProcedureQuery.registerStoredProcedureParameter(StringConstants.RESULT, Short.class, ParameterMode.OUT);

		createStoredProcedureQuery.setParameter(StringConstants.USERNAME, username);
		createStoredProcedureQuery.setParameter(StringConstants.PASSWORD, password);

		createStoredProcedureQuery.execute();

		return (Short) createStoredProcedureQuery.getOutputParameterValue(StringConstants.RESULT);
	}
}
