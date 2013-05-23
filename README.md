# Config Module for mruby
With mruby we continuously switching between Ruby and C code. mruby-config gives you an easy way to maintain configuration values on both sides.

refs: http://blog.mruby.sh/201305220423.html

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
    #include <mruby.h>
    
    static mrb_value get_config_value(mrb_state *mrb, char *key)
    {
      return mrb_funcall(mrb, mrb_top_self(mrb), "get_config", 1, mrb_str_new_cstr(mrb, key));
    }
    
    static mrb_value get_sub_config_value(mrb_state *mrb, char *tag, char *key)
    {
      return mrb_funcall(mrb, mrb_top_self(mrb), "sub_get_config", 2, mrb_str_new_cstr(mrb, tag)), mrb_str_new_cstr(mrb, key));
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
      
      mrb_value tag1_limit      = get_sub_config_value(mrb, "tag1", "AccessLimit");
      mrb_value tag2_limit      = get_sub_config_value(mrb, "tag2", "AccessLimit");
    
      // implement any function here
    
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

 - Delete configuration values
```ruby
del_config "ExtendedStatus"
```

 - Read out a configuration values
```ruby
get_config "ExtendedStatus"
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

 - Delete sub configuration values (NOT IMPLEMTED)
```ruby
del_sub_config "hoge"
```

 - Read out a sub configuration values
```ruby
get_sub_config "tag1", "AccessLimit"
```
or get sub all configureation values
```ruby
get_sub_config "tag2"
```

# License
under the MIT License:

* http://www.opensource.org/licenses/mit-license.php


