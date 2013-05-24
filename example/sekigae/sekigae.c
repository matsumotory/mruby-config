/*
**
** gcc sekigae.c -I ../../mruby/include/ ../../mruby/build/host/lib/libmruby.a
**
*/
#include <stdio.h>
#include "mruby.h"

int main() {

  FILE *fp;
  if ((fp = fopen("./member.conf", "r")) == NULL)
    return 1;

  mrb_state* mrb = mrb_open();
  mrb_load_file(mrb, fp);

  mrb_value member;
  mrb_get_config_value(mrb, "member", "o", &member);
  mrb_value mnum = mrb_funcall(mrb, member, "size", 0);

  int i;
  int seki_num = mrb_fixnum(mnum);
  for (i = 0; i < seki_num; i++) {
    mnum = mrb_funcall(mrb, member, "size", 0);
    mrb_value seki = mrb_funcall(mrb, mrb_top_self(mrb), "rand", 1, mnum);
    mrb_p(mrb, mrb_funcall(mrb, member, "[]", 1, seki));
    mrb_funcall(mrb, member, "delete_at", 1, seki);
  }

  mrb_close(mrb);

  return 0;
}
