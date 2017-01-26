package com.knightonline.login.network.packet.handlers;

import java.sql.Timestamp;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.annotation.PostConstruct;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Component;

import com.knightonline.login.data.enums.AccountLoginCodesEnum;
import com.knightonline.login.data.enums.LoginResultCodeEnum;
import com.knightonline.login.server.LoginServer;
import com.knightonline.shared.data.enums.PremiumTypeEnum;
import com.knightonline.shared.network.common.IPacketHandler;
import com.knightonline.shared.network.common.Packet;
import com.knightonline.shared.network.common.PacketWriter;
import com.knightonline.shared.persistence.dao.IAccountDAO;
import com.knightonline.shared.persistence.dao.IOnlineUserDAO;
import com.knightonline.shared.persistence.entities.Account;
import com.knightonline.shared.persistence.entities.OnlineUser;
import com.knightonline.shared.utils.DateUtils;
import com.knightonline.shared.utils.RegexValidator;
import com.knightonline.shared.utils.RegexValidator.Validator;

/**
 * @author Mamaorha
 *
 */
@Component
@Scope("prototype")
public class LoginHandler implements IPacketHandler
{
	@Autowired
	protected RegexValidator regexValidator;

	@Autowired
	protected IAccountDAO accountDAO;

	@Autowired
	protected IOnlineUserDAO onlineUserDAO;

	@Autowired
	protected PacketWriter packetWriter;

	@Autowired
	protected LoginServer loginServer;
	
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
		short resultCode = 0;

		String username = requestPacket.readStringFromByteBuffer();
		String password = requestPacket.readStringFromByteBuffer();

		if (!regexValidator.isValid(Validator.USERNAME, username) || !regexValidator.isValid(Validator.PASSWORD, password))
		{
			resultCode = LoginResultCodeEnum.AUTH_NOT_FOUND;
		}

		else
		{
			resultCode = accountLogin(username, password);
		}

		String authMessage = resultCodeMap.get(resultCode);

		String log = String.format("[ LOGIN - %s ] ID=%s Authentication=%s", DateUtils.getSimpleDate().getTimeFormat(":"), username, authMessage);
		
		System.out.println(log);
		loginServer.appendToUserLog(log);
		
		Packet result = new Packet(requestPacket.getOpcode(), requestPacket.getMessageInfo());
		result.appendInt8(resultCode);
		
		if (resultCode == LoginResultCodeEnum.AUTH_SUCCESS)
		{
			Account account = accountDAO.getAccountByUsername(username);
			PremiumTypeEnum premiumType = PremiumTypeEnum.NONE;
			
			//check if the account still have premium time left
			if(account.getPremiumExpireTime().after(new Timestamp(System.currentTimeMillis())))
			{
				premiumType = account.getPremiumType();
			}
			
			result.appendString(premiumType.getValue());
		}
		
		result.appendString(username);
		packetWriter.sendPacket(result);
	}


	private short accountLogin(String username, String password)
	{
		short resultCode = LoginResultCodeEnum.AUTH_NOT_FOUND;

		short nret = accountDAO.accountLoginProcedure(username, password);

		// NOTE: in 1298 there is a diff handler for nation, but we will ignore
		// this and just access the fact that they user able to login
		if (nret == AccountLoginCodesEnum.NO_NATION_SELECTED || nret == AccountLoginCodesEnum.KARUS || nret == AccountLoginCodesEnum.ELMORAD)
		{
			List<OnlineUser> onlineUsers = onlineUserDAO.getOnlineUsers();

			if (null != onlineUsers && !onlineUsers.isEmpty())
			{
				resultCode = LoginResultCodeEnum.AUTH_IN_GAME;
			}

			else
			{
				resultCode = LoginResultCodeEnum.AUTH_SUCCESS;
			}
		}

		else
		{
			// check if the account is banned
			if (nret == AccountLoginCodesEnum.BANNED)
			{
				resultCode = LoginResultCodeEnum.AUTH_BANNED;
			}
		}

		// NOTE: if the user doesn't have an account or they have
		// a nation other than the ones we are looking for -> return
		// LoginResultCode.AUTH_NOT_FOUND

		return resultCode;
	}
}
