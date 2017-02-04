package com.knightonline.game.network.packet.handlers;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.game.server.GameServer;
import com.knightonline.shared.data.enums.NationEnum;
import com.knightonline.shared.exception.DAOException;
import com.knightonline.shared.network.KOServer;
import com.knightonline.shared.network.packet.Packet;
import com.knightonline.shared.network.packet.handlers.LoggedInHandler;
import com.knightonline.shared.persistence.dao.IAccountDAO;
import com.knightonline.shared.persistence.dao.ICharacterDataDAO;
import com.knightonline.shared.persistence.entities.Account;

/**
 * @author Mamaorha
 *
 */
@Component
public class NationSelectHandler extends LoggedInHandler
{
	@Autowired
	protected IAccountDAO accountDAO;
	
	@Autowired
	protected ICharacterDataDAO characterDataDAO;
	
	@Autowired
	protected GameServer gameServer;

	@Override
	protected KOServer getKOServer()
	{
		return gameServer.getServer();
	}

	@Override
	protected void handlePacketImpl(Packet requestPacket)
	{
		Account account = accountDAO.getAccountByUsername(username);
		
		//make sure the user can select a nation
		if(account.getNation() != NationEnum.NO_NATION)
		{
			System.out.println("Account already have a nation");
			killConnection(requestPacket);
		}
		
		short nation = requestPacket.getByte();
		
		NationEnum nationEnum = NationEnum.forValue((int)nation);
		
		if(null == nationEnum)
		{
			System.out.println("Client sent an invalid nation");
			killConnection(requestPacket);
		}
		
		account.setNation(nationEnum);
		
		try
		{
			accountDAO.merge(account);
			 
			Packet result = new Packet(requestPacket.getOpcode(), requestPacket.getMessageInfo());
			result.appendInt8(nation);	
			
			packetWriter.sendPacket(result);
		}
		
		catch (DAOException e)
		{
			e.printStackTrace();
		}
	}
}
