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
	static constexpr const char* CHARACTER_DELETE_ERROR = "Character Delete failed";
	static constexpr const char* CHARACTER_DELETE_NAME_MISSMATCH = "name doesn't match";

	//info
	static constexpr const char* LOGIN_SERVER = "Login Server";
	static constexpr const char* PREMIUM_TYPE = "Premium: %s";
	static constexpr const char* EXPIRATION_DATE = "Expiration date: %s"; 
	static constexpr const char* OPEN_OPTIONS_VALIDATION = "Would you like to open the options ?";
	static constexpr const char* EXIT_VALIDATION = "Are you sure you want to exit ?";
	static constexpr const char* DELETE_VALIDATION = "In order to delete this character, please type the character name";
	static constexpr const char* CHR_SELECT_INFO = "Level: %s\nSpecialty: %s\nID: %s";
	static constexpr const char* CHR_SELECT_HINT = "Left click and you can create a new character.";
	static constexpr const char* CHARACTER_DELETE = "Character Delete";
	static constexpr const char* LOADING_DATA = "Loading data...";
};
#endif
