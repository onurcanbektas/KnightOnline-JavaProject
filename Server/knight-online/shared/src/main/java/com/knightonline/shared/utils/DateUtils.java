package com.knightonline.shared.utils;

import java.util.Calendar;

import com.knightonline.shared.data.common.SimpleDate;

/**
 * @author Mamaorha
 *
 */
public class DateUtils
{
	public static SimpleDate getSimpleDate()
	{
		Calendar cal = Calendar.getInstance();
		int day = cal.get(Calendar.DAY_OF_MONTH);
		int month = cal.get(Calendar.MONTH) + 1;
		int year = cal.get(Calendar.YEAR);
		int hour = cal.get(Calendar.HOUR_OF_DAY);
		int minute = cal.get(Calendar.MINUTE);
		int sec = cal.get(Calendar.SECOND);
		
		SimpleDate simpleDate = new SimpleDate();
		simpleDate.setDay(day);
		simpleDate.setMonth(month);
		simpleDate.setYear(year);
		simpleDate.setHour(hour);
		simpleDate.setMinute(minute);
		simpleDate.setSec(sec);
		
		return simpleDate;
	}
}
