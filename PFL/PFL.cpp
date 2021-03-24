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

#include <math.h>
// PFL.h already includes string so no use removing the following headers at all to win compilation speed
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define M_PI

using namespace std;


// ############################### PUBLIC ################################


float PFL::PI = 3.14159265358979323846f;

float PFL::E = 0.0001f;


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
*/
string PFL::changeExtension(const char* path, const char* ext)
{
    const string strFilename = getFilename(path);
    return strFilename.length() > 0 ? 
        getDirectory(path) + strFilename.substr(0, strFilename.find_last_of('.')) + '.' + ext : "";
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
    return value < 0.0f ? ceil(value - 0.5f) : floor(value + 0.5f);
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


PFL::PFL()
{

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






