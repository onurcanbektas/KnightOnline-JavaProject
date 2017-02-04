package com.knightonline.shared.network.packet.handlers;

import org.omg.CORBA.StringHolder;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.shared.data.AccountLoginCodes;
import com.knightonline.shared.data.enums.LoginResultCodeEnum;
import com.knightonline.shared.network.packet.Packet;
import com.knightonline.shared.persistence.dao.IAccountDAO;
import com.knightonline.shared.utils.EncryptionUtils;
import com.knightonline.shared.utils.RegexValidator;
import com.knightonline.shared.utils.RegexValidator.Validator;

/**
 * @author Mamaorha
 *
 */
@Component
public class BaseLogin
{
	@Autowired
	protected RegexValidator regexValidator;

	@Autowired
	protected IAccountDAO accountDAO;
	
	public short handlePacket(Packet requestPacket, StringHolder username)
	{
		short resultCode = 0;

		username.value = requestPacket.getString();
		String password = requestPacket.getString();
		
		if (!regexValidator.isValid(Validator.USERNAME, username.value) || !regexValidator.isValid(Validator.PASSWORD, password))
		{
			resultCode = LoginResultCodeEnum.AUTH_NOT_FOUND;
		}

		else
		{
			resultCode = accountLogin(username.value, EncryptionUtils.encryptMD5(password));
		}
		
		return resultCode;
	}
	
	private short accountLogin(String username, String password)
	{
		short resultCode = LoginResultCodeEnum.AUTH_NOT_FOUND;

		short nret = accountDAO.accountLoginProcedure(username, password);

		// NOTE: in 1298 there is a diff handler for nation, but we will ignore
		// this and just access the fact that they user able to login
		if (nret == AccountLoginCodes.NO_NATION_SELECTED || nret == AccountLoginCodes.KARUS || nret == AccountLoginCodes.ELMORAD)
		{
				resultCode = LoginResultCodeEnum.AUTH_SUCCESS;
		}

		else
		{
			// check if the account is banned
			if (nret == AccountLoginCodes.BANNED)
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
