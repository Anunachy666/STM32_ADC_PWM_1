//******************************************************************************
// Errors_1.h - Файл списка стандартных сообщений об ошибках
//******************************************************************************
//
//	FULLNAME:  Error Codes definitions
//
//	NICKNAME:
//
//	PURPOSE:   Error Codes & return Messages definitions
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

//***********************************************************
//===========================================================
//-----------------------------------------------------------
//___________________________________________________________
//***********************************************************

#ifndef ERRORS_H
#define ERRORS_H
//***********************************************************
//***********************************************************
//////////////////////////////////////////////////////////////////////////////
// Reader Error Codes                   Base Address Start:      0000
//                                      Base Address End:        -999
//////////////////////////////////////////////////////////////////////////////
#define READER_ERR_BASE_START           (   0)
#define MI_OK                           (   0)
#define MI_CHK_OK                       (   0)
#define MI_CRC_ZERO                     (   0)
// ICODE1 Error   Codes
#define I1_OK                           (   0)
#define I1_NO_ERR                       (   0)

#define MI_NOTAGERR                     (  -1)
#define MI_CHK_FAILED                   (  -1)
#define MI_CRCERR                       (  -2)
#define MI_CHK_COMPERR                  (  -2)
#define MI_EMPTY                        (  -3)
#define MI_AUTHERR                      (  -4)
#define MI_PARITYERR                    (  -5)
#define MI_CODEERR                      (  -6)

#define MI_SERNRERR                     (  -8)
#define MI_KEYERR                       (  -9)
#define MI_NOTAUTHERR                   ( -10)
#define MI_BITCOUNTERR                  ( -11)
#define MI_BYTECOUNTERR                 ( -12)
#define MI_IDLE                         ( -13)
#define MI_TRANSERR                     ( -14)
#define MI_WRITEERR                     ( -15)
#define MI_INCRERR                      ( -16)
#define MI_DECRERR                      ( -17)
#define MI_READERR                      ( -18)
#define MI_OVFLERR                      ( -19)
#define MI_POLLING                      ( -20)
#define MI_FRAMINGERR                   ( -21)
#define MI_ACCESSERR                    ( -22)
#define MI_UNKNOWN_COMMAND              ( -23)
#define MI_COLLERR                      ( -24)
#define MI_RESETERR                     ( -25)
#define MI_INITERR                      ( -25)
#define MI_INTERFACEERR                 ( -26)
#define MI_ACCESSTIMEOUT                ( -27)
#define MI_NOBITWISEANTICOLL            ( -28)
#define MI_QUIT                         ( -30)
#define MI_CODINGERR                    ( -31)
#define MI_SENDBYTENR                   ( -51)
#define MI_CASCLEVEX                    ( -52)
#define MI_SENDBUF_OVERFLOW             ( -53)
#define MI_BAUDRATE_NOT_SUPPORTED       ( -54)
#define MI_SAME_BAUDRATE_REQUIRED       ( -55)

#define MI_WRONG_PARAMETER_VALUE        ( -60)

// ICODE1 Error   Codes
#define I1_WRONGPARAM                   ( -61)
#define I1_NYIMPLEMENTED                ( -62)
#define I1_TSREADY                      ( -63)

#define I1_TIMEOUT                      ( -70)
#define I1_NOWRITE                      ( -71)
#define I1_NOHALT                       ( -72)
#define I1_MISS_ANTICOLL                ( -73)

#define I1_COMM_ABORT                   ( -82)

#define MI_NOT_ENOUGH_MEMORY            ( -98)
#define MI_BREAK                        ( -99)
#define MI_NY_IMPLEMENTED               (-100)
#define MI_NO_MFRC                      (-101)
#define MI_MFRC_NOTAUTH                 (-102)
#define MI_WRONG_DES_MODE               (-103)
#define MI_HOST_AUTH_FAILED             (-104)

#define MI_WRONG_LOAD_MODE              (-106)
#define MI_WRONG_DESKEY                 (-107)
#define MI_MKLOAD_FAILED                (-108)
#define MI_FIFOERR                      (-109)
#define MI_WRONG_ADDR                   (-110)
#define MI_DESKEYLOAD_FAILED            (-111)
#define MI_RECBUF_OVERFLOW              (-112)
#define MI_WRONG_SEL_CNT                (-114)

#define MI_WRONG_TEST_MODE              (-117)
#define MI_TEST_FAILED                  (-118)
#define MI_TOC_ERROR                    (-119)
#define MI_COMM_ABORT                   (-120)
#define MI_INVALID_BASE                 (-121)
#define MI_MFRC_RESET                   (-122)
#define MI_WRONG_VALUE                  (-123)
#define MI_VALERR                       (-124)

