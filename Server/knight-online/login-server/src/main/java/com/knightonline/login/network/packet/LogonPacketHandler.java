package com.knightonline.login.network.packet;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.login.data.enums.LogonOpcodesEnum;
import com.knightonline.login.network.packet.handlers.LoginHandler;
import com.knightonline.login.network.packet.handlers.NewsHandler;
import com.knightonline.login.network.packet.handlers.ServerListHandler;
import com.knightonline.shared.network.packet.PacketHandler;
import com.knightonline.shared.utils.ApplicationPropertiesManager;

/**
 * @author Mamaorha
 *
 */
@Component
public class LogonPacketHandler extends PacketHandler
{
	private static final String LOGIN_PACKET_HANDLER_THREAD_POOL_SIZE = "login_packet_handler_thread_pool_size";
	private static final int DEFAULT_LOGIN_PACKET_HANDLER_THREAD_POOL_SIZE = 10;

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
		registerHandler(LogonOpcodesEnum.LS_LOGIN_REQ, loginHandler);
		registerHandler(LogonOpcodesEnum.LS_SERVERLIST, serverListHandler);
		registerHandler(LogonOpcodesEnum.LS_NEWS, newsHandler);
	}
	
	@Override
	protected int getNumberOfThreads()
	{
		return applicationPropertiesManager.getIntValue(LOGIN_PACKET_HANDLER_THREAD_POOL_SIZE, DEFAULT_LOGIN_PACKET_HANDLER_THREAD_POOL_SIZE);
	}
}
