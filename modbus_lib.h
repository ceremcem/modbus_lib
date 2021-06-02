#define MODBUS_LIB_MAX_BUFFER 256

/* ----------------------- Defines ------------------------------------------*/
#define MB_ADDRESS_BROADCAST    ( 0 )   /*! Modbus broadcast address. */
#define MB_ADDRESS_MIN          ( 1 )   /*! Smallest possible slave address. */
#define MB_ADDRESS_MAX          ( 247 ) /*! Biggest possible slave address. */
#define MB_FUNC_NONE                          (  0 )
#define MB_FUNC_READ_COILS                    (  1 )
#define MB_FUNC_READ_DISCRETE_INPUTS          (  2 )
#define MB_FUNC_WRITE_SINGLE_COIL             (  5 )
#define MB_FUNC_WRITE_MULTIPLE_COILS          ( 15 )
#define MB_FUNC_READ_HOLDING_REGISTERS        (  3 )
#define MB_FUNC_READ_INPUT_REGISTER           (  4 )
#define MB_FUNC_WRITE_REGISTER                (  6 )
#define MB_FUNC_WRITE_MULTIPLE_REGISTERS      ( 16 )
#define MB_FUNC_READWRITE_MULTIPLE_REGISTERS  ( 23 )
#define MB_FUNC_DIAG_READ_EXCEPTION           (  7 )
#define MB_FUNC_DIAG_DIAGNOSTIC               (  8 )
#define MB_FUNC_DIAG_GET_COM_EVENT_CNT        ( 11 )
#define MB_FUNC_DIAG_GET_COM_EVENT_LOG        ( 12 )
#define MB_FUNC_OTHER_REPORT_SLAVEID          ( 17 )
#define MB_FUNC_ERROR                         ( 128 )

/*----------------------- Response types ------------------------------*/
#define  MBUS_RESPONSE_OK 0x00
#define  MBUS_RESPONSE_NONE 0xFF
/* MBUS_RESPONSE_ILLEGAL_FUNCTION
The function  code  received  in  the  query  is  not  an allowable   action
for   the server. This   may   be because  the  function  code  is  only
applicable  to newerdevices,  and  was  not  implemented  in  the unit
selected. It could also indicate  that the serveris  in  the  wrong  state  to
process  a  request  of  this  type,  for  example  because  it  is
unconfigured and is being asked to return register values.
*/
#define MBUS_RESPONSE_ILLEGAL_FUNCTION 0x01

/* MBUS_RESPONSE_ILLEGAL_DATA_ADDRESS
The  data  address  received  in  the  query  is  not  an allowable address
for the server. More specifically,  the  combination  of  reference  number
and  transfer  length  is  invalid. For  a  controller  with 100 registers,
the PDU addresses the first register as  0,  and  the  last  one  as  99.  If
a  request  is submitted  with  a  starting  register  address  of  96 and  a
quantity  of  registers  of  4,  then  this  request will  successfully
operate  (address-wise  at  least) on   registers   96,   97,   98,   99.   If
a   request   is submitted  with  a  starting  register  address  of  96 and
a  quantity  of  registers  of  5,  then  this  request will  fail  with
Exception  Code  0x02  “Illegal  Data Address”  since  it  attempts  to
operate  on  registers 96,  97, 98,  99  and  100,  and  there  is  no
register with address 100.
*/
#define MBUS_RESPONSE_ILLEGAL_DATA_ADDRESS  0x02

/* A  value contained  in the  query  data field  is  not  an allowable value
for server. This indicates a fault in the structure   of   the   remainder of
a   complex request,   such   as   that   the   implied   length   is
incorrect. It specifically does NOT mean that a data item submitted for
storage in a register has a value outside the expectation of the application
program,  since  the  MODBUS  protocol  is  unaware of  the  significance  of
any  particular  val ue  of  any particular register.
*/
#define  MBUS_RESPONSE_ILLEGAL_DATA_VALUE 0x03
/*
An  unrecoverable  error  occurred  while  the  server
was attempting to perform the requested action.
*/
#define MBUS_RESPONSE_SERVICE_DEVICE_FAILURE 0x04


#define MB_ADDRESS_HOLDING_REGISTER_OFFSET  (40001)

typedef struct ModbusConfig_t{
  uint16_t address;
} ModbusConfig_t;

extern uint8_t g_modbus_lib_received_telegram[MODBUS_LIB_MAX_BUFFER]; 
extern uint16_t g_modbus_lib_received_length; 

extern int modbus_lib_init(ModbusConfig_t*);
extern void modbus_lib_append_data(uint8_t);
extern void modbus_lib_end_of_telegram(void);
extern uint16_t modbus_lib_send_error(int error_code);
extern int modbus_lib_transport_write(uint8_t* buffer, uint16_t length);
extern uint16_t modbus_lib_read_handler(uint16_t la);
extern uint16_t modbus_lib_write_handler(uint16_t la, uint16_t value);
