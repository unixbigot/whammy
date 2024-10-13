/* main.c - Application main entry point */

/*
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#define __main__
#include "ztacx.h"
#if CONFIG_SHELL
#include <zephyr/shell/shell_uart.h>
#endif

ZTACX_USE_VAR(led0_duty);
ZTACX_USE_VAR(led0_cycle);

ZTACX_USE_VAR(expression_value);
//ZTACX_USE_VAR(led_strip_cursor); // only one led so cursor always zero

#ifdef CONFIG_ZTACX_LEAF_LED_STRIP
ZTACX_USE_VAR(led_strip_color);
#endif

#if CONFIG_ZTACX_LEAF_BT_CENTRAL
ZTACX_USE_VAR(bt_central_scanning);
ZTACX_USE_VAR(bt_central_connected);
ZTACX_USE_VAR(bt_central_connect);
ZTACX_USE_VAR(bt_central_subscribe16_uuid);
ZTACX_USE_VAR(bt_central_subscribe16_value);
#endif

#if CONFIG_ZTACX_LEAF_BT_PERIPHERAL
ZTACX_USE_VAR(bt_peripheral_ok);
ZTACX_USE_VAR(bt_peripheral_advertising);
ZTACX_USE_VAR(bt_peripheral_connected);
#endif


#if CONFIG_ZTACX_LEAF_IMS
ZTACX_USE_VAR(ims_read_interval_usec);
ZTACX_USE_VAR(ims_change_threshold);

ZTACX_USE_VAR(ims_level_x);
ZTACX_USE_VAR(ims_level_y);
ZTACX_USE_VAR(ims_level_z);
ZTACX_USE_VAR(ims_peak_x);
ZTACX_USE_VAR(ims_peak_y);
ZTACX_USE_VAR(ims_peak_z);
ZTACX_USE_VAR(ims_samples);
ZTACX_USE_VAR(tilt_threshold);
#endif


#if CONFIG_ZTACX_LEAF_DAC
ZTACX_USE_VAR(dac_bits);
ZTACX_USE_VAR(dac_level);
#endif

static struct ztacx_variable app_values[] = {
#if CONFIG_ZTACX_LEAF_IMS
	{"tilt_threshold",ZTACX_VALUE_INT32,{.val_int32=CONFIG_APP_TILT_THRESHOLD}},
#endif
	{"expression_value",ZTACX_VALUE_INT16,{.val_int16=0}},
};

#ifdef CONFIG_ZTACX_LEAF_BT_PERIPHERAL
#ifdef CONFIG_ZTACX_LEAF_IMS
// sender
#define SERVICE_ID 0xe3,0x97,0x25,0x12,0xb8,0xd2,0x11,0xed,0x98,0x62,0x13,0x4a,0x95,0x00,0x26,0x8e
#else
// reciever service 3fd623a2-8387-11ef-aab6-3af9d3cb546b
#define SERVICE_ID 0x6b,0x54,0xcb,0xd3,0xf9,0x3a,0xb6,0xaa,0xef,0x11,0x87,0x83,0xa2,0x23,0xd6,0x3f
#endif

static struct bt_uuid_128 service_uuid = BT_UUID_INIT_128(SERVICE_ID);

static const struct bt_data bt_adv_data_srv[]={
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, SERVICE_ID)
};

#if 0
static const struct bt_data bt_adv_data_name[]={
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, sizeof(CONFIG_BT_DEVICE_NAME)-1)
};

static const struct bt_data bt_adv_scanresp_name[]={
	BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, sizeof(CONFIG_BT_DEVICE_NAME)-1)
};
#endif

/*
 * UUIDs generated using
 *
 * uuid | perl -p -e 'print; s/-//g; s/([0-9a-f]{2})/0x$1,/g;' -e 's/,$//' -e '$_=join(",", reverse split(/,/);'
 */

