package com.knightonline.game.network.packet;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.game.data.enums.GameOpcodesEnum;
import com.knightonline.game.network.packet.handlers.GameLoginHandler;
import com.knightonline.game.network.packet.handlers.NationSelectHandler;
import com.knightonline.game.network.packet.handlers.VersionHandler;
import com.knightonline.shared.data.constants.ConfigurationConstants;
import com.knightonline.shared.network.packet.PacketHandler;
import com.knightonline.shared.utils.ApplicationPropertiesManager;

/**
 * @author Mamaorha
 *
 */
@Component
public class GamePacketHandler extends PacketHandler
{
	@Autowired
	protected ApplicationPropertiesManager applicationPropertiesManager;
	
	@Autowired
	protected VersionHandler versionHandler;
	
	@Autowired
	protected GameLoginHandler gameLoginHandler;
	
	@Autowired
	protected NationSelectHandler nationSelectHandler;
	
	@Override
	protected void initializePacketTypesHandlers()
	{
		registerHandler(GameOpcodesEnum.N3_VERSION_CHECK, versionHandler);
		registerHandler(GameOpcodesEnum.N3_GAME_SERVER_LOGIN, gameLoginHandler);
		registerHandler(GameOpcodesEnum.N3_NATION_SELECT, nationSelectHandler);
	}
	
	@Override
	protected int getNumberOfThreads()
	{
		return applicationPropertiesManager.getIntValue(ConfigurationConstants.GAME_PACKET_HANDLER_THREAD_POOL_SIZE, ConfigurationConstants.DEFAULT_GAME_PACKET_HANDLER_THREAD_POOL_SIZE);
	}
}
