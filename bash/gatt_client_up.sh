#!/bin/bash

INTERFACE=hci0

BLUEZ_PATH=

#btmgmt -i $INTERFACE power off
#btmgmt -i $INTERFACE le on
#btmgmt -i $INTERFACE connectable on
#btmgmt -i $INTERFACE name $DEV_NAME
#btmgmt -i $INTERFACE advertising on
#btmgmt -i $INTERFACE power on

$BLUEZ_PATH/tools/btgatt-client -i $INTERFACE -t random -s low -v -d $1
