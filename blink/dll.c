/*-*- mode:c;indent-tabs-mode:nil;c-basic-offset:2;tab-width:8;coding:utf-8 -*-│
│vi: set net ft=c ts=2 sts=2 sw=2 fenc=utf-8                                :vi│
╞══════════════════════════════════════════════════════════════════════════════╡
│ Copyright 2022 Justine Alexandra Roberts Tunney                              │
│                                                                              │
│ Permission to use, copy, modify, and/or distribute this software for         │
│ any purpose with or without fee is hereby granted, provided that the         │
│ above copyright notice and this permission notice appear in all copies.      │
│                                                                              │
│ THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL                │
│ WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED                │
│ WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE             │
│ AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL         │
│ DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR        │
│ PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER               │
│ TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR             │
│ PERFORMANCE OF THIS SOFTWARE.                                                │
╚─────────────────────────────────────────────────────────────────────────────*/
#include "blink/dll.h"

/**
 * @fileoverview Doubly-Linked Lists
 */

dll_list dll_remove(dll_list list, dll_element *e) {
  if (list == e) {
    if (list->prev == list) {
      list = 0;
    } else {
      list = list->prev;
    }
  }
  e->next->prev = e->prev;
  e->prev->next = e->next;
  e->next = e;
  e->prev = e;
  return list;
}

void dll_splice_after(dll_element *p, dll_element *n) {
  dll_element *p2;
  dll_element *nl;
  p2 = p->next;
  nl = n->prev;
  p->next = n;
  n->prev = p;
  nl->next = p2;
  p2->prev = nl;
}

dll_list dll_make_first(dll_list list, dll_element *e) {
  if (e) {
    if (!list) {
      list = e->prev;
    } else {
      dll_splice_after(list, e);
    }
  }
  return list;
}

dll_list dll_make_last(dll_list list, dll_element *e) {
  if (e) {
    dll_make_first(list, e->next);
    list = e;
  }
  return list;
}
