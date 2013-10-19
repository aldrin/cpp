// Copyright (C) 2013, Aldrin D'Souza.
// License: http://opensource.org/licenses/BSD-2-Clause

#include <crest/impl/acceptor.hpp>
#include <crest/impl/connection.hpp>

namespace crest {

  using std::bind;
  using boost::asio::ip::tcp;
  using std::placeholders::_1;
  using boost::system::error_code;

  server::acceptor::acceptor(server& p): server_(p),
    impl_(server_.io_service_,
          tcp::endpoint(tcp::v4(), server_.config_.get<short>(config::key::port)))
  {
    wait_for_connection();
  }

  void server::acceptor::wait_for_connection()
  {
    connection::ptr c(connection::make(server_));
    impl_.async_accept(c->socket(), bind(&acceptor::accept, this, c, _1));
  }

  void server::acceptor::accept(connection::ptr c, const error_code& error)
  {
    if(!error) { c->wait_for_data(); }
    wait_for_connection();
  }

}
