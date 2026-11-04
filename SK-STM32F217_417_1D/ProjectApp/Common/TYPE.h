//******************************************************************************
// TYPE.h
//******************************************************************************
//
//	FULLNAME:  User types definitions
//
//	NICKNAME:
//
//	PURPOSE:   User types definitions
//
//	TOOLKIT:   EWAVR 8.11 (IAR Systems)
//
//	Version: 1.01
//
//	$Revision: 101 $
//	$Date:: 2017-08-18 #$
//
//	Copyright (c) 2017, Igor Ivanovich Zorin
//
//	Permission is hereby granted, free of charge, to any person obtaining  a copy of
//	this software and  associated documentation files  (the "Software"), to  deal in
//	the Software  without restriction,  including without  limitation the  rights to
//	use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
//	the Software, and to permit persons to whom the Software is furnished to do  so,
//	subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included
//	in all copies or substantial portions of the Software.
//
//	THE SOFTWARE  IS PROVIDED  "AS IS",  WITHOUT WARRANTY  OF ANY  KIND, EXPRESS  OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
//	FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS  OR
//	COPYRIGHT HOLDERS BE LIABLE FOR  ANY CLAIM, DAMAGES OR OTHER  LIABILITY, WHETHER
//	IN  AN  ACTION OF  CONTRACT,  TORT OR  OTHERWISE,  ARISING FROM,  OUT  OF OR  IN
//	CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//******************************************************************************
#ifndef TYPE_H
#define TYPE_H
#include <stdint.h>
//#include <integer.h>
#include <stdint.h>
//#include <tchar.h>
//******************************************************************************
// Filename : type.h
// Programmer(s): 
// Created : 2004/04/
// Modified :
// Description : Re-define types to be used for AVR, 8051 etc.
//******************************************************************************
typedef	unsigned char		Byte;		// 8-bit value
//typedef	unsigned char		BYTE;		// 8-bit value
//typedef	unsigned char		uchar;		// 8-bit value
typedef	unsigned char		u_char;		// 8-bit value
typedef	unsigned char		UCHAR;		// 8-bit value
//typedef	unsigned char		u8;		// 8-bit value
//typedef	signed char		s8;		// 8-bit value
//typedef	int	        	INT;		// 16-bit value
//typedef	unsigned int		uint;		// 16-bit value
typedef	unsigned int		u_int;		// 16-bit value
//typedef	unsigned int		UINT;		// 16-bit value
//typedef	unsigned short		ushort;		// 16-bit value
typedef	unsigned short		u_short;	// 16-bit value
typedef	unsigned short		USHORT;		// 16-bit value
typedef	unsigned short		word;		// 16-bit value
//typedef	unsigned short		WORD;		// 16-bit value
//typedef	unsigned short		WCHAR;		// 16-bit value
//typedef	unsigned short		u16;		// 16-bit value
//typedef	signed short		s16;		// 16-bit value
typedef	unsigned long		ulong;		// 32-bit value
typedef	unsigned long		u_long;		// 32-bit value
typedef	unsigned long		ULONG;		// 32-bit value
typedef	unsigned long		dword;		// 32-bit value
//typedef	unsigned long		DWORD;		// 32-bit value
//typedef	unsigned long		u32;		// 32-bit value
//typedef	signed long		s32;		// 32-bit value
typedef unsigned long long	qword;		// 64-bit value
//typedef unsigned long long	QWORD;		// 64-bit value
//typedef signed long long	__int64;	// 64-bit value
typedef unsigned long long	u64;		// 64-bit value
typedef signed long long	s64;		// 64-bit value
/* unsigned integers */
/*
#ifndef u_int8_t
typedef unsigned char	        u_int8_t;
#endif
#ifndef u_int16_t
typedef unsigned short	        u_int16_t;
#endif
#ifndef u_int32_t
typedef unsigned long	        u_int32_t;
#endif
#ifndef uint8_t
typedef unsigned char	        uint8_t;
#endif
#ifndef uint16_t
typedef unsigned short	        uint16_t;
#endif
#if !defined(uint32_t)
typedef unsigned long	        uint32_t;
#endif
#ifndef uint64_t
typedef unsigned long long	uint64_t;
#endif
*/

typedef int             TError;
typedef int             TBaseInt;
typedef unsigned int    TBase;
typedef unsigned long long       TTickType;

/* signed integers */
//typedef signed char  int8_t;
//typedef short int    int16_t;
//typedef int          int32_t;

#ifndef __cplusplus
  #ifndef bool
//typedef enum { FALSE = 0, TRUE  = !FALSE } bool;
  typedef enum { false = 0, true  = !false } bool;
//typedef signed char	bool;
  #endif
#endif
// bsd

typedef UCHAR	SOCKET;

//typedef unsigned int		size_t;		// CPU Size type
//******************************************************************************
typedef void * (*CommUserFpnt_t)(void *);
typedef void   (*VoidFpnt_t)(void);

