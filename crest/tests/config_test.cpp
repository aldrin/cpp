// Copyright (C) 2013, Aldrin D'Souza.
// License: http://opensource.org/licenses/BSD-2-Clause

#include <cassert>
#include <iostream>
#include <crest/config.hpp>

using crest::config;

void test_defaults()
{
  assert(config().get<int>(config::key::port) == config::default_value<int>(config::key::port));
}

void test_set()
{
  crest::config cfg;
  cfg.set(config::key::port, 80);
  assert(cfg.get<int>(config::key::port) == 80);
}

int main()
{
  test_set();
  test_defaults();
}
