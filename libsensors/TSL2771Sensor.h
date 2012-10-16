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

#ifndef ANDROID_TSL2771_SENSOR_H
#define ANDROID_TSL2771_SENSOR_H

#include <stdint.h>
#include <errno.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#include "sensors.h"
#include "SensorBase.h"
#include "InputEventReader.h"

#define PROX_INPUT_NAME    "tsl2771_prox"
#define ALS_INPUT_NAME     "tsl2771_als"

#define PROX_NEAR_LEN 2.0f
#define PROX_FAR_LEN 8.0f
#define PROX_NEAR_THRESHOLD 0x200

/*****************************************************************************/

struct input_event;

class TSL2771Sensor : public SensorBase {
public:
            TSL2771Sensor(const char *name);
    virtual ~TSL2771Sensor();

    enum {
        light   = 0,
        proximity = 1,
        numSensors
    };

    int setInitialState();
    float mPreviousLight;
    float indexToValue(size_t index) const;

    virtual int readEvents(sensors_event_t* data, int count);
    virtual bool hasPendingEvents() const;
    virtual int setDelay(int32_t handle, int64_t ns);
    virtual int enable(int32_t handle, int enabled);

private:
    int mEnabled;
    uint32_t mPendingMask;
    InputEventCircularReader mInputReader;
    sensors_event_t mPendingEvents[numSensors];
    bool mHasPendingEvent;
    char input_sysfs_path[PATH_MAX];
    int input_sysfs_path_len;

};

/*****************************************************************************/

#endif  // ANDROID_TSL2771_SENSOR_H
