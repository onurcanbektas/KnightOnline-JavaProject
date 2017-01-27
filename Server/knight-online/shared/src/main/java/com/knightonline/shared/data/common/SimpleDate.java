package com.knightonline.shared.data.common;

/**
 * @author Mamaorha
 *
 */
public class SimpleDate
{
	public int day;
	public int month;
	public int year;
	public int hour;
	public int minute;
	public int sec;

	public int getDay()
	{
		return day;
	}

	public void setDay(int day)
	{
		this.day = day;
	}

	public int getMonth()
	{
		return month;
	}

	public void setMonth(int month)
	{
		this.month = month;
	}

	public int getYear()
	{
		return year;
	}

	public void setYear(int year)
	{
		this.year = year;
	}

	public int getHour()
	{
		return hour;
	}

	public void setHour(int hour)
	{
		this.hour = hour;
	}

	public int getMinute()
	{
		return minute;
	}

	public void setMinute(int minute)
	{
		this.minute = minute;
	}

	public int getSec()
	{
		return sec;
	}

	public void setSec(int sec)
	{
		this.sec = sec;
	}

	@Override
	public int hashCode()
	{
		final int prime = 31;
		int result = 1;
		result = prime * result + day;
		result = prime * result + hour;
		result = prime * result + minute;
		result = prime * result + month;
		result = prime * result + sec;
		result = prime * result + year;
		return result;
	}

	@Override
	public boolean equals(Object obj)
	{
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		SimpleDate other = (SimpleDate) obj;
		if (day != other.day)
			return false;
		if (hour != other.hour)
			return false;
		if (minute != other.minute)
			return false;
		if (month != other.month)
			return false;
		if (sec != other.sec)
			return false;
		if (year != other.year)
			return false;
		return true;
	}

	@Override
	public String toString()
	{
		return "SimpleDate [day=" + day + ", month=" + month + ", year=" + year + ", hour=" + hour + ", minute=" + minute + ", sec=" + sec + "]";
	}
	
	public String getDateFormat(String seperator)
	{
		return String.format("%d%s%d%s%d", day, seperator, month, seperator, year);
	}
	
	public String getTimeFormat(String seperator)
	{
		return String.format("%d%s%d%s%d", hour, seperator, minute, seperator, sec);
	}
}
