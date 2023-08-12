/*
    ###################################################################################
    PFL.cpp
    PR00F Foundation Library
    This file is part of PFL.
    Useful functions, macros.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PFL.h"

// PFL.h already includes string so no use removing the following headers at all to win compilation speed
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <cassert>
#include <cstdlib> 
#include <ctime> 
#include <math.h>
#include <random>  // cpp11

// these includes below are needed only for the gettimeofday() implementation 
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdint.h> // portable: uint64_t   MSVC: __int64 

#define M_PI

using namespace std;


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
string PFL::getExtension(const char* path)
{
    const string sf = path;
    const string::size_type ind = sf.rfind('.');
    return ( (ind == string::npos) ? "" : string( sf.substr(ind+1) ) );
} // getExtension()


/**
    Extracts the directory path from the full path.
    @param path The full path from where the directory path should be extracted.
    @return The extracted directory path. If not empty, there is always a delimiter character at the end of it.
            The delimiter character can be slash or backslash, depending on the given path.
*/
string PFL::getDirectory(const char* path)
{
    string strPath = path;
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
string PFL::getFilename(const char* path)
{
    string strPath = path;
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
    Specifying empty extension string will actually remove the extension part (e.g.: ".ext") from the string.
*/
string PFL::changeExtension(const char* path, const char* ext)
{
    // this function can be also thrown out when I switch to Cpp17:
    // https://en.cppreference.com/w/cpp/filesystem/path/replace_extension
    const string strFilename = getFilename(path);
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
    Removes trailing chars from the given string.
    Any trailing char can be removed from the end of the string.
    If not specified, the default trailing chars to be removed are space and tab chars.
    @param srcStr      The string to be modified.
    @param targetChar1 First char to be removed.
    @param targetChar2 Second char to be removed.
    @return The string's new length which is never greater than the original length.
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
    Removes leading chars from the given string.
    Any leading char can be removed from the beginning of the string.
    If not specified, the default leading chars to be removed are space and tab chars.
    @param srcStr      The string to be modified.
    @param targetChar1 First char to be removed.
    @param targetChar2 Second char to be removed.
    @return The string's new length which is never greater than the original length.
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
    Removes leading and trailing chars from the given string.
    Any leading and trailing char can be removed from the beginning and the end of the string.
    If not specified, the default chars to be removed are space and tab chars.
    @param srcStr      The string to be modified.
    @param targetChar1 First char to be removed.
    @param targetChar2 Second char to be removed.
    @return The string's new length which is never greater than the original length.
*/
unsigned int PFL::strClr(char* const srcStr, char targetChar1, char targetChar2)
{
    strClrLeads(srcStr, targetChar1, targetChar2);
    return strClrTrails(srcStr, targetChar1, targetChar2);
} // strClr()


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
  return (int) roundf(value);
} // roundi()


/**
    Constrains the given value into the given [min,max] bounds.
*/
float PFL::constrain(float value, float min, float max)
{
    if ( value < min )
        return min;
    else if ( value > max )
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
#if __cplusplus > 199711L
    // https://stackoverflow.com/questions/13445688/how-to-generate-a-random-number-in-c
    // don't forget that you have to explicitly specify /Zc:__cplusplus flag to compiler so that __cplusplus will be set properly!
    // https://devblogs.microsoft.com/cppblog/msvc-now-correctly-reports-__cplusplus/
    static std::random_device dev;
    static std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(from, to);
    return dist(rng);
#else
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
    t = max(0.f, min(t, 1.f));
    return (1 - t) * v0 + t * v1;
}


PFL::PFL()
{
    srand((unsigned)time(0));
}


PFL::PFL(const PFL&)
{

}


PFL& PFL::operator= (const PFL&)
{
    return *this;
}


PFL::~PFL()
{

}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################






