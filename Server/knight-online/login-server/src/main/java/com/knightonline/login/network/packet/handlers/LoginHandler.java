package com.knightonline.login.network.packet.handlers;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Component;

import com.knightonline.login.data.enums.AccountLoginCodesEnum;
import com.knightonline.login.data.enums.LoginResultCodeEnum;
import com.knightonline.shared.network.common.IPacketHandler;
import com.knightonline.shared.network.common.Packet;
import com.knightonline.shared.persistence.dao.IAccountDAO;
import com.knightonline.shared.persistence.dao.IOnlineUserDAO;
import com.knightonline.shared.persistence.entities.OnlineUser;
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
	
	@Override
	public void handlePacket(Packet requestPacket)
	{
		Packet result = new Packet(requestPacket.getOpcode(), requestPacket.getServer());

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

		System.out.println();
	}

	private short accountLogin(String username, String password)
	{		
		short resultCode = LoginResultCodeEnum.AUTH_NOT_FOUND;

		short nret = accountDAO.accountLoginProcedure(username, password);

		//NOTE: in 1298 there is a diff handler for nation, but we will ignore this and just access the fact that they user able to login
		if (nret == AccountLoginCodesEnum.NO_NATION_SELECTED || nret == AccountLoginCodesEnum.KARUS || nret == AccountLoginCodesEnum.ELMORAD)
		{
			List<OnlineUser> onlineUsers = onlineUserDAO.getOnlineUsers();
			
			if(null != onlineUsers && !onlineUsers.isEmpty())
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
			//check if the account is banned
			if(nret == AccountLoginCodesEnum.BANNED)
			{
				resultCode = LoginResultCodeEnum.AUTH_BANNED;
			}
		}

		// NOTE: if the user doesn't have an account or they have
		// a nation other than the ones we are looking for -> return LoginResultCode.AUTH_NOT_FOUND

		return resultCode;
	}
}
