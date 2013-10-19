// Copyright (C) 2013, Aldrin D'Souza.
// License: http://opensource.org/licenses/BSD-2-Clause

#include <crest/config.hpp>

namespace crest {

  namespace defaults {
    const char *port = "8111";
  };

  const std::map<config::key, config::spec> config::key_spec_ {
    {
      key::port,
      std::make_tuple
      (
        "port",
        "HTTP acceptor port",
        defaults::port
      )
    }
  };
}
