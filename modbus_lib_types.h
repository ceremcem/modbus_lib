#ifndef _MODBUS_LIB_H_
#define _MODBUS_LIB_H_

#include <stdint.h>

typedef enum { MBUS_OK = 0, MBUS_ERROR = -1 } mbus_status_t;

typedef enum {
  MBUS_FUNC_READ_COILS = 1,
  MBUS_FUNC_READ_DISCRETE = 2,
  MBUS_FUNC_READ_REGS = 3,
  MBUS_FUNC_READ_INPUT_REGS = 4,
  MBUS_FUNC_WRITE_COIL = 5,
  MBUS_FUNC_WRITE_REG = 6,
  MBUS_FUNC_READ_EXCEPT_STATUS = 7,
  MBUS_FUNC_DIAGNOSTICS = 8,
  MBUS_FUNC_GET_COMM_EVENT_COUNTER = 11,
  MBUS_FUNC_GET_COMM_EVENT_LOG = 12,
  MBUS_FUNC_WRITE_COILS = 15,
  MBUS_FUNC_WRITE_REGS = 16,
  MBUS_FUNC_READ_SLAVE_ID = 17,
  MBUS_FUNC_READ_FILE_RECORD = 20,
  MBUS_FUNC_WRITE_FILE_RECORD = 21,
  MBUS_FUNC_READ_WRITE_MASK_REGS = 22,
  MBUS_FUNC_READ_WRITE_REGS = 23,
  MBUS_FUNC_READ_FIFO_QUEUE = 24,
  MBUS_FUNC_READ_DEVICE_ID = 43,
  MBUS_FUNC_EXCEPTION = 0x81,
} Modbus_ConnectFuncType;

typedef enum {
  MBUS_STATE_IDLE = 0,
  MBUS_STATE_DEVADD,
  MBUS_STATE_FUNCTION,
  MBUS_STATE_REGADDR_LO,
  MBUS_STATE_REGADDR_HI,
  MBUS_STATE_REGNUM_LO,
  MBUS_STATE_REGNUM_HI,
  MBUS_STATE_DATA_LO,
  MBUS_STATE_DATA_HI,
  MBUS_STATE_DATA_SIZE,
  MBUS_STATE_DATA,
  MBUS_STATE_CRC_LO,
  MBUS_STATE_CRC_HI,
  MBUS_STATE_FINISH,
  MBUS_STATE_RESPONSE

} Modbus_StateType;

typedef enum MBUS_RESPONSE {

  MBUS_RESPONSE_OK = 0x00,
  MBUS_RESPONSE_NONE = 0xFF,
  /* MBUS_RESPONSE_ILLEGAL_FUNCTION
  The function  code  received  in  the  query  is  not  an allowable   action
  for   the server. This   may   be because  the  function  code  is  only
  applicable  to newerdevices,  and  was  not  implemented  in  the unit
  selected. It could also indicate  that the serveris  in  the  wrong  state  to
  process  a  request  of  this  type,  for  example  because  it  is
  unconfigured and is being asked to return register values.
  */
  MBUS_RESPONSE_ILLEGAL_FUNCTION = 0x01,

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
  MBUS_RESPONSE_ILLEGAL_DATA_ADDRESS = 0x02,

  /* A  value contained  in the  query  data field  is  not  an allowable value
  for server. This indicates a fault in the structure   of   the   remainder of
  a   complex request,   such   as   that   the   implied   length   is
  incorrect. It specifically does NOT mean that a data item submitted for
  storage in a register has a value outside the expectation of the application
  program,  since  the  MODBUS  protocol  is  unaware of  the  significance  of
  any  particular  val ue  of  any particular register.
  */
  MBUS_RESPONSE_ILLEGAL_DATA_VALUE = 0x03,
  /*
  An  unrecoverable  error  occurred  while  the  server
  was attempting to perform the requested action.
  */
  MBUS_RESPONSE_SERVICE_DEVICE_FAILURE = 0x04,
} Modbus_ResponseType;

#endif // _MODBUS_LIB_H_