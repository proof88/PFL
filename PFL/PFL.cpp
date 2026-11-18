#include "PFL.h"
/*
    ###################################################################################
    PFL.cpp
    PR00F Foundation Library
    This file is part of PFL.
    Useful functions, macros.
    Made by PR00F88
    ###################################################################################
*/

#include "PFL.h"

// PFL.h already includes std::string so no use removing the following headers at all to win compilation speed
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "winproof88.h" // needed only for the gettimeofday() implementation
#include <stdint.h> // needed only for the gettimeofday() implementation; portable: uint64_t, MSVC: __int64 

#include <cassert>
#include <cstdlib> 
#include <ctime> 
#include <iomanip>
#include <iostream>

#if PFL_COMPILER_SUPPORTS_CPLUSPLUS_11
#include <random>
#endif


// ############################### PUBLIC ################################


float PFL::PI = 3.14159265358979323846f;

float PFL::E = 0.0001f;


/**
    Gets current time.
    This is a replacement implementation due to lack of gettimeofday() in <sys/time.h>.
    Copied from: https://stackoverflow.com/questions/10905892/equivalent-of-gettimeday-for-windows .
    This uses Win32.
    I can get rid of this once I upgrade to newer dev env (newer MSVC).

    @return Current time in milliseconds precision (microseconds part is always milliseconds * 1000 in this implementation).
*/
int PFL::gettimeofday(struct timeval * tp, struct timezone *)
{
    // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
    // This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
    // until 00:00:00 January 1, 1970 
    static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

    SYSTEMTIME  system_time;
    FILETIME    file_time;
    uint64_t    time;

    GetSystemTime( &system_time );
    SystemTimeToFileTime( &system_time, &file_time );
    time =  ((uint64_t)file_time.dwLowDateTime )      ;
    time += ((uint64_t)file_time.dwHighDateTime) << 32;

    tp->tv_sec  = (long) ((time - EPOCH) / 10000000L);
    tp->tv_usec = (long) (system_time.wMilliseconds * 1000);

    return 0;
} // gettimeofday()


/**
    Gets difference of 2 timestamps.

    @return Value of (end - begin) in microseconds.
*/
long PFL::getTimeDiffInUs(const timeval& end, const timeval& begin)
{
    return (end.tv_sec - begin.tv_sec) * 1000000 + end.tv_usec - begin.tv_usec;
}


/**
    Updates timeRef if it is greater duration than durationUs.

    @param timeRef      The duration that might be updated.
    @param durationUs   Duration in microseconds we are checking against.

    @return True if timeRef has been updated, false otherwise.
*/
bool PFL::updateForMinDuration(timeval& timeRef, long durationUs)
{
    bool retVal = false;

    const long durationRefUs = timeRef.tv_sec * 1000000 + timeRef.tv_usec;

    if ( durationUs < durationRefUs )
    {
        retVal = true;
        timeRef.tv_sec = durationUs / 1000000;
        timeRef.tv_usec = durationUs % 1000000;
    }

    return retVal;
} // updateForMinDuration()


/**
    Updates timeRef if it is greater duration than (timeEnd - timeBegin).

    @param timeRef   The duration that might be updated.
    @param timeBegin Timestamp of the beginning of the new duration.
    @param timeEnd   Timestamp of the end of the new duration.

    @return True if timeRef has been updated, false otherwise.
*/
bool PFL::updateForMinDuration(timeval& timeRef, const timeval& timeBegin, const timeval& timeEnd)
{
    const long durationUs = getTimeDiffInUs(timeEnd, timeBegin);

    return updateForMinDuration(timeRef, durationUs);
} // updateForMinDuration()


