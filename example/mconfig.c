#include <mruby.h>
#include <mruby/variable.h>
#include <mruby/hash.h>

static mrb_value get_config_value(mrb_state *mrb, char *key)
{
  return mrb_funcall(mrb, mrb_top_self(mrb), "get_config", 1, mrb_str_new_cstr(mrb, key));
}

static mrb_value get_config_all_value(mrb_state *mrb)
{
  return mrb_funcall(mrb, mrb_top_self(mrb), "get_config", 0);
}

static void create_config(mrb_state *mrb, char *key, char *val)
{
  mrb_value conf;
  conf = mrb_hash_new(mrb);
  mrb_hash_set(mrb, conf, mrb_str_new_cstr(mrb, key), mrb_str_new_cstr(mrb, val));
  mrb_funcall(mrb, mrb_top_self(mrb), "new_config", 1, conf);
}

int main() {

  FILE *fp;
  if ((fp = fopen("./mruby.conf", "r")) == NULL)
    return 1;

  mrb_state* mrb = mrb_open();

  create_config(mrb, "Version", "2");

  mrb_load_file(mrb, fp);

  mrb_value listen_port     = get_config_value(mrb, "Listen");
  mrb_value document_root   = get_config_value(mrb, "DocumentRoot");
  mrb_value extend_status   = get_config_value(mrb, "ExtendedStatus");
  mrb_value user            = get_config_value(mrb, "User");
  mrb_value group           = get_config_value(mrb, "Group");

  mrb_close(mrb);

  // 設定の値を使って色々と処理

  return 0;
}
