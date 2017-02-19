package com.knightonline.game.network.packet;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.game.data.GameOpcodesEnum;
import com.knightonline.game.network.packet.handlers.CharacterInfoHandler;
import com.knightonline.game.network.packet.handlers.CreateNewCharacterHandler;
import com.knightonline.game.network.packet.handlers.DeleteCharacterHandler;
import com.knightonline.game.network.packet.handlers.GameLoginHandler;
import com.knightonline.game.network.packet.handlers.NationSelectHandler;
import com.knightonline.game.network.packet.handlers.OnlineUsersHandler;
import com.knightonline.game.network.packet.handlers.SelectCharacterHandler;
import com.knightonline.game.network.packet.handlers.VersionHandler;
import com.knightonline.shared.data.SharedOpCodes;
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
	
	@Autowired
	protected CharacterInfoHandler characterInfoHandler;
	
	@Autowired
	protected CreateNewCharacterHandler createNewCharacterHandler;
	
	@Autowired
	protected DeleteCharacterHandler deleteCharacterHandler;
	
	@Autowired
	protected SelectCharacterHandler selectCharacterHandler;
	
	@Autowired
	protected OnlineUsersHandler onlineUserHandler;
	
	@Override
	protected void initializePacketTypesHandlers()
	{
		registerHandler(GameOpcodesEnum.N3_VERSION_CHECK, versionHandler);
		registerHandler(GameOpcodesEnum.N3_GAME_SERVER_LOGIN, gameLoginHandler);
		registerHandler(GameOpcodesEnum.N3_NATION_SELECT, nationSelectHandler);
		registerHandler(GameOpcodesEnum.N3_ALL_CHARACTER_INFO_REQUEST, characterInfoHandler);
		registerHandler(GameOpcodesEnum.WIZ_NEW_CHAR, createNewCharacterHandler);
		registerHandler(GameOpcodesEnum.N3_DELETE_CHARACTER, deleteCharacterHandler);
		registerHandler(GameOpcodesEnum.N3_CHARACTER_SELECT, selectCharacterHandler);
		registerHandler(SharedOpCodes.ONLINE_USERS, onlineUserHandler);
	}
	
	@Override
	protected int getNumberOfThreads()
	{
		return applicationPropertiesManager.getIntValue(ConfigurationConstants.GAME_PACKET_HANDLER_THREAD_POOL_SIZE, ConfigurationConstants.DEFAULT_GAME_PACKET_HANDLER_THREAD_POOL_SIZE);
	}
}
