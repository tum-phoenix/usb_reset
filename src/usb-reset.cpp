#include <cstdlib>
#include <sstream>
#include <iostream>
#include <libusb-1.0/libusb.h>

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

int main(int argc, char** argv) {
    if(argc != 3) {
        std::cerr << "Usage: " << argv[0] << " VID PID" << std::endl;
        std::cerr << "  Example: " << argv[0] << " 0xDEAD 0xBEEF" << std::endl;
        return EXIT_FAILURE;
    }

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
}
