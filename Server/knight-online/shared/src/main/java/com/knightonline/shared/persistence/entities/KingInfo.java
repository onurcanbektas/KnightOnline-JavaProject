package com.knightonline.shared.persistence.entities;

import java.io.Serializable;

import javax.persistence.Basic;
import javax.persistence.Column;
import javax.persistence.Convert;
import javax.persistence.Id;
import javax.persistence.Table;

import org.hibernate.annotations.Cache;
import org.hibernate.annotations.CacheConcurrencyStrategy;
import org.hibernate.annotations.NamedQueries;
import org.hibernate.annotations.NamedQuery;

import com.knightonline.shared.data.constants.NamedQueriesConstants;
import com.knightonline.shared.data.enums.NationEnum;
import com.knightonline.shared.persistence.converter.ConvertNationEnum;

/**
 * @author Mamaorha
 *
 */
@javax.persistence.Entity()
@Table(name = "KING_INFO")
@Cache(usage = CacheConcurrencyStrategy.READ_WRITE)
@NamedQueries({ @NamedQuery(name = NamedQueriesConstants.GET_KING_INFO, query = "from KingInfo") })
public class KingInfo implements Serializable
{
	private static final long serialVersionUID = 7271462313730806789L;

	protected NationEnum nation;
	protected String chararcterName;
	protected String kingMessage;

	@Id()
	@Column(name = "NATION", nullable = false)
	@Convert(converter = ConvertNationEnum.class)
	public NationEnum getNation()
	{
		return nation;
	}

	public void setNation(NationEnum nation)
	{
		this.nation = nation;
	}

	@Basic
	@Column(name = "CHARACTER_NAME", unique = false, nullable = false, length = 45)
	public String getChararcterName()
	{
		return chararcterName;
	}

	public void setChararcterName(String chararcterName)
	{
		this.chararcterName = chararcterName;
	}

	@Basic
	@Column(name = "KING_MESSAGE", unique = false, nullable = false, length = 150)
	public String getKingMessage()
	{
		return kingMessage;
	}

	public void setKingMessage(String kingMessage)
	{
		this.kingMessage = kingMessage;
	}

	@Override
	public int hashCode()
	{
		final int prime = 31;
		int result = 1;
		result = prime * result + ((chararcterName == null) ? 0 : chararcterName.hashCode());
		result = prime * result + ((kingMessage == null) ? 0 : kingMessage.hashCode());
		result = prime * result + ((nation == null) ? 0 : nation.hashCode());
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
		KingInfo other = (KingInfo) obj;
		if (chararcterName == null)
		{
			if (other.chararcterName != null)
				return false;
		}
		else if (!chararcterName.equals(other.chararcterName))
			return false;
		if (kingMessage == null)
		{
			if (other.kingMessage != null)
				return false;
		}
		else if (!kingMessage.equals(other.kingMessage))
			return false;
		if (nation != other.nation)
			return false;
		return true;
	}

	@Override
	public String toString()
	{
		return "KingInfo [nation=" + nation + ", chararcterName=" + chararcterName + ", kingMessage=" + kingMessage + "]";
	}
}