//********************************************************************************
typedef union {
    int a;
    short b;
    char c[1];
    long d;
#ifdef HAVE_U64_TYPEDEF
    u64 e;
#endif
    float f;
    double g;
} PROPERLY_ALIGNED_TYPE;
//********************************************************************************
//---------------------------------------------------------------------------
#define MAX_BUFFER_LEN	252
#define MAX_VALUE_LEN	1024
//---------------------------------------------------------------------------
typedef struct
{
  unsigned long	Tag;
  unsigned long	Len;
  unsigned char* pValue;
}TTLVPrt;
//---------------------------------------------------------------------------
/*
typedef struct
{
  unsigned long	Tag;
  unsigned short	Len;
  unsigned char	Value[MAX_VALUE_LEN];
}TTLV;
*/
//---------------------------------------------------------------------------
typedef struct
{
  unsigned char	SFI;
  unsigned char	RecStart;
  unsigned char	RecEnd;
  unsigned char	Reserv;
}TAFL;
//---------------------------------------------------------------------------
//********************************************************************************
//***********************************************
#define MHZ           *1000000l
#define KHZ           *1000l
#define HZ            *1l
//***********************************************
/*
#ifndef FALSE
#define FALSE (1 == 0)
#endif

#ifndef TRUE
#define TRUE (1==1)
#endif
#ifndef false
#define false		FALSE
#endif
#ifndef true
#define true		TRUE
#endif
*/
#ifndef NULL
#define NULL		0
#endif
//------------------------------------------------------------------------------
#define HiWord(l)	((unsigned short) (((unsigned long) (l) >> 16) & 0xFFFF))
#define LoWord(l)	((unsigned short) (l))
#define HiByte(w)	((unsigned char) (((unsigned short) (w) >> 8) & 0xFF))
#define LoByte(w)	((unsigned char) (w))
#define High(w)		HiByte(w)
#define Low(w)		LoByte(w)
//------------------------------------------------------------------------------
#define pAdr         (unsigned long*)&
//------------------------------------------------------------------------------
//#define SetBit(bByte, Bit)	(bByte |= (1<<Bit))
//#define ClearBit(bByte, Bit)	(bByte &= ~(1<<Bit))
//#define CheckBit(bByte, Bit)	(bByte & (1<<Bit))
//***********************************************
#define structassign(d, s)      d = s
//***********************************************
//******************************************************************************
// Type for treating 4 byte variables with byte by byte
#pragma pack(1)
typedef union _un_l2cval
{
	u_long	lVal;
	u_char	cVal[4];
}un_l2cval;