#ifdef CONFIG_ZTACX_LEAF_IMS
static const struct bt_uuid_128 char_ims_read_interval_usec_uuid = BT_UUID_INIT_128(0x10,0xbe,0x21,0x62,0xb8,0xd8,0x11,0xed,0x8c,0x20,0x2b,0x70,0x66,0xff,0x29,0xf3);
static const struct bt_uuid_128 char_ims_change_threshold_uuid  = BT_UUID_INIT_128(0x17,0x37,0xf5,0x7c,0xb8,0xd8,0x11,0xed,0x9f,0x51,0xb3,0x9b,0x52,0x86,0x56,0xf9);
static const struct bt_uuid_128 char_ims_level_x_uuid       = BT_UUID_INIT_128(0xfe,0x25,0x34,0x90,0xb8,0xd3,0x11,0xed,0xb0,0x56,0xf3,0x40,0xb7,0xe5,0x58,0xa8);
static const struct bt_uuid_128 char_ims_level_y_uuid       = BT_UUID_INIT_128(0x04,0xf1,0x81,0x3e,0xb8,0xd4,0x11,0xed,0xab,0x57,0xeb,0x87,0xa6,0x89,0x83,0xe0);
static const struct bt_uuid_128 char_ims_level_z_uuid       = BT_UUID_INIT_128(0x09,0x2d,0xdd,0x1a,0xb8,0xd4,0x11,0xed,0xad,0xee,0x7b,0xb9,0xea,0xee,0x3f,0xe0);
static const struct bt_uuid_128 char_ims_peak_x_uuid  = BT_UUID_INIT_128(0x11,0xda,0x19,0xe2,0xb8,0xd4,0x11,0xed,0xac,0xf0,0xf7,0x04,0x2a,0xa0,0x8e,0xf1);
static const struct bt_uuid_128 char_ims_peak_y_uuid  = BT_UUID_INIT_128(0x16,0x51,0xbe,0x80,0xb8,0xd4,0x11,0xed,0x87,0x3b,0x2f,0x2a,0x93,0xa6,0xd3,0x0a);
static const struct bt_uuid_128 char_ims_peak_z_uuid  = BT_UUID_INIT_128(0x1a,0x36,0xb1,0x9a,0xb8,0xd4,0x11,0xed,0xac,0xec,0xdf,0x2d,0x84,0x9f,0x89,0xaa);
static const struct bt_uuid_128 char_ims_samples_uuid  = BT_UUID_INIT_128(0xa1,0x4d,0xed,0x9e,0xb9,0x90,0x11,0xed,0x8c,0x13,0xaf,0xf9,0x9c,0x0e,0x9b,0x5d);
#endif

static const struct bt_uuid_16 char_expression_value_uuid = BT_UUID_INIT_16(BT_UUID_GATT_GENLVL_VAL); // standard 'generic level' characteristic

// static const struct bt_uuid_128 char_expression_value_uuid  = BT_UUID_INIT_128(0x6b,0x54,0xcb,0xd3,0xf9,0x3a,0xc5,0xbc,0xef,0x11,0x87,0x83,0x88,0x61,0x74,0x96); // 96746188-8387-11ef-bcc5-3af9d3cb546b

#ifdef CONFIG_ZTACX_LEAF_IMS
static const struct bt_gatt_cpf bt_gatt_cpf_cmpsps = {
	.format = 16,   // int32
	.exponent = -2, // scale factor 1/100
	.unit = 0x2713, // unit m/s/s
};

static const struct bt_gatt_cpf bt_gatt_cpf_usec = {
	.format = 16,   // int32
	.exponent = -6, // scale factor 1/1000000
	.unit = 0x2703, // unit second
};
#endif

