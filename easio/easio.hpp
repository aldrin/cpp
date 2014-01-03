// Copyright (C) 2013, Aldrin D'Souza.
// License: http://opensource.org/licenses/BSD-2-Clause
#pragma once

#include <array>
#include <vector>
#include <memory>
#include <functional>

#include <boost/asio.hpp>

namespace easio {

  class response {
    template <typename T> response(T t) {};
  };

  class request {
    std::vector<char> data_;
  public:
    std::size_t limit_;
    char *begin() { return data_.data(); }
    char *end() { return data_.data() + limit_; }
    message(std::size_t chunk = 8192) : data_(chunk), limit_(0) {}
  };

  template class server {
    using tcp = boost::asio::ip::tcp;
    using io_service = boost::asio::io_service;
    using handler = std::function<response(request)>;
    handler handler_;

  public:
    template <typename H> server(H handler, uint16_t port = 8111) : handler_{ handler } {}
  };

}
