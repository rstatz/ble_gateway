#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#define SCAN_PARAM_TYPE     0x00
#define SCAN_PARAM_INTERVAL htobs(0x0010)
#define SCAN_PARAM_WINDOW   htobs(0x0010)
#define SCAN_PARAM_BDADDR   0x00
#define SCAN_PARAM_FILTER   0x00

struct hci_request ble_hci_request(uint16_t ocf, int clen, void* status, void* cparam) {
    struct hci_request req;
    memset(&req, 0, sizeof(req));

    req.ogf = OGF_LE_CTL;
    req.ocf = ocf;
    req.cparam = cparam;
    req.clen = clen;
    req.rparam = status;
    req.rlen = 1;

    return req;
}

// Exits if fails to open device
int ble_open_device() {
    int device = hci_open_dev(hci_get_route(NULL));
    
    if (device < 0) {
        perror("Could not open HCI device");
        exit(EXIT_FAILURE);
    }

    return device;
}

// Sets BLE scan parameters
void ble_set_scan_params(int device, int* status) {
    le_set_scan_parameters_cp scan_params; 
    struct hci_request scan_params_req;

    // configure parameters
    memset(&scan_params, 0, sizeof(scan_params));

    scan_params.type = SCAN_PARAM_TYPE;
    scan_params.interval = SCAN_PARAM_INTERVAL;
    scan_params.window = SCAN_PARAM_WINDOW;
    scan_params.own_bdaddr_type = SCAN_PARAM_BDADDR;
    scan_params.filter = SCAN_PARAM_FILTER;

    // set parameters
    scan_params_req = ble_hci_request(OCF_LE_SET_SCAN_PARAMETERS,
                                     LE_SET_SCAN_PARAMETERS_CP_SIZE,
                                     status,
                                     &scan_params);

    if (hci_send_req(device, &scan_params_req, 1000) < 0) {
        hci_close_dev(device);
        perror("Failed to set scan parameters");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv) {
    int device, status;
    
    device = ble_open_device();

    ble_set_scan_params(device, &status);

    hci_close_dev(device);
    exit(EXIT_SUCCESS);
}
