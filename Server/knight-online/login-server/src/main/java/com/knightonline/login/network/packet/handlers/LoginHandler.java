package com.knightonline.login.network.packet.handlers;

import java.sql.Timestamp;
import java.util.HashMap;
import java.util.Map;

import javax.annotation.PostConstruct;

import org.omg.CORBA.StringHolder;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.login.server.LoginServer;
import com.knightonline.shared.data.enums.LoginResultCodeEnum;
import com.knightonline.shared.data.enums.PremiumEnum;
import com.knightonline.shared.network.packet.IPacketHandler;
import com.knightonline.shared.network.packet.Packet;
import com.knightonline.shared.network.packet.PacketWriter;
import com.knightonline.shared.network.packet.handlers.BaseLogin;
import com.knightonline.shared.persistence.dao.IAccountDAO;
import com.knightonline.shared.persistence.entities.Account;
import com.knightonline.shared.utils.DateUtils;

/**
 * @author Mamaorha
 *
 */
@Component
public class LoginHandler implements IPacketHandler
{
	@Autowired
	protected BaseLogin baseLogin;
	
	@Autowired
	protected PacketWriter packetWriter;

	@Autowired
	protected LoginServer loginServer;
	
	@Autowired
	protected IAccountDAO accountDAO;
	
	private Map<Short, String> resultCodeMap;

	@PostConstruct
	private void init()
	{
		resultCodeMap = new HashMap<>();
		resultCodeMap.put(LoginResultCodeEnum.AUTH_NOT_FOUND, "NOT FOUND");
		resultCodeMap.put(LoginResultCodeEnum.AUTH_IN_GAME, "IN GAME");
		resultCodeMap.put(LoginResultCodeEnum.AUTH_BANNED, "BANNED");
		resultCodeMap.put(LoginResultCodeEnum.AUTH_SUCCESS, "SUCCESS");
	}

	@Override
	public void handlePacket(Packet requestPacket)
	{
		StringHolder username = new StringHolder();
		
		short resultCode = baseLogin.handlePacket(requestPacket, username);

		String authMessage = resultCodeMap.get(resultCode);

		String log = String.format("[ LOGIN - %s ] ID=%s Authentication=%s", DateUtils.getSimpleDate().getTimeFormat(":"), username.value, authMessage);
		
		System.out.println(log);
		loginServer.appendToUserLog(log);
		
		Packet result = new Packet(requestPacket.getOpcode(), requestPacket.getMessageInfo());
		result.appendInt8(resultCode);	
		
		if(resultCode == LoginResultCodeEnum.AUTH_SUCCESS)
		{
			Account account = accountDAO.getAccountByUsername(username.value);
			PremiumEnum premium = PremiumEnum.NONE;
			
			//check if the account still have premium time left
			if(account.getPremiumExpireTime().after(new Timestamp(System.currentTimeMillis())))
			{
				premium = account.getPremium();
			}
			
			result.appendInt8(premium.getValue());
			result.appendString(DateUtils.getDate(account.getPremiumExpireTime()));
			result.appendInt8(account.getNation().getValue());
		}
		
		packetWriter.sendPacket(result);
	}
}
