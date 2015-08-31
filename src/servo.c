#include <stdio.h>
#include <strings.h>
#include "usb.h"

#define USB_VENDOR      0x6C2
#define USB_PRODUCT     0x39
#define SERVO_MINPOS    -23
#define SERVO_MAXPOS    232

static int pdd_iid = 0;

static int setpos(struct usb_dev_handle *udev, double pos)
{
        char buf[8];
        int ret, pulse = (int)(pos * 10.6 + 243.8);

        bzero(buf, sizeof buf);

        buf[0] = pulse & 0xFF;
        buf[1] = pulse >> 8;
        ret = usb_control_msg(udev, USB_ENDPOINT_OUT |     USB_TYPE_CLASS | USB_RECIP_INTERFACE,
                        USB_REQ_SET_CONFIGURATION, 0x0200, pdd_iid,
                        (char *) buf, 6, 500);

        return ret;
}

struct usb_bus *bus;
struct usb_device *dev;
usb_dev_handle *udev;

int main(int argc, char **argv)
{
        double pos;

        if (argc < 2) {
                fprintf(stderr, "Need arg: new_pos (float)\n");
                exit(1);
        }
        pos = atol(argv[1]);
        if (pos < SERVO_MINPOS || pos > SERVO_MAXPOS) {
                fprintf(stderr, "Invalid range, servo min pos: %d, max pos: %d\n",
                        SERVO_MINPOS, SERVO_MAXPOS);
                exit(1);
        }
        usb_init();
        usb_find_busses();
        usb_find_devices();

        for (bus = usb_busses; bus; bus = bus->next) {
                for (dev = bus->devices; dev; dev = dev->next) {
                        if (dev->descriptor.idVendor != USB_VENDOR ||
                                dev->descriptor.idProduct != USB_PRODUCT)
                                        continue;
                        goto FOUND;
                }
        }

        fprintf(stderr, "Phidget device has not been found.\n");
        exit(1);
FOUND:
        udev = usb_open(dev);
        if (!udev) {
                fprintf(stderr, "Cannot open usb device.\n");
                exit(1);
        }
        setpos(udev, pos);
        return 0;
}
