/************************************************************************************** 
 * � ���� ����� ����� ����������� � ��������� SPORT ��� ����� � ������� �������
 * ���� ������� ��������� ������ � ����� ������ � ��� � ����� � ������� ������
 * ������ ���� ������� ������ ������������� ������������� ������ ��������� �����!!!
***************************************************************************************/
#include "xpander.h"
#include "timer0.h"
#include "sport0.h"
#include "ports.h"
#include "rele.h"
#include "pll.h"


static u8 XPANDER_read_byte(u8);
static void XPANDER_write_byte(u8, u8);
static void XPANDER_set_bit(u8, u8);
static void XPANDER_clear_bit(u8, u8);
static void XPANDER_toggle_bit(u8, u8);
static void XPANDER_adc_init(void);
static u16 XPANDER_adc_get_data(u8);


section("L1_code")
void pin_set(u8 port, u8 pin)
{
    u8 dir = port - 1;
    XPANDER_set_bit(port, 1 << pin);
    XPANDER_set_bit(dir, 1 << pin);
}

section("L1_code")
void pin_clr(u8 port, u8 pin)
{
    u8 dir = port - 1;
    XPANDER_clear_bit(port, 1 << pin);
    XPANDER_set_bit(dir, 1 << pin);
}

section("L1_code")
void pin_hiz(u8 port, u8 pin)
{
    u8 dir = port - 1;
    XPANDER_clear_bit(port, 1 << pin);
    XPANDER_clear_bit(dir, 1 << pin);
}

section("L1_code")
u8 pin_get(u8 port)
{
    return XPANDER_read_byte(port);
}


/**************************************************************************************** 
 * ������ ����� �� ����� - ��� ������ ���� �� 3 ����� 
 * � ������ ������ ������� ������ "0"
 * ��������: �����
 * �������:  ������
 ****************************************************************************************/
section("L1_code")
static u8 XPANDER_read_byte(u8 addr)
{
    u8 byte;

    SPORT0_write_read(0);
    delay_us(10);
    SPORT0_write_read(addr);
    delay_us(10);
    byte = SPORT0_write_read(0);
    delay_us(10);

    return byte;
}


/*************************************************************************************** 
 * ������� ��������� ������� �� SPORT � ���������, ������ ���� �� 3 �����
 * �������� ����� ��������� - 10 ���. ��������� ��������� ���� ������� �� 0.125 ���
 * � ������ ������ ������� ������ "0x80"
 * ��������: �����, ������
 * �������:  ���
  ***************************************************************************************/
section("L1_code")
static void XPANDER_write_byte(u8 addr, u8 data)
{
    SPORT0_write_read(0x80);
    delay_us(10);
    SPORT0_write_read(addr);
    delay_us(10);
    SPORT0_write_read(data);
    delay_us(10);
}

/*************************************************************************************** 
 * ��������� 1-�� ���� �� ������
 * �������� ����� ��������� - 10 ���. ��������� ��������� ���� ������� �� 0.125 ���
 * ������ ������� ������ "0x20"
 * ��������: �����, ������
 * �������:  ���
  ***************************************************************************************/
section("L1_code")
static void XPANDER_set_bit(u8 addr, u8 data)
{
    SPORT0_write_read(0x20);
    delay_us(10);
    SPORT0_write_read(addr);
    delay_us(10);
    SPORT0_write_read(data);
    delay_us(10);
}


/*************************************************************************************** 
 * ����� 1-�� ���� �� ������
 * �������� ����� ��������� - 10 ���. ��������� ��������� ���� ������� �� 0.125 ���
 * ������ ������� ������ "0x10"
 * ��������: �����, ������
 * �������:  ���
  ***************************************************************************************/
section("L1_code")
static void XPANDER_clear_bit(u8 addr, u8 data)
{
    SPORT0_write_read(0x10);
    delay_us(10);
    SPORT0_write_read(addr);
    delay_us(10);
    SPORT0_write_read(data);
    delay_us(10);
}


/*************************************************************************************** 
 * ������������ 1-�� ���� �� ������
 * �������� ����� ��������� - 10 ���. ��������� ��������� ���� ������� �� 0.125 ���
 * ������ ������� ������ "0x10"
 * ��������: �����, ������
 * �������:  ���
  ***************************************************************************************/
section("L1_code")
static void XPANDER_toggle_bit(u8 addr, u8 data)
{
    SPORT0_write_read(0x40);
    delay_us(10);
    SPORT0_write_read(addr);
    delay_us(10);
    SPORT0_write_read(data);
    delay_us(10);
}
