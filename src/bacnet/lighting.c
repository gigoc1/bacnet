/*####COPYRIGHTBEGIN####
 -------------------------------------------
 Copyright (C) 2013 Steve Karg <skarg@users.sourceforge.net>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to:
 The Free Software Foundation, Inc.
 59 Temple Place - Suite 330
 Boston, MA  02111-1307, USA.

 As a special exception, if other files instantiate templates or
 use macros or inline functions from this file, or you compile
 this file and link it with other works to produce a work based
 on this file, this file does not by itself cause the resulting
 work to be covered by the GNU General Public License. However
 the source code for this file must still be made available in
 accordance with section (3) of the GNU General Public License.

 This exception does not invalidate any other reasons why a work
 based on this file might be covered by the GNU General Public
 License.
 -------------------------------------------
####COPYRIGHTEND####*/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "bacnet/lighting.h"
#include "bacnet/bacdcode.h"

#ifndef islessgreater
#define islessgreater( x, y) ((x) < (y) || (x) > (y))
#endif

/** @file lighting.c  Manipulate BACnet lighting command values */

/**
 * Encodes into bytes from the lighting-command structure
 *
 * @param apdu - buffer to hold the bytes, or null for length
 * @param value - lighting command value to encode
 *
 * @return  number of bytes encoded
 */
int lighting_command_encode(uint8_t *apdu, BACNET_LIGHTING_COMMAND *data)
{
    int apdu_len = 0; /* total length of the apdu, return value */
    int len = 0; /* total length of the apdu, return value */
    uint8_t *apdu_offset = NULL;

    len = encode_context_enumerated(apdu, 0, data->operation);
    apdu_len += len;
    /* optional target-level */
    if (data->use_target_level) {
        if (apdu) {
            apdu_offset = &apdu[apdu_len];
        }
        len = encode_context_real(apdu_offset, 1, data->target_level);
        apdu_len += len;
    }
    /* optional ramp-rate */
    if (data->use_ramp_rate) {
        if (apdu) {
            apdu_offset = &apdu[apdu_len];
        }
        len = encode_context_real(apdu_offset, 2, data->ramp_rate);
        apdu_len += len;
    }
    /* optional step increment */
    if (data->use_step_increment) {
        if (apdu) {
            apdu_offset = &apdu[apdu_len];
        }
        len = encode_context_real(apdu_offset, 3, data->step_increment);
        apdu_len += len;
    }
    /* optional fade time */
    if (data->use_fade_time) {
        if (apdu) {
            apdu_offset = &apdu[apdu_len];
        }
        len = encode_context_unsigned(apdu_offset, 4, data->fade_time);
        apdu_len += len;
    }
    /* optional priority */
    if (data->use_priority) {
        if (apdu) {
            apdu_offset = &apdu[apdu_len];
        }
        len = encode_context_unsigned(apdu_offset, 5, data->priority);
        apdu_len += len;
    }

    return apdu_len;
}

/**
 * Encodes into bytes from the lighting-command structure
 * a context tagged chunk (opening and closing tag)
 *
 * @param apdu - buffer to hold the bytes
 * @param tag_number - tag number to encode this chunk
 * @param value - lighting command value to encode
 *
 * @return  number of bytes encoded, or 0 if unable to encode.
 */
int lighting_command_encode_context(
    uint8_t *apdu, uint8_t tag_number, BACNET_LIGHTING_COMMAND *value)
{
    int apdu_len = 0;
    uint8_t *apdu_offset = NULL;

    apdu_len += encode_opening_tag(apdu, tag_number);
    if (apdu) {
        apdu_offset = &apdu[apdu_len];
    }
    apdu_len += lighting_command_encode(apdu_offset, value);
    if (apdu) {
        apdu_offset = &apdu[apdu_len];
    }
    apdu_len += encode_closing_tag(apdu_offset, tag_number);

    return apdu_len;
}

/**
 * Decodes from bytes into the lighting-command structure
 *
 * @param apdu - buffer to hold the bytes
 * @param apdu_max_len - number of bytes in the buffer to decode
 * @param value - lighting command value to place the decoded values
 *
 * @return  number of bytes encoded
 */