BT_GATT_SERVICE_DEFINE(
	svc,
	BT_GATT_PRIMARY_SERVICE(&service_uuid), // 0
	ZTACX_BT_SENSOR(expression_value, int16,   "Expression pedal value (16-bit signed)") //1
#if CONFIG_ZTACX_LEAF_IMS
	,
	ZTACX_BT_SETTING(ims_read_interval_usec, usec,   "Read interval in microseconds"), //5
	ZTACX_BT_SETTING(ims_change_threshold,   cmpsps, "Change threshold in cm/s/s)"), //9
	ZTACX_BT_SENSOR(ims_level_x,             cmpsps, "X-axis acceleration in cm/s/s)"),//13
	ZTACX_BT_SENSOR(ims_level_y,             cmpsps, "Y-axis acceleration in cm/s/s)"),//17
	ZTACX_BT_SENSOR(ims_level_z,             cmpsps, "Z-axis acceleration in cm/s/s)"),
	ZTACX_BT_SENSOR(ims_peak_x,              cmpsps, "Peak X-axis acceleration in cm/s/s)"),
	ZTACX_BT_SENSOR(ims_peak_y,              cmpsps, "Peak Y-axis acceleration in cm/s/s)"),
	ZTACX_BT_SENSOR(ims_peak_z,              cmpsps, "Peak Z-axis acceleration in cm/s/s)"),
	ZTACX_BT_SENSOR(ims_samples,             int64,  "Number of samples taken"),
#endif
	);

const struct bt_gatt_attr *expression_value_attr = &svc.attrs[1];

#if CONFIG_ZTACX_LEAF_IMS
const struct bt_gatt_attr *x_tilt_attr = &svc.attrs[9];
const struct bt_gatt_attr *y_tilt_attr = &svc.attrs[13];
#endif
#endif

const char *role_str() 
{
#if CONFIG_APP_ROLE_TRANSMITTER && CONFIG_APP_ROLE_RECEIVER
	return "dual";
#endif
#if CONFIG_APP_ROLE_TRANSMITTER
	return "transmitter";
#endif
#if CONFIG_APP_ROLE_RECEIVER
	return "receiver";
#endif
	return "none";
}

#if CONFIG_APP_ROLE_TRANSMITTER
static struct k_work peripheral_state_change_work;
void app_bt_peripheral_state_change(struct k_work *work) 
{
	bool advertising = ztacx_variable_value_get_bool(bt_peripheral_advertising);
	bool connected = ztacx_variable_value_get_bool(bt_peripheral_connected);
	LOG_INF("NOTICE: BT peripheral state change advertising=%d connected=%d",
		(int)advertising, (int)connected);

	if (connected) {
		ztacx_variable_value_set_uint16(led0_cycle, 5000);
		ztacx_variable_value_set_uint16(led0_duty, 1);
	}
	else if (advertising) {
		ztacx_variable_value_set_uint16(led0_cycle, 1000);
		ztacx_variable_value_set_uint16(led0_duty, 1);
	}
	else {
		ztacx_variable_value_set_uint16(led0_cycle, 1000);
		ztacx_variable_value_set_uint16(led0_duty, 50);
	}
}
#endif

#if CONFIG_APP_ROLE_RECEIVER
static struct k_work central_state_change_work;
static struct k_work level_change_work;
static struct k_work_delayable central_request_connect_work;
void app_bt_central_request_connect(struct k_work *work) 
{
	LOG_INF("");
	ztacx_variable_value_set_bool(bt_central_connect, true);
}
void app_bt_central_state_change(struct k_work *work) 
{
	bool scanning = ztacx_variable_value_get_bool(bt_central_scanning);
	bool connected = ztacx_variable_value_get_bool(bt_central_connected);
	LOG_INF("NOTICE: BT central state change scanning=%d connected=%d",
		(int)scanning, (int)connected);

	if (connected) {
		ztacx_variable_value_set_string(led_strip_color, "green");

		// tell the central module to subscribe to GENERIC LEVEL
		LOG_INF("Instructing BT central to subscribe for notifications");
		ztacx_variable_value_set_uint16(bt_central_subscribe16_uuid, BT_UUID_GATT_GENLVL_VAL);
	}
	else if (scanning) {
		ztacx_variable_value_set_string(led_strip_color, "orange");
	}
	else {
		ztacx_variable_value_set_string(led_strip_color, "red");
	}
}

