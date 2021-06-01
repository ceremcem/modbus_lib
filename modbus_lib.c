/*******************************************************
 * 
 * Implementation reference: 
 * 
 *      http://www.mayor.de/lian98/doc.en/html/u_mbusser-rtu_struct.htm
 * 
 *******************************************************/

#include "modbus_lib_types.h"
#include "modbus_lib.h"

uint8_t g_modbus_lib_received_telegram[MODBUS_LIB_MAX_BUFFER]; 
uint16_t g_modbus_lib_received_length = 0; 

int modbus_lib_init(ModbusConfig_t* cfg){
    return 0; 
}

void modbus_lib_append_data(uint8_t byte){
    g_modbus_lib_received_telegram[g_modbus_lib_received_length++] = byte;
}

void modbus_lib_end_of_telegram(){
    g_modbus_lib_received_length = 0; // debugger: p/x *g_modbus_lib_received_telegram@g_modbus_lib_received_length
}