int lighting_command_decode(
    uint8_t *apdu, unsigned apdu_max_len, BACNET_LIGHTING_COMMAND *data)
{
    int len = 0;
    int apdu_len = 0;
    uint8_t tag_number = 0;
    uint32_t len_value_type = 0;
    uint32_t enum_value = 0;
    BACNET_UNSIGNED_INTEGER unsigned_value = 0;
    float real_value = 0.0;

    (void)apdu_max_len;
    /* check for value pointers */
    if (apdu_max_len && data) {
        /* Tag 0: operation */
        if (!decode_is_context_tag(&apdu[apdu_len], 0)) {
            return BACNET_STATUS_ERROR;
        }
        len = decode_tag_number_and_value(
            &apdu[apdu_len], &tag_number, &len_value_type);
        apdu_len += len;
        len = decode_enumerated(&apdu[apdu_len], len_value_type, &enum_value);
        if (len > 0) {
            if (unsigned_value <= BACNET_LIGHTS_PROPRIETARY_LAST) {
                data->operation = (BACNET_LIGHTING_OPERATION)enum_value;
            } else {
                return BACNET_STATUS_ERROR;
            }
        }
        apdu_len += len;
        /* Tag 1: target-level - OPTIONAL */
        if (decode_is_context_tag(&apdu[apdu_len], 1)) {
            len = decode_tag_number_and_value(
                &apdu[apdu_len], &tag_number, &len_value_type);
            apdu_len += len;
            len = decode_real(&apdu[apdu_len], &real_value);
            data->target_level = real_value;
            apdu_len += len;
            data->use_target_level = true;
        } else {
            data->use_target_level = false;
        }
        /* Tag 2: ramp-rate - OPTIONAL */
        if (decode_is_context_tag(&apdu[apdu_len], 2)) {
            len = decode_tag_number_and_value(
                &apdu[apdu_len], &tag_number, &len_value_type);
            apdu_len += len;
            len = decode_real(&apdu[apdu_len], &real_value);
            data->ramp_rate = real_value;
            data->use_ramp_rate = true;
        } else {
            data->use_ramp_rate = false;
        }
        /* Tag 3: step-increment - OPTIONAL */
        if (decode_is_context_tag(&apdu[apdu_len], 3)) {
            len = decode_tag_number_and_value(
                &apdu[apdu_len], &tag_number, &len_value_type);
            apdu_len += len;
            len = decode_real(&apdu[apdu_len], &real_value);
            data->step_increment = real_value;
            data->use_step_increment = true;
        } else {
            data->use_step_increment = false;
        }
        /* Tag 4: fade-time - OPTIONAL */
        if (decode_is_context_tag(&apdu[apdu_len], 4)) {
            len = decode_tag_number_and_value(
                &apdu[apdu_len], &tag_number, &len_value_type);
            apdu_len += len;
            len = decode_unsigned(
                &apdu[apdu_len], len_value_type, &unsigned_value);
            data->fade_time = (uint32_t)unsigned_value;
            data->use_fade_time = true;
        } else {
            data->use_fade_time = false;
        }
        /* Tag 5: priority - OPTIONAL */
        if (decode_is_context_tag(&apdu[apdu_len], 4)) {
            len = decode_tag_number_and_value(
                &apdu[apdu_len], &tag_number, &len_value_type);
            apdu_len += len;
            len = decode_unsigned(
                &apdu[apdu_len], len_value_type, &unsigned_value);
            data->priority = (uint8_t)unsigned_value;
            data->use_priority = true;
        } else {
            data->use_priority = false;
        }
    }

    return len;
}

/**
 * Copies one lighting-command structure to another
 *
 * @param dst - lighting command value target
 * @param src - lighting command value source
 *
 * @return  true if copy succeeded
 */
bool lighting_command_copy(
    BACNET_LIGHTING_COMMAND *dst, BACNET_LIGHTING_COMMAND *src)
{
    bool status = false;

    if (dst && src) {
        dst->operation = src->operation;
        dst->use_target_level = src->use_target_level;
        dst->use_ramp_rate = src->use_ramp_rate;
        dst->use_step_increment = src->use_step_increment;
        dst->use_fade_time = src->use_fade_time;
        dst->use_priority = src->use_priority;
        dst->target_level = src->target_level;
        dst->ramp_rate = src->ramp_rate;
        dst->step_increment = src->step_increment;
        dst->fade_time = src->fade_time;
        dst->priority = src->priority;
        status = true;
    }

    return status;
}

/**
 * Compare one lighting-command structure to another
 *
 * @param dst - lighting command value target
 * @param src - lighting command value source
 *
 * @return  true if lighting-commands are the same for values in-use
 */
bool lighting_command_same(
    BACNET_LIGHTING_COMMAND *dst, BACNET_LIGHTING_COMMAND *src)
{
    bool status = false;

    if (dst && src) {
        if ((dst->operation == src->operation) &&
            (dst->use_target_level == src->use_target_level) &&
            (dst->use_ramp_rate == src->use_ramp_rate) &&
            (dst->use_step_increment == src->use_step_increment) &&
            (dst->use_fade_time == src->use_fade_time) &&
            (dst->use_priority == src->use_priority)) {
            status = true;
            if ((dst->use_target_level) &&
                islessgreater(dst->target_level, src->target_level)) {
                status = false;
            }
            if ((dst->use_ramp_rate) &&
                islessgreater(dst->ramp_rate, src->ramp_rate)) {
                status = false;
            }
            if ((dst->use_step_increment) &&
                islessgreater(dst->step_increment, src->step_increment)) {
                status = false;
            }
            if ((dst->use_fade_time) && (dst->fade_time != src->fade_time)) {
                status = false;
            }
            if ((dst->use_priority) && (dst->priority != src->priority)) {
                status = false;
            }
        }
    }

    return status;
}
