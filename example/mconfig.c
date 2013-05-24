/*
**
** gcc mconfig.c -I ../../mruby/include/ ../../mruby/build/host/lib/libmruby.a
**
*/
#include <stdio.h>
#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/hash.h"

int main() {

  FILE *fp;
  if ((fp = fopen("./mruby.conf", "r")) == NULL)
    return 1;

  int port;
  char *droot;
  mrb_value user;

  char *tag1_file;
  mrb_value tag1_limit; 
  int tag2_limit; 

  mrb_state* mrb = mrb_open();
  mrb_load_file(mrb, fp);

  /*
  mrb_get_config_value(mrb, key, format, ...)

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

  // get a global configuration value by C data type
  mrb_get_config_value(mrb, "Listen",       "i", &port);
  mrb_get_config_value(mrb, "DocumentRoot", "z", &droot);
  mrb_get_config_value(mrb, "User",         "o", &user);

  // get a tag configuration value by C data type
  mrb_get_sub_config_value(mrb, "tag1", "Files",        "z", &tag1_file);
  mrb_get_sub_config_value(mrb, "tag1", "AccessLimit",  "o", &tag1_limit);
  mrb_get_sub_config_value(mrb, "tag2", "AccessLimit",  "i", &tag2_limit);

  // Implement code using configuration value in Ruby

  printf("=== global configuration ===\n");
  printf("port=%d droot=%s\n", port, droot);
  mrb_p(mrb, user);
  printf("\n");

  printf("=== tag configuration ===\n");
  printf("tag1_file=%s\n", tag1_file);
  mrb_p(mrb, tag1_limit);
  printf("tag2_limit=%d\n", tag2_limit);

  // convert mrb_value to any C data type vale
  int tag1_limit_c;
  mrb_config_convert_value(mrb, tag1_limit, "i", &tag1_limit_c);
  printf("tag1_limit=%d\n", tag1_limit_c);

  mrb_close(mrb);

  return 0;
}
