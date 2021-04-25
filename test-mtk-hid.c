#include <stdio.h>
#include <stdint.h>
#include "hidapi.h"

#define REPORT_SIZE		64
#define REPORT_ID_SIZE		1
#define REPORT_CMD_SIZE		2
#define REPORT_LEN_SIZE		2
#define HID_OVERHEAD_SIZE	(REPORT_ID_SIZE+REPORT_CMD_SIZE+REPORT_LEN_SIZE)

#define USB_VID			0x0e8d
#define USB_PID			0x201c

#pragma pack(push, 1)
enum command_list_t
{
    /*Command list*/
    //General commands
    CMD_SYNC_UART_DATA              = 0x0001,

    //Audio commands    
    CMD_SET_MIC_MUTE                = 0x0101,
    CMD_SET_MIC_GAIN                = 0x0102,
    CMD_SET_SPK_MUTE                = 0x0103,
    CMD_SET_SPK_GAIN                = 0x0104,
    CMD_DISABLE_AUDIO_PROCESS       = 0x0105,
    CMD_SELECT_MIC_CHANNEL          = 0x0106,
    CMD_SELECT_SPK_CHANNEL          = 0x0107,
    CMD_SET_SPK_LEVEL               = 0x0108,
    CMD_SET_MIC_LEVEL               = 0x0109,
    CMD_SET_USB_MIC_ENABLED         = 0x010A,
    CMD_GET_SPK_LEVEL               = 0x010B,
    CMD_GET_MIC_LEVEL               = 0x010C,

    // Remote Events
    CMD_REMOTE_KEY_PRESSED          = 0x0201,

    // LED Events
    CMD_LED_CTRL                    = 0x0301,

    // Micpod Events
    CMD_MICPOD_BUTTON_PRESSED       = 0x0401,

    // Others.
    CMD_STM_FW_VER_REQ              = 0x0501,
    CMD_STM_FW_VER                  = 0x0502,
    CMD_ZYNQ_FW_VER_REQ             = 0x0503,
    CMD_ZYNQ_FW_VER                 = 0x0504,
    CMD_QLCOM_FW_VER_REQ            = 0x0505,
    CMD_QLCOM_FW_VER                = 0x0506,
    CMD_DEVICE_MODE_SET             = 0x0507,
    CMD_DEVICE_MODE_GET             = 0x0508,
    CMD_STM_USB_PLUG_EVT            = 0x0509,
    CMD_ZYNQ_FLASH_READ             = 0x050A,
    CMD_ZYNQ_FLASH_WRITE            = 0x050B,
    CMD_STM_SERIAL_NUM_READ         = 0x050C,
    CMD_STM_SERIAL_NUM_WRITE        = 0x050D,
    CMD_STM_TRACK_DATA_READ         = 0x050E,
    CMD_STM_TRACK_DATA_WRITE        = 0x050F,

    CMD_MAX_SUPPORTED               = 0xFFFF
};

struct hid_report_t
{
    uint8_t id;
    uint16_t command;
    uint16_t len;
    uint8_t data[REPORT_SIZE-HID_OVERHEAD_SIZE];
};
#pragma pack(pop)

hid_device *handle;

int read_hid_data(void)
{
	uint8_t buf[REPORT_SIZE]={0};
	int ret,i;
	struct hid_report_t* decode_msg;

	ret = hid_read_timeout(handle, buf, REPORT_SIZE, 5000);
	if(ret > 0)
	{
		decode_msg = (struct hid_report_t*)buf;
		printf("Command ID : %x \n", decode_msg->command);

		switch(decode_msg->command) {
			case CMD_REMOTE_KEY_PRESSED:
				printf("CMD_REMOTE_KEY_PRESSED\n");
				break;

			default:
				printf("Unknown data received!\n");
				break;
		}
	} else {
		printf("read_hid_data is failed\n");
	}
}

int main(int argc, char **argv)
{
	int count = 0;
	int i, res, desc_size = 0;
	uint8_t buf[REPORT_SIZE-HID_OVERHEAD_SIZE];

	// Initialize the hidapi library
	res = hid_init();
	if(res == 0) {
		printf("hid_init success \n");
	} else {
		printf("hid_init failed  \n");
		return -2;
	}

	// Open the device using the VID, PID,
	handle = hid_open(USB_VID, USB_PID, NULL);
	if(handle == NULL) {
		printf("Failed to open HID device with VID: 0x%x PID: 0x%x \n", USB_VID, USB_PID);
		hid_exit();
		return -1;
	} else {
		printf("HID device open successful \n");
	}

	res = read_hid_data();
	if(res < 0)
		printf("reading hid date is failed\n");

	hid_exit();
	return 0;
}

