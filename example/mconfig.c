#include <mruby.h>
#include <mruby/variable.h>
#include <mruby/hash.h>

static mrb_value get_config_value(mrb_state *mrb, char *key)
{
  return mrb_funcall(mrb, mrb_top_self(mrb), "get_config", 1, mrb_str_new_cstr(mrb, key));
}

int main() {

  FILE *fp;
  if ((fp = fopen("./mruby.conf", "r")) == NULL)
    return 1;

  mrb_state* mrb = mrb_open();
  mrb_load_file(mrb, fp);

  mrb_value listen_port     = get_config_value(mrb, "Listen");
  mrb_value document_root   = get_config_value(mrb, "DocumentRoot");
  mrb_value extend_status   = get_config_value(mrb, "ExtendedStatus");
  mrb_value user            = get_config_value(mrb, "User");
  mrb_value group           = get_config_value(mrb, "Group");

  mrb_close(mrb);

  // Implement code using configuration value in Ruby

  return 0;
}
