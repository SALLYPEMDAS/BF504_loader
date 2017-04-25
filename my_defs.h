#ifndef _MY_DEFS_H
#define _MY_DEFS_H

#include <signal.h>
#include <stdint.h>
#include <time.h>


/* ��������� ��� 504 ��������, �� 506!!!!  */
#ifndef _WIN32			/* Embedded platform */
#include <adi_types.h>
#include <sys/exception.h>
#include <cdefBF504f.h>
#include <defBF504f.h>
#include <ccblkfn.h>
#include <float.h>
#include "config.h"


/* These types must be 16-bit, 32-bit or larger integer */
typedef int INT;
typedef unsigned int UINT;

/* These types must be 8-bit integer */
typedef char CHAR;
typedef unsigned char UCHAR;
typedef unsigned char BYTE;

/* These types must be 16-bit integer */
typedef short SHORT;
typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned short WCHAR;

/* These types must be 32-bit integer */
typedef long LONG;
typedef unsigned long ULONG;
typedef unsigned long DWORD;

#else				/* windows   */
#include <windows.h>
#include <tchar.h>
#endif

#ifndef u8
#define u8 unsigned char
#endif

#ifndef s8
#define s8 char
#endif

#ifndef c8
#define c8 char
#endif

#ifndef u16
#define u16 unsigned short
#endif


#ifndef s16
#define s16 short
#endif

#ifndef i32
#define i32  int
#endif


#ifndef u32
#define u32 unsigned long
#endif


#ifndef s32
#define s32 long
#endif


#ifndef u64
#define u64 uint64_t
#endif


#ifndef s64
#define s64 int64_t
#endif

/* long double �� �������������  */
#ifndef f32
#define f32 float32_t
#endif


#ifndef bool
#define bool u8
#endif


#ifdef true
#define true 1
#endif

#ifdef false
#define false 0
#endif

#ifndef IDEF
#define IDEF static inline
#endif

/* �� ���� ����� ����� �������� ���� ���������� */
#define 	BROADCAST_ADDR	0xff

/* ������� ������� � �����  */
#define 	NUM_ADS1282_PACK	20

#define 	ADC_CHAN			4	/* ����� �������  */
#define		MAGIC				0x4b495245


/*******************************************************************************
 * ��������� ��������� ��� �������� State machine
 *******************************************************************************/
typedef enum {
	DEV_POWER_ON_STATE = 0,
	DEV_CHOOSE_MODE_STATE = 1,
	DEV_INIT_STATE,
	DEV_GET_3DFIX_STATE,
	DEV_TUNE_Q19_STATE,	/* ��������� ��������� ������ 19 ��� */
	DEV_SLEEP_AND_DIVE_STATE,	/* ���������� � ��� ����� ������� ������ */
	DEV_WAKEUP_STATE,
	DEV_REG_STATE,
	DEV_FINISH_REG_STATE,
	DEV_BURN_WIRE_STATE,
	DEV_SLEEP_AND_POPUP_STATE,
	DEV_WAIT_GPS_STATE,
	DEV_HALT_STATE,
	DEV_COMMAND_MODE_STATE,
	DEV_POWER_OFF_STATE,
	DEV_ERROR_STATE = -1	/* ������ */
} DEV_STATE_ENUM;


/**
 * � ���� ���������� ������������ ��������� ��������
 * ������������� ����� ������:
 * 16.10.12 17.15.22	// ����� ������ �����������
 * 17.10.12 08.15.20	// ����� ��������� �����������
 * 18.10.12 11.17.00	// ����� ��������, �������� 5 ���. ������� ��������
 * ....
 */
#pragma pack(4)
typedef struct {
	u32 wakeup_time;	/* ����� ������ ���������� ����� ������������ */
	u32 start_time;		/* ����� ������ �����������  */
	u32 finish_time;	/* ����� ��������� ����������� */
	u32 burn_time;		/* ����� ������ �������� ��������  */
	u32 popup_time;		/* ������ ������ �������� */
	u32 gps_time;		/* ����� ��������� gps ����� ������� �������� */
	u32 am3_popup_time;	/* ��������� ����� �������� �� ������   */
	u8  am3_h0_time;
	u8  am3_m0_time;
	u8  am3_h1_time;
	u8  am3_m1_time;
} TIME_RUN_STRUCT;



/* ���������, ����������� �����, �������� c8 �� u8 - ����� �� �.�. ������������� */
#pragma pack(1)
typedef struct {
	u8 sec;
	u8 min;
	u8 hour;
	u8 week;		/* ���� ������...�� ����������� */
	u8 day;
	u8 month;		/* 1 - 12 */
#define mon 	month
	u16 year;
} TIME_DATE;
#define TIME_LEN  8		/* ���� */


/**
 * ���� ��������� ���� � ����������   
 */
#pragma pack(4)
struct timespec {
	time_t tv_sec;		/* �������  */
	long   tv_nsec;		/* ����������� */
};


/**
 * � ��� �������� ������� �������� ��������� ��� ������ �� SD ����� (pack �� 1 �����!)
 */
#pragma pack(1)
typedef struct {
	c8 DataHeader[12];	/* ��������� ������ SeismicData\0  */
	c8 HeaderSize;		/* ������ ��������� - 80 ���� */
	c8 ConfigWord;
	c8 ChannelBitMap;
	u16 BlockSamples;
	c8 SampleBytes;

	union {
		TIME_DATE time;
		u32 dword[2];
		u64 qword;
	} SampleTime;		/* �������� ����� ������ */

	u16 Bat;
	u16 Temp;

	u8  Rev;  		/* ������� = 2 ������  */
	u16 Board;

	u8  NumberSV;
	s16 Drift;

	union {
		TIME_DATE time;
		u32 dword[2];	/* �� ������������ ����  */
		u64 qword;
	} SedisTime;

	union {
		TIME_DATE time;	/* ����� ������������� */
		u32 dword[2];	/* �� ������������ ����  */
		u64 qword;
	} GPSTime;

	union {
		struct {
			c8 rsvd2;
			bool comp;	/* ������ */
			c8 pos[23];	/* ������� (����������) */
			c8 rsvd3[3];
		} coord;	/* ���������� */

		u32 dword[7];	/* ������ */
	} params;
} ADC_HEADER;


