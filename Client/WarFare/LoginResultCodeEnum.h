#ifndef LoginResultCodeEnum_H
#define LoginResultCodeEnum_H
enum LoginResultCodeEnum
{
	AUTH_SUCCESS = 0x01,
	AUTH_NOT_FOUND = 0x02,
	AUTH_INVALID = 0x03,
	AUTH_BANNED = 0x04,
	AUTH_IN_GAME = 0x05
};
#endif