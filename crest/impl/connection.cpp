// Copyright (C) 2013, Aldrin D'Souza.
// License: http://opensource.org/licenses/BSD-2-Clause

#include <crest/impl/connection.hpp>

namespace crest {

  using std::bind;
  using boost::asio::buffer;
  using boost::asio::ip::tcp;
  using std::placeholders::_1;
  using std::placeholders::_2;
  using boost::asio::async_write;
  using boost::system::error_code;

  extern std::vector<boost::asio::const_buffer> to_buffers(response&);

  void server::connection::wait_for_data()
  {
    socket_.async_read_some(buffer(buffer_), bind(&connection::read, shared_from_this(), _1, _2));
  }

  void server::connection::write(const error_code& error)
  {
    if(!error) {
      error_code ignore;
      socket_.shutdown(tcp::socket::shutdown_both, ignore);
    }
  }

  void server::connection::read(const error_code& error, std::size_t length)
  {
    if(!error) {
      if(request_.update(buffer_.data(), length)) {
        if(request_.is_complete()) {
          server_.handle(request_, response_);
        } else {
          wait_for_data();
          return;
        }
      } else {
        response_.status = http_status::bad_request;
      }
      async_write(socket_, to_buffers(response_), bind(&connection::write, shared_from_this(), _1));
    }
  }
}
