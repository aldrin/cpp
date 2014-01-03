// Copyright (C) 2013, Aldrin D'Souza.
// License: http://opensource.org/licenses/BSD-2-Clause

#pragma once

#include <map>
#include <vector>
#include <memory>
#include <string>

#include <boost/noncopyable.hpp>

#include <crest/request.hpp>
#include <crest/response.hpp>

namespace crest {
  typedef std::function<void(const request &, response &)> handler;

  class router : private boost::noncopyable {
  public:
    typedef std::pair<std::string, handler> route;

    void add_route(const std::string &url, handler h)
    { routes_.push_back(std::make_pair(url, h)); }

    void add_status_handler(http_status status, handler h)
    { status_handlers_.insert(std::make_pair(status, h)); }

    handler get_handler(http_status status) const
    {
      auto found = status_handlers_.find(status);
      auto basic = [status](const request &in, response &out) { out.status = status; };
      return found != status_handlers_.end() ? found->second : basic;
    }

    handler get_handler(const std::string &uri) const
    {
      auto match = [uri](const route &x) { return x.first == uri; };
      auto found = std::find_if(routes_.begin(), routes_.end(), match);
      return found != routes_.end() ? found->second : get_handler(http_status::not_found);
    }

  private:
    std::vector<route> routes_;
    std::map<http_status, handler> status_handlers_;
  };
}
