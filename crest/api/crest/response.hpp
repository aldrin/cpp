// Copyright (C) 2013, Aldrin D'Souza.
// License: http://opensource.org/licenses/BSD-2-Clause

#pragma once

#include <map>
#include <vector>

#include <boost/noncopyable.hpp>
#include <boost/lexical_cast.hpp>

namespace crest {
enum class http_status {
  ok = 200,
  created = 201,
  accepted = 202,
  no_content = 204,

  multiple_choices = 300,
  moved_permanently = 301,
  moved_temporarily = 302,
  not_modified = 304,

  bad_request = 400,
  unauthorized = 401,
  forbidden = 403,
  not_found = 404,
  method_not_allowed = 405,
  unsupported_media_type = 415,

  internal_server_error = 500,
  not_implemented = 501,
  bad_gateway = 502,
  service_unavailable = 503,
  unsupported_http_version = 505
};

class response: private boost::noncopyable {
public:
  http_status status;
  std::string content;

  template<typename T>
  void add_header(const std::string& name, const T& value)
  {
    std::string& existing = headers[name];
    if(!existing.empty()) {existing.append(","); }
    existing.append(boost::lexical_cast<std::string>(value));
  }

  std::map<std::string, std::string> headers;
};
}
