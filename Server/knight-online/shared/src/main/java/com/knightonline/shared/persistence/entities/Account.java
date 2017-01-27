package com.knightonline.shared.persistence.entities;

import java.io.Serializable;
import java.sql.Timestamp;

import javax.persistence.Basic;
import javax.persistence.Column;
import javax.persistence.Convert;
import javax.persistence.EnumType;
import javax.persistence.Enumerated;
import javax.persistence.Id;
import javax.persistence.Table;

import org.hibernate.annotations.Cache;
import org.hibernate.annotations.CacheConcurrencyStrategy;
import org.hibernate.annotations.NamedQueries;
import org.hibernate.annotations.NamedQuery;

import com.knightonline.shared.data.constants.NamedQueriesConstants;
import com.knightonline.shared.data.enums.AuthorityEnum;
import com.knightonline.shared.data.enums.NationEnum;
import com.knightonline.shared.data.enums.PremiumTypeEnum;
import com.knightonline.shared.persistence.converter.ConvertNationEnum;

/**
 * @author Mamaorha
 *
 */
@javax.persistence.Entity()
@Table(name = "ACCOUNT")
@Cache(usage = CacheConcurrencyStrategy.READ_WRITE)
@NamedQueries({ @NamedQuery(name = NamedQueriesConstants.GET_ACCOUNT_BY_USERNAME, query = "from Account where username = :username") })
public class Account implements Serializable
{
	private static final long serialVersionUID = -7246922798686452510L;

	protected String username;
	protected String password;
	protected String email;
	protected Timestamp premiumExpireTime;
	protected PremiumTypeEnum premiumType;
	protected AuthorityEnum authority;
	protected NationEnum nation;
	
	@Id()
	@Column(name = "USERNAME", unique = true, nullable = false, length = 50)
	public String getUsername()
	{
		return username;
	}

	public void setUsername(String username)
	{
		this.username = username;
	}

	@Basic()
	@Column(name = "PASSWORD", unique = false, nullable = false, length = 150)
	public String getPassword()
	{
		return password;
	}

	public void setPassword(String password)
	{
		this.password = password;
	}

	@Basic()
	@Column(name = "EMAIL", unique = false, nullable = false, length = 250)
	public String getEmail()
	{
		return email;
	}

	public void setEmail(String email)
	{
		this.email = email;
	}

	@Basic()
	@Column(name = "PREMIUM_EXPIRE_TIME", unique = false, nullable = true, precision = 6)
	public Timestamp getPremiumExpireTime()
	{
		return premiumExpireTime;
	}

	public void setPremiumExpireTime(Timestamp premiumExpireTime)
	{
		this.premiumExpireTime = premiumExpireTime;
	}

	@Enumerated(EnumType.STRING)
	@Column(name = "PREMIUM_TYPE", nullable = true)
	public PremiumTypeEnum getPremiumType()
	{
		return premiumType;
	}

	public void setPremiumType(PremiumTypeEnum premiumType)
	{
		this.premiumType = premiumType;
	}

	@Enumerated(EnumType.STRING)
	@Column(name = "AUTHORITY", nullable = true)
	public AuthorityEnum getAuthority()
	{
		return authority;
	}

	public void setAuthority(AuthorityEnum authority)
	{
		this.authority = authority;
	}

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

	@Override
	public int hashCode()
	{
		final int prime = 31;
		int result = 1;
		result = prime * result + ((authority == null) ? 0 : authority.hashCode());
		result = prime * result + ((email == null) ? 0 : email.hashCode());
		result = prime * result + ((nation == null) ? 0 : nation.hashCode());
		result = prime * result + ((password == null) ? 0 : password.hashCode());
		result = prime * result + ((premiumExpireTime == null) ? 0 : premiumExpireTime.hashCode());
		result = prime * result + ((premiumType == null) ? 0 : premiumType.hashCode());
		result = prime * result + ((username == null) ? 0 : username.hashCode());
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
		Account other = (Account) obj;
		if (authority != other.authority)
			return false;
		if (email == null)
		{
			if (other.email != null)
				return false;
		}
		else if (!email.equals(other.email))
			return false;
		if (nation != other.nation)
			return false;
		if (password == null)
		{
			if (other.password != null)
				return false;
		}
		else if (!password.equals(other.password))
			return false;
		if (premiumExpireTime == null)
		{
			if (other.premiumExpireTime != null)
				return false;
		}
		else if (!premiumExpireTime.equals(other.premiumExpireTime))
			return false;
		if (premiumType != other.premiumType)
			return false;
		if (username == null)
		{
			if (other.username != null)
				return false;
		}
		else if (!username.equals(other.username))
			return false;
		return true;
	}

	@Override
	public String toString()
	{
		return "Account [username=" + username + ", password=" + password + ", email=" + email + ", premiumExpireTime=" + premiumExpireTime + ", premiumType=" + premiumType + ", authority=" + authority + ", nation=" + nation + "]";
	}
}
