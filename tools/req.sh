#!/bin/bash
curl -H "X-Forwarded-For: $2" --data-binary @$1 'http://127.0.0.1/Service/Echo'
