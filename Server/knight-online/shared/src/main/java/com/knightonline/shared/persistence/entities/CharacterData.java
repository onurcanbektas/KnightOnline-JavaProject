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
import com.knightonline.shared.persistence.converter.ConvertHairColorEnum;
import com.knightonline.shared.persistence.converter.ConvertRaceEnum;
import com.knightonline.shared.persistence.converter.ConvertSelectCharacterPositionEnum;
import com.knightonline.shared.persistence.converter.ConvertSpecialityEnum;

/**
 * @author Mamaorha
 *
 */
@javax.persistence.Entity()
@Table(name = "CHARACTER_DATA")
@Cache(usage = CacheConcurrencyStrategy.READ_WRITE)
@NamedQueries({
	@NamedQuery(name = NamedQueriesConstants.GET_CHARACTER_DATA_BY_USERNAME, query = "from CharacterData where username = :username"),
	@NamedQuery(name = NamedQueriesConstants.GET_CHARACTER_DATA_BY_CHARACTER_NAME, query = "from CharacterData where characterName = :characterName") 
	})
public class CharacterData implements Serializable
{
	private static final long serialVersionUID = 8602465771292965198L;

	protected String characterName;
	protected String username;
	protected SelectCharacterPositionEnum selectCharacterPosition;
	protected RaceEnum race;
	protected SpecialityEnum speciality;
	protected HairColorEnum hairColor;
	protected FaceEnum face;
	protected short level;
	//exp
	//loyalty
	//loyaltyMonthly
	//manner_point
	//health_point
	//mana_point
	//strength
	//health
	//dexterity
	//intelligence
	//magic_power
	//character_points
	//skill_point
	//gold
	protected short zone;
	//location_x
	//location_y
	//location_z
	//create_time
	
	@Id()
	@Column(name = "CHARACTER_NAME", unique = true, nullable = false, length = 45)
	public String getCharacterName()
	{
		return characterName;
	}

	public void setCharacterName(String characterName)
	{
		this.characterName = characterName;
	}

	@Basic
	@Column(name = "USERNAME", unique = false, nullable = false, length = 50)
	public String getUsername()
	{
		return username;
	}

	public void setUsername(String username)
	{
		this.username = username;
	}
	
	@Column(name = "SELECT_CHARACTER_POSITION", unique = false, nullable = false)
	@Convert(converter = ConvertSelectCharacterPositionEnum.class)
	public SelectCharacterPositionEnum getSelectCharacterPosition()
	{
		return selectCharacterPosition;
	}

	public void setSelectCharacterPosition(SelectCharacterPositionEnum selectCharacterPosition)
	{
		this.selectCharacterPosition = selectCharacterPosition;
	}

	@Column(name = "RACE", unique = false, nullable = false)
	@Convert(converter = ConvertRaceEnum.class)
	public RaceEnum getRace()
	{
		return race;
	}

	public void setRace(RaceEnum race)
	{
		this.race = race;
	}

	@Column(name = "SPECIALITY", unique = false, nullable = false)
	@Convert(converter = ConvertSpecialityEnum.class)
	public SpecialityEnum getSpeciality()
	{
		return speciality;
	}

	public void setSpeciality(SpecialityEnum speciality)
	{
		this.speciality = speciality;
	}

	@Column(name = "HAIR_COLOR", unique = false, nullable = false)
	@Convert(converter = ConvertHairColorEnum.class)
	public HairColorEnum getHairColor()
	{
		return hairColor;
	}

	public void setHairColor(HairColorEnum hairColor)
	{
		this.hairColor = hairColor;
	}

	@Column(name = "FACE", unique = false, nullable = false)
	@Convert(converter = ConvertHairColorEnum.class)
	public FaceEnum getFace()
	{
		return face;
	}

	public void setFace(FaceEnum face)
	{
		this.face = face;
	}

	@Basic()
	@Column(name = "LEVEL", unique = false, nullable = false)
	public short getLevel()
	{
		return level;
	}

	public void setLevel(short level)
	{
		this.level = level;
	}

	@Basic()
	@Column(name = "ZONE", unique = false, nullable = false)
	public short getZone()
	{
		return zone;
	}

	public void setZone(short zone)
	{
		this.zone = zone;
	}
}
