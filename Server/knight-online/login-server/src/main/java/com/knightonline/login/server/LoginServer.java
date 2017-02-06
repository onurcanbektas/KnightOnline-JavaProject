package com.knightonline.login.server;

import java.io.FileWriter;
import java.io.IOException;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.login.network.packet.LogonPacketHandler;
import com.knightonline.shared.data.constants.ConfigurationConstants;
import com.knightonline.shared.data.constants.StringConstants;
import com.knightonline.shared.helper.CommonHelper;
import com.knightonline.shared.helper.FileHelper;
import com.knightonline.shared.network.KOServer;
import com.knightonline.shared.network.common.HandlerTypeEnum;
import com.knightonline.shared.network.common.ServerConfiguration;
import com.knightonline.shared.utils.ApplicationPropertiesManager;
import com.knightonline.shared.utils.DateUtils;
import com.knightonline.shared.utils.KOApplicationContext;

@Component
public class LoginServer
{
	public static final String LOGIN_SERVER_TITLE = "Login Server for Knight Online v%s";
	private static final String UNABLE_TO_OPEN_USER_LOG_FILE = "ERROR: Unable to open user log file.";

	@Autowired
	protected ApplicationPropertiesManager applicationPropertiesManager;

	@Autowired
	protected FileHelper fileHelper;

	@Autowired
	protected LogonPacketHandler logonPacketHandler;

	@Autowired
	protected CommonHelper commonHelper;
	
	@Autowired
	protected KOServer koServer;
	
	protected String version;
	protected ServerConfiguration configuration;
	protected FileWriter userLog;

	public boolean startup()
	{
		if (!initLogs())
		{
			return false;
		}

		KOApplicationContext.getInstance().init();

		version = applicationPropertiesManager.getValue(ConfigurationConstants.SERVER_VERSION, ConfigurationConstants.DEFAULT_SERVER_VERSION);

		System.out.println(commonHelper.printSection(String.format(LOGIN_SERVER_TITLE, version)));
		System.out.println(StringConstants.CONNECTED_TO_DB);

		configuration = new ServerConfiguration();
		configuration.setIp(applicationPropertiesManager.getValue(ConfigurationConstants.LOGIN_SERVER_IP, ConfigurationConstants.DEFAULT_LOGIN_SERVER_IP));
		configuration.setPort(applicationPropertiesManager.getIntValue(ConfigurationConstants.LOGIN_SERVER_PORT, ConfigurationConstants.DEFAULT_LOGIN_SERVER_PORT));
		configuration.setMsgTimeout(applicationPropertiesManager.getIntValue(ConfigurationConstants.LOGIN_SERVER_MSG_TIMEOUT, ConfigurationConstants.DEFAULT_LOGIN_SERVER_MSG_TIMEOUT));
		configuration.setReceiveBufferSize(applicationPropertiesManager.getIntValue(ConfigurationConstants.LOGIN_SERVER_RECIEVE_BUFFER_SIZE, ConfigurationConstants.DEFAULT_LOGIN_SERVER_RECIEVE_BUFFER_SIZE));
		configuration.setHandlerType(HandlerTypeEnum.ByteBuffer);
		configuration.setPacketHandler(logonPacketHandler);

		koServer.init(configuration);

		return true;
	}

	private boolean initLogs()
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
	}

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
}
