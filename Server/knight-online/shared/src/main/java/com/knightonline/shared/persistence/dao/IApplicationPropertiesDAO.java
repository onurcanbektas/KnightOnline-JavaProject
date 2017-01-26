package com.knightonline.shared.persistence.dao;

import java.util.List;

import com.knightonline.shared.exception.DAOException;
import com.knightonline.shared.persistence.entities.ApplicationProperties;

/**
 * @author Mamaorha
 *
 */
public interface IApplicationPropertiesDAO
{
	public List<ApplicationProperties> getProperties() throws DAOException;
}
