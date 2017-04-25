/* ������ ���������   */
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <bfrom.h>

#include "my_defs.h"
#include "timer0.h"
#include "xpander.h"
#include "ports.h"
#include "flash.h"
#include "rele.h"
#include "led.h"
#include "rsi.h"
#include "pll.h"
#include "fatfs.h"

#define LOADER_RAM_NAME	"loader.ram"
#define LOADER_ROM_NAME	"loader.rom"
#define BOOT_LOG_NAME	"boot.log"


#define  BUFFER_SIZE		1024
#define  LOADER_RAM_ADDRESS	0x20010000
#define  LOADER_ROM_ADDRESS	0x20020000

static u8 buf[BUFFER_SIZE];	// ������ ���� ����
static FIL loader_ram;
static FIL loader_rom;
static FIL boot_log;
static FATFS fatfs;		/* File system object */
static char *str[] = {
    "Warning: loader.ram doesn't exist. Continue booting\n",//0
    "Warning: loader.rom doesn't exist\n",                  //1
    "Error  : can't to erase FLASH\n",                      //2
    "Info   : %d bytes written to flash\n",                 //3
    "Success: load loader.ram\n",                           //4
    "Success: load loader.rom\n",                           //5
    "Info:    Continue booting\n\n"                           //6
};                                                          


/* �-��� main()  */
int main(void)
{
    int res;
    UINT bw, num;
    u16 word;
    int i;
    PLL_init();			/* ����. PLL �� ������ 19.2 ��� � ������ ������! */
    init_bf_ports();
    init_atmega_ports();
    select_sdcard_to_bf();
    delay_ms(10);
    LED_on(LED_RED);
  
    /* ��������� �� */
    do {

	// 1
	res = f_mount(0, &fatfs);
	if (res != 0) {		// �� �����������-�������� ����������
	    break;
	}
	//2
	/* ���� ��� �� - ������� boot_log */
	res = f_open(&boot_log, BOOT_LOG_NAME, FA_WRITE | FA_READ | FA_OPEN_ALWAYS);
	if (res) {
	    break;
	}
	// ��������� ������	
	i = f_size(&boot_log);
	if(i > 0x7F000000) {
           f_truncate(&boot_log);
           i = 0;
         }

	// ���������� ��������� �����
        f_lseek (&boot_log, i);

	// 3
	/* ��������� ������������ ���� ��� RAM */
	res = f_open(&loader_ram, LOADER_RAM_NAME, FA_READ | FA_OPEN_EXISTING);
	if (res) {
	    f_write(&boot_log, str[0], strlen(str[0]), &bw);
	    f_close(&boot_log);
	    break;
	}


	// 4
	/* ��������� ������������ ���� */
	res = f_open(&loader_rom, LOADER_ROM_NAME, FA_READ | FA_OPEN_EXISTING);
	if (res) {
	    f_write(&boot_log, str[1], strlen(str[1]), &bw);
	    f_close(&boot_log);
	    break;
	}

      // ���� ����� � ��� ����	
       RELE_on(RELEPOW);	// �������� ����������  
       LED_on(LED_GREEN);
       LED_off(LED_RED);
       delay_ms(10);

	// 5
	/* 2 ����� � ��� ���� - ������� 5 ������ �������� */
	FLASH_init();
	for (i = 0; i < 5; i++) {
	    res = FLASH_erase_page(LOADER_RAM_ADDRESS + i * 0x10000);
	    if (res) {
		f_write(&boot_log, str[2], strlen(str[2]), &bw);
		f_close(&boot_log);
		LED_on(LED_RED);	// �e������ ����������  
		break;
	    }
	    LED_toggle(LED_GREEN);
	}

	// 6 - ������ ������, ����� � ����        
	i = 0;
	while (1) {
	    res = f_read(&loader_ram, buf, BUFFER_SIZE, &num);
	    FLASH_write_buf(LOADER_RAM_ADDRESS + i, buf, num);
	    i += num;		// �������� ����� �� ���������� ����� ����
	    if (num < BUFFER_SIZE) {
		sprintf((char *) buf, str[3], i);
		f_write(&boot_log, buf, strlen((char *) buf), &bw);
		break;
	    }
	    LED_toggle(LED_GREEN);
	}
	f_write(&boot_log, str[4], strlen((char *) str[4]), &bw);

	// 7 - ������ ������, ����� � ����        
	i = 0;
	while (1) {
	    res = f_read(&loader_rom, buf, BUFFER_SIZE, &num);
	    FLASH_write_buf(LOADER_ROM_ADDRESS + i, buf, num);
	    i += num;		// �������� ����� �� ���������� ����� ����
	    if (num < BUFFER_SIZE) {
		sprintf((char *) buf, str[3], i);
		f_write(&boot_log, buf, strlen((char *) buf), &bw);
		break;
	    }
	    LED_toggle(LED_GREEN);
	}
	f_write(&boot_log, str[5], strlen((char *) str[5]), &bw);



	// 8 ��� ��������-��������� ���� � ������� ��
	f_write(&boot_log, str[6], strlen((char *) str[6]), &bw);
	f_close(&boot_log);
	f_unlink(LOADER_RAM_NAME);
	f_unlink(LOADER_ROM_NAME);
	f_mount(0, NULL);
    } while (0);

    // ������ �������� ���������
    bfrom_MemBoot((void *) 0x20010000, 0, 0, NULL);
    return 0;
}
