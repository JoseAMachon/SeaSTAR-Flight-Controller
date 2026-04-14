/* 
 * File:   TelemetryStream.h
 * Author: Jose Machon
 *
 * Created on April 1, 2026, 9:24 AM
 */

#ifndef TELEMETRYSTREAM_H
#define TELEMETRYSTREAM_H

#include "main.h"
#include "stdint.h"
#include <stdint.h>

typedef struct {
    int16_t temp;
    int32_t depth;
    int32_t pressure;
} environmental_packet_t;

typedef struct  {
    int32_t yaw;
    int32_t pitch;
    int32_t roll;
} positional_packet_t;

void TelemetryStream_Init(void);

void TelemetryStream_SendEnvironmental(void);

void TelemetryStream_SendOrientation(void);

#endif