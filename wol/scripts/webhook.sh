#!/bin/bash

WEBHOOK_URL="https://discord.com/api/webhooks/"

PAYLOAD=$(cat <<EOF
{
  "content": "@everyone\nPVE shutdown",
  "embeds": null,
  "attachments": []
}
EOF
)

curl -X POST -H "Content-Type: application/json" -d "$PAYLOAD" "$WEBHOOK_URL"