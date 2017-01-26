package com.knightonline.shared.data.constants;

public class ConfigurationConstants
{
	//keys
	public static final String SERVER_VERSION = "server_version";
	public static final String LOGIN_SERVER_IP = "login_server_ip";
	public static final String LOGIN_SERVER_PORT = "login_server_port";
	public static final String LOGIN_SERVER_MSG_TIMEOUT = "login_server_msg_timeout";
	public static final String LOGIN_SERVER_RECIEVE_BUFFER_SIZE = "login_server_recieve_buffer_size";
	
	//default values
	public static final String DEFAULT_SERVER_VERSION = "1298";
	public static final String DEFAULT_LOGIN_SERVER_IP = "0.0.0.0";
	public static final Integer DEFAULT_LOGIN_SERVER_PORT = 15100;
	public static final Integer DEFAULT_LOGIN_SERVER_MSG_TIMEOUT = 3000;
	public static final Integer DEFAULT_LOGIN_SERVER_RECIEVE_BUFFER_SIZE = 1048576;
}