//////////////////////////////////////////////////////////////////////////////////////////
//                        �� �����������
//////////////////////////////////////////////////////////////////////////////////////////

/**
 * ����� ������ �� ����������� - 20 ��������� ������������� -  240 ����, �������� �� 1!!!
 */
#pragma pack(1)
typedef struct {
	u8 len;			/* ����� ������ ��� ����������� ����� */
	u8 adc;			/* ����� ���������� ��� - ������ ���� ������ */
	u16 msec;		/* ������������ ������� ��������� */
	u32 sec;		/* UNIX TIME ������� ��������� */

	struct {		/* 3-� ������� ����� (* 4) */
		unsigned x:24;
		unsigned y:24;
		unsigned z:24;
		unsigned h:24;
	} data[NUM_ADS1282_PACK];

	u16 rsvd;		/* ������������ */
	u16 crc16;		/* ����������� ����� ������  */
} ADS1282_PACK_STRUCT;



/** 
 *  ������ � ������ ��������� �� ����������� 64 ����. �������� ������ ���������� ��� ������ (������ 2 ����� + 1 len + 2 CRC16)
 */
#pragma pack(4)
typedef struct {
	u8  len;		/* ����� ������ ��� ����������� ����� */
	u8  st_main;            /* �������: ��� �������, ��� ��������, ������ � �������, �� �������, �����������, ������������ ������, ����������, ����� ������ */
	u8  st_test;            /* ���������������� � ������: 0 - �����, 1 - ������ T&P, 2 - �������/ ������, 3 - �����, 4 - GPS, 5 - EEPROM, 6 - ����� SD, 7 - flash */
	u8  st_reset;		/* ������� ������ */

	u32  rsvd0[5];		/* ������ */

	u32  eeprom;		/* ������ EEPROM */

	s16  am_power_volt;	/* �� 1 ����-����� 4 ������ ���������� */
	s16  burn_ext_volt;	

	s16  burn_volt;
	s16  regpwr_volt;

	s16  iam_sense;	        /* 3 ������ ���� */
	s16  ireg_sense;	

	s16  iburn_sense;
	s16  temper;		/* ����������� = t * 10 */

	u32  press;		/* �������� �� */

	s16  acc_x;		/* ������������ - X */
	s16  acc_y;		/* ������������ - Y */

	s16  acc_z;		/* ������������ - Z */
	s16  comp_x;		/* ������ - X */

	s16  comp_y;		/* ������ - Y */
	s16  comp_z;		/* ������ - Z */

	u16   rsvd1;
	u16   crc16;		/* ����������� ����� ������  */
} DEV_STATUS_STRUCT;


/**
 * ����� ������ ������� �� �����������
 */
#pragma pack(1)
typedef struct {
	u8 len;			/* ����� ����  */
	u32 time_work;		/* ����� ������ � ������ ����� ������ */
	u32 time_cmd;		/* ����� ������ � ��������� ������ */
	u32 time_modem;		/* ����� ������ ������ */
	u16 crc16;		/* CRC16  */
} DEV_WORK_TIME_STRUCT;


/* ��� � ����� */
#pragma pack(2)
typedef struct {
	u8  len;		/* ����� */
	u8  rev;		/* ������� */
	u16 addr;		/* ����� ����� */
	u32 time;		/* ����� ���������� */
	u16 crc16;
} DEV_ADDR_STRUCT;


/**
 *  ��������� ��� ������ ������. �� ����� ��������� �� offset : 24
 */
#pragma pack (1)
typedef struct {
	u8  len;
	u8  chan;	/* ����� ������ */
	u32 offset;	/* ����������� 1 - �������� */
	u32 gain;	/* ����������� 2 - �������� */
	u16 crc16;
} DEV_CHAN_STRUCT;


/**
 * ������� ������� ������ � UART
 */
#pragma pack (4)
typedef struct {
    u8  cmd;
    u8  rsvd[3];

    union {
      u8  cPar[16];
      u16 wPar[8];
      u32 lPar[4];
      f32 fPar[4];
    } u;
} DEV_UART_CMD;

/**
 * ��� ����� ������� � eeprom � ���� u32
 * ������ eeprom ������ ������� - ����� * 2 � + 1 2-� �����
 * ���������� ������ � �������������� �����!!!
 * ��� ������� ���������� ���� ����� �������!
 */
enum eeprom_id {
   EEPROM_MOD_ID = 0,
   EEPROM_RSVD0,
   EEPROM_RSVD1,
   EEPROM_TIME_WORK,
   EEPROM_TIME_CMD,
   EEPROM_TIME_MODEM,
   EEPROM_DAC19_COEF,
   EEPROM_DAC4_COEF,
   EEPROM_RSVD2,
   EEPROM_ADC_OFS0,
   EEPROM_ADC_FSC0,
   EEPROM_ADC_OFS1,
   EEPROM_ADC_FSC1,
   EEPROM_ADC_OFS2,
   EEPROM_ADC_FSC2,
   EEPROM_ADC_OFS3,
   EEPROM_ADC_FSC3,
   EEPROM_RESET_CAUSE,
   EEPROM_END_OF_VAR
};


#pragma pack(4)

#endif				/* my_defs.h */