void app_level_change(struct k_work *work) 
{
	int16_t level = ztacx_variable_value_get_int16(bt_central_subscribe16_value);
	int16_t level_was = ztacx_variable_value_get_int16(expression_value);
	LOG_INF("Received level change %d => %d", (int)level_was, (int)level);
	ztacx_variable_value_set_int16(expression_value, level);
}


#endif

static int app_init(void) 
{
	LOG_INF("NOTICE >INIT app");
	printk("Accelerando BT Whammy Pedal (%s) app init\n", role_str());
	printk("Device name \"%s\" version %s\n", CONFIG_BT_DEVICE_NAME, CONFIG_MCUBOOT_IMGTOOL_SIGN_VERSION);

	ztacx_variables_register(app_values, ARRAY_SIZE(app_values));
	ZTACX_VAR_FIND(expression_value);

	ZTACX_SETTING_FIND(led0_duty);
	ZTACX_SETTING_FIND(led0_cycle);

#ifdef CONFIG_ZTACX_LEAF_LED_STRIP
	ZTACX_VAR_FIND(led_strip_color);
#endif

#if CONFIG_ZTACX_LEAF_IMS
	ZTACX_SETTING_FIND_OPT(ims_read_interval_usec);
	ZTACX_SETTING_FIND_OPT(ims_change_threshold);

	ZTACX_VAR_FIND_OPT(ims_level_x);
	ZTACX_VAR_FIND_OPT(ims_level_y);
	ZTACX_VAR_FIND_OPT(ims_level_z);
	ZTACX_VAR_FIND_OPT(ims_peak_x);
	ZTACX_VAR_FIND_OPT(ims_peak_y);
	ZTACX_VAR_FIND_OPT(ims_peak_z);
	ZTACX_VAR_FIND_OPT(ims_samples);
	ZTACX_VAR_FIND_OPT(tilt_threshold);
	ZTACX_VAR_FIND(expression_value);
#endif

#if CONFIG_ZTACX_LEAF_DAC
	ZTACX_SETTING_FIND_OPT(dac_bits);
	ZTACX_VAR_FIND_OPT(dac_level);
#endif

#if CONFIG_ZTACX_LEAF_BT_CENTRAL
	ZTACX_VAR_FIND(bt_central_scanning);
	ZTACX_VAR_FIND(bt_central_connected);
	ZTACX_VAR_FIND(bt_central_connect);
	ZTACX_VAR_FIND(bt_central_subscribe16_uuid);
	ZTACX_VAR_FIND(bt_central_subscribe16_value);
#endif

#if CONFIG_ZTACX_LEAF_BT_PERIPHERAL
	ZTACX_VAR_FIND(bt_peripheral_ok);
	ZTACX_VAR_FIND(bt_peripheral_advertising);
	ZTACX_VAR_FIND(bt_peripheral_connected);
#endif

#if CONFIG_APP_ROLE_TRANSMITTER
	LOG_INF("register change handler for bluetooth peripheral state");
	k_work_init(&peripheral_state_change_work, &app_bt_peripheral_state_change);
	if (bt_peripheral_ok) {
		ztacx_variable_ptr_set_onchange(bt_peripheral_ok, &peripheral_state_change_work);
	}
	if (bt_peripheral_advertising) {
		ztacx_variable_ptr_set_onchange(bt_peripheral_advertising, &peripheral_state_change_work);
	}
	if (bt_peripheral_connected) {
		ztacx_variable_ptr_set_onchange(bt_peripheral_connected, &peripheral_state_change_work);
	}
#endif

#if CONFIG_APP_ROLE_RECEIVER
	LOG_INF("register change handler for bluetooth central state");
	k_work_init(&central_state_change_work, &app_bt_central_state_change);
	k_work_init(&level_change_work, &app_level_change);
	k_work_init_delayable(&central_request_connect_work, &app_bt_central_request_connect);
	if (bt_central_scanning) {
		ztacx_variable_ptr_set_onchange(bt_central_scanning, &central_state_change_work);
	}
	if (bt_central_connected) {
		ztacx_variable_ptr_set_onchange(bt_central_connected, &central_state_change_work);
	}
	if (bt_central_subscribe16_value) {
		ztacx_variable_ptr_set_onchange(bt_central_subscribe16_value, &level_change_work);
	}

	LOG_INF("setting RGB led to RED");
	ztacx_variable_value_set_string(led_strip_color, "red");
#endif
	
	LOG_INF("NOTICE <READY app");
	return 0;
}

