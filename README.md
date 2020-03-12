### Bluetooth Wireless Gateway using Gateworks Ventana SBC

## Overview 
Project Dependencies:
    - lbluetooth 
    - glib-2.0
    
This project is built off the btgatt server/client example provided in the bluez library developer package. The intention is to demonstrate the capabilities of the Gateworks GW16126 Bluetooth 5 expansion card by creating a bluetooth gateway linked to an Azure database.

There is a client program and a server program. The client program can send messages to the server and the server will send the messages to the cloud. 
When the server is running the client can connect and setup communication. A bluetooth handle is created for the message service on the server side and the client sends the message to this handle.

## Server 
On boot, the server first connects to the azure database. After connecting the server queries for the realtime boolean and post rate from the data base. Realtime tells if the server should post messages immediately upon receiving them or on a timer. If realtime is disabled the server will set a post timer and post messages when it triggers. If realtime is enabled the server will store messages in a buffer and flush them out on a timer. 

After connecting to the database the server listens for a connection from the client. After connecting, the server populates three handles:
1. The GAP service: (Generic Access Protocol) Defines how bluetooth devices communicate
2. The GATT service: (Generic Attribute Profile) Defines data transfer between the server and client
3. The message service: Our custom handle used for transfering text messages 

For the remainder of the server's life, the server listens for messages from the client and posts them the the database. 
    
options
-----------
## Client 
## Azure 
## Getting Started 
    
to add:
- options explination
    - security
    - address type
    - how to run
