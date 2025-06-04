#include "app.h"
#include "CodeLib.h"

#include "SEGGER_RTT.h"

uint8_t read_temp(int32_t *out_temp) {
  static int16_t temp_buf; 
  static uint8_t sensor_driver_state = 0;
  static hdl_i2c_message_t msg;
  switch (sensor_driver_state) {
    case 0:
      msg.address = 0x48;
      msg.buffer = (uint8_t[]){0x00};
      msg.length = 1;
      msg.options = HDL_I2C_MESSAGE_START | HDL_I2C_MESSAGE_ADDR;
      sensor_driver_state++;
      /* fall through */
    case 1:
    case 4:
      if(!hdl_take(&mod_i2c, (void*)1) || !hdl_i2c_transfer(&mod_i2c, &msg)) break;
      sensor_driver_state++;
      /* fall through */
    case 5:
    case 2:
      if(!(msg.status & HDL_I2C_MESSAGE_STATUS_COMPLETE)) break;
      if(msg.status & (HDL_I2C_MESSAGE_FAULT_ARBITRATION_LOST | HDL_I2C_MESSAGE_FAULT_BUS_ERROR | HDL_I2C_MESSAGE_FAULT_BAD_STATE | HDL_I2C_MESSAGE_STATUS_NACK)) {
        sensor_driver_state = 0;
        hdl_give(&mod_i2c, (void*)1);
        break;
      }
      sensor_driver_state++;
      break;
    case 3:
      msg.address = 0x48;
      msg.buffer = (uint8_t *)&temp_buf;
      msg.length = 2;
      msg.options = HDL_I2C_MESSAGE_START | HDL_I2C_MESSAGE_ADDR | HDL_I2C_MESSAGE_MRSW | HDL_I2C_MESSAGE_NACK_LAST | HDL_I2C_MESSAGE_STOP;
      sensor_driver_state++;
      break;
    case 6:
    default: {
      temp_buf = swap_bytes(temp_buf);
      //if(!(temp_buf & 1)) temp_buf >>= 1;
      temp_buf /= 16;
      int32_t res = (625L * (int32_t)(temp_buf + 880)) - 550000L;
      if(out_temp != NULL) *out_temp = res;
      hdl_give(&mod_i2c, (void*)1);
      sensor_driver_state = 0;
      return HDL_TRUE;
    }
  }
  return HDL_FALSE;
}

typedef struct {
    uint8_t dummy[2]; /* align 4 */
    uint8_t ctrl_reg0; /* 0x1E */
    uint8_t temp_cfg_reg;
    uint8_t ctrl_reg1;
    uint8_t ctrl_reg2;
    uint8_t ctrl_reg3;
    uint8_t ctrl_reg4;
    uint8_t ctrl_reg5;
    uint8_t ctrl_reg6;
    uint8_t reference;
    uint8_t status_reg;
    uint16_t out_x;
    uint16_t out_y;
    uint16_t out_z;
    uint8_t fifo_ctrl_reg;
    uint8_t fifo_src_reg;
    uint8_t int1_cfg;
    uint8_t int1_src;
    uint8_t int1_ths;
    uint8_t int1_duration;
    uint8_t int2_cfg;
    uint8_t int2_src;
    uint8_t int2_ths;
    uint8_t int2_duration;
    uint8_t click_cfg;
    uint8_t click_src;
    uint8_t click_ths;
    uint8_t time_limit;
    uint8_t time_latency;
    uint8_t time_window;
    uint8_t act_ths;
    uint8_t act_dir;
} axel_data_t;

