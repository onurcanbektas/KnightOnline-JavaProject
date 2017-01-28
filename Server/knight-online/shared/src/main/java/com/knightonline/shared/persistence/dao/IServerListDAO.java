package com.knightonline.shared.persistence.dao;

import java.util.List;

import com.knightonline.shared.persistence.entities.ServerList;

/**
 * @author Mamaorha
 *
 */
public interface IServerListDAO extends IGenericDAO<ServerList, Long>
{
	public List<ServerList> getServerList();
}