/**
    Updates timeRef if it is less duration than durationUs.

    @param timeRef      The duration that might be updated.
    @param durationUs   Duration in microseconds we are checking against.

    @return True if timeRef has been updated, false otherwise.
*/
bool PFL::updateForMaxDuration(timeval& timeRef, long durationUs)
{
    bool retVal = false;

    const long durationRefUs = timeRef.tv_sec * 1000000 + timeRef.tv_usec;

    if ( durationUs > durationRefUs )
    {
        retVal = true;
        timeRef.tv_sec = durationUs / 1000000;
        timeRef.tv_usec = durationUs % 1000000;
    }

    return retVal;
} // updateForMaxDuration()


/**
    Updates timeRef if it is less duration than (timeEnd - timeBegin).

    @param timeRef   The duration that might be updated.
    @param timeBegin Timestamp of the beginning of the new duration.
    @param timeEnd   Timestamp of the end of the new duration.

    @return True if timeRef has been updated, false otherwise.
*/
bool PFL::updateForMaxDuration(timeval& timeRef, const timeval& timeBegin, const timeval& timeEnd)
{
    const long durationUs = getTimeDiffInUs(timeEnd, timeBegin);

    return updateForMaxDuration(timeRef, durationUs);
} // updateForMaxDuration()


/**
    Determines whether the given file exists.

    @return True if a file with the given filename exists, false otherwise.
*/
bool PFL::fileExists(const char* filename)
{
    struct stat buffer;   
    return (stat (filename, &buffer) == 0);
} // fileExists()


/**
    Extracts the extension from the path.

    @return The extension in the given filename.
*/
std::string PFL::getExtension(const char* path)
{
    const std::string sf = path;
    const std::string::size_type ind = sf.rfind('.');
    return ( (ind == std::string::npos) ? "" : std::string( sf.substr(ind+1) ) );
} // getExtension()


/**
    Extracts the directory path from the full path.

    @param path The full path from where the directory path should be extracted.

    @return The extracted directory path. If not empty, there is always a delimiter character at the end of it.
            The delimiter character can be slash or backslash, depending on the given path.
*/
std::string PFL::getDirectory(const char* path)
{
    std::string strPath = path;
    char tmpPath[1024];
    strcpy_s(tmpPath, 1024, strPath.c_str());
    int newLen = strClr(tmpPath);
    // to here ###
    if ( newLen == 0 )
    {
        return "";
    }

    const bool bHadSlashAtTheEnd = (tmpPath[newLen-1] == '/') || (tmpPath[newLen-1] == '\\');
    char delimiter = '/';
    if ( bHadSlashAtTheEnd )
        delimiter = tmpPath[newLen-1];
    strClr(tmpPath, '/');
    newLen = strClr(tmpPath, '\\');
    if ( newLen == 0 )
    {
        return "";
    }
    strPath = tmpPath;

    if ( strchr(strPath.c_str(), '\\') )
        delimiter = '\\';

    if ( bHadSlashAtTheEnd )
        strPath = strPath + delimiter;
    
    return newLen > 0 ? strPath.substr(0, strPath.find_last_of(delimiter) + 1) : "";
} // getDirectory()


/**
    Extracts the file name from the path.
*/
std::string PFL::getFilename(const char* path)
{
    std::string strPath = path;
    char tmpPath[1024];
    strcpy_s(tmpPath, 1024, strPath.c_str());
    strClr(tmpPath);
    strClr(tmpPath, '/', '\\');
    // to here ###
    int newLen = strClrTrails(tmpPath, '.');
    strPath = tmpPath;
    char delimiter = '/';
    if ( strchr(strPath.c_str(), '\\') )
        delimiter = '\\';
    return newLen > 0 ? strPath.substr(strPath.find_last_of(delimiter) + 1) : "";
} // getFilename()


/**
    Changes the file extension in the given path.
    Specifying empty extension std::string will actually remove the extension part (e.g.: ".ext") from the std::string.
*/
std::string PFL::changeExtension(const char* path, const char* ext)
{
    // this function can be also thrown out when I switch to Cpp17:
    // https://en.cppreference.com/w/cpp/filesystem/path/replace_extension
    const std::string strFilename = getFilename(path);
    if (strFilename.empty())
    {
        return "";
    }
    return std::string(ext).length() > 0 ? 
        getDirectory(path) + strFilename.substr(0, strFilename.find_last_of('.')) + '.' + ext :
        getDirectory(path) + strFilename.substr(0, strFilename.find_last_of('.'));
} // changeExtension()


