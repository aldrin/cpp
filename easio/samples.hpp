// Copyright (C) 2013, Aldrin D'Souza.
// License: http://opensource.org/licenses/BSD-2-Clause
#pragma once

#include <easio/easio.hpp>

using namespace easio;

void simplest_server()
{
  auto echo = [](request in) { return response("hello"); };
  server(echo);
}
