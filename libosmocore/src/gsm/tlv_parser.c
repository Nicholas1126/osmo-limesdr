/* (C) 2008-2017 by Harald Welte <laforge@gnumonks.org>
 * (C) 2016-2017 by sysmocom - s.f.m.c. GmbH
 *
 * All Rights Reserved
 *
 * SPDX-License-Identifier: GPL-2.0+
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <osmocom/core/utils.h>
#include <osmocom/gsm/tlv.h>

/*! \addtogroup tlv
 *  @{
 *  Osmocom TLV Parser
 *
 *  The Osmocom TLV parser is intended to operate as a low-level C
 *  implementation without dynamic memory allocations.  Basically, it
 *  iterates over the IE (Information Elements) of the message and fills
 *  an array of pointers, indexed by the IEI (IE Identifier).  The
 *  parser output is thus an array of pointers to the start of the
 *  respective IE inside the message.
 *
 *  The TLV parser is configured by a TLV parser definition, which
 *  determines which if the IEIs for a given protocol are of which
 *  particular type.  Types are e.g. TV (Tag + single byte value), Tag +
 *  fixed-length value, TLV with 8bit length, TLV with 16bit length, TLV
 *  with variable-length length field, etc.
 *
 * \file tlv_parser.c */

struct tlv_definition tvlv_att_def;
struct tlv_definition vtvlv_gan_att_def;

/*! Dump parsed TLV structure to stdout */
int tlv_dump(struct tlv_parsed *dec)
{
	int i;

	for (i = 0; i <= 0xff; i++) {
		if (!dec->lv[i].val)
			continue;
		printf("T=%02x L=%d\n", i, dec->lv[i].len);
	}
	return 0;
}

/*! Copy \ref tlv_parsed using given talloc context
 *  \param[in] tp_orig Parsed TLV structure
 *  \param[in] ctx Talloc context for allocations
 *  \returns NULL on errors, \ref tlv_parsed pointer otherwise
 */
struct tlv_parsed *osmo_tlvp_copy(const struct tlv_parsed *tp_orig, void *ctx)
{
	struct tlv_parsed *tp_out;
	size_t i, len;

	tp_out = talloc_zero(ctx, struct tlv_parsed);
	if (!tp_out)
		return NULL;

	/* if the original is NULL, return empty tlvp */
	if (!tp_orig)
		return tp_out;

	for (i = 0; i < ARRAY_SIZE(tp_orig->lv); i++) {
		len = tp_orig->lv[i].len;
		tp_out->lv[i].len = len;
		if (len && tp_out->lv[i].val) {
			tp_out->lv[i].val = talloc_zero_size(tp_out, len);
			if (!tp_out->lv[i].val) {
				talloc_free(tp_out);
				return NULL;
			}
			memcpy((uint8_t *)tp_out->lv[i].val, tp_orig->lv[i].val,
			       len);
		}
	}

	return tp_out;
}

/*! Merge all \ref tlv_parsed attributes of 'src' into 'dst'
 *  \param[in] dst Parsed TLV structure to merge into
 *  \param[in] src Parsed TLV structure to merge from
 *  \returns 0 on success, negative on error
 */
int osmo_tlvp_merge(struct tlv_parsed *dst, const struct tlv_parsed *src)
{
	size_t i, len;
	for (i = 0; i < ARRAY_SIZE(dst->lv); i++) {
		len = src->lv[i].len;
		if (len == 0 || src->lv[i].val == NULL)
			continue;
		if (dst->lv[i].val) {
			talloc_free((uint8_t *) dst->lv[i].val);
			dst->lv[i].len = 0;
		}
		dst->lv[i].val = talloc_zero_size(dst, len);
		if (!dst->lv[i].val)
			return -ENOMEM;
		memcpy((uint8_t *) dst->lv[i].val, src->lv[i].val, len);
	}
	return 0;
}

/*! Parse a single TLV encoded IE
 *  \param[out] o_tag the tag of the IE that was found
 *  \param[out] o_len length of the IE that was found
 *  \param[out] o_val pointer to the data of the IE that was found
 *  \param[in] def structure defining the valid TLV tags / configurations
 *  \param[in] buf the input data buffer to be parsed
 *  \param[in] buf_len length of the input data buffer
 *  \returns number of bytes consumed by the TLV entry / IE parsed; negative in case of error
 */