/**
    Gets how many times the given character appears in the given buffer.

    @return Number of appearance of the given character in the given buffer.
*/
unsigned int PFL::numCharAppears(const char searchFor, const char* buffer, size_t buffer_size)
{
    if ( buffer == NULL )
        return 0;

    unsigned int times = 0;
    const char* srchbuffer = buffer;
    while ( srchbuffer < (char*) buffer + buffer_size ) 
    {
        if ( *srchbuffer == searchFor )
            times++;
        srchbuffer++;
    }

    return times;
} // numCharAppears()


/**
    Removes trailing chars from the given std::string.
    Any trailing char can be removed from the end of the std::string.
    If not specified, the default trailing chars to be removed are space and tab chars.

    @param srcStr      The std::string to be modified.
    @param targetChar1 First char to be removed.
    @param targetChar2 Second char to be removed.

    @return The std::string's new length which is never greater than the original length.
*/
unsigned int PFL::strClrTrails(char* srcStr, char targetChar1, char targetChar2)
{
    int newStrLen = strlen(srcStr);
    char* p = &srcStr[newStrLen-1];
    while ( (*p == targetChar1 || *p == targetChar2) && p >= srcStr)
    {
        *p = '\0';
        p--;
        newStrLen--;
    }    
    return newStrLen;
} // strClrTrails()


/**
    Removes leading chars from the given std::string.
    Any leading char can be removed from the beginning of the std::string.
    If not specified, the default leading chars to be removed are space and tab chars.

    @param srcStr      The std::string to be modified.
    @param targetChar1 First char to be removed.
    @param targetChar2 Second char to be removed.

    @return The std::string's new length which is never greater than the original length.
*/
unsigned int PFL::strClrLeads(char* const srcStr, char targetChar1, char targetChar2)
{
    int origLen;
    int newStrLen = origLen = strlen(srcStr);
    char* p = srcStr;
    while ( (*p == targetChar1 || *p == targetChar2) && p < srcStr+origLen)
    {
        p++;
        newStrLen--;
    }   
    if ( p != srcStr )
    {
        int i = 0;
        while ( p[i] != '\0' )
        {
            srcStr[i] = p[i];
            i++;
        }
        srcStr[i] = '\0';
    }
    return newStrLen;
} // strClrLeads()


/**
    Removes leading and trailing chars from the given std::string.
    Any leading and trailing char can be removed from the beginning and the end of the std::string.
    If not specified, the default chars to be removed are space and tab chars.

    @param srcStr      The std::string to be modified.
    @param targetChar1 First char to be removed.
    @param targetChar2 Second char to be removed.

    @return The std::string's new length which is never greater than the original length.
*/
unsigned int PFL::strClr(char* const srcStr, char targetChar1, char targetChar2)
{
    strClrLeads(srcStr, targetChar1, targetChar2);
    return strClrTrails(srcStr, targetChar1, targetChar2);
} // strClr()


/**
    Calculates a hash for the given string.
    Useful if you want to assign a unique id based on string content.

    The calculated hash will be the same for the same input across multiple executions of the program.
    Note that the hash produced by std::hash is not guaranteed to be the same across multiple executions of the program, hence
    it is advised to use this PFL functionality instead if you want the same hash.

    @return Hash of given string.
*/
PFL::StringHash PFL::calcHash(const std::string& str)
{
    PFL::StringHash hash = 5381;

    for (const auto& c : str)
    {
        hash = (hash << 5) + hash + static_cast<uint32_t>(c);
    }

    return hash;
}


