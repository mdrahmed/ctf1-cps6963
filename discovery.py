#!/usr/bin/env python3

import sys
import time
from pymodbus.client.sync import ModbusTcpClient as ModbusClient
from pymodbus.exceptions import ConnectionException

ip = sys.argv[1]
client = ModbusClient(ip, port=502)
client.connect()

try:
    while True:
        # Reading Input Registers (ColorSensor and RangeSensor)
        rr_input = client.read_input_registers(0, 4)  # Address range: %IW0 to %IW3
        if not rr_input.isError():
            print("Input Registers:", rr_input.registers)
        else:
            print(f"Error reading Input Registers: {rr_input}")

        # Reading Coils (Pump, Doser_RED, Doser_BLUE, treatmentComplete)
        rr_coils = client.read_coils(0, 8)  # Address range: %QX0.0 to %QX0.7
        if not rr_coils.isError():
            print("Coils:", rr_coils.bits)
        else:
            print(f"Error reading Coils: {rr_coils}")

        # Reading Holding Registers (stage, measuredDistance)
        rr_holding = client.read_holding_registers(0, 2)  # Address range: %QW0, %QD1 (assuming QD1 maps to 2 consecutive holding registers)
        if not rr_holding.isError():
            print("Holding Registers:", rr_holding.registers)
        else:
            print(f"Error reading Holding Registers: {rr_holding}")

        time.sleep(1)

except KeyboardInterrupt:
    pass  # Exit the loop on Ctrl-C

client.close()
