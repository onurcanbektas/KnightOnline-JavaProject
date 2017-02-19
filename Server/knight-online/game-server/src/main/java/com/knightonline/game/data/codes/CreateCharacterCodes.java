package com.knightonline.game.data.codes;

/**
 * @author Mamaorha
 *
 */
public class CreateCharacterCodes
{
	public static final byte CHARACTER_CREATE_SUCCESS = 1;
	public static final byte NO_MORE_CHARACTER = 2;
	public static final byte INVALID_CHARACTER_POSITION = 3;
	public static final byte INVALID_CHARACTER_NAME = 4;
	public static final byte INVALID_RACE = 5;
	public static final byte INVALID_SPECIALITY = 6;
	public static final byte INVALID_NATION_AND_INVALID_RACE = 7;
	public static final byte INVALID_STAT_POINT = 8;
	public static final byte OVERLAPPED_CHARACTER_NAME = 9;
	public static final byte SERVER_ERROR = 10;
	/*
	
	
	public static final short ERROR_CHARACTER_CREATE_INVALID_NATION_AND_INVALID_RACE = 0x02;
	

			
			ERROR_CHARACTER_CREATE_OVERLAPPED_ID = 3,
			ERROR_CHARACTER_CREATE_DB_CREATE = 4,
			ERROR_CHARACTER_CREATE_INVALID_NAME_HAS_SPECIAL_LETTER = 5, 
			ERROR_CHARACTER_CREATE_INVALID_CLASS,
			ERROR_CHARACTER_CREATE_REMAIN_BONUS_POINT*/
}
