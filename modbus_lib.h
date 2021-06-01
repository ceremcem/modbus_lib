#define MODBUS_LIB_MAX_BUFFER 256

typedef struct ModbusConfig_t{
  uint16_t address;
} ModbusConfig_t;

extern uint8_t g_modbus_lib_received_telegram[MODBUS_LIB_MAX_BUFFER]; 
extern uint16_t g_modbus_lib_received_length; 

extern int modbus_lib_init(ModbusConfig_t*);
extern void modbus_lib_append_data(uint8_t);
extern void modbus_lib_end_of_telegram(void);
