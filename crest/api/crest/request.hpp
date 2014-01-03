// Copyright (C) 2013, Aldrin D'Souza.
// License: http://opensource.org/licenses/BSD-2-Clause

#pragma once

#include <map>
#include <memory>
#include <string>

#include <boost/noncopyable.hpp>
#include <boost/lexical_cast.hpp>

namespace crest {
  class request : private boost::noncopyable {
  public:
    request();

    ~request();

    const std::string &url() const { return url_; }

    const std::string &method() const { return method_; }

    const std::string &content() const { return content_; }

    bool is_complete() const { return !method_.empty(); }

    bool has_header(const std::string &k) const { return headers_.find(k) != headers_.end(); }

    bool has_parameter(const std::string &k) const { return params_.find(k) != params_.end(); }

    bool has_content() const { return !content_.empty(); }

    const std::string &header(const std::string &k) const { return headers_.find(k)->second; }

    const std::string &parameter(const std::string &k) const { return params_.find(k)->second; }

    template <typename T> T header_as(const std::string &k) const
    { return boost::lexical_cast<T>(header(k)); }

    template <typename T> T parameter_as(const std::string &k) const
    { return boost::lexical_cast<T>(parameter(k)); }

    bool update(const char *buffer, size_t length);

    bool update(const char *buffer) { return update(buffer, strlen(buffer)); }

  private:
    std::string url_;
    std::string method_;
    std::string content_;
    std::map<std::string, std::string> params_;
    std::map<std::string, std::string> headers_;

    class parser;
    std::unique_ptr<parser> parser_;
  };
}
