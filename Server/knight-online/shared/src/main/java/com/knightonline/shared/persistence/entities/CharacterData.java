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
import com.knightonline.shared.data.enums.FaceEnum;
import com.knightonline.shared.data.enums.HairColorEnum;
import com.knightonline.shared.data.enums.RaceEnum;
import com.knightonline.shared.data.enums.SelectCharacterPositionEnum;
import com.knightonline.shared.data.enums.SpecialityEnum;
import com.knightonline.shared.data.enums.ZoneEnum;
import com.knightonline.shared.persistence.converter.ConvertFaceEnum;
import com.knightonline.shared.persistence.converter.ConvertHairColorEnum;
import com.knightonline.shared.persistence.converter.ConvertRaceEnum;
import com.knightonline.shared.persistence.converter.ConvertSelectCharacterPositionEnum;
import com.knightonline.shared.persistence.converter.ConvertSpecialityEnum;
import com.knightonline.shared.persistence.converter.ConvertZoneEnum;

/**
 * @author Mamaorha
 *
 */
@javax.persistence.Entity()
@Table(name = "CHARACTER_DATA")
@Cache(usage = CacheConcurrencyStrategy.READ_WRITE)
@NamedQueries({ @NamedQuery(name = NamedQueriesConstants.GET_CHARACTER_DATA_BY_USERNAME, query = "from CharacterData where username = :username"), @NamedQuery(name = NamedQueriesConstants.GET_CHARACTER_DATA_BY_CHARACTER_NAME, query = "from CharacterData where characterName = :characterName") })
public class CharacterData implements Serializable
{
	private static final long serialVersionUID = 8602465771292965198L;

	@Id()
	@Column(name = "CHARACTER_NAME", unique = true, nullable = false, length = 45)
	protected String characterName;

	@Basic
	@Column(name = "USERNAME", unique = false, nullable = false, length = 50)
	protected String username;

	@Column(name = "SELECT_CHARACTER_POSITION", unique = false, nullable = false)
	@Convert(converter = ConvertSelectCharacterPositionEnum.class)
	protected SelectCharacterPositionEnum selectCharacterPosition;

	@Column(name = "RACE", unique = false, nullable = false)
	@Convert(converter = ConvertRaceEnum.class)
	protected RaceEnum race;

	@Column(name = "SPECIALITY", unique = false, nullable = false)
	@Convert(converter = ConvertSpecialityEnum.class)
	protected SpecialityEnum speciality;

	@Column(name = "HAIR_COLOR", unique = false, nullable = false)
	@Convert(converter = ConvertHairColorEnum.class)
	protected HairColorEnum hairColor;

	@Column(name = "FACE", unique = false, nullable = false)
	@Convert(converter = ConvertFaceEnum.class)
	protected FaceEnum face;

	@Basic()
	@Column(name = "LEVEL", unique = false, nullable = false)
	protected short level;
	// exp
	// loyalty
	// loyaltyMonthly
	// manner_point
	// health_point
	// mana_point
	@Basic()
	@Column(name = "STRENGTH", unique = false, nullable = false)
	protected short strength;

	@Basic()
	@Column(name = "STAMINA", unique = false, nullable = false)
	protected short stamina;

	@Basic()
	@Column(name = "DEXTERITY", unique = false, nullable = false)
	protected short dexterity;

	@Basic()
	@Column(name = "INTELLIGENCE", unique = false, nullable = false)
	protected short intelligence;

	@Basic()
	@Column(name = "MAGIC_POWER", unique = false, nullable = false)
	protected short magicPower;

	@Basic()
	@Column(name = "CHARACTER_POINTS", unique = false, nullable = false)
	protected short characterPoints;
	// skill_point
	// gold

	@Column(name = "ZONE", unique = false, nullable = false)
	@Convert(converter = ConvertZoneEnum.class)
	protected ZoneEnum zone;

	@Basic()
	@Column(name = "LOCATION_X", unique = false, nullable = false)
	protected int location_x;

	@Basic()
	@Column(name = "LOCATION_Y", unique = false, nullable = false)
	protected int location_y;

	@Basic()
	@Column(name = "LOCATION_Z", unique = false, nullable = false)
	protected int location_z;
	// protected Timestamp create_time;
	// protected Timestamp update_time;

	public String getCharacterName()
	{
		return characterName;
	}

	public void setCharacterName(String characterName)
	{
		this.characterName = characterName;
	}

	public String getUsername()
	{
		return username;
	}

	public void setUsername(String username)
	{
		this.username = username;
	}

	public SelectCharacterPositionEnum getSelectCharacterPosition()
	{
		return selectCharacterPosition;
	}

	public void setSelectCharacterPosition(SelectCharacterPositionEnum selectCharacterPosition)
	{
		this.selectCharacterPosition = selectCharacterPosition;
	}

	public RaceEnum getRace()
	{
		return race;
	}

	public void setRace(RaceEnum race)
	{
		this.race = race;
	}