// Type for treating 2 byte variables with byte by byte
typedef union _un_i2cval
{
	u_int	iVal;
	u_char	cVal[2];
}un_i2cval;
#pragma pack()
//******************************************************************************
#pragma pack(1)
typedef struct
{
  unsigned short 	Length;
  char* 		Text;
} TShortString;	// Size 2+2 = 4 b
//******************************************************************************
//TCardUID - Card Information UID, ATQ, SAK
typedef struct
{
	unsigned long	UID[3];		// UID - Card Serial Number up to Level 3
	unsigned short	ATQ;		// Answer to Request
	unsigned char	SAK;		// Answer to Select
	unsigned char	Type;		// UID Type
} TCardUID; //TCardUID;
#pragma pack()
// UID - содержат информацию о серийном номере карты по уровням антиколизии
// NNNNNNNN 00000000 00000000 - 4 BYTE UID
// 88NNNNNN NNNNNNNN 00000000 - 7 BYTE UID
// 88NNNNNN 88NNNNNN NNNNNNNN - 10 BYTE UID
// Type - размер UID: 0 - 4 BYTE UID, 1 - 7 BYTE UID, 2 - 10 BYTE UID
//------------------------------------------------------------------------------
//            The Card replies the ATQ.
//
//            ATQ:
//
//                    +----+----+----+----+----+----+----+----+
//            LSByte  | b8 | b7 | b6 | b5 | b4 | b3 | b2 | b1 |
//                    +----+----+----+----+----+----+----+----+
//                    |         |    |                        |
//                    | UID size| RFU|   bit-frame anticoll   |
//                    |         |    |                        |
//                    | 00..std |    |  (if any bit set .. Y, |
//                    | 01..dbl |    |             else .. N) |
//                    | 10..tpl |    |                        |
//
//
//
//                    +----+----+----+----+----+----+----+----+
//            MSByte  | b8 | b7 | b6 | b5 | b4 | b3 | b2 | b1 |
//                    +----+----+----+----+----+----+----+----+
//                    |                                       |
//                    |                 RFU                   |
//                    |                                       |
//
//------------------------------------------------------------------------------
//            Select codes:
//
//            +----+----+----+----+----+----+----+----+
//            | b8 | b7 | b6 | b5 | b4 | b3 | b2 | b1 |
//            +-|--+-|--+-|--+-|--+----+----+----+-|--+
//              |    |    |    |  |              | |
//                                |              |
//              1    0    0    1  | 001..std     | 1..bit frame anticoll
//                                | 010..double  |
//                                | 011..triple  |
//
//            SAK:
//
//            +----+----+----+----+----+----+----+----+
//            | b8 | b7 | b6 | b5 | b4 | b3 | b2 | b1 |
//            +-|--+-|--+-|--+-|--+-|--+-|--+-|--+-|--+
//              |    |    |    |    |    |    |    |
//              |         |    |    |    |	   |
//              |   RFU   |    |    |    |   RFU   |
//		|	  |    |    |    |         |
//	     FulComp      |  Plus Stand  |       Light
//                        |              |
//                        1              0 .. UID complete, ATS available
//                        0              0 .. UID complete, ATS not available
//                        X              1 .. UID not complete
//
//		For UltraLight All Features = 0;
//______________________________________________________________________________
//******************************************************************************
//******************************************************************************
typedef long int __time_t;
//typedef __time_t time_t;
//******************************************************************************
#define TIME_SEC	0
#define TIME_MIN	(TIME_SEC+1)
#define TIME_HOUR	(TIME_MIN+1)
#define TIME_DAY	(TIME_HOUR+1)
#define TIME_DATE	(TIME_DAY+1)
#define TIME_MON	(TIME_DATE+1)
#define TIME_YEAR	(TIME_MON+1)
#define TIME_MASK	(TIME_YEAR+1)
#define TIME_SIZE	(TIME_MASK+1)
//--------------------------------
#pragma pack(1)
typedef struct
{
	unsigned char 	Sec;			// Секунды
	unsigned char 	Min;			// Минуты
	unsigned char 	Hour;			// Часы
	unsigned char 	Day;			// День недели
	unsigned char 	Date;			// Дата
	unsigned char 	Mon;			// Месяц
	unsigned char 	Year;			// Год (2 мл. цифры)
	unsigned char 	Mask;
}TTimeDate;	// Size 1+1+1+1+1+1+1+1 = 8 b
#pragma pack()
//--------------------------------
/*
#pragma pack(1)
typedef struct
{
	unsigned short	DayOfYear:9, Year:7;
}TDayFromMill;	// Size 5+4+7 = 16b = 2 B
typedef struct
{
	unsigned short	Date:5, Mon:4, Year:7;
}TPackDate;	// Size 5+4+7 = 16b = 2 B
typedef struct
{
	unsigned short	Min:6, Hour:5, OperNr:2, Reserv:3;
}TPackTime;	// Size 6+5+2+3 = 16b = 2 B
typedef struct
{
	unsigned short	Min:6, Reserv:1, Hour:5, OperNr:2, Reserv1:2;
}TPackTime1;	// Size 6+1+5+2+2 = 16b = 2 B
#pragma pack()
#pragma pack(1)
typedef struct
{
	unsigned short 		Time;
	unsigned short 		Date;
}TPackTimeDate;	// Size 2+2 = 4 B
#define	DAYSTART	0
#define	MONSTART	5
#define	YEARSTART	9
//#define	SECSTART	0
#define	MINSTART	0
#define	HOURSTART	6
#define	PACKTIMEMASK	0x07FF
typedef union
{
  unsigned long		RawData;
  struct
  {
    TPackTime 		Time;
    TPackDate 		Date;
  }Pack;
}TpTimeDate;	// Size 2+2 = 4 B
#pragma pack()
//--------------------------------
#pragma pack(1)
typedef union
{
  unsigned long		DateTime;
  TPackTimeDate		PackTimeDate;
}TPDateTime;
*/
//--------------------------------
typedef struct
{
  unsigned long long	Sec:6, Min:6, Hour:5, Date:5, Mon:4, Year:12, Oper:2, Reserv:24;
}TDateTimel;	// Size 8 B
typedef struct
{
  unsigned long		Sec:6, Min:6, Hour:5, Oper:3, Reserv:4, Flags:8;
}TPackTimel;	// Size 4 B
//******************************************************************************
typedef struct
{
  unsigned char 	DevID;			// ID устройства
  unsigned char 	SoftVersionBE[4];	// Версия ПО 4 byte BE
  char     	        Date[12];		// Дата
  char    	        Time[9];		// Время
  char 		        TextComment[46];	// Текстовый комментарий
}TSoftStamp;	// Size 1+4+12+9+46 = 72 b (0x48)
#pragma pack()
//******************************************************************************

//******************************************************************************
// Functions definition
//******************************************************************************
#define	cmemcpy		memcpy_P
#define	cmemcmp		memcmp_P
#define	cstrcpy		strcpy_P
#define	cstrcmp		strcmp_P
#define	cstrlen		strlen_P
#ifdef __IAR_ARM_C
#define	const	flash	
#endif
//******************************************************************************

// inline functions & macroses
//--------------------------------------------------------------------------------
inline unsigned long asDWORD(unsigned char* a){return ((unsigned long)*a) + (((unsigned long)*(a+1))<<8) + (((unsigned long)*(a+2))<<16) + (((unsigned long)*(a+3))<<24);};
inline unsigned short toWORD(unsigned char* a){return (((unsigned short)*(a+1))<<8) + *a;};
//********************************************************************************
#define ABS(x)         (x < 0) ? (-x) : x
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
//********************************************************************************
#define WBVAL(x) (x & 0xFF),((x >> 8) & 0xFF)


//********************************************************************************
//*******************************************************************
#endif

