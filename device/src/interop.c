/******************************************************************************
 *
 *  Copyright (C) 2015 Google, Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#define LOG_TAG "bt_device_interop"

#include <assert.h>
#include <string.h> // For memcmp

#include "device/include/interop.h"
#include "device/include/interop_database.h"
#include "osi/include/log.h"

#define CASE_RETURN_STR(const) case const: return #const;

static const char* interop_feature_string(const interop_feature_t feature) {
  switch (feature) {
    CASE_RETURN_STR(INTEROP_DISABLE_LE_SECURE_CONNECTIONS)
    CASE_RETURN_STR(INTEROP_AUTO_RETRY_PAIRING)
    CASE_RETURN_STR(INTEROP_DISABLE_SDP_AFTER_PAIRING)
    CASE_RETURN_STR(INTEROP_DISABLE_AUTH_FOR_HID_POINTING)
    CASE_RETURN_STR(INTEROP_DISABLE_SNIFF_DURING_SCO)
    CASE_RETURN_STR(INTEROP_INCREASE_AG_CONN_TIMEOUT)
    CASE_RETURN_STR(INTEROP_DISABLE_ABSOLUTE_VOLUME)
  }

  return "UNKNOWN";
}

// Interface functions

bool interop_addr_match(const interop_feature_t feature, const bt_bdaddr_t *addr) {
  assert(addr);

  const size_t db_size = sizeof(interop_addr_database) / sizeof(interop_addr_t);

  for (size_t i = 0; i != db_size; ++i) {
    if (feature == interop_addr_database[i].feature &&
        memcmp(addr, &interop_addr_database[i].addr, interop_addr_database[i].len) == 0) {
      char bdstr[20] = {0};
      LOG_WARN("%s() Device %s is a match for interop addr workaround %s", __func__,
          bdaddr_to_string(addr, bdstr, sizeof(bdstr)), interop_feature_string(feature));
      return true;
    }
  }

  return false;
}

bool interop_name_match(const interop_feature_t feature, const char *name) {
  assert(name);

  const size_t db_size = sizeof(interop_name_database) / sizeof(interop_name_t);

  for (size_t i = 0; i != db_size; ++i) {
    if (feature == interop_name_database[i].feature &&
        strncmp(name, interop_name_database[i].name, strlen(name)) == 0) {
      char bdstr[20] = {0};
      LOG_WARN("%s() Device with name: %s is a match for interop name workaround %s", __func__,
          name, interop_feature_string(feature));
      return true;
    }
  }

  return false;
}

bool interop_manufacturer_match(const interop_feature_t feature, uint16_t manufacturer) {
  assert(manufacturer);

  const size_t db_size = sizeof(interop_manufctr_database) / sizeof(interop_manufacturer_t);

  for (size_t i = 0; i != db_size; ++i) {
    if (feature == interop_manufctr_database[i].feature &&
        manufacturer == interop_manufctr_database[i].manufacturer) {
      char bdstr[20] = {0};
    LOG_WARN("%s() Device with manufacturer id: %d is a match for interop manufacturer "
        "workaround %s", __func__, manufacturer, interop_feature_string(feature));
      return true;
    }
  }

  return false;
}

