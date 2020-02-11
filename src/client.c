#include <stdio.h>
// POSIX sys lib: fork, pipe, I/O (read, write)
#include <unistd.h>
// superset of unistd, same
#include <stdlib.h>

//Bluetooth
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/sco.h>

//socket
#include <sys/socket.h>


int main(int argc, char **argv)
{
    int flag=0;

    struct sockaddr_rc addrress = { 0 };
    int s, status;

    char dest[18]="";// = "B0:10:41:3F:6E:80";//My destination address Laptop
    char namelaptop[20]="ss";


    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    ///create a socket

    // set the connection parameters (who to connect to)
    addrress.rc_family = AF_BLUETOOTH;
    addrress.rc_channel = (uint8_t) 1;//must use sdp to work in real devices
    //may this channel not ready


    printf("Search for BT Devices...\n");

///search   

    inquiry_info *ii = NULL;
    int max_rsp, num_rsp;
    int dev_id, sock, len, flags;
    int i;

    char addr[18] = { 0 };
    char name[248] = { 0 };

    dev_id = hci_get_route(NULL);
    sock = hci_open_dev( dev_id );
    if (dev_id < 0 || sock < 0) {
        perror("opening socket");
        exit(1);
    }

    len = 8;
    max_rsp = 2;
    flags = IREQ_CACHE_FLUSH;
    ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));

    num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
    if( num_rsp < 0 ) {
        perror("hci_inquiry");
    }

    for (i = 0; i < num_rsp; i++) {
        ba2str(&(ii+i)->bdaddr, addr);
        memset(name, 0, sizeof(name));
        if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name),
                                 name, 0) < 0)
            strcpy(name, "[unknown]");

        else{
            printf("\nFind #%d\n",i);

            printf("Addr:%s    Name:%s\n", addr, name);

            int a=strcmp(name, namelaptop);
            //printf("compare:%d\n",a);

            if (!a){   // if name mached
                str2ba( addr, &addrress.rc_bdaddr );//copy dest-->addr.rc_bdaddr
                flag =1;
            }
        }

    }


/// End Search 



///Connect and send

    if (flag==0){
        printf("Not find dest: %s\n",name);
        exit(0);
    }

    // connect to server, throw socket s
    status = connect(s, (struct sockaddr *)&addrress, sizeof(addrress));
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

    ///close the socket
    close(s);

///End connect and send


    free( ii );
    close( sock );

    return 0;
}