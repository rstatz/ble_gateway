sed -i '/^ExecStart=/ s/$/ -E/' /lib/systemd/system/bluetooth.service 
# restart bluetoothd
systemctl daemon-reload
systemctl restart bluetooth
# attach HCI UART
dev=$(basename $(ls -d /sys/bus/usb/drivers/ftdi_sio/*/ttyUSB*))
modprobe hci_uart
btattach -B /dev/$dev -S 1000000 -P h4 &
