// Copyright (C) 2013, Aldrin D'Souza.
// License: http://opensource.org/licenses/BSD-2-Clause

#include <crest/server.hpp>

namespace crest {
struct server::acceptor: public std::enable_shared_from_this<acceptor>
{
  typedef std::shared_ptr<acceptor> ptr;
  static ptr make(server& p) {return ptr(new acceptor(p)); }

private:
  server& server_;
  acceptor(server& p);
  void wait_for_connection();
  boost::asio::ip::tcp::acceptor impl_;
  void accept(std::shared_ptr<connection>, const boost::system::error_code&);
};
}
