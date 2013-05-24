/*
** config.c - Config module
**
** See Copyright Notice in mruby.h
*/

#include <stdarg.h>
#include <string.h>

#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/data.h"
#include "mruby/hash.h"
#include "mruby/string.h"

#define GLOBAL_CONFIG_KEY       "$mrb_g_config"
#define GLOBAL_SUB_CONFIG_KEY   "$mrb_g_sub_config"

/*
** Kernel#{new_config,get_config,add_config,del_config}
** Config#{get,add,del}
*/

static mrb_value mrb_config_init(mrb_state *mrb, mrb_value self)
{
  mrb_value config;

  mrb_get_args(mrb, "o", &config);
  mrb_gv_set(mrb, mrb_intern(mrb, GLOBAL_CONFIG_KEY), config);

  return config;
}

static mrb_value mrb_config_get(mrb_state *mrb, mrb_value self)
{
  mrb_value config, key;

  config = mrb_gv_get(mrb, mrb_intern(mrb, GLOBAL_CONFIG_KEY));
  if (mrb_get_args(mrb, "|o", &key) == 1) {
    return mrb_hash_get(mrb, config, key);
  } else {
    return config;
  }
}

static mrb_value mrb_config_add(mrb_state *mrb, mrb_value self)
{
  mrb_value config, setting;

  mrb_get_args(mrb, "o", &setting);
  config = mrb_gv_get(mrb, mrb_intern(mrb, GLOBAL_CONFIG_KEY));
  if (mrb_nil_p(config)) {
    config = mrb_hash_new(mrb);  
  }
  config = mrb_funcall(mrb, config, "merge", 1, setting);
  mrb_gv_set(mrb, mrb_intern(mrb, GLOBAL_CONFIG_KEY), config);

  return config;
}

static mrb_value mrb_config_del(mrb_state *mrb, mrb_value self)
{
  mrb_value config, key;

  mrb_get_args(mrb, "o", &key);
  config = mrb_gv_get(mrb, mrb_intern(mrb, GLOBAL_CONFIG_KEY));
  mrb_hash_delete_key(mrb, config, key);
  mrb_gv_set(mrb, mrb_intern(mrb, GLOBAL_CONFIG_KEY), config);

  return config;
}

/*
** Kernel#{new,add,get,del}_sub_config
** Config::Sub#{add,get,del}
*/

static mrb_value mrb_config_sub_create(mrb_state *mrb, mrb_value self)
{
  mrb_value config, tag, setting;

  mrb_get_args(mrb, "oo", &tag, &setting);
  config = mrb_hash_new(mrb);
  mrb_hash_set(mrb, config, tag, setting);
  mrb_gv_set(mrb, mrb_intern(mrb, GLOBAL_SUB_CONFIG_KEY), config);

  return config;
}

static mrb_value mrb_config_sub_add(mrb_state *mrb, mrb_value self)
{
  mrb_value config, add_config, tag, setting;

  mrb_get_args(mrb, "oo", &tag, &setting);
  add_config = mrb_hash_new(mrb);
  mrb_hash_set(mrb, add_config, tag, setting);
  config = mrb_gv_get(mrb, mrb_intern(mrb, GLOBAL_SUB_CONFIG_KEY));
  if (mrb_nil_p(config)) {
    config = mrb_hash_new(mrb);  
  }
  config = mrb_funcall(mrb, config, "merge", 1, add_config);
  mrb_gv_set(mrb, mrb_intern(mrb, GLOBAL_SUB_CONFIG_KEY), config);

  return config;
}

static mrb_value mrb_config_sub_get(mrb_state *mrb, mrb_value self)
{
  int argc;
  mrb_value tag, key, config;

  config = mrb_gv_get(mrb, mrb_intern(mrb, GLOBAL_SUB_CONFIG_KEY));
  argc = mrb_get_args(mrb, "o|oo", &tag, &key); 
  if (argc == 1) {
    return mrb_hash_get(mrb, config, tag);
  } else if (argc == 2) {
    return mrb_hash_get(mrb, mrb_hash_get(mrb, config, tag), key);
  } else {
    return config;
  }
}

static mrb_value mrb_config_sub_del(mrb_state *mrb, mrb_value self)
{
  int argc;
  mrb_value tag, key, config;

  config = mrb_gv_get(mrb, mrb_intern(mrb, GLOBAL_SUB_CONFIG_KEY));
  argc = mrb_get_args(mrb, "o|oo", &tag, &key); 
  if (argc == 1) {
    mrb_hash_delete_key(mrb, config, tag);
  } else if (argc == 2) {
    mrb_hash_delete_key(mrb, mrb_hash_get(mrb, config, tag), key);
  } 
  mrb_gv_set(mrb, mrb_intern(mrb, GLOBAL_SUB_CONFIG_KEY), config);

  return config;
}

//
// C functions for mruby-config
//

  /*
  mrb_get_config_value(mrb, key, format, ...)
  mrb_get_sub_config_value(mrb, tag, key, format, ...)

  format specifiers (like mrb_get_args):

   o: Object [mrb_value]
   S: String [mrb_value]          //Not Implemented
   A: Array [mrb_value]           //Not Implemented
   H: Hash [mrb_value]            //Not Implemented
   s: String [char*,int]          //Not Implemented
   z: String [char*]
   a: Array [mrb_value*,mrb_int]  //Not Implemented
   f: Float [mrb_float]           //Not Implemented
   i: Integer [mrb_int]
  */

