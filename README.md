# USB Reset Script

Resets a USB device by VendorID and ProductID

## Installation

    mkdir build
    cd build
    cmake ..
    make
    make install

After installation, the setuid bit should be set

    sudo chown root:root /usr/local/bin/usb-reset
    sudo chmod u+s /usr/local/bin/usb-reset

## Usage

    usb-reset <vendorId> <productId>

Example

    usb-reset 0xDEAD 0xBEEF

VendorId and ProductId can be determined via `lsusb`.
