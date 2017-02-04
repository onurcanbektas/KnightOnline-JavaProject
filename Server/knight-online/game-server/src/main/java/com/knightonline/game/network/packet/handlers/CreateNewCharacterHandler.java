package com.knightonline.game.network.packet.handlers;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.game.data.CreateCharacterCodes;
import com.knightonline.game.server.GameServer;
import com.knightonline.shared.data.enums.FaceEnum;
import com.knightonline.shared.data.enums.HairColorEnum;
import com.knightonline.shared.data.enums.NationEnum;
import com.knightonline.shared.data.enums.RaceEnum;
import com.knightonline.shared.data.enums.SelectCharacterPositionEnum;
import com.knightonline.shared.data.enums.SpecialityEnum;
import com.knightonline.shared.data.enums.ZoneEnum;
import com.knightonline.shared.exception.DAOException;
import com.knightonline.shared.helper.ZoneManager;
import com.knightonline.shared.network.KOServer;
import com.knightonline.shared.network.packet.Packet;
import com.knightonline.shared.network.packet.handlers.LoggedInHandler;
import com.knightonline.shared.persistence.dao.IAccountDAO;
import com.knightonline.shared.persistence.dao.ICharacterDataDAO;
import com.knightonline.shared.persistence.entities.CharacterData;
import com.knightonline.shared.persistence.entities.Zone;
import com.knightonline.shared.utils.RegexValidator;
import com.knightonline.shared.utils.RegexValidator.Validator;

/**
 * @author Mamaorha
 *
 */
@Component
public class CreateNewCharacterHandler extends LoggedInHandler
{
	@Autowired
	protected GameServer gameServer;

	@Autowired
	protected IAccountDAO accountDAO;
	
	@Autowired
	protected ICharacterDataDAO characterDataDAO;

	@Autowired
	protected ZoneManager zoneManager;
	
	@Autowired
	protected RegexValidator regexValidator;

	@Override
	protected KOServer getKOServer()
	{
		return gameServer.getServer();
	}

	@Override
	protected void handlePacketImpl(Packet requestPacket)
	{
		Packet result = new Packet(requestPacket.getOpcode(), requestPacket.getMessageInfo());
		result.appendInt8(getResultCode(requestPacket));

		packetWriter.sendPacket(result);
	}

	private short getResultCode(Packet requestPacket)
	{
		List<CharacterData> characters = characterDataDAO.getCharacterDataByUsername(username);

		// can't open new characters
		if (characters.size() >= SelectCharacterPositionEnum.values().length)
		{
			return CreateCharacterCodes.NO_MORE_CHARACTER;
		}

		SelectCharacterPositionEnum selectCharacterPosition = SelectCharacterPositionEnum.forValue((int) requestPacket.getByte());

		if (null == selectCharacterPosition)
		{
			return CreateCharacterCodes.INVALID_CHARACTER_POSITION;
		}

		String characterName = requestPacket.getString();

		if (!regexValidator.isValid(Validator.CHARACTER_NAME, characterName))
		{
			return CreateCharacterCodes.INVALID_CHARACTER_NAME;
		}
		
		RaceEnum race = RaceEnum.forValue((int) requestPacket.getByte());
		
		if(null == race)
		{
			return CreateCharacterCodes.INVALID_RACE;
		}
		
		NationEnum nation = accountDAO.getAccountByUsername(username).getNation();
		
		if(nation == NationEnum.NO_NATION)
		{
			killConnection(requestPacket);
		}
		
		if(!race.name().startsWith(nation.name()))
		{
			return CreateCharacterCodes.INVALID_NATION_AND_INVALID_RACE;
		}
		
		SpecialityEnum speciality = SpecialityEnum.forValue(Integer.valueOf(requestPacket.getShort()));
		
		if(null == speciality || !speciality.name().startsWith(nation.name()))
		{	
			return CreateCharacterCodes.INVALID_SPECIALITY;
		}
		
		FaceEnum face = FaceEnum.forValue((int) requestPacket.getByte());
		HairColorEnum hairColor = HairColorEnum.forValue((int) requestPacket.getByte());

		short strength = requestPacket.getByte();
		short stamina = requestPacket.getByte();
		short dexterity = requestPacket.getByte();
		short intelligence = requestPacket.getByte();
		short magicPower = requestPacket.getByte();

		short minStrength;
		short minStamina;
		short mindexterity;
		short minIntelligence;
		short minMagicPower;
		
		short sparePoints = 10;
		
		switch (race)
		{
			case ELMORAD_BABARIAN:
			case KARUS_ARK_TUAREK:
			case KARUS_TUAREK:
				minStrength = 65;
				minStamina = 65;
				mindexterity = 60;
				minIntelligence = 50;
				minMagicPower = 50;
				break;
				
			case ELMORAD_MAN:
				minStrength = 60;
				minStamina = 60;
				mindexterity = 70;
				minIntelligence = 50;
				minMagicPower = 50;
				break;
			
			case ELMORAD_WOMEN:
			case KARUS_WRINKLE_TUAREK:
				minStrength = 50;
				minStamina = 50;
				mindexterity = 70;
				minIntelligence = 70;
				minMagicPower = 50;
				break;
				
			case KARUS_PURI_TUAREK:
				minStrength = 50;
				minStamina = 60;
				mindexterity = 60;
				minIntelligence = 70;
				minMagicPower = 50;
				break;
				
				default:
					return CreateCharacterCodes.INVALID_NATION_AND_INVALID_RACE;
					
		}
		
		//validate min points
		if(strength < minStrength  || stamina < minStamina || dexterity < mindexterity || intelligence < minIntelligence || magicPower < minMagicPower)
		{
			return CreateCharacterCodes.INVALID_STAT_POINT;
		}
		
		sparePoints -= strength - minStrength;
		sparePoints -= stamina - minStamina;
		sparePoints -= dexterity - mindexterity;
		sparePoints -= intelligence - minIntelligence;
		sparePoints -= magicPower - minMagicPower;
		
		//user tried to give more than 10 points
		if(sparePoints < 0)
		{
			return CreateCharacterCodes.INVALID_STAT_POINT;
		}
		
		CharacterData characterData = new CharacterData();
		characterData.setUsername(username);
		characterData.setCharacterName(characterName);
		characterData.setSelectCharacterPosition(selectCharacterPosition);
		characterData.setRace(race);
		characterData.setSpeciality(speciality);
		characterData.setHairColor(hairColor);
		characterData.setFace(face);
		characterData.setLevel((short)1);
		characterData.setStrength(strength);
		characterData.setStamina(stamina);
		characterData.setDexterity(dexterity);
		characterData.setIntelligence(intelligence);
		characterData.setMagicPower(magicPower);
		characterData.setCharacterPoints(sparePoints);
		
		Zone baseZone = zoneManager.getBaseZone();
		characterData.setZone(ZoneEnum.forValue(Integer.valueOf(baseZone.getId())));
		characterData.setLocation_x(baseZone.getInit_x());
		characterData.setLocation_y(baseZone.getInit_y());
		characterData.setLocation_z(baseZone.getInit_z());
		
		try
		{
			characterDataDAO.persist(characterData);
		}
		
		catch (DAOException e)
		{
			return CreateCharacterCodes.OVERLAPPED_CHARACTER_NAME;
		}
		
		return CreateCharacterCodes.CHARACTER_CREATE_SUCCESS;
	}
}
