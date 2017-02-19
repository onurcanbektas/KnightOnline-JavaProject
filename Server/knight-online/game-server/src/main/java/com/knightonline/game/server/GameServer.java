package com.knightonline.game.server;

import java.io.FileWriter;
import java.io.IOException;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.game.network.packet.GamePacketHandler;
import com.knightonline.shared.data.constants.ConfigurationConstants;
import com.knightonline.shared.data.constants.StringConstants;
import com.knightonline.shared.data.enums.NationEnum;
import com.knightonline.shared.data.enums.ZoneEnum;
import com.knightonline.shared.helper.CommonHelper;
import com.knightonline.shared.helper.FileHelper;
import com.knightonline.shared.network.KOServer;
import com.knightonline.shared.network.common.HandlerTypeEnum;
import com.knightonline.shared.network.common.ServerConfiguration;
import com.knightonline.shared.persistence.dao.IOnlineUserDAO;
import com.knightonline.shared.utils.ApplicationPropertiesManager;
import com.knightonline.shared.utils.KOApplicationContext;

@Component
public class GameServer
{
	public static final String GAME_SERVER_TITLE = "Game Server for Knight Online v%s";

	@Autowired
	protected ApplicationPropertiesManager applicationPropertiesManager;

	@Autowired
	protected CGameServerDlg cGameServerDlg;
	
	@Autowired
	protected FileHelper fileHelper;

	@Autowired
	protected GamePacketHandler gamePacketHandler;

	@Autowired
	protected IOnlineUserDAO onlineUserDAO;
	
	@Autowired
	protected CommonHelper commonHelper;
	
	@Autowired
	protected KOServer koServer;
	
	protected ServerConfiguration configuration;
	protected FileWriter userLog;
	protected String version;
	
	public boolean startup()
	{
		if(!cGameServerDlg.startup())
		{
			return false;
		}
		
		else
		{
			//clear remain users
			onlineUserDAO.deleteOnlineUsers(ConfigurationConstants.GAME_SERVER_IP);
			
			//start logs
			
			//load notice data
			
			//init lua
			
			//conect to aiServer
			
			//init server commands
			
			//init chat commands
			
			//Reset Battle Zone Variables.
		}
		
		/*if (!initLogs())
		{
			return false;
		}*/

		KOApplicationContext.getInstance().init();

		version = applicationPropertiesManager.getValue(ConfigurationConstants.SERVER_VERSION, ConfigurationConstants.DEFAULT_SERVER_VERSION);

		System.out.println(commonHelper.printSection(String.format(GAME_SERVER_TITLE, version)));
		System.out.println(StringConstants.CONNECTED_TO_DB);

		configuration = new ServerConfiguration();
		configuration.setIp(applicationPropertiesManager.getValue(ConfigurationConstants.GAME_SERVER_IP, ConfigurationConstants.DEFAULT_GAME_SERVER_IP));
		configuration.setPort(applicationPropertiesManager.getIntValue(ConfigurationConstants.GAME_SERVER_PORT, ConfigurationConstants.DEFAULT_GAME_SERVER_PORT));
		configuration.setMsgTimeout(applicationPropertiesManager.getIntValue(ConfigurationConstants.GAME_SERVER_MSG_TIMEOUT, ConfigurationConstants.DEFAULT_GAME_SERVER_MSG_TIMEOUT));
		configuration.setReceiveBufferSize(applicationPropertiesManager.getIntValue(ConfigurationConstants.GAME_SERVER_RECIEVE_BUFFER_SIZE, ConfigurationConstants.DEFAULT_GAME_SERVER_RECIEVE_BUFFER_SIZE));
		configuration.setHandlerType(HandlerTypeEnum.ByteBuffer);
		configuration.setPacketHandler(gamePacketHandler);

		koServer.init(configuration);

		return true;
	}

	/*private boolean initLogs()
	{
		fileHelper.createDir("Logs");

		try
		{
			userLog = new FileWriter(String.format("./Logs/Login_%s.log", DateUtils.getSimpleDate().getDateFormat("_")), true);
		}

		catch (IOException e)
		{
			System.out.println(UNABLE_TO_OPEN_USER_LOG_FILE);
			e.printStackTrace();
			return false;
		}

		return true;
	}*/

	public void closeLogs()
	{
		try
		{
			if (null != userLog)
			{
				userLog.close();
			}
		}

		catch (IOException e)
		{
			e.printStackTrace();
		}
	}

	public void appendToUserLog(String str)
	{
		try
		{
			userLog.write(str + "\n");
			userLog.flush();
		}

		catch (Exception e)
		{
			e.printStackTrace();
		}
	}
	
	public String getVersion()
	{
		return version;
	}
	
	public boolean isWarOpen(ZoneEnum zone)
	{
		//TODO
		return false;
	}
	
	public NationEnum getVictoryNation()
	{
		//TODO
		return NationEnum.ELMORAD;
	}
}
