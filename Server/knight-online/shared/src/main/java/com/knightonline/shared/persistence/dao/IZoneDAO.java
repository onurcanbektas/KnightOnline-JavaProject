package com.knightonline.shared.persistence.dao;

import java.util.List;

import com.knightonline.shared.persistence.entities.Zone;

/**
 * @author Mamaorha
 *
 */
public interface IZoneDAO extends IGenericDAO<Zone, Long>
{
	public List<Zone> getZones();
}
