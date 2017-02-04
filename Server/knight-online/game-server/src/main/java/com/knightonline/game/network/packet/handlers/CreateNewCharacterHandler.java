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
import com.knightonline.shared.network.KOServer;
import com.knightonline.shared.network.packet.Packet;
import com.knightonline.shared.network.packet.handlers.LoggedInHandler;
import com.knightonline.shared.persistence.dao.IAccountDAO;
import com.knightonline.shared.persistence.dao.ICharacterDataDAO;
import com.knightonline.shared.persistence.entities.CharacterData;
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
	protected ICharacterDataDAO accountCharacterDAO;

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
		List<CharacterData> characters = accountCharacterDAO.getCharacterDataByUsername(username);

		// can't open new characters
		if (characters.size() >= SelectCharacterPositionEnum.values().length)
		{
			return CreateCharacterCodes.NO_MORE_CHARACTER;
		}

		SelectCharacterPositionEnum selectCharacterPositionEnum = SelectCharacterPositionEnum.forValue((int) requestPacket.getByte());

		if (null == selectCharacterPositionEnum)
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
		
		SpecialityEnum speciality = SpecialityEnum.forValue(requestPacket.getShort());
		
		if(null == speciality || !speciality.name().startsWith("LOW"))
		{	
			return CreateCharacterCodes.INVALID_SPECIALITY;
		}
		
		FaceEnum face = FaceEnum.forValue((int) requestPacket.getByte());
		HairColorEnum hairColor = HairColorEnum.forValue((int) requestPacket.getByte());

		short strength = requestPacket.getByte();
		short stamina = requestPacket.getByte();
		short dextirity = requestPacket.getByte();
		short intelligence = requestPacket.getByte();
		short magicAttack = requestPacket.getByte();

		return CreateCharacterCodes.CHARACTER_CREATE_SUCCESS;
	}
}
