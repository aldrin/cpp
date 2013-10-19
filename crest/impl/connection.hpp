// Copyright (C) 2013, Aldrin D'Souza.
// License: http://opensource.org/licenses/BSD-2-Clause

#include <crest/server.hpp>

namespace crest {

  struct server::connection : public std::enable_shared_from_this<connection> {

    void wait_for_data();
    typedef std::shared_ptr<connection> ptr;
    boost::asio::ip::tcp::socket& socket() { return socket_; }
    static ptr make(server& s) { return ptr(new connection(s)); }

  private:
    server& server_;
    request request_;
    response response_;
    boost::array<char, 8192> buffer_;
    boost::asio::ip::tcp::socket socket_;
    void write(const boost::system::error_code&);
    void read(const boost::system::error_code&, std::size_t);
    connection(server& p): server_(p), socket_(server_.io_service_) {}
  };

}