#define MI_COMM_ABORT		          (-120)
#define MI_CALLOPEN		            (-121)
//-----------------------------------------------------------
#define IDLE		0x00	// select request idle
#define ALL		0x01	// select request all
#define KEYA		0x00	// select master key A
//#define KEYB		0x04	// select master key B
//-----------------------------------------------------------
//***********************************************************
// User Program Bit Error
//-----------------------------------------------------------
#define	TOUTF		5		//TimeOut Flag
#define	ERRORF		6		//Error Flag
#define	BUSYF		7		//Busy Flag
//***********************************************************
// User Programm Error
//-----------------------------------------------------------
#ifndef NULL
#define	NULL		0
#endif
#define	OK			(0)					// Успешное выполнение функции
//#define	Ok			NULL					// Успешное выполнение функции
#define	COMMAND_OK		OK						// Command condition OK
#define	COMND_ERROR		MI_WRONG_PARAMETER_VALUE// Command error
#define	TIMEOUT_ERROR		MI_NOTAGERR				// TimeOut
#define	COMMAND_TIMEOUT		TIMEOUT_ERROR			// Command condition Error
#define	CRC_ERROR		MI_CRCERR				// CheckSumm Error
#define	CEMPTY_ERROR		MI_EMPTY				// Empty
#define	DATA_ERROR		MI_WRONG_VALUE			// Invalid Data
#define	NO_DATA_ERROR		MI_EMPTY				// No Data present
#define WRITE_ERROR		MI_WRITEERR				// Write Operation error
#define	ADDRESS_ERROR		MI_WRONG_ADDR			// Invalid Address
#define	OVERFLOW_ERROR		MI_OVFLERR				// Overflow detected
#define READ_ERROR		MI_READERR				// Read Operation error
#define	BREAK_ERROR		MI_BREAK				// Break Operation Error
#define	BUSY_ERROR		I1_TSREADY				// Device Busy
#define	COMPARE_ERROR		( -64)					// Comparation Error
#define	PAPER_END_ERROR		( -65)	        		// Paper End Error
#define	ADC_ERROR		( -66)					// ADC Error
#define	UNKNOWN_ERROR		(-128)					// Unknown Error Detected
//***********************************************************
//------------------------------------------------------------------------------
#define	LOW_BATTERY_ERROR	(-90)
//-----------------------------------------------------------
#define	EEPROM_ERROR		(-0x60)		//-96
#define	FP_ERROR		(-0x61)		//-97
#define	KL_ERROR		(-0x62)		//-98
#define	TIMER_ERROR		(-0x63)		//-99
#define	TPU_ERROR		(-0x64)		//-98
#define	NOFP_ERROR		(-0x71)		//-113
#define	NOKL_ERROR		(-0x72)		//-114
#define	HARD_ERROR		(-0x78)		//-120
#define	EKLZ_ERROR		(-0x79)		//-121

#define	FLP_ERROR		0x40
#define	COMP_ERROR		COMPARE_ERROR
#define	FPBUSY_ERROR		BUSY_ERROR
#define	FPERASE_ERROR		(-122)
//-----------------------------------------------------------
#define	BLOCK_ERROR	0xF3
//-----------------------------------------------------------
#define	E_MIF		0x80000000
#define	E_NOTAGERR	MI_NOTAGERR		// Тайм-аут в устройстве
#define	E_CRCERR	MI_CRCERR		// Неверная контрольная	сумма
#define	E_EMPTY		MI_EMPTY		// Переполнение	значения
#define	E_WRITE		MI_ACCESSERR	// Ошибка записи (в порт)
#define	E_QUIT		MI_QUITERR		// Предварительно посланная команда прерывания
#define	E_TO_ERROR	MI_ACCESSTIMEOUT// Отсутствует соединение с ридером
//-----------------------------------------------------------
//***********************************************************

//***********************************************************
//-----------------------------------------------------------
#define	FIL_ERRORS	(0x200)
//***********************************************************

//***********************************************************
//-----------------------------------------------
#define	CARDSNR_ERROR	(-0x50)

#define	HEADUP_ERROR	(-0x51)
#define	PE_ERROR		(-0x52)
#define	POWER_ERROR		(-0x53)
#define	POWER_ON_ERROR	(-0x54)
#define	ESRAM_ERROR		(-0x41)
#define	EUART_ERROR		(-0x42)
#define	NOINDICAT_ERROR	(-0x46)
#define	INDICATOR_ERROR	(-0x47)
#define	SRAMRD_ERROR	(-0x64)

#define	NOCIF_ERROR		(-0x5A)
#define	NOCARD_ERROR	(-0x82)
#define	WAITREPAIR_ERROR	(-0x1F)
//-----------------------------------------------------------
//***********************************************************
//***********************************************************

#endif