int tlv_parse_one(uint8_t *o_tag, uint16_t *o_len, const uint8_t **o_val,
		  const struct tlv_definition *def,
		  const uint8_t *buf, int buf_len)
{
	uint8_t tag;
	int len;

	tag = *buf;
	*o_tag = tag;

	/* single octet TV IE */
	if (def->def[tag & 0xf0].type == TLV_TYPE_SINGLE_TV) {
		*o_tag = tag & 0xf0;
		*o_val = buf;
		*o_len = 1;
		return 1;
	}

	/* FIXME: use tables for known IEI */
	switch (def->def[tag].type) {
	case TLV_TYPE_T:
		/* GSM TS 04.07 11.2.4: Type 1 TV or Type 2 T */
		*o_val = buf;
		*o_len = 0;
		len = 1;
		break;
	case TLV_TYPE_TV:
		*o_val = buf+1;
		*o_len = 1;
		len = 2;
		break;
	case TLV_TYPE_FIXED:
		*o_val = buf+1;
		*o_len = def->def[tag].fixed_len;
		len = def->def[tag].fixed_len + 1;
		break;
	case TLV_TYPE_TLV:
tlv:		/* GSM TS 04.07 11.2.4: Type 4 TLV */
		if (buf + 1 > buf + buf_len)
			return -1;
		*o_val = buf+2;
		*o_len = *(buf+1);
		len = *o_len + 2;
		if (len > buf_len)
			return -2;
		break;
	case TLV_TYPE_vTvLV_GAN:	/* 44.318 / 11.1.4 */
		/* FIXME: variable-length TAG! */
		if (*(buf+1) & 0x80) {
			/* like TL16Vbut without highest bit of len */
			if (2 > buf_len)
				return -1;
			*o_val = buf+3;
			*o_len = (*(buf+1) & 0x7F) << 8 | *(buf+2);
			len = *o_len + 3;
			if (len > buf_len)
				return -2;
		} else {
			/* like TLV */
			goto tlv;
		}
		break;
	case TLV_TYPE_TvLV:
		if (*(buf+1) & 0x80) {
			/* like TLV, but without highest bit of len */
			if (buf + 1 > buf + buf_len)
				return -1;
			*o_val = buf+2;
			*o_len = *(buf+1) & 0x7f;
			len = *o_len + 2;
			if (len > buf_len)
				return -2;
			break;
		}
		/* like TL16V, fallthrough */
	case TLV_TYPE_TL16V:
		if (2 > buf_len)
			return -1;
		*o_val = buf+3;
		*o_len = *(buf+1) << 8 | *(buf+2);
		len = *o_len + 3;
		if (len > buf_len)
			return -2;
		break;
	default:
		return -3;
	}

	return len;
}

/*! Parse an entire buffer of TLV encoded Information Elements.
 * In case of multiple occurences of an IE, keep only the first occurence.
 * Most GSM related protocols clearly indicate that in case of duplicate
 * IEs, only the first occurrence shall be used, while any further occurrences
 * shall be ignored.  See e.g. 3GPP TS 24.008 Section 8.6.3.
 * For multiple occurences, use tlv_parse2().
 *  \param[out] dec caller-allocated pointer to \ref tlv_parsed
 *  \param[in] def structure defining the valid TLV tags / configurations
 *  \param[in] buf the input data buffer to be parsed
 *  \param[in] buf_len length of the input data buffer
 *  \param[in] lv_tag an initial LV tag at the start of the buffer
 *  \param[in] lv_tag2 a second initial LV tag following the \a lv_tag
 *  \returns number of TLV entries parsed; negative in case of error
 */
int tlv_parse(struct tlv_parsed *dec, const struct tlv_definition *def,
	      const uint8_t *buf, int buf_len, uint8_t lv_tag,
	      uint8_t lv_tag2)
{
	return tlv_parse2(dec, 1, def, buf, buf_len, lv_tag, lv_tag2);
}

