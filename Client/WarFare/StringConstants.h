#ifndef STRING_CONSTANTS_H
#define STRING_CONSTANTS_H

struct StringConstants
{
public:
	//errors
	static constexpr const char* ERROR_TITLE = "Error";
	static constexpr const char* NO_SERVER_LIST = "No server list";
	static constexpr const char* AUTHENTICATION_FAILED = "Authentication failed";
	static constexpr const char* BANNED_ACCOUNT = "This account is banned";
	static constexpr const char* ACCOUNT_ALREADY_CONNECTED = "Account already connected";
	static constexpr const char* SERVER_ERROR = "Current Server Error";
	static constexpr const char* ACCOUNT_INGAME = "Account was already connected";

	//info
	static constexpr const char* LOGIN_SERVER = "Login Server";
	static constexpr const char* PREMIUM_TYPE = "Premium: %s";
	static constexpr const char* EXPIRATION_DATE = "Expiration date: %s"; 
};
#endif
