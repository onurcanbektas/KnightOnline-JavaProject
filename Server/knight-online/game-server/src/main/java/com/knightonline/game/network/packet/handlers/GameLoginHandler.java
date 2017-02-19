package com.knightonline.game.network.packet.handlers;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.InetAddress;
import java.net.URL;
import java.net.URLConnection;
import java.sql.Timestamp;

import javax.annotation.PostConstruct;

import org.omg.CORBA.StringHolder;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.shared.data.LoginResultCodeEnum;
import com.knightonline.shared.data.enums.PremiumEnum;
import com.knightonline.shared.exception.InternalServerError;
import com.knightonline.shared.network.KOServer;
import com.knightonline.shared.network.packet.Packet;
import com.knightonline.shared.network.packet.PacketHandlerBase;
import com.knightonline.shared.network.packet.PacketWriter;
import com.knightonline.shared.network.packet.handlers.BaseLogin;
import com.knightonline.shared.persistence.dao.IAccountDAO;
import com.knightonline.shared.persistence.dao.IServerListDAO;
import com.knightonline.shared.persistence.entities.Account;
import com.knightonline.shared.persistence.entities.ServerList;

/**
 * @author Mamaorha
 *
 */
@Component
public class GameLoginHandler extends PacketHandlerBase
{
	@Autowired
	protected PacketWriter packetWriter;

	@Autowired
	protected BaseLogin baseLogin;

	@Autowired
	protected KOServer koServer;

	@Autowired
	protected IServerListDAO serverListDAO;

	@Autowired
	protected IAccountDAO accountDAO;
	
	private ServerList serverList;

	@PostConstruct
	private void init()
	{
		try
		{
			String lanIp = InetAddress.getLocalHost().getHostAddress();
			serverList = serverListDAO.getServerByIP(lanIp);

			if (null == serverList)
			{
				URL connection = new URL("http://checkip.amazonaws.com/");
				URLConnection con = connection.openConnection();
				BufferedReader reader = new BufferedReader(new InputStreamReader(con.getInputStream()));

				lanIp = reader.readLine();
				reader.close();

				serverList = serverListDAO.getServerByIP(lanIp);

				if (null == serverList)
				{
					throw new InternalServerError("Server isn't defined in server_list table");
				}
			}
		}

		catch (IOException e)
		{
			throw new InternalServerError(e);
		}
	}

	@Override
	public void handlePacket(Packet requestPacket)
	{
		StringHolder username = new StringHolder();

		short resultCode = baseLogin.handlePacket(requestPacket, username);

		Packet result = new Packet(requestPacket.getOpcode(), requestPacket.getMessageInfo());

		if (resultCode == LoginResultCodeEnum.AUTH_SUCCESS)
		{
			//check if account was already logged in
			if (koServer.isConnectedAccount(username.value))
			{
				resultCode = LoginResultCodeEnum.AUTH_IN_GAME;
				koServer.killAccount(username.value);
			}

			else
			{
				//check if max free users are connected
				if(serverList.getFreeLimit() < koServer.getLoggedInClients())
				{
					Account account = accountDAO.getAccountByUsername(username.value);
					
					PremiumEnum premium = PremiumEnum.NONE;
					
					//check if the account still have premium time left
					if(account.getPremiumExpireTime().after(new Timestamp(System.currentTimeMillis())))
					{
						premium = account.getPremium();
					}
					
					//if the user dont have premium or he does but the server premium users limit is reached give 'server is full' message
					if(premium == PremiumEnum.NONE || serverList.getPremiumLimit() < koServer.getLoggedInClients())
					{
						resultCode = LoginResultCodeEnum.SERVER_IS_FULL;
					}
				}

				if(resultCode == LoginResultCodeEnum.AUTH_SUCCESS)
				{
					koServer.connectAccount(username.value, requestPacket.getMessageInfo().getChannelId());
				}
			}
		}
		
		result.appendInt8(resultCode);
		packetWriter.sendPacket(result);
	}
}