/**
    Returns PI.
*/
float PFL::pi()
{
    return PFL::PI;
} // pi()


/**
    Rounds the given value to the nearest whole number.

    @return The rounded value as a float.
*/
float PFL::roundf(float value)
{
    return static_cast<float>(value < 0.0f ? ceil(value - 0.5f) : floor(value + 0.5f));
} // roundf()


/**
    Rounds the given value to the nearest whole number.

    @return The rounded value as a signed integer.
*/
int PFL::roundi(float value)
{
    return (int)roundf(value);
} // roundi()

/**
    Prints the given floating-point value into the given string in the shortest possible
    decimal (non-scientific) format, with special rounding rule determined by the required
    count of decimal digits: the kept decimal part is rounded up or down based on the
    removed decimal part, WITHOUT changing the integer part.
    The integer part is changed only if maxDecimalDigits is 0, since in this case
    we totally remove the decimal part and explicitly request an integer number that
    must be rounded in this case using the full omitted decimal part.
    
    Thus, this function gives a different printing compared to a simple printf() or
    stream::operator<<().

    Trailing zeroes or decimal dot are also removed automatically.

    If the given value is close to zero but still interpreted as non-zero, but
    maxDecimalDigits is insufficient, then either "~0" or "~-0" will be printed to clearly
    show the value is non-zero. This behavior is only for values close to zero but not to
    other integer values.

    Examples:
     - printFloatToStringShort(0.00001, outString, 0)
       => outString will be: "~0" instead of "0" so it is clear
          that value is not exactly 0 but we have too few decimal
          digits allowed.

     - printFloatToStringShort(0.00001, outString, 1)
       => outString will be: "~0" instead of "0" so it is clear
          that value is not exactly 0 but we have too few decimal
          digits allowed.

     - printFloatToStringShort(0.00001, outString, 4)
       => outString will be: "~0" instead of "0" so it is clear
          that value is not exactly 0 but we have too few decimal
          digits allowed.

     - printFloatToStringShort(0.00001, outString, 5)
       => outString will be: "0.00001".

     - printFloatToStringShort(0.00001, outString, 10)
       => outString will be: "0.00001".

     - printFloatToStringShort(0.00001f, outString, 5, epsilon = 0.0001f)
       => outString will be: "0" because of epsilon is bigger than value
          so we have to treat this non-zero value as zero.

     - printFloatToStringShort(4.15, outString, 1)
       => outString will be: "4.2".

     - printFloatToStringShort(4.15, outString, 2)
       => outString will be: "4.15".

     - printFloatToStringShort(4.15, outString, 0)
       => outString will be: "4".

     - printFloatToStringShort(-4.15, outString, 1)
       => outString will be: "-4.2".

     - printFloatToStringShort(-4.15, outString, 2)
       => outString will be: "-4.15".

     - printFloatToStringShort(-4.15, outString, 0)
       => outString will be: "-4".

     - printFloatToStringShort(440.99997, outString, 5)
       => outString will be: "440.99997".

     - printFloatToStringShort(440.99997, outString, 1)
       => outString will be: "440.9".
          Here integer part is NOT changed due to the first
          decimal digit is still present, even though
          it looks truncated now.

     - printFloatToStringShort(440.99997, outString, 0)
       => outString will be: "441".
          Here integer part is changed due to losing
          full decimal part requires integer part to be rounded up.

    @param value            Number to be printed in shortest but non-scientific form.
    @param outString        String where we print the number.
                            The string will be overwritten.
    @param maxDecimalDigits Maximum count of decimal digits to be printed.
                            If negative, "err" will be printed to the given string.
    @param epsilon          Optionally specified your own epsilon value.
                            If value is closer to 0 than the epsilon, it will be printed as "0".
*/
void PFL::printFloatToStringShort(float value, std::string& outString, unsigned int maxDecimalDigits, float epsilon)
{
    if (abs(value) < epsilon)
    {
        outString = "0";
        return;
    }

    if (maxDecimalDigits == 0)
    {
        const auto roundedValue = std::lroundf(value);
        if (roundedValue == 0)
        {
            // we already passed the comparison with epsilon above, so if
            // we are here it means that "value" shall not be treated as 0,
            // so we shall print "~0" instead of "0", so it can be still
            // differentiated from a pure zero.
            outString = (value < 0.f) ? "~-0" : "~0";
            return;
        }
        outString = std::to_string(roundedValue);
        return;
    }

    if (maxDecimalDigits > static_cast<unsigned int>(INT_MAX))
    {
        // because sprintf() would parse it as integer
        outString = "err";
        return;
    }

    const double multiplier = std::pow(10.0, maxDecimalDigits);

    // round to the requested decimal precision
    // e.g. if value is 0.00001 and maxDecimalDigits is 5,
    // then roundedValue = round(0.00001 * 10^5) / 10^5 = 1 / 10^5 = 0.00001
    double magnifiedValue = std::round(value * multiplier);
    double roundedValue = magnifiedValue / multiplier;

    if (magnifiedValue == 0.f)
    {
        // this means that "value" is so small that it cannot be expressed
        // with the given "maxDecimalDigits" count of digits, however if
        // "value" is not exactly zero, we shall print "~0" instead of "0",
        // so it can be still differentiated from a pure zero.
        outString = (value < 0.f) ? "~-0" : "~0";
        return;
    }

    // prevent rounding from changing the integer part
    if (std::floor(std::abs(roundedValue)) > std::floor(std::abs(static_cast<double>(value))))
    {
        roundedValue = std::trunc(value * multiplier) / multiplier;
    }

    static char buff[80];
    sprintf_s(buff, sizeof(buff), "%.*f", static_cast<int>(maxDecimalDigits), roundedValue);

    // remove unnecessary trailing decimal zeroes
    size_t newLen = strlen(buff);

    while ((newLen > 0) && (buff[newLen - 1] == '0'))
    {
        buff[newLen - 1] = '\0';
        newLen--;
    }

    // remove trailing decimal point
    if ((newLen > 0) && (buff[newLen - 1] == '.'))
    {
        buff[newLen - 1] = '\0';
        newLen--;
    }

    outString = buff;
} // printFloatToStringShort()