/*! Like tlv_parse(), but capable of decoding multiple occurences of the same IE.
 * Parse an entire buffer of TLV encoded Information Elements.
 * To decode multiple occurences of IEs, provide in dec an _array_ of tlv_parsed, and
 * pass the size of that array in dec_multiples. The first occurence of each IE
 * is stored in dec[0], the second in dec[1] and so forth. If there are more
 * occurences than the array length given in dec_multiples, the remaining
 * occurences are dropped.
 *  \param[out] dec caller-allocated pointer to \ref tlv_parsed
 *  \param[in] dec_multiples length of the tlv_parsed[] in \a dec.
 *  \param[in] def structure defining the valid TLV tags / configurations
 *  \param[in] buf the input data buffer to be parsed
 *  \param[in] buf_len length of the input data buffer
 *  \param[in] lv_tag an initial LV tag at the start of the buffer
 *  \param[in] lv_tag2 a second initial LV tag following the \a lv_tag
 *  \returns number of TLV entries parsed; negative in case of error
 */
int tlv_parse2(struct tlv_parsed *dec, int dec_multiples,
	       const struct tlv_definition *def, const uint8_t *buf, int buf_len,
	       uint8_t lv_tag, uint8_t lv_tag2)
{
	int ofs = 0, num_parsed = 0;
	uint16_t len;
	int dec_i;

	for (dec_i = 0; dec_i < dec_multiples; dec_i++)
		memset(&dec[dec_i], 0, sizeof(*dec));

	if (lv_tag) {
		const uint8_t *val;
		uint16_t parsed_len;
		if (ofs > buf_len)
			return -1;
		val = &buf[ofs+1];
		len = buf[ofs];
		parsed_len = len + 1;
		if (ofs + parsed_len > buf_len)
			return -2;
		num_parsed++;
		ofs += parsed_len;
		/* store the resulting val and len */
		for (dec_i = 0; dec_i < dec_multiples; dec_i++) {
			if (dec[dec_i].lv[lv_tag].val != NULL)
				continue;
			dec->lv[lv_tag].val = val;
			dec->lv[lv_tag].len = len;
			break;
		}
	}
	if (lv_tag2) {
		const uint8_t *val;
		uint16_t parsed_len;
		if (ofs > buf_len)
			return -1;
		val = &buf[ofs+1];
		len = buf[ofs];
		parsed_len = len + 1;
		if (ofs + parsed_len > buf_len)
			return -2;
		num_parsed++;
		ofs += parsed_len;
		/* store the resulting val and len */
		for (dec_i = 0; dec_i < dec_multiples; dec_i++) {
			if (dec[dec_i].lv[lv_tag2].val != NULL)
				continue;
			dec->lv[lv_tag2].val = val;
			dec->lv[lv_tag2].len = len;
			break;
		}
	}

	while (ofs < buf_len) {
		int rv;
		uint8_t tag;
		const uint8_t *val;

		rv = tlv_parse_one(&tag, &len, &val, def,
		                   &buf[ofs], buf_len-ofs);
		if (rv < 0)
			return rv;
		for (dec_i = 0; dec_i < dec_multiples; dec_i++) {
			if (dec[dec_i].lv[tag].val != NULL)
				continue;
			dec[dec_i].lv[tag].val = val;
			dec[dec_i].lv[tag].len = len;
			break;
		}
		ofs += rv;
		num_parsed++;
	}
	//tlv_dump(dec);
	return num_parsed;
}

/*! take a master (src) tlvdev and fill up all empty slots in 'dst'
 *  \param dst TLV parser definition that is to be patched
 *  \param[in] src TLV parser definition whose content is patched into \a dst */
void tlv_def_patch(struct tlv_definition *dst, const struct tlv_definition *src)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(dst->def); i++) {
		if (src->def[i].type == TLV_TYPE_NONE)
			continue;
		if (dst->def[i].type == TLV_TYPE_NONE)
			dst->def[i] = src->def[i];
	}
}

static __attribute__((constructor)) void on_dso_load_tlv(void)
{
	int i;
	for (i = 0; i < ARRAY_SIZE(tvlv_att_def.def); i++)
		tvlv_att_def.def[i].type = TLV_TYPE_TvLV;

	for (i = 0; i < ARRAY_SIZE(vtvlv_gan_att_def.def); i++)
		vtvlv_gan_att_def.def[i].type = TLV_TYPE_vTvLV_GAN;
}

/*! Advance the data pointer, subtract length and assign value pointer
 *  \param data pointer to the pointer to data
 *  \param data_len pointer to size_t containing \arg data length
 *  \param[in] len the length that we expect the fixed IE to hav
 *  \param[out] value pointer to pointer of value part of IE
 *  \returns length of IE value; negative in case of error
 */
