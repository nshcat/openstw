import can

interface = 'socketcan'
channel = 'can0'

with can.Bus(channel=channel, interface=interface) as bus:
    for message in bus:
        print(message)
