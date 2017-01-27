package com.knightonline.shared.persistence.dao.impl;

import java.util.List;

import javax.ejb.TransactionAttribute;
import javax.ejb.TransactionAttributeType;

import org.springframework.stereotype.Repository;

import com.knightonline.shared.data.constants.NamedQueriesConstants;
import com.knightonline.shared.persistence.dao.IKingInfoDAO;
import com.knightonline.shared.persistence.entities.KingInfo;

/**
 * @author Mamaorha
 *
 */
@Repository
@TransactionAttribute(value = TransactionAttributeType.REQUIRED)
public class KingInfoHibernateDAO extends AbstractHibernateDAO<KingInfo, Long> implements IKingInfoDAO
{
	@Override
	public List<KingInfo> getKingInfo()
	{
		return executeNamedQuery(NamedQueriesConstants.GET_KING_INFO, true);
	}
	
}
