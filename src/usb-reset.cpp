#include <cstdlib>
#include <sstream>
#include <iostream>
#include <libusb-1.0/libusb.h>
#include <fcntl.h>
#include <linux/usbdevice_fs.h>
#include <unistd.h>
#include <cstdio>
#include <sys/ioctl.h>

int reset(uint16_t vendorId, uint16_t productId) {
    libusb_context *context = NULL;
    int rc = 0;

    rc = libusb_init(&context);
    if(rc != 0) {
        return rc;
    }

    libusb_device_handle* dev = libusb_open_device_with_vid_pid(context, vendorId, productId);
    if(dev == NULL) {
        std::cout << "Device not found!" << std::endl;
    } else {
        rc = libusb_reset_device(dev);
        if(rc == 0) {
            std::cout << "Device has been reset!" << std::endl;
        } else {
            std::cout << "Error resetting device: " << rc << std::endl;
        }
        libusb_close(dev);
    }

    return rc;
}

int resetByFilename(const char* filename) {
    int fd = open(filename, O_WRONLY);
    if (fd < 0) {
        perror("Error opening output file");
        return false;
    }

    int rc = ioctl(fd, USBDEVFS_CONNECTINFO, 0);
    if (rc != 0) {
        perror("Connectinfo failed");
        return false;
    }

    rc = ioctl(fd, USBDEVFS_RESET, 0);
    if (rc < 0) {
        perror("Error in ioctl");
        return false;
    }

    std::cout << "Reset successful" << std::endl;

    close(fd);

    return true;
}

int main(int argc, char** argv) {
    if(argc != 3 && argc != 2) {
        std::cerr << "Usage: " << argv[0] << " VID PID" << std::endl;
        std::cerr << "  Example: " << argv[0] << " 0xDEAD 0xBEEF" << std::endl;
        std::cerr << std::endl;
        std::cerr << "Alternative usage: " << argv[0] << " FILENAME" << std::endl;
        return EXIT_FAILURE;
    }

    if(argc == 3) {
        uint16_t vendorId  = 0;
        uint16_t productId = 0;

        // Parse inputs
        {
            std::string vid(argv[1]);
            std::istringstream vidStream(vid);
            vidStream >> std::hex >> vendorId;

            std::string pid(argv[2]);
            std::istringstream pidStream(pid);
            pidStream >> std::hex >> productId;
        }

        if( !vendorId || !productId ) {
            std::cerr << "Invalid vendor or product id!" << std::endl;
            return EXIT_FAILURE;
        }

        std::cout << std::hex << std::showbase << vendorId << " " << productId << std::endl;

        int ret = reset(vendorId, productId);
        if(ret == 0) {
            return EXIT_SUCCESS;
        } else {
            return ret;
        }
    } else if(argc == 2) {
        return resetByFilename(argv[1]) ? EXIT_SUCCESS : EXIT_FAILURE;
    }
}
