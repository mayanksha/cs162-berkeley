#!/bin/bash

echo "filename is ${1}"

cat "${1}" | tr " " "\n" | tr -s "\n" | tr "[:upper:]" "[:lower:]" | tr -d -C "[:lower:]\n" | sort | uniq -c | sort -n
