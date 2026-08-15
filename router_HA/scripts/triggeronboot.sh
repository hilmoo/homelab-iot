#!/bin/bash

TARGET_IP="192.168.1.2"
URL="http://${TARGET_IP}/on"

echo "Starting boot trigger script..."

while ! ping -c 1 -W 2 "$TARGET_IP" > /dev/null 2>&1; do
    sleep 5
done

echo "Target $TARGET_IP is alive. Sending curl request..."

HTTP_STATUS=$(curl -s -o /dev/null -w "%{http_code}" "$URL")

if [ "$HTTP_STATUS" -eq 200 ]; then
    echo "SUCCESS: Trigger sent successfully. Received HTTP 200."
else
    echo "ERROR: Expected HTTP 200, but received HTTP $HTTP_STATUS."
    exit 2
fi

echo "Waiting 3 minutes before running f609-ssid false..."
sleep 180

source /root/f609-ssid/.env
/root/f609-ssid/f609-ssid false

exit 0