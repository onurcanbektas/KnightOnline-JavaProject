package com.knightonline.game.network.packet.handlers;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.game.data.codes.DeleteCharacterCodes;
import com.knightonline.shared.data.enums.NationEnum;
import com.knightonline.shared.exception.DAOException;
import com.knightonline.shared.network.KOServer;
import com.knightonline.shared.network.packet.Packet;
import com.knightonline.shared.network.packet.handlers.LoggedInHandler;
import com.knightonline.shared.persistence.dao.IAccountDAO;
import com.knightonline.shared.persistence.dao.ICharacterDataDAO;
import com.knightonline.shared.persistence.entities.Account;
import com.knightonline.shared.persistence.entities.CharacterData;

/**
 * @author Mamaorha
 *
 */
@Component
public class DeleteCharacterHandler extends LoggedInHandler
{
	@Autowired
	protected IAccountDAO accountDAO;

	@Autowired
	protected ICharacterDataDAO characterDataDAO;

	@Autowired
	protected KOServer koServer;
	
	@Override
	protected void handlePacketImpl(Packet requestPacket)
	{
		String characterName = requestPacket.getString();

		Packet result = new Packet(requestPacket.getOpcode(), requestPacket.getMessageInfo());
		result.appendInt8(getResult(characterName));

		packetWriter.sendPacket(result);
	}

	private byte getResult(String characterName)
	{
		CharacterData characterDataByCharacterName = characterDataDAO.getCharacterDataByCharacterName(characterName);

		if (null == characterDataByCharacterName || !username.equals(characterDataByCharacterName.getUsername()))
		{
			return DeleteCharacterCodes.CHARACTER_DOESNT_EXIST;
		}

		if(koServer.isConnectedAccount(username))
		{
			System.out.println("Can't delete character while the username have a connected character");
			return DeleteCharacterCodes.CHARACTER_DELETE_FAILED;
		}
		
		try
		{
			characterDataDAO.remove(characterDataByCharacterName);
		}
		catch (DAOException e)
		{
			e.printStackTrace();
			return DeleteCharacterCodes.CHARACTER_DELETE_FAILED;
		}

		//if the user doesnt have more character - reset its nation
		if (characterDataDAO.getCharacterDataByUsername(username).size() == 0)
		{
			Account accountByUsername = accountDAO.getAccountByUsername(username);
			accountByUsername.setNation(NationEnum.NO_NATION);

			try
			{
				accountDAO.merge(accountByUsername);
			}
			catch (DAOException e)
			{
				e.printStackTrace();
			}
		}

		//TODO - kill character if currently active!
		return DeleteCharacterCodes.CHARACTER_DELETE_SUCCESS;
	}
}
