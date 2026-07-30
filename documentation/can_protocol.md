# OpenSTW CAN-bus protocol

## Overview

- Messages are exchanged between the host (**H**) and any of the area controllers (**C**)
- The system uses standard CAN frames
- The message ids are used as the CAN frame ids. Messages directed to the controllers start with `0x100`, and messages directed to the host start with `0x000` -allowing easy filtering and inate prioritization of event messages originating from controllers.

## General messages

### Messages overview

| Id      | Direction | Name                   | Description                                                    |
|:------- |:--------- |:---------------------- |:-------------------------------------------------------------- |
| `0x110` | H -> C    | MSG_GEN_HEARTBEAT_RQ   | Requests a heartbeat response from a specific area controller. |
| `0x011` | C -> H    | MSG_GEN_HEARTBEAT_RESP | Heartbeat response from a specific controller.                 |
| `0x012` | C -> H    | MSG_GEN_C_ACK          | Controller signals acknowledgement of a previous message.      |

### Message payload structure

#### [`0x110`] Heartbeat request (`MSG_GEN_HEARTBEAT_RQ`)

**Length:** 1 byte

**Structure:**

| Byte | Bits    | Function           | Valid Values |
|:---- |:------- |:------------------ |:------------ |
| `0`  | `[7:0]` | Controller Address | `uint8_t`    |

#### [`0x011`] Heartbeat response (`MSG_GEN_HEARTBEAT_RESP`)

**Length:** 1 byte

**Structure:**

| Byte | Bits    | Function           | Valid Values |
| ---- | ------- | ------------------ | ------------ |
| `0`  | `[7:0]` | Controller Address | `uint8_t`    |

#### [`0x012`] Message acknowledged by controller (`MSG_GEN_C_ACK`)

**Length:** 2 bytes

**Structure:**

| Byte | Bits    | Function           | Valid Values |
| ---- | ------- | ------------------ | ------------ |
| `0`  | `[7:0]` | Controller Address | `uint8_t`    |
| `1`  | `[7:0]` | Message cookie     | `uint8_t`    |

## Input and Output

### Messages overview

| Id      | Direction | Name                | Description                                    |
| ------- | --------- | ------------------- | ---------------------------------------------- |
| `0x120` | H -> C    | MSG_IO_CONF_LEDS    | Configures LED layout of a specific controller |
| `0x121` | H -> C    | MSG_IO_SET_LEDS_CH0 | Set state of up to 3 LEDs on channel 0         |
| `0x122` | H -> C    | MSG_IO_SET_LEDS_CH1 | Set state of up to 3 LEDs on channel 1         |
| `0x023` | C -> H    | MSG_IO_BTT_STATE    | Reports the state of a button                  |

### Message payload structure

#### [`0x120`] Configure LEDs (`MSG_IO_CONF_LEDS`)

**Length:** 4 bytes

**Structure:**

| Byte | Bits    | Function                   | Valid Values |
| ---- | ------- | -------------------------- | ------------ |
| `0`  | `[7:0]` | Controller Address         | `uint8_t`    |
| `1`  | `[7:0]` | Message cookie             | `uint8_t`    |
| `2`  | `[7:0]` | Number of LEDs (Channel 0) | `uint8_t`    |
| `3`  | `[7:0]` | Number of LEDs (Channel 1) | `uint8_t`    |

#### [`0x121`] Set state of LEDs on channel 0 (`MSG_IO_SET_LEDS_CH0`)

Sets the state of up to three LEDs connected to channel 0 of a specific area controller. Passing an LED index value of `0xFF` signals that no LED state should be changed for that part of the payload.

**Length:** 8 bytes

**Structure:**

| Byte | Bits    | Function           | Valid Values                                                                 |
| ---- | ------- | ------------------ | ---------------------------------------------------------------------------- |
| `0`  | `[7:0]` | Controller Address | `uint8_t`                                                                    |
| `1`  | `[7:0]` | Message Cookie     | `uint8_t`                                                                    |
| `2`  | `[7:0]` | LED 1 Index        | `[0, 254]`, `0xFF` means "None"                                              |
| `3`  |         | LED 1 State        |                                                                              |
|      | `[1:0]` | Mode               | `0b00`: Off, `0b01`: On, `0b10`: Blink                                       |
|      | `[4:2]` | Color              | `0b000`: White, `0b001`: Red, `0b010`: Green, `0b011`: Yellow, `0b100`: Blue |
| `4`  | `[7:0]` | LED 2 Index        | `[0, 254]`, `0xFF` means "None"                                              |
| `5`  |         | LED 2 State        | _see above_                                                                  |
| `6`  | `[7:0]` | LED 3 Index        | `[0, 254]`, `0xFF` means "None"                                              |
| `7`  |         | LED 3 State        | _see above_                                                                  |

#### [`0x122`] Set state of LEDs on channel 1 (`MSG_IO_SET_LEDS_CH1`)

**Length:** 8 bytes

**Structure:**

| Byte | Bits    | Function           | Valid Values                                                                 |
| ---- | ------- | ------------------ | ---------------------------------------------------------------------------- |
| `0`  | `[7:0]` | Controller Address | `uint8_t`                                                                    |
| `1`  | `[7:0]` | Message Cookie     | `uint8_t`                                                                    |
| `2`  | `[7:0]` | LED 1 Index        | `[0, 254]`, `0xFF` means "None"                                              |
| `3`  |         | LED 1 State        |                                                                              |
|      | `[1:0]` | Mode               | `0b00`: Off, `0b01`: On, `0b10`: Blink                                       |
|      | `[4:2]` | Color              | `0b000`: White, `0b001`: Red, `0b010`: Green, `0b011`: Yellow, `0b100`: Blue |
| `4`  | `[7:0]` | LED 2 Index        | `[0, 254]`, `0xFF` means "None"                                              |
| `5`  |         | LED 2 State        | _see above_                                                                  |
| `6`  | `[7:0]` | LED 3 Index        | `[0, 254]`, `0xFF` means "None"                                              |
| `7`  |         | LED 3 State        | _see above_                                                                  |

#### [`0x023`] Report button state (`MSG_IO_BTT_STATE`)

**Length:** 3 bytes

**Structure:**

| Byte | Bits    | Function           | Valid Values                    |
| ---- | ------- | ------------------ | ------------------------------- |
| `0`  | `[7:0]` | Controller Address | `uint8_t`                       |
| `1`  | `[7:0]` | Button index       | `uint8_t`                       |
| `2`  | `[7:0]` | Button state       | `0x0`: Released, `0x1`: Pressed |
