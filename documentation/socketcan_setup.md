# Setting up USB2CAN adapter via `socketcan`

- `sudo slcand -f -o -c -s5 /dev/ttyACM0`

- `sudo ip link set can0 up type can bitrate 800000`
