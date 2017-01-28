package com.knightonline.shared.persistence.dao;

import java.util.List;

import com.knightonline.shared.persistence.entities.KingInfo;

/**
 * @author Mamaorha
 *
 */
public interface IKingInfoDAO extends IGenericDAO<KingInfo, Long>
{
	public List<KingInfo> getKingInfo();
}
