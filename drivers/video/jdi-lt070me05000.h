#pragma once

#include <common.h>
#include <asm/gpio.h>

struct lt070me_panel_priv {
	struct gpio_desc **gpios;
};
