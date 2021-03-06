/*
 * nghttp2 - HTTP/2 C Library
 *
 * Copyright (c) 2014 Tatsuhiro Tsujikawa
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "nghttp2_buf_test.h"

#include <CUnit/CUnit.h>

#include "nghttp2_buf.h"
#include "nghttp2_test_helper.h"

void test_nghttp2_bufs_add(void)
{
  int rv;
  nghttp2_bufs bufs;
  uint8_t data[2048];

  rv = nghttp2_bufs_init(&bufs, 1000, 3);
  CU_ASSERT(0 == rv);

  CU_ASSERT(bufs.cur->buf.pos == bufs.cur->buf.last);

  rv = nghttp2_bufs_add(&bufs, data, 493);
  CU_ASSERT(0 == rv);
  CU_ASSERT(493 == nghttp2_buf_len(&bufs.cur->buf));
  CU_ASSERT(493 == nghttp2_bufs_len(&bufs));
  CU_ASSERT(507 == nghttp2_bufs_cur_avail(&bufs));

  rv = nghttp2_bufs_add(&bufs, data, 507);
  CU_ASSERT(0 == rv);
  CU_ASSERT(1000 == nghttp2_buf_len(&bufs.cur->buf));
  CU_ASSERT(1000 == nghttp2_bufs_len(&bufs));
  CU_ASSERT(bufs.cur == bufs.head);

  rv = nghttp2_bufs_add(&bufs, data, 1);
  CU_ASSERT(0 == rv);
  CU_ASSERT(1 == nghttp2_buf_len(&bufs.cur->buf));
  CU_ASSERT(1001 == nghttp2_bufs_len(&bufs));
  CU_ASSERT(bufs.cur == bufs.head->next);

  nghttp2_bufs_free(&bufs);
}

void test_nghttp2_bufs_addb(void)
{
  int rv;
  nghttp2_bufs bufs;
  ssize_t i;

  rv = nghttp2_bufs_init(&bufs, 1000, 3);
  CU_ASSERT(0 == rv);

  rv = nghttp2_bufs_addb(&bufs, 14);
  CU_ASSERT(0 == rv);
  CU_ASSERT(1 == nghttp2_buf_len(&bufs.cur->buf));
  CU_ASSERT(1 == nghttp2_bufs_len(&bufs));
  CU_ASSERT(14 == *bufs.cur->buf.pos);

  for(i = 0; i < 999; ++i) {
    rv = nghttp2_bufs_addb(&bufs, 254);

    CU_ASSERT(0 == rv);
    CU_ASSERT(i + 2 == nghttp2_buf_len(&bufs.cur->buf));
    CU_ASSERT(i + 2 == nghttp2_bufs_len(&bufs));
    CU_ASSERT(254 == *(bufs.cur->buf.last - 1));
    CU_ASSERT(bufs.cur == bufs.head);
  }

  rv = nghttp2_bufs_addb(&bufs, 253);
  CU_ASSERT(0 == rv);
  CU_ASSERT(1 == nghttp2_buf_len(&bufs.cur->buf));
  CU_ASSERT(1001 == nghttp2_bufs_len(&bufs));
  CU_ASSERT(253 == *(bufs.cur->buf.last - 1));
  CU_ASSERT(bufs.cur == bufs.head->next);

  rv = nghttp2_bufs_addb_hold(&bufs, 15);
  CU_ASSERT(0 == rv);
  CU_ASSERT(1 == nghttp2_buf_len(&bufs.cur->buf));
  CU_ASSERT(1001 == nghttp2_bufs_len(&bufs));
  CU_ASSERT(15 == *(bufs.cur->buf.last));

  /* test fast version */

  nghttp2_bufs_fast_addb(&bufs, 240);

  CU_ASSERT(2 == nghttp2_buf_len(&bufs.cur->buf));
  CU_ASSERT(1002 == nghttp2_bufs_len(&bufs));
  CU_ASSERT(240 == *(bufs.cur->buf.last - 1));

  nghttp2_bufs_fast_addb_hold(&bufs, 113);

  CU_ASSERT(2 == nghttp2_buf_len(&bufs.cur->buf));
  CU_ASSERT(1002 == nghttp2_bufs_len(&bufs));
  CU_ASSERT(113 == *(bufs.cur->buf.last));

  /* addb_hold when last == end */
  bufs.cur->buf.last = bufs.cur->buf.end;

  rv = nghttp2_bufs_addb_hold(&bufs, 19);
  CU_ASSERT(0 == rv);
  CU_ASSERT(0 == nghttp2_buf_len(&bufs.cur->buf));
  CU_ASSERT(2000 == nghttp2_bufs_len(&bufs));
  CU_ASSERT(19 == *(bufs.cur->buf.last));

  nghttp2_bufs_free(&bufs);
}

void test_nghttp2_bufs_orb(void)
{
  int rv;
  nghttp2_bufs bufs;

  rv = nghttp2_bufs_init(&bufs, 1000, 3);
  CU_ASSERT(0 == rv);

  *(bufs.cur->buf.last) = 0;

  rv = nghttp2_bufs_orb_hold(&bufs, 15);
  CU_ASSERT(0 == rv);
  CU_ASSERT(0 == nghttp2_buf_len(&bufs.cur->buf));
  CU_ASSERT(0 == nghttp2_bufs_len(&bufs));
  CU_ASSERT(15 == *(bufs.cur->buf.last));

  rv = nghttp2_bufs_orb(&bufs, 240);
  CU_ASSERT(0 == rv);
  CU_ASSERT(1 == nghttp2_buf_len(&bufs.cur->buf));
  CU_ASSERT(1 == nghttp2_bufs_len(&bufs));
  CU_ASSERT(255 == *(bufs.cur->buf.last - 1));

  *(bufs.cur->buf.last) = 0;
  nghttp2_bufs_fast_orb_hold(&bufs, 240);
  CU_ASSERT(240 == *(bufs.cur->buf.last));

  nghttp2_bufs_fast_orb(&bufs, 15);
  CU_ASSERT(255 == *(bufs.cur->buf.last - 1));

  nghttp2_bufs_free(&bufs);
}