int osmo_shift_v_fixed(uint8_t **data, size_t *data_len,
			size_t len, uint8_t **value)
{
	if (len > *data_len)
		goto fail;

	if (value)
		*value = *data;

	*data += len;
	*data_len -= len;

	return len;

fail:
	*data += *data_len;
	*data_len = 0;
	return -1;
}

/*! Match tag, check length and assign value pointer
 *  \param data pointer to the pointer to data
 *  \param data_len pointer to size_t containing \arg data length
 *  \param[in] tag the tag (IEI) that we expect at \arg data
 *  \param[in] len the length that we expect the fixed IE to have
 *  \param[out] value pointer to pointer of value part of IE
 *  \returns length of IE value; negative in case of error
 */
int osmo_match_shift_tv_fixed(uint8_t **data, size_t *data_len,
			      uint8_t tag, size_t len,
			      uint8_t **value)
{
	size_t ie_len;

	if (*data_len == 0)
		goto fail;

	if ((*data)[0] != tag)
		return 0;

	if (len > *data_len - 1)
		goto fail;

	if (value)
		*value = *data + 1;

	ie_len = len + 1;
	*data += ie_len;
	*data_len -= ie_len;

	return ie_len;

fail:
	*data += *data_len;
	*data_len = 0;
	return -1;
}

/*! Verify TLV header and advance data / subtract length
 *  \param data pointer to the pointer to data
 *  \param data_len pointer to size_t containing \arg data length
 *  \param[in] expected_tag the tag (IEI) that we expect at \arg data
 *  \param[out] value pointer to pointer of value part of IE
 *  \param[out] value_len pointer to length of \arg value
 *  \returns length of IE value; negative in case of error
 */
int osmo_match_shift_tlv(uint8_t **data, size_t *data_len,
			 uint8_t expected_tag, uint8_t **value,
			 size_t *value_len)
{
	int rc;
	uint8_t tag;
	uint8_t *old_data = *data;
	size_t old_data_len = *data_len;

	rc = osmo_shift_tlv(data, data_len, &tag, value, value_len);

	if (rc > 0 && tag != expected_tag) {
		*data = old_data;
		*data_len = old_data_len;
		return 0;
	}

	return rc;
}

/*! Extract TLV and advance data pointer + subtract length
 *  \param data pointer to the pointer to data
 *  \param data_len  pointer to size_t containing \arg data lengt
 *  \param[out] tag extract the tag (IEI) at start of \arg data
 *  \param[out] value extracted pointer to value part of TLV
 *  \param[out] value_len extracted length of \arg value
 *  \returns number of bytes subtracted
 */
int osmo_shift_tlv(uint8_t **data, size_t *data_len,
	      uint8_t *tag, uint8_t **value, size_t *value_len)
{
	size_t len;
	size_t ie_len;

	if (*data_len < 2)
		goto fail;

	len = (*data)[1];
	if (len > *data_len - 2)
		goto fail;

	if (tag)
		*tag = (*data)[0];
	if (value)
		*value = *data + 2;
	if (value_len)
		*value_len = len;

	ie_len = len + 2;

	*data += ie_len;
	*data_len -= ie_len;

	return ie_len;

fail:
	*data += *data_len;
	*data_len = 0;
	return -1;
}

/*! Extract LV and advance data pointer + subtract length
 *  \param data pointer to the pointer to data
 *  \param data_len  pointer to size_t containing \arg data lengt
 *  \param[out] value extracted pointer to value part of TLV
 *  \param[out] value_len extracted length of \arg value
 *  \returns number of bytes subtracted
 */
int osmo_shift_lv(uint8_t **data, size_t *data_len,
		  uint8_t **value, size_t *value_len)
{
	size_t len;
	size_t ie_len;

	if (*data_len < 1)
		goto fail;

	len = (*data)[0];
	if (len > *data_len - 1)
		goto fail;

	if (value)
		*value = *data + 1;
	if (value_len)
		*value_len = len;

	ie_len = len + 1;
	*data += ie_len;
	*data_len -= ie_len;

	return ie_len;

fail:
	*data += *data_len;
	*data_len = 0;
	return -1;
}

/*! @} */
