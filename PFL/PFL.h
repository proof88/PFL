#pragma once

/*
    ###################################################################################
    PFL.h
    PR00F Foundation Library
    This file is part of PFL.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include <string>

/**
    Bit manipulation macros.
    Good article: https://www.coranac.com/documents/working-with-bits-and-bitfields/ written by Jasper Vijn (cearn@coranac.com).
*/

/**
    Sets the specified bit b to 1 in value v.

    @return Changed value v after set operation.
*/
#define BIT_SET(v,b) ( v |= ( 1 << (b) ) )


/**
    Sets the specified bit b to 0 in value v.

    @return Changed value v after clear operation.
*/
#define BIT_CLEAR(v,b) ( v &= ~( 1 << (b) ) )


/**
    Gets value of bit b of value v.

    @return Value of bit b of value v.
*/
#define BIT_READ(v,b) ( (0u == ( v & ( 1 << (b) ) ) ) ? 0u : 1u )


/**
    Toggles/flaps/flips/inverts bit b of value v.

    @return Changed value v after toggle operation.
*/
#define BIT_TOGGLE(v,b) ( v ^= ( 1 << (b) ) )


/**
    Creates a value where only the specified bit n is set to 1.

    @return Value where only bit n is set to 1 (2^n).
*/
#define BIT(n) ((unsigned)( 1 << (n) ))


/**
    Bitfield manipulation macros.
*/


/**
    Creates a bitmask where n bits starting from bit 0 are 1.

    Example: BIT_MASK(3) = 111

    @return Bitmask where n bits starting from bit 0 are 1 (2^n-1).
*/
#define BIT_MASK(n) ( BIT(n)-1 )


/**
    Creates a bitfield mask where n bits starting from s are 1.

    Example: BITF_MASK(2,3) = 11100

    @return Bitfield mask where n bits starting from s are 1.
*/
#define BITF_MASK(s,n) ( BIT_MASK(n) << (s) )


/**
    Gets the specified n-length bitfield starting at bit s from value v.

    @return Value of n-length bitfield starting at bit s from value v.
*/
#define BITF_READ(v,s,n) ( ( (v) >> (s) ) & BIT_MASK(n) )


/**
    Prepare an n-length bitmask starting at bit s for combining by setting it to x.

    Example: BITF_PREP(6,2,3) = ( ((6) & BIT_MASK(3)) << (2) ) =
                              = ( ((110) & 111) << (2) ) =
                              = ( 110 << (2) ) =
                              = ( 11000 )

    @param x Value to be stored (our input bitfield).
    @param s At which bit the value is stored in the return value.
    @param n Length of the input bitfield.

    @return A value where the n-length bitfield starting at bit s is set to x.
*/
#define BITF_PREP(x,s,n) ( ((x) & BIT_MASK(n)) << (s) )


/**
    Sets the specified n-length bitfield starting at bit s in value v to x.

    Example:
    v = 0
    BITF_SET(v,3,2,3) = ( v=((3) &~ BITF_MASK(2,3)) | BITF_PREP(6,2,3) ) =
                      = ( v=((11) &~ 11100) | 11000 ) =
                      = ( v=(11 & 00011) | 11000 ) =
                      = ( v= 11 | 11000 ) = 11011
                      https://www.coranac.com/documents/working-with-bits-and-bitfields/

    @param v Target variable where we are writing our bitfield to.
    @param x Value to be stored (our input bitfield).
    @param s At which bit the value is stored in v.
    @param n Length of the input bitfield.

    @return Changed value v where the n-length bitfield starting at bit s is set to x.
*/
#define BITF_SET(v,x,s,n) ( v=((v) &~ BITF_MASK(s,n)) | BITF_PREP(x,s,n) )


/**
    PR00F Foundation Library class.
    Useful functions.

    Not following the Pimpl idiom here as there are no hidden members in this class for the outside world.
*/
class PFL
{

public:
    // when I switch to C++11 I won't need this struct here and the custom gettimeofday() implementation
    // MSVC defines this in winsock2.h!?
    typedef struct timeval {
        long tv_sec;
        long tv_usec;
    } timeval;

    static float PI;
    static float E;

    // ---------------------------------------------------------------------------

    static int  gettimeofday(struct timeval * tp, struct timezone * tzp);                                  /**< Gets current time. */
    static long getTimeDiffInUs(const timeval& end, const timeval& begin);                                 /**< Gets difference of 2 timestamps. */
    static bool updateForMinDuration(timeval& timeRef, long durationUs);                                   /**< Updates timeRef if it is greater duration than durationUs. */
    static bool updateForMinDuration(timeval& timeRef, const timeval& timeBegin, const timeval& timeEnd);  /**< Updates timeRef if it is greater duration than (timeEnd - timeBegin). */
    static bool updateForMaxDuration(timeval& timeRef, long durationUs);                                   /**< Updates timeRef if it is less duration than durationUs. */
    static bool updateForMaxDuration(timeval& timeRef, const timeval& timeBegin, const timeval& timeEnd);  /**< Updates timeRef if it is less duration than (timeEnd - timeBegin). */

    static bool         fileExists(const char* path);     /**< Determines whether the given file exists. */
    static std::string  getExtension(const char* path);   /**< Extracts the extension from the path. */
    static std::string  getDirectory(const char* path);   /**< Extracts the directory from the path. */
    static std::string  getFilename(const char* path);    /**< Extracts the file name from the path. */
    static std::string  changeExtension(
        const char* path, const char* ext);               /**< Changes the file extension in the given path. */

    static unsigned int numCharAppears(
        const char searchFor, const char* buffer,
        size_t buffer_size);                              /**< Gets how many times the given character appears in the given buffer. */

    static unsigned int strClrTrails(
        char* srcStr,
        char targetChar1 = ' ', char targetChar2 = '\t'); /**< Removes trailing spaces and tabs from the given string. */
    static unsigned int strClrLeads(
        char* const srcStr,
        char targetChar1 = ' ', char targetChar2 = '\t'); /**< Removes leading spaces and tabs from the given string. */
    static unsigned int strClr(
        char* const srcStr,
        char targetChar1 = ' ', char targetChar2 = '\t'); /**< Removes leading and trailing spaces and tabs from the given string. */

    static float pi();                          /**< Returns PI. */

    static float roundf(float value);           /**< Rounds the given value to the nearest whole number. */
    static int   roundi(float value);           /**< Rounds the given value to the nearest whole number. */

    static float constrain(
        float value, float min, float max);     /**< Constrains the given value into the given [min,max] bounds. */

    static float degToRad(float degree);        /**< Converts the given angle from degrees to radians. */
    static float radToDeg(float radian);        /**< Converts the given angle from radians to degrees. */

    static int random(int from, int to);        /**< Generates a random number between from and to. */

    static float lerp(
        float v0, float v1, float t);           /**< Linear interpolation between v0 and v1. */

    static float smooth(
        float fCurrent, float fTarget, float fSpeed, float fEpsilon = 0.001f);  /**< Smoothly approach fTarget from fCurrent. */

    // ---------------------------------------------------------------------------

    PFL(); 
    PFL(const PFL&);
    PFL& operator= (const PFL&);
    virtual ~PFL();

}; // class PFL

