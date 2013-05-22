/*
** config.c - Config module
**
** See Copyright Notice in mruby.h
*/

#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/data.h"

#define GLOBAL_CONFIG_KEY    "$mrb_g_config"
#define INSTANCE_CONFIG_KEY  "@mrb_i_config"

// get config func for C

mrb_value mrb_config_cget(mrb_state *mrb, mrb_value self, char *key)
{
  return mrb_funcall(mrb, mrb_top_self(mrb), "get_config", 1, mrb_str_new_cstr(mrb, key));
}

// Kernel#{new_config,get_config,add_config,del_config}
// Config#{get,add,del}

static mrb_value mrb_config_g_init(mrb_state *mrb, mrb_value self)
{
  mrb_value config;

  mrb_get_args(mrb, "o", &config);
  mrb_gv_set(mrb, mrb_intern(mrb, GLOBAL_CONFIG_KEY), config);

  return config;
}

static mrb_value mrb_config_g_get(mrb_state *mrb, mrb_value self)
{
  mrb_value config;
  mrb_value key;
  config = mrb_gv_get(mrb, mrb_intern(mrb, GLOBAL_CONFIG_KEY));
  if (mrb_get_args(mrb, "|S", &key) == 1) {
    return mrb_funcall(mrb, config, "[]", 1, key);
  } else {
    return config;
  }
}

static mrb_value mrb_config_g_add(mrb_state *mrb, mrb_value self)
{
  mrb_value config;
  mrb_value setting;
  mrb_get_args(mrb, "o", &setting);
  config = mrb_gv_get(mrb, mrb_intern(mrb, GLOBAL_CONFIG_KEY));
  config = mrb_funcall(mrb, config, "merge", 1, setting);
  mrb_gv_set(mrb, mrb_intern(mrb, GLOBAL_CONFIG_KEY), config);

  return config;
}

static mrb_value mrb_config_g_del(mrb_state *mrb, mrb_value self)
{
  mrb_value config;
  mrb_value key;
  mrb_get_args(mrb, "S", &key);
  config = mrb_gv_get(mrb, mrb_intern(mrb, GLOBAL_CONFIG_KEY));
  mrb_funcall(mrb, config, "delete", 1, key);
  mrb_gv_set(mrb, mrb_intern(mrb, GLOBAL_CONFIG_KEY), config);

  return config;
}


// Config.{new,get,add,del}
/*
static mrb_value mrb_config_init(mrb_state *mrb, mrb_value self)
{
  mrb_value config;

  mrb_get_args(mrb, "o", &config);
  mrb_iv_set(mrb, self, mrb_intern(mrb, INSTANCE_CONFIG_KEY), config);

  return config;
}

static mrb_value mrb_config_get(mrb_state *mrb, mrb_value self)
{
  mrb_value config;
  mrb_value key;
  mrb_get_args(mrb, "S", &key);
  config = mrb_iv_get(mrb, self, mrb_intern(mrb, INSTANCE_CONFIG_KEY));

  return mrb_funcall(mrb, config, "[]", 1, key);
}

static mrb_value mrb_config_add(mrb_state *mrb, mrb_value self)
{
  mrb_value config;
  mrb_value setting;
  mrb_get_args(mrb, "o", &setting);
  config = mrb_iv_get(mrb, self, mrb_intern(mrb, INSTANCE_CONFIG_KEY));
  config = mrb_funcall(mrb, config, "merge", 1, setting);
  mrb_iv_set(mrb, self, mrb_intern(mrb, INSTANCE_CONFIG_KEY), config);

  return config;
}

static mrb_value mrb_config_del(mrb_state *mrb, mrb_value self)
{
  mrb_value config;
  mrb_value key;
  mrb_get_args(mrb, "S", &key);
  config = mrb_iv_get(mrb, self, mrb_intern(mrb, INSTANCE_CONFIG_KEY));
  mrb_funcall(mrb, config, "delete", 1, key);
  mrb_iv_set(mrb, self, mrb_intern(mrb, INSTANCE_CONFIG_KEY), config);

  return config;
}
*/

void mrb_mruby_config_gem_init(mrb_state *mrb)
{
  struct RClass *config;

  mrb_define_method(mrb, mrb->kernel_module, "new_config", mrb_config_g_init, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, mrb->kernel_module, "add_config", mrb_config_g_add, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, mrb->kernel_module, "del_config", mrb_config_g_del, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, mrb->kernel_module, "get_config", mrb_config_g_get, MRB_ARGS_OPT(1));

  config = mrb_define_class(mrb, "Config", mrb->object_class);
  mrb_define_class_method(mrb, config, "add", mrb_config_g_add, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, config, "del", mrb_config_g_del, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, config, "get", mrb_config_g_get, MRB_ARGS_OPT(1));

  //mrb_define_method(mrb, config, "initialize", mrb_config_init, MRB_ARGS_REQ(1));
  //mrb_define_method(mrb, config, "add", mrb_config_add, MRB_ARGS_REQ(1));
  //mrb_define_method(mrb, config, "del", mrb_config_del, MRB_ARGS_REQ(1));
  //mrb_define_method(mrb, config, "get", mrb_config_get, MRB_ARGS_REQ(1));
}

void mrb_mruby_config_gem_final(mrb_state *mrb)
{
}

