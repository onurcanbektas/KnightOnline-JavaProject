package com.knightonline.shared.persistence.dao.impl;

import java.util.List;

import javax.ejb.TransactionAttribute;
import javax.ejb.TransactionAttributeType;

import org.springframework.stereotype.Repository;

import com.knightonline.shared.data.constants.NamedQueriesConstants;
import com.knightonline.shared.exception.DAOException;
import com.knightonline.shared.persistence.dao.IApplicationPropertiesDAO;
import com.knightonline.shared.persistence.entities.ApplicationProperties;

/**
 * @author Mamaorha
 *
 */
@Repository
@TransactionAttribute(value = TransactionAttributeType.REQUIRED)
public class ApplicationPropertiesHibernateDAO extends AbstractHibernateDAO<ApplicationProperties, Long> implements IApplicationPropertiesDAO
{
	@Override
	public List<ApplicationProperties> getProperties() throws DAOException
	{
		return executeNamedQuery(NamedQueriesConstants.GET_APPLICATION_PROPERTIES, true);
	}
	
}
