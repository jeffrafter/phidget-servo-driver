#include <stdio.h>
#include <strings.h>
#include "usb.h"

#define USB_VENDOR         0x6C2
#define USB_PRODUCT_SINGLE 0x39
#define USB_PRODUCT        0x3A
#define SERVO_MINPOS       -23
#define SERVO_MAXPOS       232
#define SERVO_VERSION      6

static int pdd_iid = 0;

// Set the position to -23 (SERVO_MINPOS) to disengage
static int setpos(struct usb_dev_handle *udev, int product, int index, double pos)
{
    if (product == USB_PRODUCT_SINGLE) {
        fprintf(stdout, "Initializing single servo controller\n");

        char buf[8];
        int ret, pulse = (int)(pos * 10.6 + 243.8);

        bzero(buf, sizeof buf);

        buf[0] = pulse & 0xFF;
        buf[1] = pulse >> 8;
        ret = usb_control_msg(udev, USB_ENDPOINT_OUT |     USB_TYPE_CLASS | USB_RECIP_INTERFACE,
                        USB_REQ_SET_CONFIGURATION, 0x0200, pdd_iid,
                        (char *) buf, 6, 500);

        return ret;

    } else if (product == USB_PRODUCT) {
        // PHIDID_ADVANCEDSERVO_8MOTOR 0x3A

        fprintf(stdout, "Initializing advanced servo controller\n");

        char buf[8];
        int ret = 0;
        int pwm = 0, velocity = 0, accel = 0;
        bzero(buf, sizeof buf);

        buf[0] = (index << 5);
        buf[1] = 0;

        pwm = (int)((pos * 10.6 + 243.8) * 12);
        int velocityMaxLimit = (50/12.0) * 16384; //68266.67 us/s
        int velocityMin = 0;
        velocity = (int)(((128/12.0*316) / velocityMaxLimit) * 16384);
        accel = (int)0x200;

        fprintf(stdout, "Preparing the position command\n\nindex: %d\nvelocity: %d\naccel: %d\npwm: %d\n", index, velocity, accel, pwm);

        buf[2] = ((pwm >> 8) & 0xff);
        buf[3] = (pwm & 0xff);
        buf[4] = ((velocity >> 8) & 0xff);
        buf[5] = (velocity & 0xff);
        buf[6] = ((accel >> 8) & 0xff);
        buf[7] = (accel & 0xff);

        fprintf(stdout, "Sending the position command\n");

        ret = usb_control_msg(udev, USB_ENDPOINT_OUT | USB_TYPE_CLASS | USB_RECIP_INTERFACE,
                        USB_REQ_SET_CONFIGURATION, 0x0200, pdd_iid,
                        (char *) buf, 8, 500);

        fprintf(stdout, "Sent the position command: %d\n", ret);

        if(ret < 0)
        {
          switch(ret)
          {
          // case -ETIMEDOUT: //important case?
          //     fprintf(stderr, "Timeout");
          //     break;
          // case -ENODEV:
          //     //device is gone - unplugged.
          //     fprintf(stderr, "Device was unplugged - detach.");
          //     break;
          default:
              fprintf(stderr, "Sending usb_control_msg failed with error code: %d \"%s\"", ret, strerror(-ret));
              break;
          }
        }


        return ret;
    }

  	return -1;
}

struct usb_bus *bus;
struct usb_device *dev;
usb_dev_handle *udev;

int main(int argc, char **argv)
{
        double pos;
        int index;
        int product;

        if (argc < 2) {
                fprintf(stderr, "Phidget servo controller version %d\n\nNeed arg: index (int), new_pos (float)\n",
                        SERVO_VERSION);
                exit(1);
        }
        index = atoi(argv[1]);
        if (index < 0 || index > 7) {
                fprintf(stderr, "Invalid range, servo min index: %d, max index: %d\n",
                        0, 7);
                exit(1);
        }
        pos = atol(argv[2]);
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
                                (dev->descriptor.idProduct != USB_PRODUCT && dev->descriptor.idProduct != USB_PRODUCT_SINGLE))
                                        continue;
                        product = dev->descriptor.idProduct;
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
        int ret = 0;
        if((ret = usb_claim_interface(udev, pdd_iid)) < 0)
        {
          fprintf(stderr, "usb_claim_interface failed with error code: %d \"%s\"", ret, strerror(-ret));
          if((ret = usb_close(udev)) < 0)
          {
             fprintf(stderr, "usb_close failed with error code: %d \"%s\"", ret, strerror(-ret));
          }
          exit(1);
        }

        ret = setpos(udev, product, index, pos);
        fprintf(stdout, "Setting position to %f.\n\n Return: %i\n", pos, ret);
        return 0;
}

