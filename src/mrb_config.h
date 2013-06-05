/*
// mrb_config.h - config module
//
// See Copyright Notice in mruby.h
*/

#ifndef MRB_CONFIG_H
#define MRB_CONFIG_H

void mrb_get_config_value(mrb_state *mrb, char *key, const char *format, ...);
void mrb_get_sub_config_value(mrb_state *mrb, char *tag, char *key, const char *format, ...);
void mrb_config_convert_value(mrb_state *mrb, mrb_value val, const char *format, ...);
void mrb_config_new_config_str(mrb_state *mrb, char *key, char *val);
void mrb_config_add_config_int(mrb_state *mrb, char *key, int val);
void mrb_config_add_config_boolean(mrb_state *mrb, char *key, int val);
void mrb_mruby_config_gem_init(mrb_state *mrb);

#endif
