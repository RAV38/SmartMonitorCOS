#ifndef ATB_MIG_H_
#define ATB_MIG_H_

extern const hdl_interrupt_controller_t mod_ic;
extern const hdl_time_counter_t mod_timer_ms;
extern const hdl_gpio_pin_t mod_sns_pwr_pin;
extern const hdl_gpio_pin_t mod_acc_pwr_pin;
extern const hdl_gpio_pin_t mod_acc_int1_pin;
extern const hdl_gpio_pin_t mod_acc_int2_pin;
extern const hdl_adc_t mod_adc;
extern const hdl_uart_t mod_uart;
extern const hdl_i2c_t mod_i2c;
extern const hdl_nvm_t mod_storage;
extern hdl_interrupt_t acc_int1;
extern hdl_interrupt_t acc_int2;
extern const uint32_t adc_src_id_cur;
extern const uint32_t adc_src_id_volt;

#endif // ATB_MIG_H_
