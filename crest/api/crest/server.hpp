// Copyright (C) 2013, Aldrin D'Souza.
// License: http://opensource.org/licenses/BSD-2-Clause

#pragma once

#include <string>
#include <memory>

#include <boost/asio.hpp>

#include <crest/config.hpp>
#include <crest/router.hpp>

namespace crest {

  class server {

  public:
    server(const config& cfg = config());

    template<typename HANDLER> void add_route(const std::string& url, HANDLER h) {
      router_.add_route(url, handler(h));
    }

    void run() {
      io_service_.run();
    }

  private:

    config config_;
    router router_;
    struct acceptor;
    struct connection;
    void handle(request&, response&);
    boost::asio::io_service io_service_;
    std::shared_ptr<acceptor> acceptor_;
  };
}
