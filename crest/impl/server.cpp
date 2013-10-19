// Copyright (C) 2013, Aldrin D'Souza.
// License: http://opensource.org/licenses/BSD-2-Clause

#include <crest/server.hpp>
#include <crest/impl/acceptor.hpp>
#include <crest/impl/connection.hpp>

namespace crest {
  server::server(const config& c): config_(c), io_service_(), acceptor_(acceptor::make(*this))
  {
  }

  void preprocess(request& r)
  {
  }

  void postprocess(response& r)
  {
    r.add_header("Server", "cREST");
    if(r.content.size()) {
      r.add_header("Content-Length", r.content.size());
    }
  }

  void server::handle(request& in, response& out)
  {
    try {
      preprocess(in);
      router_.get_handler(in.parameter("url_path"))(in, out);
      postprocess(out);
    } catch(std::exception& e) {
      router_.get_handler(http_status::internal_server_error)(in, out);
    }
  }
}
