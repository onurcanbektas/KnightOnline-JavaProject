package com.knightonline.shared.data.constants;

public class ConfigurationConstants
{
	public static final String SERVER_VERSION = "server_version";
	
	public static final String LOGIN_SERVER_IP = "login_server_ip";
	public static final String LOGIN_SERVER_PORT = "login_server_port";
	public static final String LOGIN_SERVER_MSG_TIMEOUT = "login_server_msg_timeout";
	public static final String LOGIN_SERVER_RECIEVE_BUFFER_SIZE = "login_server_recieve_buffer_size";
	public static final String LOGIN_PACKET_HANDLER_THREAD_POOL_SIZE = "login_packet_handler_thread_pool_size";
	
	public static final String GAME_SERVER_IP = "game_server_ip";
	public static final String GAME_SERVER_PORT = "game_server_port";
	public static final String GAME_SERVER_MSG_TIMEOUT = "game_server_msg_timeout";
	public static final String GAME_SERVER_RECIEVE_BUFFER_SIZE = "game_server_recieve_buffer_size";
	public static final String GAME_PACKET_HANDLER_THREAD_POOL_SIZE = "game_packet_handler_thread_pool_size";
	
	//default values
	public static final String DEFAULT_SERVER_VERSION = "1298";
	
	public static final String DEFAULT_LOGIN_SERVER_IP = "0.0.0.0";
	public static final Integer DEFAULT_LOGIN_SERVER_PORT = 15100;
	public static final Integer DEFAULT_LOGIN_SERVER_MSG_TIMEOUT = 3000;
	public static final Integer DEFAULT_LOGIN_SERVER_RECIEVE_BUFFER_SIZE = 1048576;
	public static final int DEFAULT_LOGIN_PACKET_HANDLER_THREAD_POOL_SIZE = 10;
	
	public static final String DEFAULT_GAME_SERVER_IP = "0.0.0.0";
	public static final Integer DEFAULT_GAME_SERVER_PORT = 15001;
	public static final Integer DEFAULT_GAME_SERVER_MSG_TIMEOUT = 3000;
	public static final Integer DEFAULT_GAME_SERVER_RECIEVE_BUFFER_SIZE = 1048576;
	public static final int DEFAULT_GAME_PACKET_HANDLER_THREAD_POOL_SIZE = 10;
	
}
