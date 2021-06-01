# Description 

Easy to use/port Modbus RTU slave library, written microcontrollers in mind. 

# Usage 

1. Include the header file: 

```c
#include "modbus_lib.h"
```

Include the header and source file paths within your Makefile (or your IDE):

```Makefile
C_SOURCES =  \
    ... \
    modbus_lib/modbus_lib.c

C_INCLUDES =  \
    ... \
    modbus_lib
```

2. Implement the following functions in your application: 

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
                return modbus_lib_send_error(MBUS_RESPONSE_ILLEGAL_DATA_VALUE);
            } else {
                return some_other_value;
            }
    }

    // else, return error
    return modbus_lib_send_error(MBUS_RESPONSE_ILLEGAL_DATA_ADDRESS);
}

```

* Write handler: 

```c
uint16_t modbus_lib_write_handler(uint16_t la, uint16_t value){
{
    if ( la > 40000 && la <= 40013 ){
        my_buffer_reg_4xxxx[la-40001] = value;
    }
    if (la == 40018 ){
         output = (value >> 8) | (value << 8);
    }
        
    if (some_error){
        return modbus_lib_send_error(MBUS_RESPONSE_ILLEGAL_DATA_ADDRESS); 
    }
    return 0; // data is successfully written
}

```

* Place the data receive function inside your incoming stream:

```c
void USART2_DataHandler(void)
{
    char buff; 
    HAL_UART_Receive(&huart2, (uint8_t *)&buff, 1, 400);
    modbus_lib_append_data(buff); // append byte-by-byte
    ...
}
```

* Trigger the end of telegram after a timeout: 

```c
void USART2_IdleHandler(void)
{
    modbus_lib_end_of_telegram();
}
```

* Implement `write` function: 

```c
int modbus_lib_transport_write(uint8_t* buffer, uint16_t length){
    HAL_UART_Transmit(&huart2, buffer, length, 1000);
    return 0; 
}
```

3. Make Modbus slave configuration and initialize: 

```c
ModbusConfig_t modbus_cfg = {
    .address = 1
}; 

modbus_lib_init(&modbus_cfg);
```

4. Debug: 

* Verify that `uint8_t g_modbus_lib_received_telegram[]` is filled with received telegram. 
* Verify that `uint8_t g_modbus_lib_received_length` has correct value. 
* Examine `uint8_t g_modbus_lib_outgoing_telegram[]` contents (use `uint8_t g_modbus_lib_outgoing_telegram_length`).