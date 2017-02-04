package com.knightonline.shared.persistence.dao.impl;

import java.util.List;

import javax.ejb.TransactionAttribute;
import javax.ejb.TransactionAttributeType;

import org.springframework.stereotype.Repository;

import com.knightonline.shared.data.constants.NamedQueriesConstants;
import com.knightonline.shared.persistence.dao.IZoneDAO;
import com.knightonline.shared.persistence.entities.Zone;

/**
 * @author Mamaorha
 *
 */
@Repository
@TransactionAttribute(value = TransactionAttributeType.REQUIRED)
public class ZoneHibernateDAO extends AbstractHibernateDAO<Zone, Long> implements IZoneDAO
{
	@Override
	public List<Zone> getZones()
	{
		List<Zone> list = executeNamedQuery(NamedQueriesConstants.GET_ZONES, true);
		return list;
	}
}
