/*******************************************************************************
* Project Name: ble_beacon_demo
*
* Version: 1.0
*
* Description:
* In this project zephyr, ble uses beacon example
* 
*
********************************************************************************
* Copyright (2022-23) , LearnZephyr
*******************************************************************************/

/* include headers */
#include <zephyr.h>
#include <logging/log.h>
#include <sys/util.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>


#define BT_DEVICE_NAME		CONFIG_BT_DEVICE_NAME
#define BT_DEVICE_NAME_LEN	(sizeof(BT_DEVICE_NAME)-1)

/* register the log level */
LOG_MODULE_REGISTER(becon,LOG_LEVEL_DBG);

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR),
	BT_DATA_BYTES(BT_DATA_UUID16_ALL, 0xaa, 0xfe),
	BT_DATA_BYTES(BT_DATA_SVC_DATA16,
				0xaa,0xfe,
				0x10,
				0x00,
				'z','e','p','h','y','r',
				'p','r','o','j','e','t',
				0x08)
};

/* set response data */
static const struct bt_data sd[] = {
	BT_DATA(BT_DATA_NAME_COMPLETE,BT_DEVICE_NAME,BT_DEVICE_NAME_LEN),
};

static void bt_ready(int err)
{
	char addr_s[BT_ADDR_LE_STR_LEN];
	bt_addr_le_t addr;
	size_t count;

	if(err)
	{
		LOG_ERR("bluetooth init err code %d",err);
		return;
	}

	LOG_INF("bluetooth init done");

	err = bt_le_adv_start(BT_LE_ADV_NCONN_IDENTITY,ad,ARRAY_SIZE(ad),
				sd,ARRAY_SIZE(sd));

	if(err)
	{
		LOG_ERR("Advertising start to fail err:%d",err);
		return;
	}

	bt_id_get(&addr,&count);
	bt_addr_le_to_str(&addr,addr_s,sizeof(addr_s));

	LOG_INF("BLE beacon started advertising as %s\n",addr_s);
}

void main(void)
{
	int err;

	k_sleep(K_SECONDS(1));

	LOG_INF("ble beacon demo");

	err = bt_enable(bt_ready);

	if(err)
	{
		LOG_ERR("bt init failed err;%d",err);
	}
}
