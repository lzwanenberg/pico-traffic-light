#!/bin/bash

source_file="./build/release/pico-traffic-light.uf2"
pico_mount="/media/luc/RPI-RP2"
waiting=false

while [ ! -d "$pico_mount" ]; do
    if [ "$waiting" = false ]; then
        echo "Pico not mounted. Waiting for device at $pico_mount..."
        waiting=true
    fi
    sleep 0.1
done

if [ "$waiting" = true ]; then
    # Copying immediately after plugging in will give result in a Permisision denied
    # Therefore wait half a second
    sleep 0.5
fi

echo "Pico found at $pico_mount"

cp "$source_file" "$pico_mount" && echo "$source_file copied to $pico_mount successfully!"
