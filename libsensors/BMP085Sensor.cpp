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
#include <math.h>
#include <poll.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/select.h>

#include <cutils/log.h>

#include "BMP085Sensor.h"

/*****************************************************************************/

BMP085Sensor::BMP085Sensor()
    : SensorBase(NULL, "bmp085"),
      mEnabled(0),
      mPendingMask(0),
      mInputReader(32),
      mHasPendingEvent(false)
{
    mPendingEvents[Pressure].version = sizeof(sensors_event_t);
    mPendingEvents[Pressure].sensor = ID_PRESS;
    mPendingEvents[Pressure].type = SENSOR_TYPE_PRESSURE;

    mPendingEvents[Temperature].version = sizeof(sensors_event_t);
    mPendingEvents[Temperature].sensor = ID_TEMP;
    mPendingEvents[Temperature].type = SENSOR_TYPE_TEMPERATURE;

    if (data_fd) {
        strcpy(input_sysfs_path, "/sys/bus/i2c/drivers/bmp085/4-0077/");
        input_sysfs_path_len = strlen(input_sysfs_path);
    }
}

BMP085Sensor::~BMP085Sensor() {
    if (mEnabled) {
        enable(ID_PRESS, 0);
        enable(ID_TEMP, 0);
    }
}

int BMP085Sensor::enable(int32_t handle, int en) {
    int flags = en ? 1 : 0;
    int fd;

    strcpy(&input_sysfs_path[input_sysfs_path_len], "enable");
    fd = open(input_sysfs_path, O_RDWR);
    if (fd >= 0) {
        if (handle == ID_PRESS) {
            if (mEnabled & 0x2)
                flags = flags ? 3 : 2;
            else
                flags = flags ? 1 : 0;
        }
        if (handle == ID_TEMP) {
            if (mEnabled & 0x1)
                flags = flags ? 3 : 1;
            else
                flags = flags ? 2 : 0;
        }
        char buffer[2];
        snprintf(buffer, 2, "%d", flags);
        write(fd, buffer, sizeof(buffer));
        close(fd);
        mEnabled = flags;
        return 0;
    }
    return 0;
}

bool BMP085Sensor::hasPendingEvents() const {
    return mHasPendingEvent;
}

int BMP085Sensor::readEvents(sensors_event_t* data, int count)
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
        if (type == EV_ABS) {
            if (event->code == EVENT_TYPE_PRESSURE) {
                    ALOGD("BMP085: Got pressure value %d\n", event->value);
                    mPendingEvents[Pressure].pressure = event->value * CONVERT_PRESS;
                    mPendingMask |= 1 << Pressure;
                    mInputReader.next();
            } else if (event->code == EVENT_TYPE_TEMPERATURE) {
                    ALOGD("BMP085: Got temperature value %d\n", event->value);
                    mPendingEvents[Temperature].temperature = event->value * CONVERT_TEMP;
                    mPendingMask |= 1 << Temperature;
                    mInputReader.next();
            }
        } else if (type == EV_SYN) {
            int64_t time = timevalToNano(event->time);
            for (int j=0 ; count && mPendingMask && j<numSensors ; j++) {
              if (mPendingMask & (1<<j)) {
                    mPendingMask &= ~(1<<j);
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
            ALOGE("BMP085Sensor: unknown event (type=%d, code=%d)",
                    type, event->code);
        }
    }

    return numEventReceived;
}
