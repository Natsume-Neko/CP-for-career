{ pkgs, lib, config, inputs, ... }:

{
  env.NIX_CFLAGS_COMPILE = "-std=c++23";
}
