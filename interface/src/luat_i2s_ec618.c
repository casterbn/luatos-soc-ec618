/*
 * Copyright (c) 2022 OpenLuat & AirM2M
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "luat_i2s_ec618.h"
extern void soc_i2s_init(void);
extern int soc_i2s_start(uint8_t bus_id, uint8_t is_play, uint32_t sample, uint8_t channel_num);
extern void soc_i2s_base_setup(uint8_t bus_id, uint8_t mode,  uint8_t frame_size);
extern void soc_i2s_tx(uint8_t bus_id, uint8_t* address, uint32_t byte_len, CBFuncEx_t cb, void *param);
extern void soc_i2s_tx_stop(uint8_t bus_id);
extern void soc_i2s_deinit(uint8_t bus_id);
extern void soc_i2s_pause(uint8_t bus_id);
void luat_i2s_init(void)
{
	soc_i2s_init();
}

void luat_i2s_base_setup(uint8_t bus_id, uint8_t mode,  uint8_t frame_size)
{
	soc_i2s_base_setup(bus_id, mode, frame_size);
}

int luat_i2s_start(uint8_t bus_id, uint8_t is_play, uint32_t sample, uint8_t channel_num)
{
	return soc_i2s_start(bus_id, is_play, sample, channel_num);
}

void luat_i2s_no_block_tx(uint8_t bus_id, uint8_t* address, uint32_t byte_len, CBFuncEx_t cb, void *param)
{
	soc_i2s_tx(bus_id, address, byte_len, cb, param);
}

void luat_i2s_tx_stop(uint8_t bus_id)
{
	soc_i2s_tx_stop(bus_id);
}

void luat_i2s_deinit(uint8_t bus_id)
{
	soc_i2s_deinit(bus_id);
}
void luat_i2s_pause(uint8_t bus_id)
{
	soc_i2s_pause(bus_id);
}
