#include "app.h"

const hdl_module_base_t mod_app = {
  .dependencies = hdl_module_dependencies(
    &mod_ic,
    &mod_timer_ms,
    &mod_sns_pwr_pin,
    &mod_acc_pwr_pin,
    &mod_acc_int1_pin,
    &mod_acc_int2_pin,
    &mod_adc,
    &mod_uart,
    &mod_i2c,
    &mod_storage
  ),
  .mod_var = static_malloc(HDL_MODULE_VAR_SIZE)
};
