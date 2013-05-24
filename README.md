# Config Module for mruby
With mruby we continuously switching between Ruby and C code. mruby-config gives you an easy way to maintain configuration values on both sides.

refs: http://blog.mruby.sh/201305220423.html

 - Kernel#{get,del,add,new}_config
 - Config#{get,del,add}
 - Kernel#{get,del,add,new}_sub_config
 - Config::Sub#{get,del,add}

## install by mrbgems
 - add conf.gem line to `build_config.rb`
```ruby
MRuby::Build.new do |conf|

    # ... (snip) ...

    conf.gem :git => 'https://github.com/matsumoto-r/mruby-config.git'
end
```

## How to use
 - An example Ruby configuration ```mruby.conf```

    ```ruby
    new_config(
        "Listen"            => 80,
        "DocumentRoot"      => "/var/www/html",
        "ExtendedStatus"    => "On",
        "User"              => "apache",
        "Group"             => "apache",
    )
    
    sub_new_config tag1, {
        "Files"         => "index.cgi",
        "AccessLimit"   => 5
    }
    
    sub_add_config tag2, {
        "Files"       => "mt.cgi",
        "AccessLimit" => 1
    }
    ```


 - The corresponding C code to read the configuration values in C

    ```c
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
      mrb_get_sub_config_value(mrb, tag, key, format, ...)
      mrb_config_convert_value(mrb, val, format, ...)
    
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
    ```

### Other mruby-config method
 - To work on Ruby side with these values you can, create a new configuration value
```ruby
new_config(
    "Listen"            => 80,
    "DocumentRoot"      => "/var/www/html",
    "User"              => "apache",
    "Group"             => "apache",
)
```

 - add or modify a new configuration value
```ruby
add_config "ExtendedStatus" => "On"
```
or
```ruby
Config::add "ExtendedStatus" => "On"
```

 - Delete configuration values
```ruby
del_config "ExtendedStatus"
```
or
```ruby
Config::del "ExtendedStatus"
```

 - Read out a configuration values
```ruby
get_config "ExtendedStatus"
```
or
```ruby
Config::get "ExtendedStatus"
```
or get all configureation values
```ruby
get_config
```

 - To work on Ruby side with these values you can, create a new sub configuration value
```ruby
new_sub_config tag1, {
    "Files"       => "mt.cgi",
    "AccessLimit" => 1
}
add_sub_config tag2, {
    "Files"       => "index.cgi",
    "AccessLimit" => 5
}
```

 - add or modify a new sub configuration value
```ruby
add_sub_config "tag1", "Directory" => "/var/www/"
```
or
```ruby
Config::Sub::add "tag1", "Directory" => "/var/www/"
```

 - Delete sub configuration values (NOT IMPLEMTED)
```ruby
del_sub_config "hoge"
```
or
```ruby
Config::Sub::del "hoge"
```

 - Read out a sub configuration values
```ruby
get_sub_config "tag1", "AccessLimit"
```
or
```ruby
Config::Sub::get "tag1", "AccessLimit"
```
or get sub all configureation values
```ruby
get_sub_config "tag2"
```

# License
under the MIT License:

* http://www.opensource.org/licenses/mit-license.php


