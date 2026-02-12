#ifndef DATETIMEPARSER_H
#define DATETIMEPARSER_H

#include <QString>
#include <QtEndian>
#include <array>
#include <cstring>

/**
 * @brief Utility class for parsing Hotline protocol date/time format
 * 
 * The Hotline protocol uses a custom date format:
 * - 2 bytes: Year (big-endian)
 * - 2 bytes: Milliseconds (big-endian)
 * - 4 bytes: Seconds since start of year (big-endian)
 * 
 * This class provides conversion from this format to human-readable strings.
 */
class DateTimeParser {
public:
    /**
     * @brief Parsed date/time components
     */
    struct DateTime {
        quint16 year;
        quint8 month;        // 1-12
        quint8 day;          // 1-31
        quint8 hour;         // 0-23
        quint8 minute;       // 0-59
        quint8 second;       // 0-59
        
        bool isValid() const {
            return year > 0 && month >= 1 && month <= 12 && 
                   day >= 1 && day <= 31 && hour < 24 && minute < 60 && second < 60;
        }
    };
    
    /**
     * @brief Parse Hotline protocol date/time from binary data
     * 
     * @param data Pointer to 8 bytes of date/time data
     * @return Parsed DateTime structure
     */
    static DateTime parse(const char* data) {
        if (!data) {
            return DateTime{0, 0, 0, 0, 0, 0};
        }
        
        // Read big-endian values
        quint16 year;
        quint16 millis;
        quint32 seconds;
        
        memcpy(&year, data, 2);
        year = qFromBigEndian(year);
        
        memcpy(&millis, data + 2, 2);
        millis = qFromBigEndian(millis);
        
        memcpy(&seconds, data + 4, 4);
        seconds = qFromBigEndian(seconds);
        
        // Add milliseconds to seconds
        quint32 totalSeconds = seconds + millis / 1000;
        
        // Calculate day of year (starting from day 1)
        constexpr quint32 SECONDS_IN_DAY = 86400;
        quint32 day = 1;
        while (totalSeconds > SECONDS_IN_DAY) {
            day++;
            totalSeconds -= SECONDS_IN_DAY;
        }
        
        // Determine if leap year
        bool isLeap = isLeapYear(year);
        
        // Convert day-of-year to month and day-of-month
        quint8 month = 1;
        for (int i = 0; i < 12; i++) {
            quint32 daysInMonth = getDaysInMonth(i + 1, isLeap);
            
            if (day > daysInMonth) {
                month++;
                day -= daysInMonth;
            } else {
                break;
            }
        }
        
        // Calculate time components
        quint8 hour = 0;
        quint8 minute = 0;
        quint8 second = 0;
        
        while (totalSeconds >= 3600) {
            hour++;
            totalSeconds -= 3600;
        }
        
        while (totalSeconds >= 60) {
            minute++;
            totalSeconds -= 60;
        }
        
        second = static_cast<quint8>(totalSeconds);
        
        return DateTime{year, month, static_cast<quint8>(day), hour, minute, second};
    }
    
    /**
     * @brief Format DateTime as human-readable string
     * 
     * Format: "DD Month YYYY at HH:MM:SS"
     * Example: "15 March 2024 at 14:30:45"
     * 
     * @param dt DateTime structure
     * @return Formatted string, or "Unknown" if invalid
     */
    static QString format(const DateTime& dt) {
        if (!dt.isValid() || dt.day > 31) {
            return QString::fromUtf8("Unknown");
        }
        
        QString result = QString::number(dt.day) + QString::fromUtf8(" ");
        result += getMonthName(dt.month);
        result += QString::number(dt.year);
        result += QString::fromUtf8(" at ");
        
        // Format time with leading zeros
        result += formatTimePart(dt.hour) + QString::fromUtf8(":");
        result += formatTimePart(dt.minute) + QString::fromUtf8(":");
        result += formatTimePart(dt.second);
        
        return result;
    }
    
    /**
     * @brief Parse and format in one call
     * 
     * @param data Pointer to 8 bytes of date/time data
     * @return Formatted date/time string
     */
    static QString parseAndFormat(const char* data) {
        return format(parse(data));
    }

private:
    /**
     * @brief Check if a year is a leap year
     */
    static constexpr bool isLeapYear(quint16 year) {
        if (year % 4 != 0) {
            return false;
        }
        if (year % 100 == 0 && year % 400 != 0) {
            return false;
        }
        return true;
    }
    
    /**
     * @brief Get number of days in a month
     * 
     * @param month Month number (1-12)
     * @param isLeap Whether the year is a leap year
     * @return Number of days in the month
     */
    static constexpr quint8 getDaysInMonth(quint8 month, bool isLeap) {
        constexpr std::array<quint8, 12> daysInMonth = {
            31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
        };
        
        if (month < 1 || month > 12) {
            return 0;
        }
        
        quint8 days = daysInMonth[month - 1];
        
        // February in leap year has 29 days
        if (month == 2 && isLeap) {
            days = 29;
        }
        
        return days;
    }
    
    /**
     * @brief Get month name
     * 
     * @param month Month number (1-12)
     * @return Month name with trailing space
     */
    static QString getMonthName(quint8 month) {
        constexpr std::array<const char*, 12> monthNames = {
            "January ", "February ", "March ", "April ", "May ", "June ",
            "July ", "August ", "September ", "October ", "November ", "December "
        };
        
        if (month < 1 || month > 12) {
            return QString::fromUtf8("Unknown ");
        }
        
        return QString::fromUtf8(monthNames[month - 1]);
    }
    
    /**
     * @brief Format time part with leading zero if needed
     * 
     * @param value Time component (hour, minute, or second)
     * @return Formatted string (e.g., "09" or "15")
     */
    static QString formatTimePart(quint8 value) {
        if (value < 10) {
            return QString::fromUtf8("0") + QString::number(value);
        }
        return QString::number(value);
    }
};

#endif // DATETIMEPARSER_H