/**
    Prints the given floating-point value into the given stream in the shortest possible
    decimal (non-scientific) format, with special rounding rule as explained for
    printFloatToStringShort().

    Content of the given stream will NOT be replaced, the printed value will be added to the stream.

    @param value            Number to be printed in shortest but non-scientific form.
    @param outStream        Stream where we print the number.
    @param maxDecimalDigits Maximum count of decimal digits to be printed.
                            If negative, "err" will be printed to the given string.
    @param epsilon          Optionally specified your own epsilon value.
                            If value is closer to 0 than the epsilon, it will be printed as "0".
*/
std::ostream& PFL::printFloatToStreamShort(float value, std::ostream& outStream, unsigned int maxDecimalDigits, float epsilon)
{
    std::string tmpString;
    PFL::printFloatToStringShort(value, tmpString, maxDecimalDigits, epsilon);
    outStream << tmpString;
    return outStream;
} // printFloatToSStreamShort()

/**
    Makes a PrintFloatShort type object that can be sent to a stream using ostream's operator<<() override. 

    Arguments as explained for printFloatToStringShort().
*/
PFL::PrintFloatShort PFL::makePrintFloatShort(float value, unsigned int maxDecimalDigits, float epsilon)
{
    return { value, maxDecimalDigits, epsilon };
}

/**
    Constrains the given value into the given [min,max] bounds.
*/
float PFL::constrain(float value, float min, float max)
{
    if (value < min)
        return min;
    else if (value > max)
        return max;

    return value;
} // constrain()


/**
    Converts the given angle from degrees to radians.

    @return The given angle in radians.
*/
float PFL::degToRad(float degree)
{
    return degree * PFL::PI / 180.0f;
} // degToRad()


