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

#define FLAG_SCAN_ENABLE 0x01
#define FLAG_SCAN_DISABLE 0x00
#define FLAG_FILTER_DISABLE 0x00

struct hci_request ble_hci_request(uint16_t ocf, int clen, void* status, void* cparam) {
    struct hci_request req = {0};

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
        perror("Failed to open HCI device");
        exit(EXIT_FAILURE);
    }

    return device;
}

// Sets BLE scan parameters
void ble_set_scan_params(int device, int* status) {
    le_set_scan_parameters_cp scan_params = {0}; 
    struct hci_request scan_params_req;

    // configure parameters
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

void ble_set_events_report_mask(int device, int* status) {
    le_set_event_mask_cp event_mask = {0};
    struct hci_request event_mask_req;
    int i;
    
    for (i = 0; i < 8; i++) event_mask.mask[i] = 0xFF;

    event_mask_req = ble_hci_request(OCF_LE_SET_EVENT_MASK, 
                                   LE_SET_EVENT_MASK_CP_SIZE, 
                                   status, 
                                   &event_mask);

    if (hci_send_req(device, &event_mask_req, 1000) < 0) {
        hci_close_dev(device);
        perror("Failed to set event mask");
        exit(EXIT_FAILURE);
    }
}

// enables BLE scanning
void ble_enable_scanning(int device, int* status) {
    le_set_scan_enable_cp scan_cp = {0};
    struct hci_request enable_req;

    scan_cp.enable = FLAG_SCAN_ENABLE;
    scan_cp.filter_dup = FLAG_FILTER_DISABLE;

    enable_req = ble_hci_request(OCF_LE_SET_SCAN_ENABLE, 
                                 LE_SET_SCAN_ENABLE_CP_SIZE, 
                                 status, 
                                 &scan_cp);

    if (hci_send_req(device, &enable_req, 1000) < 0) {
        hci_close_dev(device);
        perror("Failed to enable scanning");
        exit(EXIT_FAILURE);       
    }
}

void hci_set_socket_options(int device) {
    struct hci_filter nf;

    hci_filter_clear(&nf);
    hci_filter_set_ptype(HCI_EVENT_PKT, &nf);
    hci_filter_set_event(EVT_LE_META_EVENT, &nf);

    if (setsockopt(device, SOL_HCI, HCI_FILTER, &nf, sizeof(nf)) < 0) {
        hci_close_dev(device);
        perror("Failed to set socket options");
        exit(EXIT_FAILURE);
    }
}

void ble_scan(int device) {
    uint8_t buf[HCI_MAX_EVENT_SIZE];
    evt_le_meta_event* meta_event;
    le_advertising_info* info;
    int len;

    uint8_t reports_count;
    void* offset;

    char addr[18];

    while ( 1 ) {
        len = read(device, buf, sizeof(buf));
        
        if ( len >= HCI_EVENT_HDR_SIZE ) {
            meta_event = (evt_le_meta_event*)(buf+HCI_EVENT_HDR_SIZE+1);

            if ( meta_event->subevent == EVT_LE_ADVERTISING_REPORT ) {
                reports_count = meta_event->data[0];
                offset = meta_event->data + 1;
                
                while ( reports_count-- ) {
                    info = (le_advertising_info *)offset;
                    ba2str(&(info->bdaddr), addr);
                    printf("%s - RSSI %d\n", addr, (char)info->data[info->length]);
                    offset = info->data + info->length + 2;
                }
            }
        }
    }
}

void ble_disable_scanning(int device, int* status) {
    le_set_scan_enable_cp scan_cp = {0};
    struct hci_request disable_req;

    scan_cp.enable = FLAG_SCAN_DISABLE;

    disable_req = ble_hci_request(OCF_LE_SET_SCAN_ENABLE,
                                  LE_SET_SCAN_ENABLE_CP_SIZE,
                                  &status,
                                  &scan_cp);
   
    if (hci_send_req(device, &disable_req, 1000) < 0) {
        hci_close_dev(device);
        perror("Failed to enable scanning");
        exit(EXIT_FAILURE);       
    }    
}

void connect_lap(int s) {
    struct sockaddr_rc addrress = { 0 };

    char dest[18] = "B0:10:41:3F:6E:80";//My destination address Laptop

    addrress.rc_family = AF_BLUETOOTH;
    addrress.rc_channel = (uint8_t) 1;//must use sdp to work in real devices
    //may this channel not ready


    int status = connect(s, (struct sockaddr *)&addrress, sizeof(addrress));
    //successful, connect() returns 0.

    printf("connection status: %d\n\n",status);//0 show OK

    // send a message to server
    if( status == 0 ) {
        status = write(s, "hello!", 6);
        if (status == 6){
            printf("Send data to server done\n");
        }
    }
    else
    if( status < 0 ){
        perror("send message to server Failed\n");
    }

    printf("Closing socket\n");

    //close the socket
    close(s);

    //End connect and send

//
//    free( ii );
//    close( sock );

    return 0;
}

int main(int argc, char **argv) {
    int device, status;
    
    device = ble_open_device();

    ble_set_scan_params(device, &status);
    ble_set_events_report_mask(device, &status);
    ble_enable_scanning(device, &status);
    hci_set_socket_options(device);

    ble_scan(device);

    printf("Scanning...\n");

    connect_lap(device);
    //ble_scan(device);

    ble_disable_scanning(device, &status);

    hci_close_dev(device);
    exit(EXIT_SUCCESS);
}
