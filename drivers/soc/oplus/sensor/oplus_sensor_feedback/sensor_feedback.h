// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2018-2020 Oplus. All rights reserved.
 */


#ifndef __SENSOR_FEEDBACK_H__
#define __SENSOR_FEEDBACK_H__

#include <linux/miscdevice.h>
#ifdef CONFIG_ARM
#include <linux/sched.h>
#else
#include <linux/wait.h>
#endif
#include <linux/spinlock.h>
#include <linux/workqueue.h>
#include <linux/kobject.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/param.h>
#include <linux/proc_fs.h>


#ifdef CONFIG_DRM_MSM
#include <linux/msm_drm_notify.h>
#endif

#ifdef CONFIG_FB
#include <linux/fb.h>
#include <linux/notifier.h>
#endif
#include <linux/time.h>


#define THREAD_WAKEUP  0
#define THREAD_SLEEP   1



enum sensor_fb_event_id {
	FD_HEAD_EVENT_ID = 0,
	//1~100
	PS_INIT_FAIL_ID = 1,
	PS_I2C_ERR_ID = 2,
	PS_ALLOC_FAIL_ID = 3,
	PS_ESD_REST_ID = 4,
	PS_NO_INTERRUPT_ID = 5,
	PS_FIRST_REPORT_DELAY_COUNT_ID = 6,
	PS_ORIGIN_DATA_TO_ZERO_ID = 7,

	//100~200
	ALS_INIT_FAIL_ID = 100,
	ALS_I2C_ERR_ID = 101,
	ALS_ALLOC_FAIL_ID = 102,
	ALS_ESD_REST_ID = 103,
	ALS_NO_INTERRUPT_ID = 104,
	ALS_FIRST_REPORT_DELAY_COUNT_ID = 105,
	ALS_ORIGIN_DATA_TO_ZERO_ID = 106,


	//200~300
	ACCEL_INIT_FAIL_ID = 200,
	ACCEL_I2C_ERR_ID = 201,
	ACCEL_ALLOC_FAIL_ID = 202,
	ACCEL_ESD_REST_ID = 203,
	ACCEL_NO_INTERRUPT_ID = 204,
	ACCEL_FIRST_REPORT_DELAY_COUNT_ID = 205,
	ACCEL_ORIGIN_DATA_TO_ZERO_ID = 206,

	//300~400
	GYRO_INIT_FAIL_ID = 300,
	GYRO_I2C_ERR_ID = 301,
	GYRO_ALLOC_FAIL_ID = 302,
	GYRO_ESD_REST_ID = 303,
	GYRO_NO_INTERRUPT_ID = 304,
	GYRO_FIRST_REPORT_DELAY_COUNT_ID = 305,
	GYRO_ORIGIN_DATA_TO_ZERO_ID = 306,

	//300~400
	MAG_INIT_FAIL_ID = 400,
	MAG_I2C_ERR_ID = 401,
	MAG_ALLOC_FAIL_ID = 402,
	MAG_ESD_REST_ID = 403,
	MAG_NO_INTERRUPT_ID = 404,
	MAG_FIRST_REPORT_DELAY_COUNT_ID = 405,
	MAG_ORIGIN_DATA_TO_ZERO_ID = 406,

	//600~700
	POWER_SENSOR_INFO_ID = 600,
	POWER_ACCEL_INFO_ID = 601,
	POWER_GYRO_INFO_ID = 602,
	POWER_MAG_INFO_ID = 603,
	POWER_PROXIMITY_INFO_ID = 604,
	POWER_LIGHT_INFO_ID = 605,
	POWER_WISE_LIGHT_INFO_ID = 606,
	POWER_WAKE_UP_RATE_ID = 607,
	POWER_ADSP_SLEEP_RATIO_ID = 608,


	//1000
	ALAILABLE_SENSOR_LIST_ID = 1000,
	ALAILABLE_SENSOR_NAME_ID = 1001,

	// 10000 , sensor-hal
	HAL_SENSOR_NOT_FOUND = 10000,
	HAL_QMI_ERROR = 10001,
	HAL_SENSOR_TIMESTAMP_ERROR = 10002,
};


struct fd_data {
	int data_x;
	int data_y;
	int data_z;
};

#define EVNET_DATA_LEN 3
struct sns_fb_event {
	unsigned short event_id;
	unsigned int count;
	union {
		int buff[EVNET_DATA_LEN];
		struct fd_data data;
	};
};


#define EVNET_NUM_MAX 109
struct fb_event_smem {
	struct sns_fb_event event[EVNET_NUM_MAX];
};


struct sensor_fb_conf {
	uint16_t event_id;
	char *fb_field;
	char *fb_event_id;
};


struct sensor_fb_cxt {
	/*struct miscdevice sensor_fb_dev;*/
	struct platform_device *sensor_fb_dev;
	spinlock_t   rw_lock;
	wait_queue_head_t wq;
	struct notifier_block fb_notif;
	uint64_t start_time;
	uint64_t end_time;
	uint64_t sleep_time;
	uint64_t sleep_ratio;
	uint64_t last_accumulated_duration;
	int master_id;
	int smem_id;
	struct task_struct *report_task; /*kernel thread*/
	uint16_t adsp_event_counts;
	struct fb_event_smem fb_smem;
	uint16_t node_type;
	unsigned long wakeup_flag;
	uint32_t sensor_list[2];
	struct proc_dir_entry  *proc_sns;
	uint32_t gyro_name_index;
};
#endif /*__SENSOR_FEEDBACK_H__*/

void set_sensor_crash_cause(char *str);