uint8_t read_axel(axel_data_t *out_data) {
  static uint8_t sensor_driver_state = 0;
  static hdl_i2c_message_t msg;
  switch (sensor_driver_state) {
    case 0:
      msg.address = 0x18;
      msg.buffer = (uint8_t[]){0x1E};
      msg.length = 1;
      msg.options = HDL_I2C_MESSAGE_START | HDL_I2C_MESSAGE_ADDR;
      sensor_driver_state++;
      /* fall through */
    case 1:
    case 4:
      if(!hdl_take(&mod_i2c, (void*)2) || !hdl_i2c_transfer(&mod_i2c, &msg)) break;
      sensor_driver_state++;
      /* fall through */
    case 5:
    case 2:
      if(!(msg.status & HDL_I2C_MESSAGE_STATUS_COMPLETE)) break;
      if(msg.status & (HDL_I2C_MESSAGE_FAULT_ARBITRATION_LOST | HDL_I2C_MESSAGE_FAULT_BUS_ERROR | HDL_I2C_MESSAGE_FAULT_BAD_STATE | HDL_I2C_MESSAGE_STATUS_NACK)) {
        sensor_driver_state = 0;
        hdl_give(&mod_i2c, (void*)2);
        break;
      }
      sensor_driver_state++;
      break;
    case 3:
      msg.address = 0x18;
      msg.buffer = (((uint8_t *)out_data) + 2); /* +2 to pointer: skip dummy */
      msg.length = sizeof(axel_data_t) - 2; /* -2 from size: sub dummy size */
      msg.options = HDL_I2C_MESSAGE_START | HDL_I2C_MESSAGE_ADDR | HDL_I2C_MESSAGE_MRSW | HDL_I2C_MESSAGE_NACK_LAST | HDL_I2C_MESSAGE_STOP;
      sensor_driver_state++;
      break;
    case 6:
    default: {
      out_data->out_x = swap_bytes(out_data->out_x);
      out_data->out_y = swap_bytes(out_data->out_y);
      out_data->out_z = swap_bytes(out_data->out_z);
      hdl_give(&mod_i2c, (void*)2);
      return HDL_TRUE;
    }
  }
  return HDL_FALSE;
}

void main() {

  SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);

  SEGGER_RTT_WriteString(0, "SEGGER Real-Time-Terminal Sample\r\n\r\n");
  SEGGER_RTT_WriteString(0, "###### Testing SEGGER_printf() ######\r\n");

  hdl_enable(&mod_app);

  while (!hdl_init_complete()) {
    cooperative_scheduler(cl_false);
  }

  /* gpio exapmle */
  hdl_gpio_set_active(&mod_sns_pwr_pin);
  // hdl_gpio_set_active(&mod_acc_pwr_pin);

  /* storage write exapmle */
  uint8_t store[] = "Hello world";
  hdl_nvm_message_t storage_message = {
    .address = 0x0100,
    .size = sizeof(store),
    .buffer = store,
    .options = HDL_NVM_OPTION_WRITE
  };
  storage_message.status = HDL_NVM_STATE_COMPLETE;

//  hdl_nvm_transfer(&mod_storage, &storage_message);

  /* uart read&write exapmle */
  stream_test(uart_stream_init());

  int32_t temp;
  axel_data_t axel;
  while (1) {
    /* adc exapmle */
    uint32_t adc_age = hdl_adc_age(&mod_adc);
    uint32_t current = hdl_adc_get(&mod_adc, adc_src_id_cur);
    uint32_t voltage = hdl_adc_get(&mod_adc, adc_src_id_volt);
    (void)adc_age; (void)current; (void)voltage;

    /* i2c temp sns exapmle */
     if(read_temp(&temp)) {
       __NOP();
     }

    /* i2c axel sns exapmle */
    // if(read_axel(&axel)) {
    //   __NOP();
    // }

    /* storage result & read restart exapmle */
    if(storage_message.status & HDL_NVM_STATE_COMPLETE) {
      storage_message.options = HDL_NVM_OPTION_READ;
      mem_set(store, 0, sizeof(store));
      hdl_nvm_transfer(&mod_storage, &storage_message);
    }

    /* run timer example */
    static uint32_t log_time = 0;
    static uint32_t loops = 0;
    uint32_t now = hdl_time_counter_get(&mod_timer_ms);
    if((now - log_time) >= 100) {
      SEGGER_RTT_printf(0, RTT_CTRL_TEXT_YELLOW"SEGGER RTT Sample. Loops: %u. Uptime: %ums.\n\r", loops, now);
      loops = 0;
      log_time = now;
    }
    cooperative_scheduler(cl_false);
    loops++;
  }
}