void mrb_get_config_value(mrb_state *mrb, char *key, const char *format, ...)
{
  mrb_value val;
  const char *p;
  va_list args;

  va_start(args, format);
  p = format;
  val = mrb_funcall(mrb, mrb_top_self(mrb), "get_config", 1, mrb_str_new_cstr(mrb, key));

  switch (*p) {
  case 'o':
    {
      mrb_value *o;

      o = va_arg(args, mrb_value*);
      *o = val;
    }
    break;
  case 'i':
    {
      int *i;
      mrb_value tmp;

      i = va_arg(args, int*);
      tmp = mrb_convert_type(mrb, val, MRB_TT_FIXNUM, "Integer", "to_int");
      *i = mrb_fixnum(tmp);
    }
    break;
  case 'z':
    {
      char **z;
      struct RString *s;

      z = va_arg(args, char**);
      s = mrb_str_ptr(val);
      if (strlen(s->ptr) != s->len) {
        mrb_raise(mrb, E_ARGUMENT_ERROR, "String contains NUL");
      }
      *z = s->ptr;
    }
    break;
  }
  va_end( args );
}

void mrb_get_sub_config_value(mrb_state *mrb, char *tag, char *key, const char *format, ...)
{
  mrb_value val;
  const char *p;
  va_list args;

  va_start(args, format);
  p = format;
  val = mrb_funcall(mrb, mrb_top_self(mrb), "get_sub_config", 2, mrb_str_new_cstr(mrb, tag), mrb_str_new_cstr(mrb, key));

  switch (*p) {
  case 'o':
    {
      mrb_value *o;

      o = va_arg(args, mrb_value*);
      *o = val;
    }
    break;
  case 'i':
    {
      int *i;
      mrb_value tmp;

      i = va_arg(args, int*);
      tmp = mrb_convert_type(mrb, val, MRB_TT_FIXNUM, "Integer", "to_int");
      *i = mrb_fixnum(tmp);
    }
    break;
  case 'z':
    {
      char **z;
      struct RString *s;

      z = va_arg(args, char**);
      s = mrb_str_ptr(val);
      if (strlen(s->ptr) != s->len) {
        mrb_raise(mrb, E_ARGUMENT_ERROR, "String contains NUL");
      }
      *z = s->ptr;
    }
    break;
  }
  va_end( args );
}

void mrb_config_convert_value(mrb_state *mrb, mrb_value val, const char *format, ...)
{
  const char *p;
  va_list args;

  va_start(args, format);
  p = format;

  switch (*p) {
  case 'o':
    {
      mrb_value *o;

      o = va_arg(args, mrb_value*);
      *o = val;
    }
    break;
  case 'i':
    {
      int *i;
      mrb_value tmp;

      i = va_arg(args, int*);
      tmp = mrb_convert_type(mrb, val, MRB_TT_FIXNUM, "Integer", "to_int");
      *i = mrb_fixnum(tmp);
    }
    break;
  case 'z':
    {
      char **z;
      struct RString *s;

      z = va_arg(args, char**);
      s = mrb_str_ptr(val);
      if (strlen(s->ptr) != s->len) {
        mrb_raise(mrb, E_ARGUMENT_ERROR, "String contains NUL");
      }
      *z = s->ptr;
    }
    break;
  }
  va_end( args );
}

void mrb_mruby_config_gem_init(mrb_state *mrb)
{
  struct RClass *config;
  struct RClass *sub_config;

  mrb_define_method(mrb, mrb->kernel_module, "new_config",     mrb_config_init,       MRB_ARGS_REQ(1));
  mrb_define_method(mrb, mrb->kernel_module, "add_config",     mrb_config_add,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, mrb->kernel_module, "del_config",     mrb_config_del,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, mrb->kernel_module, "get_config",     mrb_config_get,        MRB_ARGS_OPT(1));
  mrb_define_method(mrb, mrb->kernel_module, "new_sub_config", mrb_config_sub_create, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, mrb->kernel_module, "add_sub_config", mrb_config_sub_add,    MRB_ARGS_REQ(2));
  mrb_define_method(mrb, mrb->kernel_module, "del_sub_config", mrb_config_sub_del,    MRB_ARGS_OPT(2));
  mrb_define_method(mrb, mrb->kernel_module, "get_sub_config", mrb_config_sub_get,    MRB_ARGS_OPT(2));

  config = mrb_define_class(mrb, "Config", mrb->object_class);
  mrb_define_class_method(mrb, config, "add", mrb_config_add, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, config, "del", mrb_config_del, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, config, "get", mrb_config_get, MRB_ARGS_OPT(1));

  sub_config = mrb_define_class_under(mrb, config, "Sub", mrb->object_class);
  mrb_define_class_method(mrb, sub_config, "add", mrb_config_sub_add, MRB_ARGS_REQ(2));
  mrb_define_class_method(mrb, sub_config, "del", mrb_config_sub_del, MRB_ARGS_REQ(2));
  mrb_define_class_method(mrb, sub_config, "get", mrb_config_sub_get, MRB_ARGS_OPT(2));
}

void mrb_mruby_config_gem_final(mrb_state *mrb)
{
}

