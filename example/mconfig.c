#include <stdio.h>
#include <mruby.h>
#include <mruby/variable.h>
#include <mruby/hash.h>

static mrb_value get_config_value(mrb_state *mrb, char *key)
{
  return mrb_funcall(mrb, mrb_top_self(mrb), "get_config", 1, mrb_str_new_cstr(mrb, key));
}

static mrb_value get_sub_config_value(mrb_state *mrb, char *tag, char *key)
{
  return mrb_funcall(mrb, mrb_top_self(mrb), "get_sub_config", 2, mrb_str_new_cstr(mrb, tag), mrb_str_new_cstr(mrb, key));
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

  mrb_value tag1_file       = get_sub_config_value(mrb, "tag1", "Files");
  mrb_value tag1_limit      = get_sub_config_value(mrb, "tag1", "AccessLimit");
  mrb_value tag2_file       = get_sub_config_value(mrb, "tag2", "Files");
  mrb_value tag2_limit      = get_sub_config_value(mrb, "tag2", "AccessLimit");

  printf("=== global configuration ===\n");
  mrb_p(mrb, listen_port);
  mrb_p(mrb, document_root);
  mrb_p(mrb, extend_status);
  mrb_p(mrb, user);
  mrb_p(mrb, group);
  printf("\n");

  printf("=== tag configuration ===\n");
  mrb_p(mrb, tag1_file);
  mrb_p(mrb, tag1_limit);
  mrb_p(mrb, tag2_file);
  mrb_p(mrb, tag2_limit);

  mrb_close(mrb);

  // Implement code using configuration value in Ruby

  return 0;
}
