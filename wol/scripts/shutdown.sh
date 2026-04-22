#!/bin/bash

MAX_RETRIES=10
WAIT_TIME=5

while true; do
    attempt=0

    while [ $attempt -lt $MAX_RETRIES ]; do
        result=$(ping -qc10 ${ESP_IP} 2>&1 | awk -F'/' 'END{ print (/^rtt/? "OK":"FAIL") }')

        if [ "$result" == "OK" ]; then
            echo "Ping successful."
            break
        else
            echo "Ping failed. Attempt $((attempt + 1)) of $MAX_RETRIES."
            attempt=$((attempt + 1))
            if [ $attempt -lt $MAX_RETRIES ]; then
                echo "Waiting $WAIT_TIME seconds before retry..."
                sleep $WAIT_TIME
            fi
        fi
    done

    if [ $attempt -eq $MAX_RETRIES ]; then
        echo "All attempts failed. Shutting down..."
        /root/webhook.sh
        shutdown now
    fi

    sleep 30
done