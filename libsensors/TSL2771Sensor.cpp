/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <fcntl.h>
#include <errno.h>
#include <poll.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/select.h>
#include <cutils/log.h>

#include "TSL2771Sensor.h"

#define DEBUG_TSL2771 0

/*****************************************************************************/

TSL2771Sensor::TSL2771Sensor(const char *name)
    : SensorBase(NULL, name),
      mEnabled(0),
      mPendingMask(0),
      mInputReader(32),
      mHasPendingEvent(false)

{
    mPendingEvents[light].version = sizeof(sensors_event_t);
    mPendingEvents[light].sensor = ID_L;
    mPendingEvents[light].type = SENSOR_TYPE_LIGHT;

    mPendingEvents[proximity].version = sizeof(sensors_event_t);
    mPendingEvents[proximity].sensor = ID_P;
    mPendingEvents[proximity].type = SENSOR_TYPE_PROXIMITY;

    if (data_fd) {
        strcpy(input_sysfs_path, "/sys/bus/i2c/drivers/tsl2771/4-0039/");
        input_sysfs_path_len = strlen(input_sysfs_path);
    }

    if (strcmp(name, ALS_INPUT_NAME)) {
        enable(ID_L, 1);
    }
}

TSL2771Sensor::~TSL2771Sensor() {
    if (mEnabled) {
        enable(ID_L, 0);
        enable(ID_P, 0);
    }
}

int TSL2771Sensor::setDelay(int32_t handle, int64_t ns)
{
    return 0;
}

int TSL2771Sensor::enable(int32_t handle, int en)
{
    int flags = en ? 1 : 0;
    int als_fd = -1;
    int prox_fd = -1;

    if (handle == ID_L) {
        strcpy(&input_sysfs_path[input_sysfs_path_len], "als_enable");
        als_fd = open(input_sysfs_path, O_RDWR);
        if (als_fd >= 0) {
            char buffer[2];
            snprintf(buffer, 2, "%d", flags);
            write(als_fd, buffer, sizeof(buffer));
            close(als_fd);
            mEnabled = flags << light;
        } else {
            return -1;
        }
    } else if (handle == ID_P) {
        strcpy(&input_sysfs_path[input_sysfs_path_len], "prox_enable");
        prox_fd = open(input_sysfs_path, O_RDWR);
        if (prox_fd >= 0) {
            char buffer[2];
            snprintf(buffer, 2, "%d", flags);
            write(prox_fd, buffer, sizeof(buffer));
            close(prox_fd);
            mEnabled = flags << proximity;
        } else {
            return -1;
        }
    } else {
        return -1;
    }

    return 0;
}

bool TSL2771Sensor::hasPendingEvents() const {
    return mHasPendingEvent;
}

float TSL2771Sensor::indexToValue(size_t index) const
{
    return index > PROX_NEAR_THRESHOLD ? PROX_NEAR_LEN : PROX_FAR_LEN;
}

int TSL2771Sensor::readEvents(sensors_event_t* data, int count)
{
    if (count < 1)
        return -EINVAL;

    ssize_t n = mInputReader.fill(data_fd);
    if (n < 0)
        return n;

    int numEventReceived = 0;
    input_event const* event;

    while (count && mInputReader.readEvent(&event)) {
        int type = event->type;
        if (type == EV_LED) {
            if (event->code == EVENT_TYPE_LIGHT) {
                mPendingEvents[light].light = event->value;
                mPendingMask |= 1 << light;
                mInputReader.next();
#ifdef DEBUG_TSL2771
                ALOGD("TSL2771Sensor: Received LUX value=%d", event->value);
#endif
            }
        } else if (type == EV_ABS) {
            if (event->code == EVENT_TYPE_PROXIMITY) {
                mPendingEvents[proximity].distance = indexToValue(event->value);
                mPendingMask |= 1 << proximity;
                mInputReader.next();
#ifdef DEBUG_TSL2771
                ALOGD("TSL2771Sensor: Received Proximity value=%d", event->value);
#endif
            }
        } else if (type == EV_SYN) {
           int64_t time = timevalToNano(event->time);
            for (int j=0 ; count && mPendingMask && j<numSensors ; j++) {
              if (mPendingMask & (1 << j)) {
                    mPendingMask &= ~(1 << j);
                    mPendingEvents[j].timestamp = time;
                    *data++ = mPendingEvents[j];
                        count--;
                        numEventReceived++;
              }
            }
            if (!mPendingMask) {
                mInputReader.next();
            }
        } else {
            ALOGE("TSL2771Sensor: unknown event (type=%d, code=%d)",
                    type, event->code);
        }
    }

    return numEventReceived;
}
