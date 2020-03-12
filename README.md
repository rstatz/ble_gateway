### Bluetooth Wireless Gateway using Gateworks Ventana SBC

## Overview 
Project Dependencies:
    - lbluetooth 
    - glib-2.0
    
This project is built off the btgatt server/client example provided in the bluez library developer package(https://github.com/Vudentz/BlueZ/tree/master/tools). The intention is to demonstrate the capabilities of the Gateworks GW16126 Bluetooth 5 expansion card by creating a Bluetooth gateway linked to an Azure database.

There is a client program and a server program. The client program can send messages to the server and the server will post the messages to the Azure cloud-based MySQL database. 
Once the server is running, the client can connect and setup communication. A Bluetooth handle is created for the message service on the server side and the client sends the message to this handle.

## Server 
On boot, the server first connects to the azure database. After connecting the server queries for the 'realtime' boolean and 'postrate' values from the data base. Realtime tells if the server should post messages immediately upon receiving them or on a timer. If realtime is disabled the server will create a post timer, store any messages it has received in a buffer, and post the messages in the buffer when the timer expires. Postrate describes the time in seconds between posting events.

After connecting to the database the server listens for a connection from the client. After connecting, the server populates three handles:
1. The GAP service: (Generic Access Protocol) Defines how bluetooth devices communicate
2. The GATT service: (Generic Attribute Profile) Defines data transfer between the server and client
3. The message service: Our custom handle used for transfering text messages 

For the remainder of the server's life, the server listens for messages from the client and posts them the the database. 
    
options:
- 'i' Specify adapter index, typically hci0 
- 'm' The ATT MTU to use
- 's' security-level [low|medium|high]
- 't' The source address type [random|public]
- 'v' Enable extra logging
- 'r' Enable Messaging Service
- 'h' help

## Client 
Before running the client you must run a scan for the server 'hcitool -i <hciX> lescan | grep [SERVER NAME]'. Assuming the server is running, the scan will show the server's MAC address. The client program takes the server's MAC address and requests a connection to the server. 
After the client and server connect; the GATT and GAP service set up the communication protocol. From this point the user can send messages to the server using the 'msg' command. The msg will be sent to the msg handle on the server side. 

options: 
- 'd' remote address (MAC address of the server)
- 'i' Specify adapter index, typically hci0 
- 'm' The ATT MTU to use
- 's' security-level [low|medium|high]
- 't' The source address type [random|public]
- 'v' Enable extra logging
- 'h' help

## Getting Started
This project requires the use of two GW5220 SBCs, each with a GW16126 expansion card and internet connectivity.
Install:
```apt-get install git libbluetooth-dev pkg-config build-essential libglib2.0-dev libmysqlclient-dev```

After cloning, change directories into the project directory. First `make` to compile all of the source code. Use the scripts below to start the Bluetooth service, GATT server, and GATT client. 

In the `bash/` folder, there are three scripts: 
- `ble_startup` : this script attaches the Bluetooth chip via hci_uart to the hci0 interface on the gateworks board. This script must be run before the client or server program on both SBCs. 
- `gatt_server_up` : this script starts up the server. DEV_NAME allows you to name the server whatever you like. 
- `gatt_client_up` : this script starts up the client to attach to the server. This script takes in a single argument of the MAC address of the server. 

All of the above scipts must be run from the root directory of this project. For example, to start the Bluetooth service run:
```./bash/ble_startup```

To start the GATT server:
```./bash/gatt_server_up```

To start the GATT client:
```./bash/gatt_client_up <server_MAC>```

## Notes
The GATT server assumes it can connect to an Azure MySQL database (https://docs.microsoft.com/en-us/azure/mysql/quickstart-create-mysql-server-database-using-azure-portal). The server, user, password, and database name can be updated in `src/azure/mysqldb.h`. If any changes are made, use `make clean` and `make` to update GATT server and client executables. The database is assumed to have two tables, 'config' and 'messages'. 

The commands below can be used to create the necessary tables using the MySQL command prompt:

```CREATE TABLE config (postrate INT, realtime CHAR(1), ID INT, PRIMARY KEY (ID));```

```CREATE TABLE messages (msg VARCHAR(140), created_at DATETIME DEFAULT CURRENT_TIMESTAMP);```

Values for postrate, realtime, and ID must be initialized before running the GATT server.
