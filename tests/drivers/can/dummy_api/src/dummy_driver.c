/*
 * Copyright (c) 2019 Alexander Wachter
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <drivers/can.h>
#include <ztest.h>
#include <strings.h>
#include <device.h>

#define DUMMY_DRIVER_NAME "can_dev"

const struct device *can_dev;

static void test_can_send(void)
{
	int ret;
	ret = can_send(can_dev, NULL, K_MSEC(0), NULL, NULL);
	zassert_not_equal(ret, 0, "Dummy can send succeeded");
}

static void test_can_attach_msgq(void)
{
	int ret;
	ret = can_attach_msgq(can_dev, NULL, NULL);
	zassert_equal(ret, CAN_NO_FREE_FILTER, "No filter left");
}

static void test_can_detach(void)
{
	int filter_id;
	filter_id = can_attach_msgq(can_dev, NULL, NULL);
	can_detach(can_dev, filter_id);
}

static void test_can_configure(void)
{
	int ret;
	ret = can_configure(can_dev, CAN_NORMAL_MODE, 0);
	zassert_not_equal(ret, 0, "Dummy can configure succeeded");
}

static void test_can_get_state(void)
{
	enum can_state ret; 
	ret = can_get_state(can_dev, NULL);
	zassert_not_equal(ret, CAN_BUS_UNKNOWN, "Unknown state");
}

static void test_can_recover(void)
{
	int ret;
	ret = can_recover(can_dev, K_MSEC(0));
	zassert_equal(ret, CAN_TIMEOUT, "Not timeout");
}

int dummy_init(const struct device *dev)
{
	return 0;
}

DEVICE_AND_API_INIT(can_dev, DUMMY_DRIVER_NAME, &dummy_init,
		NULL, NULL, APPLICATION, 
		CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, NULL);

void test_main(void)
{
	can_dev = device_get_binding(DUMMY_DRIVER_NAME);
	zassert_not_null(can_dev, "Device not found");

	ztest_test_suite(dummy_can_driver,
			ztest_user_unit_test(test_can_send),
			ztest_user_unit_test(test_can_attach_msgq),
			ztest_user_unit_test(test_can_detach),
			ztest_user_unit_test(test_can_configure),
			ztest_user_unit_test(test_can_get_state),
			ztest_user_unit_test(test_can_recover));
	ztest_run_test_suite(dummy_can_driver);
}