void test_nghttp2_bufs_remove(void)
{
  int rv;
  nghttp2_bufs bufs;
  nghttp2_buf_chain *chain;
  int i;
  uint8_t *out;
  ssize_t outlen;

  rv = nghttp2_bufs_init(&bufs, 1000, 3);
  CU_ASSERT(0 == rv);

  nghttp2_buf_shift_right(&bufs.cur->buf, 10);

  rv = nghttp2_bufs_add(&bufs, "hello ", 6);
  CU_ASSERT(0 == rv);

  for(i = 0; i < 2; ++i) {
    chain = bufs.cur;

    rv = nghttp2_bufs_advance(&bufs);
    CU_ASSERT(0 == rv);

    CU_ASSERT(chain->next == bufs.cur);
  }

  rv = nghttp2_bufs_add(&bufs, "world", 5);
  CU_ASSERT(0 == rv);

  outlen = nghttp2_bufs_remove(&bufs, &out);
  CU_ASSERT(11 == outlen);

  CU_ASSERT(0 == memcmp("hello world", out, outlen));
  CU_ASSERT(0 == nghttp2_bufs_len(&bufs));
  CU_ASSERT(bufs.cur->buf.pos == bufs.cur->buf.begin);

  free(out);
  nghttp2_bufs_free(&bufs);
}

void test_nghttp2_bufs_reset(void)
{
  int rv;
  nghttp2_bufs bufs;
  nghttp2_buf_chain *ci;
  ssize_t offset = 9;

  rv = nghttp2_bufs_init3(&bufs, 250, 3, 1, offset);
  CU_ASSERT(0 == rv);

  rv = nghttp2_bufs_add(&bufs, "foo", 3);
  CU_ASSERT(0 == rv);

  rv = nghttp2_bufs_advance(&bufs);
  CU_ASSERT(0 == rv);

  rv = nghttp2_bufs_add(&bufs, "bar", 3);
  CU_ASSERT(0 == rv);

  CU_ASSERT(6 == nghttp2_bufs_len(&bufs));

  nghttp2_bufs_reset(&bufs);

  CU_ASSERT(0 == nghttp2_bufs_len(&bufs));
  CU_ASSERT(bufs.cur == bufs.head);

  for(ci = bufs.head; ci; ci = ci->next) {
    CU_ASSERT(offset == ci->buf.pos - ci->buf.begin);
    CU_ASSERT(ci->buf.pos == ci->buf.last);
  }

  CU_ASSERT(bufs.head->next == NULL);

  nghttp2_bufs_free(&bufs);
}

void test_nghttp2_bufs_advance(void)
{
  int rv;
  nghttp2_bufs bufs;
  int i;

  rv = nghttp2_bufs_init(&bufs, 250, 3);
  CU_ASSERT(0 == rv);

  for(i = 0; i < 2; ++i) {
    rv = nghttp2_bufs_advance(&bufs);
    CU_ASSERT(0 == rv);
  }

  rv = nghttp2_bufs_advance(&bufs);
  CU_ASSERT(NGHTTP2_ERR_BUFFER_ERROR == rv);

  nghttp2_bufs_free(&bufs);
}

void test_nghttp2_bufs_seek_last_present(void)
{
  int rv;
  nghttp2_bufs bufs;

  rv = nghttp2_bufs_init(&bufs, 250, 3);
  CU_ASSERT(0 == rv);

  rv = nghttp2_bufs_advance(&bufs);
  CU_ASSERT(0 == rv);

  rv = nghttp2_bufs_addb(&bufs, 5);
  CU_ASSERT(0 == rv);

  bufs.cur = bufs.head;

  /* cur is unchanged because cur is empty */
  nghttp2_bufs_seek_last_present(&bufs);

  CU_ASSERT(bufs.cur == bufs.head);

  rv = nghttp2_bufs_addb(&bufs, 1);
  CU_ASSERT(0 == rv);

  nghttp2_bufs_seek_last_present(&bufs);

  CU_ASSERT(bufs.cur == bufs.head->next);

  /* cur is unchanged */
  nghttp2_bufs_seek_last_present(&bufs);

  CU_ASSERT(bufs.cur == bufs.head->next);

  nghttp2_bufs_free(&bufs);
}

void test_nghttp2_bufs_next_present(void)
{
  int rv;
  nghttp2_bufs bufs;

  rv = nghttp2_bufs_init(&bufs, 250, 3);
  CU_ASSERT(0 == rv);

  CU_ASSERT(0 == nghttp2_bufs_next_present(&bufs));

  rv = nghttp2_bufs_advance(&bufs);
  CU_ASSERT(0 == rv);

  nghttp2_bufs_rewind(&bufs);

  CU_ASSERT(0 == nghttp2_bufs_next_present(&bufs));

  bufs.cur = bufs.head->next;

  rv = nghttp2_bufs_addb(&bufs, 1);
  CU_ASSERT(0 == rv);

  nghttp2_bufs_rewind(&bufs);

  CU_ASSERT(0 != nghttp2_bufs_next_present(&bufs));

  nghttp2_bufs_free(&bufs);
}
