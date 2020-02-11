#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>



int main(int argc, char **argv) {
    int device;

    device = hci_open_dev(hci_get_route(NULL));

}
