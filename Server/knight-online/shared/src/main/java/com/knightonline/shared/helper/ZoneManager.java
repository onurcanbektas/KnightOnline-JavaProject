package com.knightonline.shared.helper;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.annotation.PostConstruct;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.shared.data.enums.ZoneEnum;
import com.knightonline.shared.persistence.dao.IZoneDAO;
import com.knightonline.shared.persistence.entities.Zone;

/**
 * @author Mamaorha
 *
 */
@Component
public class ZoneManager
{
	@Autowired
	protected IZoneDAO zoneDAO;
	
	protected Map<ZoneEnum, Zone> zones;
	
	@PostConstruct
	private void init()
	{
		zones = new HashMap<>();
		
		List<Zone> zonesList = zoneDAO.getZones();
		
		for (Zone zone : zonesList)
		{
			zones.put(ZoneEnum.forValue(Integer.valueOf(zone.getId())), zone);
		}
		
		for (ZoneEnum temp : ZoneEnum.values())
		{
			if(zones.get(temp) == null)
			{
				throw new InternalError(String.format("DB doesn't have info about %s", temp.name()));
			}
		}
	}
	
	public Zone getBaseZone()
	{
		return zones.get(ZoneEnum.Moradon);
	}
}
