// Copyright (C) 2013, Aldrin D'Souza.
// License: http://opensource.org/licenses/BSD-2-Clause

#include <map>
#include <vector>
#include <string>

#include <boost/asio/buffer.hpp>

#include <crest/response.hpp>

namespace crest {

  using boost::asio::buffer;
  using boost::asio::const_buffer;

  namespace {
    const char name_value_separator[] = { ':', ' ' };
    const char crlf[] = { '\r', '\n' };
    const std::map<http_status, std::string> status_lines = {
      {http_status::ok, "HTTP/1.0 200 OK\r\n"},
      {http_status::created, "HTTP/1.0 201 Created\r\n"},
      {http_status::accepted, "HTTP/1.0 202 Accepted\r\n"},
      {http_status::no_content, "HTTP/1.0 204 No Content\r\n"},
      {http_status::multiple_choices, "HTTP/1.0 300 Multiple Choices\r\n"},
      {http_status::moved_permanently, "HTTP/1.0 301 Moved Permanently\r\n"},
      {http_status::moved_temporarily, "HTTP/1.0 302 Found\r\n"},
      {http_status::not_modified, "HTTP/1.0 304 Not Modified\r\n"},
      {http_status::bad_request, "HTTP/1.0 400 Bad Request\r\n"},
      {http_status::unauthorized, "HTTP/1.0 401 Unauthorized\r\n"},
      {http_status::forbidden, "HTTP/1.0 403 Forbidden\r\n"},
      {http_status::not_found, "HTTP/1.0 404 Not Found\r\n"},
      {http_status::method_not_allowed, "HTTP/1.0 405 Method Not Allowed\r\n"},
      {http_status::unsupported_media_type, "HTTP/1.0 415 Unsupported Media Type\r\n"},
      {http_status::internal_server_error, "HTTP/1.0 500 Internal Server Error\r\n"},
      {http_status::not_implemented, "HTTP/1.0 501 Not Implemented\r\n"},
      {http_status::bad_gateway, "HTTP/1.0 502 Bad Gateway\r\n"},
      {http_status::service_unavailable, "HTTP/1.0 503 Service Unavailable\r\n"},
      {http_status::unsupported_http_version, "HTTP/1.0 505 HTTP Version not supporte\r\nd"}
    };
  }

  std::vector<const_buffer> to_buffers(response& r)
  {
    std::vector<const_buffer> buffers;
    buffers.push_back(buffer(status_lines.find(r.status)->second));
    for(auto i = r.headers.begin(); i != r.headers.end(); ++i) {
      buffers.push_back(buffer(i->first));
      buffers.push_back(buffer(name_value_separator));
      buffers.push_back(buffer(i->second));
      buffers.push_back(buffer(crlf));
    }
    buffers.push_back(buffer(crlf));
    buffers.push_back(boost::asio::buffer(r.content));
    return buffers;
  }
}
