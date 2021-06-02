/*******************************************************
 * 
 * Implementation reference: 
 * 
 *      http://www.mayor.de/lian98/doc.en/html/u_mbusser-rtu_struct.htm
 * 
 *******************************************************/

#include "modbus_lib_types.h"
#include "modbus_lib.h"
#include "port.h"
#include "modbus_crc.h"

uint8_t g_modbus_lib_received_telegram[MODBUS_LIB_MAX_BUFFER]; 
uint16_t g_modbus_lib_received_length = 0; 
ModbusConfig_t* config; 

int modbus_lib_init(ModbusConfig_t* cfg){
    config = cfg; 
    return 0; 
}

void modbus_lib_append_data(uint8_t byte){
    g_modbus_lib_received_telegram[g_modbus_lib_received_length++] = byte;
}

void modbus_lib_end_of_telegram(){
    if (crc_ok()){
        0; // debugger: printf "crc okay\n"
    } else {
        0; // debugger: printf "wrong crc!\n"
    }
    g_modbus_lib_received_length = 0; // debugger: p/x *g_modbus_lib_received_telegram@g_modbus_lib_received_length
}

inline int crc_ok(){
    uint16_t expected = usMBCRC16(g_modbus_lib_received_telegram, g_modbus_lib_received_length-2);
    uint16_t got = (g_modbus_lib_received_telegram[g_modbus_lib_received_length-1] << 8) \
        | g_modbus_lib_received_telegram[g_modbus_lib_received_length-1];

    if (got == expected){
        return 1; 
    } else {
        return 0;
    }
}