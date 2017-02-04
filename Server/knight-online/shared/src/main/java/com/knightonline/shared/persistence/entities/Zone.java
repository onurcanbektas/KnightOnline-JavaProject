package com.knightonline.shared.persistence.entities;

import java.io.Serializable;

import javax.persistence.Basic;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;

import org.hibernate.annotations.Cache;
import org.hibernate.annotations.CacheConcurrencyStrategy;
import org.hibernate.annotations.NamedQueries;
import org.hibernate.annotations.NamedQuery;

import com.knightonline.shared.data.constants.NamedQueriesConstants;

/**
 * @author Mamaorha
 *
 */
@Entity()
@Table(name = "ZONE")
@Cache(usage = CacheConcurrencyStrategy.READ_WRITE)
@NamedQueries({ @NamedQuery(name = NamedQueriesConstants.GET_ZONES, query = "from Zone") })
public class Zone implements Serializable
{
	private static final long serialVersionUID = -7246922798686452510L;

	protected short id;
	protected String name;
	protected String smd;
	protected int init_x;
	protected int init_y;
	protected int init_z;
	protected short type;
	protected short roomEvent;

	@Id()
	@Column(name = "ID", unique = true, nullable = false)	
	public short getId()
	{
		return id;
	}

	public void setId(short id)
	{
		this.id = id;
	}
	
	@Basic()
	@Column(name = "NAME", unique = true, nullable = false, length = 50)
	public String getName()
	{
		return name;
	}

	public void setName(String name)
	{
		this.name = name;
	}

	@Basic()
	@Column(name = "SMD", unique = false, nullable = false, length = 50)
	public String getSmd()
	{
		return smd;
	}

	public void setSmd(String smd)
	{
		this.smd = smd;
	}

	@Basic()
	@Column(name = "INIT_X", unique = false, nullable = false)
	public int getInit_x()
	{
		return init_x;
	}

	public void setInit_x(int init_x)
	{
		this.init_x = init_x;
	}

	@Basic()
	@Column(name = "INIT_Y", unique = false, nullable = false)
	public int getInit_y()
	{
		return init_y;
	}

	public void setInit_y(int init_y)
	{
		this.init_y = init_y;
	}

	@Basic()
	@Column(name = "INIT_Z", unique = false, nullable = false)
	public int getInit_z()
	{
		return init_z;
	}

	public void setInit_z(int init_z)
	{
		this.init_z = init_z;
	}

	@Basic()
	@Column(name = "TYPE", unique = false, nullable = false)
	public short getType()
	{
		return type;
	}

	public void setType(short type)
	{
		this.type = type;
	}

	@Basic()
	@Column(name = "ROOM_EVENT", unique = false, nullable = false)
	public short getRoomEvent()
	{
		return roomEvent;
	}

	public void setRoomEvent(short roomEvent)
	{
		this.roomEvent = roomEvent;
	}

	@Override
	public int hashCode()
	{
		final int prime = 31;
		int result = 1;
		result = prime * result + id;
		result = prime * result + init_x;
		result = prime * result + init_y;
		result = prime * result + init_z;
		result = prime * result + ((name == null) ? 0 : name.hashCode());
		result = prime * result + roomEvent;
		result = prime * result + ((smd == null) ? 0 : smd.hashCode());
		result = prime * result + type;
		return result;
	}

	@Override
	public boolean equals(Object obj)
	{
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		Zone other = (Zone) obj;
		if (id != other.id)
			return false;
		if (init_x != other.init_x)
			return false;
		if (init_y != other.init_y)
			return false;
		if (init_z != other.init_z)
			return false;
		if (name == null)
		{
			if (other.name != null)
				return false;
		}
		else if (!name.equals(other.name))
			return false;
		if (roomEvent != other.roomEvent)
			return false;
		if (smd == null)
		{
			if (other.smd != null)
				return false;
		}
		else if (!smd.equals(other.smd))
			return false;
		if (type != other.type)
			return false;
		return true;
	}

	@Override
	public String toString()
	{
		return "Zone [id=" + id + ", name=" + name + ", smd=" + smd + ", init_x=" + init_x + ", init_y=" + init_y + ", init_z=" + init_z + ", type=" + type + ", roomEvent=" + roomEvent + "]";
	}
}
