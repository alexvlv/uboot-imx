// SPDX-License-Identifier: GPL-2.0+

#include "jdi-lt070me05000.h"

#include <linux/err.h>

#include <dm.h>
#include <dm/device_compat.h>

static const char * const regulator_names[] = {
	"vddp-supply",
	"iovcc-supply"
};

//-------------------------------------------------------------------------
int lt070me_power_init(struct udevice *dev)
{
	struct lt070me_panel_priv *priv = dev_get_priv(dev);
	int ret=0, i;

	priv->regulators = calloc(ARRAY_SIZE(regulator_names),sizeof(struct udevice *));

	for (i = 0; i < ARRAY_SIZE(regulator_names) && ret==0; i++) {
		ret = device_get_supply_regulator(dev, regulator_names[i], &priv->regulators[i]);
		if (ret && ret != -ENOENT) {
			dev_err(dev, "Warning: cannot get power supply [%s]\n",regulator_names[i]);
		}
	}
	printf("!!! %s done, ret:%d !!!!\n", __func__, ret);
	return ret;
}
//-------------------------------------------------------------------------
int lt070me_power_on(struct udevice *dev)
{
	struct lt070me_panel_priv *priv = dev_get_priv(dev);
	int ret=0, i;

	priv->regulators = calloc(ARRAY_SIZE(regulator_names),sizeof(struct udevice *));

	for (i = 0; i < ARRAY_SIZE(regulator_names) && ret==0; i++) {
		ret = regulator_set_enable(&priv->regulators[i], true);
        if (ret) {
			dev_err(dev, "Warning: cannot power ON [%s]\n",regulator_names[i]);
		}
	}
	return ret;
}
//-------------------------------------------------------------------------