#if CONFIG_ZTACX_LEAF_IMS
static struct k_work x_change_work;

void x_change(struct k_work *work) 
{
	int64_t count = ztacx_variable_value_get_int64(ims_samples);
	int32_t level = ztacx_variable_value_get_int32(ims_level_x);
	int16_t value = level;
	
#if CONFIG_ZTACX_LEAF_BT_PERIPHERAL
	LOG_INF("Notify X change %d (sample count=%lld)", (int)level, (long long)count);
	//bt_gatt_notify(NULL, x_tilt_attr, &level, sizeof(level));
	ztacx_variable_value_set_int16(expression_value, value);
	bt_gatt_notify(NULL, expression_value_attr, &value, sizeof(value));
#endif	
}
#endif

static int app_start(void) 
{
	LOG_INF("NOTICE >START app %s v%s", CONFIG_BT_DEVICE_NAME, CONFIG_MCUBOOT_IMGTOOL_SIGN_VERSION);
#if CONFIG_ZTACX_LEAF_BT_PERIPHERAL
	LOG_INF("Registering advertising data");
	int bt_err=0;
	//bt_err = ztacx_bt_adv_register(bt_adv_data_srv, ARRAY_SIZE(bt_adv_data_src), bt_adv_scanresp_name, ARRAY_SIZE(bt_adv_scanresp_name));
	bt_err = ztacx_bt_adv_register(bt_adv_data_srv, ARRAY_SIZE(bt_adv_data_srv), NULL, 0);
	//bt_err = ztacx_bt_adv_register(bt_adv_data_name, ARRAY_SIZE(bt_adv_data_name), NULL, 0);
	if (bt_err != 0) {
		LOG_ERR("Bluetooth advertising register failed (err %d)", bt_err);
	}
#else
	LOG_INF("BT peripheral leaf is disabled");
#endif

#if CONFIG_ZTACX_LEAF_IMS
	LOG_INF("set trigger for IMS change");
	if (ims_level_x) {
		k_work_init(&x_change_work, x_change);
		ztacx_variable_ptr_set_onchange(ims_level_x, &x_change_work);
	}
#endif
#if CONFIG_SHELL
	static char prompt[32];
	snprintf(prompt, sizeof(prompt), "%s %s:~$ ", CONFIG_BT_DEVICE_NAME, CONFIG_MCUBOOT_IMGTOOL_SIGN_VERSION);
	if (shell_prompt_change(shell_backend_uart_get_ptr(), prompt) !=0) {
		LOG_ERR("Unable to change shell prompt\n");
	}
#endif

#if CONFIG_APP_ROLE_RECEIVER
	k_work_schedule(&central_request_connect_work, K_MSEC(250));
#endif

	LOG_INF("NOTICE <STARTED app");
	return 0;
}


SYS_INIT(app_init, APPLICATION, ZTACX_APP_INIT_PRIORITY);
SYS_INIT(app_start, APPLICATION, ZTACX_APP_START_PRIORITY);


int main(void)
{
	printk("Accelerando Bluetooth Whammy Pedal (%s)\n", role_str());
}
