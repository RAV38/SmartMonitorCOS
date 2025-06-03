#ifndef APP_H_
#define APP_H_

#include "hdl_iface.h"
#include "mig.h"

extern const hdl_module_base_t mod_app;

void main();

void gpio_test();

void spi_client_test();

hdl_stream_buffer_t *uart_stream_init();
void stream_test(hdl_stream_buffer_t * hdl_stream);

#endif /* APP_H_ */
