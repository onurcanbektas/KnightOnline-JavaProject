package com.knightonline.login.network.packet;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.login.data.LogonOpcodes;
import com.knightonline.login.network.packet.handlers.LoginHandler;
import com.knightonline.login.network.packet.handlers.NewsHandler;
import com.knightonline.login.network.packet.handlers.ServerListHandler;
import com.knightonline.shared.data.constants.ConfigurationConstants;
import com.knightonline.shared.network.packet.PacketHandler;
import com.knightonline.shared.utils.ApplicationPropertiesManager;

/**
 * @author Mamaorha
 *
 */
@Component
public class LogonPacketHandler extends PacketHandler
{
	@Autowired
	protected ApplicationPropertiesManager applicationPropertiesManager;
	
	@Autowired
	protected LoginHandler loginHandler;
	
	@Autowired
	protected ServerListHandler serverListHandler;
	
	@Autowired
	protected NewsHandler newsHandler;
	
	@Override
	protected void initializePacketTypesHandlers()
	{
		registerHandler(LogonOpcodes.LS_LOGIN_REQ, loginHandler);
		registerHandler(LogonOpcodes.LS_SERVERLIST, serverListHandler);
		registerHandler(LogonOpcodes.LS_NEWS, newsHandler);
	}
	
	@Override
	protected int getNumberOfThreads()
	{
		return applicationPropertiesManager.getIntValue(ConfigurationConstants.LOGIN_PACKET_HANDLER_THREAD_POOL_SIZE, ConfigurationConstants.DEFAULT_LOGIN_PACKET_HANDLER_THREAD_POOL_SIZE);
	}
}
