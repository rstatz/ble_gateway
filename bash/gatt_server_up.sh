#!/bin/bash

INTERFACE=hci0
DEV_NAME="GW_JAKE"

BLUEZ_PATH=/root/bluez

btmgmt -i $INTERFACE power off
btmgmt -i $INTERFACE le on
btmgmt -i $INTERFACE connectable on
btmgmt -i $INTERFACE name $DEV_NAME
btmgmt -i $INTERFACE advertising on
btmgmt -i $INTERFACE power on

$BLUEZ_PATH/tools/btgatt-server -i $INTERFACE -s low -t random -r -v
