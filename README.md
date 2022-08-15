# Description 

Easy to use/port Modbus RTU slave library, written microcontrollers in mind. 

# Implemented Functions 

* Function 03: Read Holding Registers
* Function 06: Write Single Register

Planned: 

* Function 16: Write Multiple Registers

# Usage 

1. Include the header and source file paths within your toolchain (Makefile or your IDE):

```Makefile
C_SOURCES =  \
    ... \
    modbus_lib/modbus_lib.c \
    modbus_lib/modbus_crc.c

C_INCLUDES =  \
    ... \
    -Imodbus_lib
```

2. Include the header file within your `main.c`: 

```c
#include "modbus_lib.h"
```

3. Implement the following functions in your application: 

* Read handler:

```c
uint16_t modbus_lib_read_handler(uint16_t la){ // la: logical_address
    switch(la){
        case 40001:
            return something; 
        case 40002:
            return something_else;
        case 40003: 
            if (some_error){
                return modbus_lib_send_error(MBUS_RESPONSE_SERVICE_DEVICE_FAILURE);
            } else {
                return some_other_value;
            }
        default:
            return modbus_lib_send_error(MBUS_RESPONSE_ILLEGAL_DATA_ADDRESS); 
    }
}

```

* Write handler: 

```c
uint16_t modbus_lib_write_handler(uint16_t la, uint16_t value){
    if ( la >= 40001 && la <= 40013 ){
        my_buffer_reg_4xxxx[la-40001] = value;
    }
    if( la > 40013 && la < 40018){
        if(some_error_during_writing_to_eeprom){
            return 	MBUS_RESPONSE_SERVICE_DEVICE_FAILURE;
        }
    }
    if (la == 40018 ){
        output = (value >> 8) | (value << 8);
    }
    
    if (la > 40018){
	    return MBUS_RESPONSE_ILLEGAL_DATA_ADDRESS;
    }
    return MBUS_RESPONSE_OK; // data is successfully written
}
```


* Place the data receive function inside your incoming stream handler:

```c
void USART2_dataHandler(void)
{
  uint8_t buff[1]; 
  HAL_UART_Receive (&huart2, buff, 1, 400);  
  modbus_lib_append_data(buff[0]); // append byte-by-byte
}
```

* Mark the end of incoming telegram after a timeout: 

```c
void USART2_idleHandler(void)
{
    modbus_lib_end_of_telegram();
}
```

* Implement the `modbus_lib_transport_write` function: 

```c
int modbus_lib_transport_write(uint8_t* buffer, uint16_t length){
    HAL_UART_Transmit(&huart2, buffer, length, 1000);
    return 0; 
}
```

4. Make Modbus slave configuration and initialize in your `main()` function: 

```c
ModbusConfig_t modbus_cfg = {
    .address = 1
}; 

modbus_lib_init(&modbus_cfg);
```

5. Debug: 

* Verify that `uint8_t g_modbus_lib_received_telegram[]` is filled with received telegram. 
* Verify that `uint8_t g_modbus_lib_received_length` has correct value. 
* Examine `uint8_t outgoing_telegram[]` contents for outgoing telegram (use `uint16_t oindex` for length) inside `modbus_lib.c`.

# Example 

STM32F407 Discovery board example is available [here](https://github.com/ceremcem/modbus_example). 

# Address Indexes

First Modbus register address is 40001.