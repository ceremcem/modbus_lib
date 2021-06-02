/*******************************************************
 * 
 * Implementation reference: 
 * 
 *      http://www.mayor.de/lian98/doc.en/html/u_mbusser-rtu_struct.htm
 *      https://www.modbustools.com/modbus.html
 * 
 * Example telegrams: 
 *      Read holding registers: 01039C410002BA4F (read 2 registers starting from 40001)
 *      Read holding registers: 01039C4100043A4D (read 4 registers starting from 40001)
 * 
 *******************************************************/

#include "modbus_lib_types.h"
#include "modbus_lib.h"
#include "port.h"
#include "modbus_crc.h"

uint8_t g_modbus_lib_received_telegram[MODBUS_LIB_MAX_BUFFER]; 
uint16_t g_modbus_lib_received_length = 0; 
uint8_t g_modbus_lib_exception_occurred = 0; 
ModbusConfig_t* config; 

int modbus_lib_init(ModbusConfig_t* cfg){
    config = cfg; 
    return 0; 
}

void modbus_lib_append_data(uint8_t byte){
    g_modbus_lib_received_telegram[g_modbus_lib_received_length++] = byte;
}

void modbus_lib_end_of_telegram(){
    // Check length 
    if (g_modbus_lib_received_length < MODBUS_LIB_MIN_TELEGRAM_SIZE){
        return modbus_lib_send_error(MBUS_RESPONSE_NONE);
    }

    // Check CRC
    CRC_t expected = usMBCRC16(g_modbus_lib_received_telegram, g_modbus_lib_received_length-2);
    UCHAR got_low = g_modbus_lib_received_telegram[g_modbus_lib_received_length-2];
    UCHAR got_high = g_modbus_lib_received_telegram[g_modbus_lib_received_length-1];
    if ((expected.bytes.low != got_low) || (expected.bytes.high != got_high)){
        return modbus_lib_send_error(MBUS_RESPONSE_NONE);
    } 

    // Check address 
    if (config->address != g_modbus_lib_received_telegram[0]){
        return modbus_lib_send_error(MBUS_RESPONSE_NONE);
    }

    // Telegram is okay, call the relevant handler 
    // -------------------------------------------
    uint8_t outgoing_telegram[MODBUS_LIB_MAX_BUFFER];
    uint16_t oindex = 0; 

    outgoing_telegram[oindex++] = config->address; 

    volatile MbDataField start_addr, count, res;

    switch (g_modbus_lib_received_telegram[1]){
        case MB_FUNC_READ_HOLDING_REGISTERS: 
            start_addr.bytes.high = g_modbus_lib_received_telegram[2]; 
            start_addr.bytes.low = g_modbus_lib_received_telegram[3]; 
            count.bytes.high = g_modbus_lib_received_telegram[4];
            count.bytes.low = g_modbus_lib_received_telegram[5];

            outgoing_telegram[oindex++] = MB_FUNC_READ_HOLDING_REGISTERS;  // function code 
            outgoing_telegram[oindex++] = count.value * 2;                 // byte count 

            for (uint16_t i=0; i < count.value; i++){  
                res.value = modbus_lib_read_handler(start_addr.value + i + MB_ADDRESS_HOLDING_REGISTER_OFFSET);
                if (g_modbus_lib_exception_occurred){
                    g_modbus_lib_exception_occurred = 0; 
                    return; 
                }
                outgoing_telegram[oindex++] = res.bytes.high; 
                outgoing_telegram[oindex++] = res.bytes.low; 
            }

            CRC_t crc = usMBCRC16(outgoing_telegram, oindex);
            outgoing_telegram[oindex++] = crc.bytes.low; 
            outgoing_telegram[oindex++] = crc.bytes.high; 

            modbus_lib_transport_write(outgoing_telegram, oindex);
            break;
    }

    g_modbus_lib_received_length = 0; //// debugger: p/x *g_modbus_lib_received_telegram@g_modbus_lib_received_length
}

#define MB_EXCEPTION_LENGTH 5 

uint16_t modbus_lib_send_error(int error_code){
    if (error_code != MBUS_RESPONSE_NONE){
        g_modbus_lib_exception_occurred = 1; 
        uint8_t res[MB_EXCEPTION_LENGTH] = {
            config->address, 
            g_modbus_lib_received_telegram[1], 
            error_code
        };
        CRC_t crc = usMBCRC16(res, MB_EXCEPTION_LENGTH - 2);
        res[MB_EXCEPTION_LENGTH - 2] = crc.bytes.low; 
        res[MB_EXCEPTION_LENGTH - 1] = crc.bytes.high; 

        modbus_lib_transport_write(res, MB_EXCEPTION_LENGTH); 
    }
    g_modbus_lib_received_length = 0;
    return -1; 
}