/************************************************************************************** 
 * ��� ���������� BF504F � ���������� ������� 
 * CLKIN    = 19.2 ���
 * ��������� ������� � ����� ���:
 * CCLK     =  288, 230.4, 192, 96 ���
 * SCLK     =  72, 64, 48, 32, 24  ���
 * ����� ����� ����� �������� � ������� �������, �� ������� SCLK ������ ���� �����!
 * ����� ��������� ������� ��� ���������� ������������������ � ����
 * ������� ������� ������ �� ���������
 * ������� ����� ��������!
 * CCLK = 192MHZ SCLK = 48MHZ
 **************************************************************************************/
#ifndef	__PLL_H__
#define __PLL_H__

#include "my_defs.h"


/* ������� ������ ������� - ����������� �����. �� ������� � eeprom  */
#define		CAUSE_POWER_OFF		0x12345678
#define		CAUSE_EXT_RESET		0xabcdef90
#define		CAUSE_BURN_OUT		0xaa55aa55		
#define		CAUSE_UNKNOWN		0xFFFFFFFF	


/* ������� ������, ������������ �� ����� �����, ��� ��������� ����� ��� SD ����� */
#define     QUARTZ_CLK_FREQ      19200000


#define SCLK_VALUE	    (48000000UL)
#define PLLCTL_VALUE        0x1400 	/* MSEL[5:0] = 10 - �������� VCO = 192��� �� 19.2 */
#define PLLDIV_VALUE        0x0004	/* CSEL[1:0] = 0  - �������� CCLK = VSO/ 1 = 192���, SSEL[3:0] = 6  - �������� SCLK = VSO/4 = 48��� */

/* ��� �������� ��� Power Managements */
#define PLLSTAT_VALUE       0x0000	/* NB: ������ ������!!!  */
#define PLLLOCKCNT_VALUE    0x0200 	/* ����� 512 ������ ������� */
#define PLLVRCTL_VALUE      (1 << 9)| (1 << 11) /* ����������� �� ����������� �� ����� PF8 � PF9 */

/* ������� ������� ���������� � PLL  */
#define TIMER_PERIOD (SCLK_VALUE)


void PLL_init(void);
void PLL_sleep(DEV_STATE_ENUM);

void PLL_reset(void);
void PLL_fullon(void);
void PLL_hibernate(void);

#endif				/* pll.h */
