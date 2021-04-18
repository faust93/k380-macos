/*
 * based on
 * k480_conf.c by Emir Bucalovic <betoneful@gmail.com> www.betoneful.com
 *          https://betoneful.com/tech/logitech-k480-on-ubuntu-and-fn-buttons-default-behaviour/
 * k810_conf.c by Mario Scholz <mario@expires-2013.mail.trial-n-error.net>
 *          http://www.trial-n-error.de/posts/2012/12/31/logitech-k810-keyboard-configurator/
 * pairing_tool.c from Benjamin Tissoires <benjamin.tissoires@gmail.com>
 *          see also https://lkml.org/lkml/2011/9/22/367
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "hidapi.h"

#define HID_VENDOR_ID_LOGITECH          0x46d
#define HID_DEVICE_ID_K380              0xb342

const unsigned char k380_seq_fkeys_on[]  = {0x10, 0xff, 0x0b, 0x1e, 0x00, 0x00, 0x00};
const unsigned char k380_seq_fkeys_off[] = {0x10, 0xff, 0x0b, 0x1e, 0x01, 0x00, 0x00};

const char opt_on[]  = "on";
const char opt_off[] = "off";


int main(int argc, char **argv)
{
	int flag_fkeys = 1;
	int c,res;
	hid_device *handle;
        struct hid_device_info *devs, *cur_dev;

	if (argc < 2)
	{
		printf("Logitech K380 Keyboard Configurator (by faust93)\n\n");
		printf("Usage: %s -f {on|off}:\n\n", argv[0]);
		printf("-f <on|off>\n"
		       "   To enable direct access to F-keys.\n");
		printf("-l\n"
		       "   To list enabled HID devices.\n");
		printf("\n");
		exit(0);
	}

	while ((c = getopt (argc, argv, "lf:")) != -1)
	{
		switch (c)
		{
                        case 'l':
                                printf("list\n");
                                devs = hid_enumerate(0x46d, 0x0);
                                cur_dev = devs;
                                while (cur_dev) {
                                    printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls", cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
                                    printf("\n");
                                    printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
                                    printf("  Product:      %ls\n", cur_dev->product_string);
                                    printf("  Release:      %hx\n", cur_dev->release_number);
                                    printf("  Interface:    %d\n",  cur_dev->interface_number);
                                    printf("  Usage (page): 0x%hx (0x%hx)\n", cur_dev->usage, cur_dev->usage_page);
                                    printf("\n");
                                    cur_dev = cur_dev->next;
                                }
                                hid_free_enumeration(devs);
                                exit(0);
			case 'f':
				if (strcmp(opt_on, optarg) == 0)
				{
					flag_fkeys = 1;
				}
				else if (strcmp(opt_off, optarg) == 0)
				{
					flag_fkeys = 0;
				}
				else
				{
					fprintf (stderr, "Option -%c requires argument '%s' or '%s'.\n", optopt, opt_on, opt_off);
					return 1;
				}
				break;
			case '?':
				if (optopt == 'f')
				{
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				}
				else if (isprint (optopt))
				{
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				}
				else
				{
					fprintf (stderr,
							"Unknown option character `\\x%x'.\n",
							optopt);
				}
				return 1;
			default:
				abort ();
		}
	}

	if (hid_init()) {
		perror("Unable to init HID subsystem");
		return -1;
	}

	handle = hid_open(HID_VENDOR_ID_LOGITECH, HID_DEVICE_ID_K380, NULL);
	if (!handle) {
		printf("unable to open device\n");
 		return 1;
	}

	if (flag_fkeys)
	{
		res = hid_write(handle, k380_seq_fkeys_on, 7);
                if (res != 7)
                    fprintf(stderr, "Unable to switch F-n keys\n");
                else
                    printf("F-n keys: ON\n");
	}
	else
	{
		res = hid_write(handle, k380_seq_fkeys_off, 7);
                if (res != 7)
                    fprintf(stderr, "Unable to switch F-n keys\n");
                else
                    printf("F-n keys: OFF\n");
	}

	hid_close(handle);
	hid_exit();

	return 0;
}
