/*
 * Copyright (c) 2026 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>
#include <service_app.h>
#include <notification.h>
#include <notification_list.h>
#include <dlog.h>

#undef LOG_TAG
#define LOG_TAG "NOTI_LIST_SVC"

#define STR(s) ((s) ? (s) : "(null)")

static const char *__type_to_str(notification_type_e type)
{
	switch (type) {
	case NOTIFICATION_TYPE_NOTI:
		return "NOTI";
	case NOTIFICATION_TYPE_ONGOING:
		return "ONGOING";
	case NOTIFICATION_TYPE_NONE:
		return "NONE";
	default:
		return "UNKNOWN";
	}
}

static void __print_notification(int idx, notification_h noti)
{
	int priv_id = -1;
	int group_id = -1;
	time_t insert_time = 0;
	char *pkgname = NULL;
	char *title = NULL;
	char *content = NULL;
	notification_type_e type = NOTIFICATION_TYPE_NONE;

	notification_get_id(noti, &group_id, &priv_id);
	notification_get_type(noti, &type);
	notification_get_pkgname(noti, &pkgname);
	notification_get_insert_time(noti, &insert_time);
	notification_get_text(noti, NOTIFICATION_TEXT_TYPE_TITLE, &title);
	notification_get_text(noti, NOTIFICATION_TEXT_TYPE_CONTENT, &content);

	dlog_print(DLOG_INFO, LOG_TAG,
			"[%d] priv_id(%d) group_id(%d) type(%s) pkg(%s) insert_time(%ld) title(%s) content(%s)",
			idx, priv_id, group_id, __type_to_str(type), STR(pkgname),
			(long)insert_time, STR(title), STR(content));
}

static void __dump_notification_list(void)
{
	notification_list_h list = NULL;
	notification_list_h iter;
	notification_h noti;
	int count = 0;
	int ret;

	ret = notification_get_list(NOTIFICATION_TYPE_NONE, -1, &list);
	if (ret != NOTIFICATION_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG,
				"notification_get_list() failed: %d (%s)",
				ret, get_error_message(ret));
		return;
	}
	dlog_print(DLOG_INFO, LOG_TAG, "notification_get_list() success");

	for (iter = notification_list_get_head(list); iter;
			iter = notification_list_get_next(iter)) {
		noti = notification_list_get_data(iter);
		if (noti == NULL)
			continue;

		__print_notification(count, noti);
		count++;
	}

	dlog_print(DLOG_INFO, LOG_TAG, "Total notification count: %d", count);

	if (list)
		notification_free_list(list);
}

static bool __app_create(void *data)
{
	dlog_print(DLOG_INFO, LOG_TAG, "service app create");
	__dump_notification_list();
	return true;
}

static void __app_control(app_control_h app_control, void *data)
{
	dlog_print(DLOG_INFO, LOG_TAG, "service app control");
}

static void __app_terminate(void *data)
{
	dlog_print(DLOG_INFO, LOG_TAG, "service app terminate");
}

int main(int argc, char *argv[])
{
	service_app_lifecycle_callback_s event_callback = {
		.create = __app_create,
		.terminate = __app_terminate,
		.app_control = __app_control,
	};

	return service_app_main(argc, argv, &event_callback, NULL);
}
