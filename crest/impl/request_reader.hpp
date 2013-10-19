// Copyright (C) 2013, Aldrin D'Souza.
// License: http://opensource.org/licenses/BSD-2-Clause

#include <crest/request.hpp>
#include "http_parser.h"

#include <tuple>

namespace crest {

  class request::parser {
    request& request_;
    http_parser parser_;
    http_parser_settings parser_settings_;

    typedef std::tuple<request *, std::string, std::string> context_t;

    static void update_headers(context_t *context) {
      request *req = std::get<0>(*context);
      std::string& name = std::get<1>(*context);
      std::string& value = std::get<2>(*context);
      if(!(name.empty() || value.empty())) {
        std::string& existing = req->headers_[name];
        if(!existing.empty()) { existing.append(","); }
        existing.append(value);
        name.clear();
        value.clear();
      }
    }

    static int on_message_complete(http_parser *parser) {
      static const char *url_field_names [] = {
        "url_schema",
        "url_host",
        "url_port",
        "url_path",
        "url_query",
        "url_fragment",
        "url_userinfo"
      };

      context_t *context(static_cast<context_t *>(parser->data));
      update_headers(context);

      request *req(std::get<0>(*context));
      req->method_ = http_method_str(http_method(parser->method));

      struct http_parser_url u;

      if(!http_parser_parse_url(req->url_.c_str(), req->url_.size(), 0, &u)) {
        for(int f = UF_SCHEMA; f < UF_MAX; f++) {
          if(u.field_set & (1 << f)) {
            req->params_.insert
            (std::make_pair
             (url_field_names[f],
              req->url_.substr(u.field_data[f].off, u.field_data[f].len)));
          }
        }
      }

      return 0;
    }

    static int on_url(http_parser *parser, const char *at, size_t len) {
      context_t *context(static_cast<context_t *>(parser->data));
      std::get<0>(*context)->url_.append(at, len);
      return 0;
    }

    static int on_headers_complete(http_parser *parser) {
      context_t *context(static_cast<context_t *>(parser->data));
      update_headers(context);
      return 0;
    }

    static int on_header_field(http_parser *parser, const char *at, size_t len) {
      context_t *context(static_cast<context_t *>(parser->data));
      update_headers(context);
      std::get<1>(*context).append(at, len);
      return 0;
    }

    static int on_header_value(http_parser *parser, const char *at, size_t len) {
      context_t *context(static_cast<context_t *>(parser->data));
      std::get<2>(*context).append(at, len);
      return 0;
    }

    static int on_body(http_parser *parser, const char *at, size_t len) {
      context_t *context(static_cast<context_t *>(parser->data));
      std::get<0>(*context)->content_.append(at, len);
      return 0;
    }

  public:
    parser(request& parent): request_(parent) {
      http_parser_init(&parser_, HTTP_REQUEST);
      parser_.data = new context_t(&request_, std::string(), std::string());

      parser_settings_.on_url = parser::on_url;
      parser_settings_.on_body = parser::on_body;
      parser_settings_.on_message_begin = nullptr;
      parser_settings_.on_status_complete = nullptr;
      parser_settings_.on_header_field = parser::on_header_field;
      parser_settings_.on_header_value = parser::on_header_value;
      parser_settings_.on_headers_complete = parser::on_headers_complete;
      parser_settings_.on_message_complete = parser::on_message_complete;
    }

    ~parser() {
      delete static_cast<context_t *>(parser_.data);
    }

    bool update(const char *buffer, size_t length) {
      http_parser_execute(&parser_, &parser_settings_, buffer, length);
      return HTTP_PARSER_ERRNO(&parser_) == HPE_OK;
    }
  };
}
