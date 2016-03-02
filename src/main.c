/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "platform.h"
#include "ff.h"
#include "xil_io.h"
#include "xparameters.h"
#include "xtime_l.h"

void handle_fresult(FRESULT result) {
	switch (result) {
		case FR_OK:
			print("FRESULT = FR_OK\n\r");
			break;
		case FR_DISK_ERR:
			print("FRESULT = FR_DISK_ERR\n\r");
			break;
		case FR_INT_ERR:
			print("FRESULT = FR_INT_ERR\n\r");
			break;
		case FR_NOT_READY:
			print("FRESULT = FR_NOT_READY\n\r");
			break;
		case FR_NO_FILE:
			print("FRESULT = FR_NO_FILE\n\r");
			break;
		case FR_NO_PATH:
			print("FRESULT = FR_NO_PATH\n\r");
			break;
		case FR_INVALID_NAME:
			print("FRESULT = FR_INVALID_NAME\n\r");
			break;
		case FR_DENIED:
			print("FRESULT = FR_DENIED\n\r");
			break;
		case FR_EXIST:
			print("FRESULT = FR_EXIST\n\r");
			break;
		case FR_INVALID_OBJECT:
			print("FRESULT = FR_INVALID_OBJECT\n\r");
			break;
		case FR_WRITE_PROTECTED:
			print("FRESULT = FR_WRITE_PROTECTED\n\r");
			break;
		case FR_INVALID_DRIVE:
			print("FRESULT = FR_INVALID_DRIVE\n\r");
			break;
		case FR_NOT_ENABLED:
			print("FRESULT = FR_NOT_ENABLED\n\r");
			break;
		case FR_NO_FILESYSTEM:
			print("FRESULT = FR_NO_FILESYSTEM\n\r");
			break;
		case FR_MKFS_ABORTED:
			print("FRESULT = FR_MKFS_ABORTED\n\r");
			break;
		case FR_TIMEOUT:
			print("FRESULT = FR_TIMEOUT\n\r");
			break;
		case FR_LOCKED:
			print("FRESULT = FR_LOCKED\n\r");
			break;
		case FR_NOT_ENOUGH_CORE:
			print("FRESULT = FR_NOT_ENOUGH_CORE\n\r");
			break;
		case FR_TOO_MANY_OPEN_FILES:
			print("FRESULT = FR_TOO_MANY_OPEN_FILES\n\r");
			break;
		case FR_INVALID_PARAMETER:
			print("FRESULT = FR_INVALID_PARAMETER\n\r");
			break;
		default:
			print("Returned FRESULT value is unknown\n\r");
	}
}
int main()
{
	print("About to init_platform\n\r");
    init_platform();
    //print("Hello World\n\r");

    print("Mounting file system...\n\r");
    FATFS fs; // File system object
    FRESULT status; // FatFs return code

    status = f_mount(&fs, "", 0);
    if (status) {
    	handle_fresult(status);
    }
    else {
    	if (_FS_READONLY) {
    		print("Warning: File system is in read-only mode");
    	}

		FIL file; // File object
		print("Opening file to record SRAM values...\n\r");
		status = f_open(&file, "hello.txt", FA_WRITE);
		if (status) {
			handle_fresult(status);
		}
		else {
			print("Successfully opened file!\n\r");
			print("Right now it contains:\n\r");
			char line[82];
			while (f_gets(line, sizeof(line), &file)) {
				print(line);
			}

			//char line[82]; // Line buffer
			UINT btw;
			UINT *bw;
			int addr = XPAR_PS7_RAM_0_S_AXI_BASEADDR;
			int result;
			char header[100];
			strcpy(header, "Address Value");
			status = f_write(&file, header, sizeof(header), bw);
			if (status) {
				handle_fresult(status);
			}
			/*while (addr < XPAR_PS7_RAM_0_S_AXI_HIGHADDR / 1000) {
				result = Xil_In32(addr);
				char line[71];
				snprintf(line, sizeof(line), "Address: %x; Value: %x\n", addr, result);
				f_write(&file, line, sizeof(line), bw);
				if (*bw < sizeof(line)) {
					print("SD card is full!\n\r");
					break;
				}
				addr += 4;
				//sleep(1);
			}*/
			f_close(&file);
		}
		f_mount(NULL, "", 0);
    }

	print("All done, cleaning up now...\n\r");
	cleanup_platform();
	print("Exiting...\n\r\n\r");
    return 0;
}
