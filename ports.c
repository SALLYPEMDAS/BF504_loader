#include "xpander.h"
#include "ports.h"
#include "sport0.h"
#include "timer0.h"
#include "rele.h"
#include "led.h"

/**
 * ������������� ������ �� �� + sport0 � SPI1
 * ��-��������� ��� ����� �� ����� � Z - ��������� (page 9 - 3)
 * ������ �������� ��� �� �������
 * 
 */
section("L1_code")
void init_bf_ports(void)
{
	/* ����� F: ��������� ������� � ��� �� ���� */
	*pPORTF_FER = 0;
	*pPORTFIO_DIR = 0;

	/* �� ���� ������ ��������� ������� */
	*pPORTG_FER &= ~(PG0 | PG1 | PG2 | PG3 | PG4 | PG5 | PG6 | PG7 | PG8 | PG9 | PG10 | PG11 /* | PG12 | PG13*/  | PG14 | PG15);
	*pPORTGIO_CLEAR = (PG3 | PG4 | PG11);	/* ������ � ���� */	
	*pPORTGIO_DIR = (PG3 | PG4 | PG11);	/* �� �����, ��� ��������� �� ����  */

	*pPORTH_FER &= ~(PH0 | PH1 | PH2);	/* ����� H: PH1 �� ����-��������� �� ����� � � ���� */
	*pPORTHIO_CLEAR = (PH0 | PH2);		/* ������ � ���� */	
	*pPORTHIO_DIR = (PH0 | PH2);

	 SPORT0_config();			/* �����0  */
}

/**
 * ������������� ������ �� ������
 * ��-��������� ��� ����� �� ����� � Z - ��������� (page 9 - 3)
 */
section("L1_code")
void init_atmega_ports(void)
{
	/* ������� � ������ reset � ���������� PH0 */
	*pPORTH_FER &= ~PH0;			/* ��������� ������� */
	*pPORTHIO_CLEAR = PH0;			/* ������ 0 �� ����� */	
	*pPORTHIO_DIR |= PH0;			/* ������ �� �� ����� */
	ssync();	
	*pPORTHIO_SET = PH0;			/* ������ 1 �� ����� */
	ssync();

	delay_ms(WAIT_START_ATMEGA);	 /* ��������, �.�. ��� DSP ����� ������� - �� �������� ���������������� Atmega ��������!!! */

	pin_clr(PORTG,  0x0F);	/* �� ����� ����� G - ����� */
	pin_set(SD_SRCSEL_PORT, SD_SRCSEL_PIN);

	RELE_init();	/* ����. ���� ���� */
}


/**
 * �������� SD ����� 
 * SD ����� ���������� � BF
 */
void select_sdcard_to_bf(void)
{
	pin_clr(SD_SRCSEL_PORT, SD_SRCSEL_PIN);
	delay_ms(100);
	pin_clr(SD_EN_PORT, SD_EN_PIN);
	delay_ms(10);
	pin_set(AT_SD_WP_PORT, AT_SD_WP_PIN);
	delay_ms(100);
	pin_clr(AT_SD_WP_PORT, AT_SD_WP_PIN);
	delay_ms(10);
	pin_set(AT_SD_CD_PORT, AT_SD_CD_PIN);
	delay_ms(10);	
}

/**
 */
void unselect_ports(void)
{
	pin_set(SD_EN_PORT, SD_EN_PIN);
	delay_ms(10);

	/* ������� ��������� PH0 */
	*pPORTH_FER &= ~PH0;			/* ��������� ������� */
	*pPORTHIO_CLEAR = PH0;			/* ������ 0 �� ����� */	
	*pPORTHIO_DIR |= PH0;			/* ������ �� �� ����� */
	ssync();
	delay_ms(10);		
}