	public SpecialityEnum getSpeciality()
	{
		return speciality;
	}

	public void setSpeciality(SpecialityEnum speciality)
	{
		this.speciality = speciality;
	}

	public HairColorEnum getHairColor()
	{
		return hairColor;
	}

	public void setHairColor(HairColorEnum hairColor)
	{
		this.hairColor = hairColor;
	}

	public FaceEnum getFace()
	{
		return face;
	}

	public void setFace(FaceEnum face)
	{
		this.face = face;
	}

	public short getLevel()
	{
		return level;
	}

	public void setLevel(short level)
	{
		this.level = level;
	}

	public short getStrength()
	{
		return strength;
	}

	public void setStrength(short strength)
	{
		this.strength = strength;
	}

	public short getStamina()
	{
		return stamina;
	}

	public void setStamina(short stamina)
	{
		this.stamina = stamina;
	}

	public short getDexterity()
	{
		return dexterity;
	}

	public void setDexterity(short dexterity)
	{
		this.dexterity = dexterity;
	}

	public short getIntelligence()
	{
		return intelligence;
	}

	public void setIntelligence(short intelligence)
	{
		this.intelligence = intelligence;
	}

	public short getMagicPower()
	{
		return magicPower;
	}

	public void setMagicPower(short magicPower)
	{
		this.magicPower = magicPower;
	}

	public short getCharacterPoints()
	{
		return characterPoints;
	}

	public void setCharacterPoints(short characterPoints)
	{
		this.characterPoints = characterPoints;
	}

	public ZoneEnum getZone()
	{
		return zone;
	}

	public void setZone(ZoneEnum zone)
	{
		this.zone = zone;
	}

	public int getLocation_x()
	{
		return location_x;
	}

	public void setLocation_x(int location_x)
	{
		this.location_x = location_x;
	}

	public int getLocation_y()
	{
		return location_y;
	}

	public void setLocation_y(int location_y)
	{
		this.location_y = location_y;
	}

	public int getLocation_z()
	{
		return location_z;
	}

	public void setLocation_z(int location_z)
	{
		this.location_z = location_z;
	}
	
	@Override
	public int hashCode()
	{
		final int prime = 31;
		int result = 1;
		result = prime * result + ((characterName == null) ? 0 : characterName.hashCode());
		result = prime * result + characterPoints;
		result = prime * result + dexterity;
		result = prime * result + ((face == null) ? 0 : face.hashCode());
		result = prime * result + ((hairColor == null) ? 0 : hairColor.hashCode());
		result = prime * result + stamina;
		result = prime * result + intelligence;
		result = prime * result + level;
		result = prime * result + location_x;
		result = prime * result + location_y;
		result = prime * result + location_z;
		result = prime * result + magicPower;
		result = prime * result + ((race == null) ? 0 : race.hashCode());
		result = prime * result + ((selectCharacterPosition == null) ? 0 : selectCharacterPosition.hashCode());
		result = prime * result + ((speciality == null) ? 0 : speciality.hashCode());
		result = prime * result + strength;
		result = prime * result + ((username == null) ? 0 : username.hashCode());
		result = prime * result + ((zone == null) ? 0 : zone.hashCode());
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
		CharacterData other = (CharacterData) obj;
		if (characterName == null)
		{
			if (other.characterName != null)
				return false;
		}
		else if (!characterName.equals(other.characterName))
			return false;
		if (characterPoints != other.characterPoints)
			return false;
		if (dexterity != other.dexterity)
			return false;
		if (face != other.face)
			return false;
		if (hairColor != other.hairColor)
			return false;
		if (stamina != other.stamina)
			return false;
		if (intelligence != other.intelligence)
			return false;
		if (level != other.level)
			return false;
		if (location_x != other.location_x)
			return false;
		if (location_y != other.location_y)
			return false;
		if (location_z != other.location_z)
			return false;
		if (magicPower != other.magicPower)
			return false;
		if (race != other.race)
			return false;
		if (selectCharacterPosition != other.selectCharacterPosition)
			return false;
		if (speciality != other.speciality)
			return false;
		if (strength != other.strength)
			return false;
		if (username == null)
		{
			if (other.username != null)
				return false;
		}
		else if (!username.equals(other.username))
			return false;
		if (zone != other.zone)
			return false;
		return true;
	}

	@Override
	public String toString()
	{
		return "CharacterData [characterName=" + characterName + ", username=" + username + ", selectCharacterPosition=" + selectCharacterPosition + ", race=" + race + ", speciality=" + speciality + ", hairColor=" + hairColor + ", face=" + face + ", level=" + level + ", strength=" + strength + ", stamina=" + stamina + ", dexterity=" + dexterity + ", intelligence=" + intelligence + ", magicPower=" + magicPower + ", characterPoints=" + characterPoints + ", zone=" + zone + ", location_x=" + location_x + ", location_y=" + location_y + ", location_z=" + location_z + "]";
	}
}
