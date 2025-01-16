#pragma once

/*
    ###################################################################################
    bitmanip.h
    Bit manipulation macros.
    Good article: https://www.coranac.com/documents/working-with-bits-and-bitfields/ written by Jasper Vijn (cearn@coranac.com).
    This file is part of PFL (PR00F Foundation Library).
    Made by PR00F88
    2024
    ###################################################################################
*/

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
