package com.knightonline.game.network.packet.handlers;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.game.data.codes.SelectCharacterCodes;
import com.knightonline.game.server.GameServer;
import com.knightonline.shared.data.common.ConnectedCharacterInfo;
import com.knightonline.shared.data.enums.NationEnum;
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

/**
 * @author Mamaorha
 *
 */
@Component
public class SelectCharacterHandler extends LoggedInHandler
{
	@Autowired
	protected IAccountDAO accountDAO;
	
	@Autowired
	protected ICharacterDataDAO characterDataDAO;

	@Autowired
	protected ZoneManager zoneManager;
	
	@Autowired
	protected GameServer gameServer;
	
	@Autowired
	protected KOServer koServer;
	
	@Override
	protected void handlePacketImpl(Packet requestPacket)
	{
		String characterName = requestPacket.getString();

		CharacterData characterData = characterDataDAO.getCharacterDataByCharacterName(characterName);
		
		byte returnCode;

		if (null == characterData || !characterData.getUsername().equals(username))
		{
			returnCode = SelectCharacterCodes.CHARACTER_SELECT_FAILED;
		}

		else
		{
			Zone zoneinfo = zoneManager.getZoneinfo(characterData.getZone());
			
			boolean validZone = true;
			
			switch (zoneinfo.getType())
			{
				case WAR:
					validZone = gameServer.isWarOpen(characterData.getZone());
					break;
					
				case ELMORAD:
					validZone = accountDAO.getAccountByUsername(characterData.getUsername()).getNation() == NationEnum.ELMORAD;
					break;
					
				case KARUS:
					validZone = accountDAO.getAccountByUsername(characterData.getUsername()).getNation() == NationEnum.KARUS;
					break;
					
				default:
					break;
			}
			
			if(!validZone)
			{
				Zone baseZone = zoneManager.getBaseZone();
				
				characterData.setZone(ZoneEnum.forValue((int)baseZone.getId()));
				characterData.setLocation_x(baseZone.getInit_x());
				characterData.setLocation_y(baseZone.getInit_y());
				characterData.setLocation_z(baseZone.getInit_z());
				
				try
				{
					characterDataDAO.persist(characterData);
				}
				catch (DAOException e)
				{
					e.printStackTrace();
				}
			}
			
			returnCode = SelectCharacterCodes.CHARACTER_SELECT_SUCCESS;
		}

		Packet result = new Packet(requestPacket.getOpcode(), requestPacket.getMessageInfo());
		result.appendInt8(returnCode);

		if (returnCode == SelectCharacterCodes.CHARACTER_SELECT_SUCCESS)
		{
			//zone
			result.appendInt8(characterData.getZone().getValue());
			result.appendInt(characterData.getLocation_x());
			result.appendInt(characterData.getLocation_y());
			result.appendInt(characterData.getLocation_z());
			
			//victory nation
			result.appendInt8(gameServer.getVictoryNation().getValue());
		}

		ConnectedCharacterInfo connectedCharacterInfo = new ConnectedCharacterInfo();
		connectedCharacterInfo.setCharacterData(characterData);
		
		koServer.connectCharacter(requestPacket.getMessageInfo().getChannelId(), connectedCharacterInfo);
		packetWriter.sendPacket(result);
	}
}