/**
    Converts the given angle from radians to degrees.

    @return The given angle in degrees.
*/
float PFL::radToDeg(float radian)
{
    return radian * 180.0f / PFL::PI;
} // radToDeg()


/**
    Generates a random number between from and to.
*/
int PFL::random(int from, int to)
{
    assert(from <= to);

#if PFL_COMPILER_SUPPORTS_CPLUSPLUS_11
    // https://stackoverflow.com/questions/13445688/how-to-generate-a-random-number-in-c
    // don't forget that you have to explicitly specify /Zc:__cplusplus flag to compiler so that __cplusplus will be set properly!
    // https://devblogs.microsoft.com/cppblog/msvc-now-correctly-reports-__cplusplus/
    static std::random_device dev;
    static std::mt19937 rng(dev());

    // WA: because std::mt19937 initializes the mersenne twister engine with unsigned int type, we need to
    // avoid integer overflow when 'from' is negative.
    bool bWa = false;
    const auto nAbsFrom = std::abs(from);
    if (from < 0)
    {
        bWa = true;
        to += nAbsFrom;
        from = 0;
    }

    std::uniform_int_distribution<std::mt19937::result_type> dist(from, to);
    const auto nRandom = dist(rng);

    return bWa ? (nRandom - nAbsFrom) : nRandom;
#else
    static bool bSeedSet = false;
    if (!bSeedSet)
    {
        bSeedSet = true;
        srand((unsigned)time(0));
    }
    return rand() % (to - from + 1) + from;
#endif
}


/**
   Linear interpolation between v0 and v1.

   @param v0 Start of interval.
   @param v1 End of interval.
   @param t  Factor in range [0,1] where 0 results in v0, 1 results in v1.
             Value is clamped into [0,1] range.

   @return Interpolated value between v0 and v1 by factor of t.
*/
float PFL::lerp(float v0, float v1, float t) {
    assert(v0 < v1);
    t = std::max(0.f, std::min(t, 1.f));
    return (1 - t) * v0 + t * v1;
}


/**
   Smoothly approach fTarget from fCurrent.
   The idea is to repeatedly invoke this function in multiple iterations with the same fTarget and fSpeed, but with
   continuously changing fCurrent, where fCurrent is the return value of this function in the previous iteration.

   @param fCurrent Initial value.
   @param fTarget  Target value we want to reach by repeated calls to this function.
   @param fSpeed   Speed of approaching fTarget, where bigger number means slower approach thus more iterations to reach target.
                   Minimum value is 1.f where fTarget is reached from fCurrent in exactly 1 iteration.
                   Any fSpeed value less than 1.f will be treated as 1.f.
   @param fEpsilon Distance between fCurrent and fTarget where both are considered equal.

   @return An f value where fCurrent < f <= fTarget.
*/
float PFL::smooth(float fCurrent, float fTarget, float fSpeed, float fEpsilon)
{
    if (fSpeed < 1.f) {
        fSpeed = 1.f;
    }
    fCurrent += ((fTarget - fCurrent) / fSpeed);
    if (std::abs(fTarget - fCurrent) <= fEpsilon)
    {
        return fTarget;
    }
    return fCurrent;
}

/**
    Convenient way of sending a float to a stream in printFloatToStreamShort() form.

    Prints the given floating-point value into the given stream in the shortest possible
    decimal (non-scientific) format, with special rounding rule as explained for
    printFloatToStringShort().

    Content of the given outSStream will NOT be replaced, the printed value will be added to the stream.

    @param f        A struct created using PFL::makePrintFloatShort().
    @param os       Stream where we print the number.
*/
std::ostream& operator<<(std::ostream& os, const PFL::PrintFloatShort& f)
{
    PFL::printFloatToStreamShort(f.m_value, os, f.m_maxDecimalDigits, f.m_epsilon);
    return os;
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################
