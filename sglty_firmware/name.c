// found  at https://medium.com/@j0hnm4r5/changing-teensy-serial-name-and-port-name-2ca76552d26d
#include <usb_names.h>

#define MANUFACTURER_NAME          \
	{                          \
		'S', 'C', 'I', 'G', 'N', 'A', 'L', 'S' \
	}
#define MANUFACTURER_NAME_LEN 8

#define PRODUCT_NAME                                                            \
	{                                                                       \
		'S', 'I', 'G', 'N', 'A', 'L', 'I', 'N', 'O' \
	}
#define PRODUCT_NAME_LEN 9

#define SERIAL_NUMBER                                            \
	{                                                        \
		'S', 'G', 'N', 'O', '_', 'T', '_', '0', '0', '1' \
	}
#define SERIAL_NUMBER_LEN 10

struct usb_string_descriptor_struct usb_string_manufacturer_name = {
	2 + MANUFACTURER_NAME_LEN * 2,
	3,
	MANUFACTURER_NAME};

struct usb_string_descriptor_struct usb_string_product_name = {
	2 + PRODUCT_NAME_LEN * 2,
	3,
	PRODUCT_NAME};

struct usb_string_descriptor_struct usb_string_serial_number = {
	2 + SERIAL_NUMBER_LEN * 2,
	3,
	SERIAL_NUMBER};