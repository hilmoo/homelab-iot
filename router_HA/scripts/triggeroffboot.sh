#!/bin/bash

source /root/f609-ssid/.env
/root/f609-ssid/f609-ssid true

TARGET_IP="192.168.1.2"
URL="http://${TARGET_IP}/off"

echo "Starting boot trigger script..."

if ! ping -c 3 -W 5 "$TARGET_IP" > /dev/null 2>&1; then
    echo "ERROR: Target IP $TARGET_IP is unreachable via ping. Exiting."
    exit 0
fi

echo "Target $TARGET_IP is alive. Sending curl request..."

HTTP_STATUS=$(curl -s -o /dev/null -w "%{http_code}" "$URL")

if [ "$HTTP_STATUS" -eq 200 ]; then
    echo "SUCCESS: Trigger sent successfully. Received HTTP 200."
    exit 0
else
    echo "ERROR: Expected HTTP 200, but received HTTP $HTTP_STATUS."
    exit 0
fi