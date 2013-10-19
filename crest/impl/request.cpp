// Copyright (C) 2013, Aldrin D'Souza.
// License: http://opensource.org/licenses/BSD-2-Clause

#include "request_reader.hpp"

namespace crest {
  request::~request()
  {
  }

  request::request(): parser_(new parser(*this))
  {
  }

  bool request::update(const char *buffer, size_t length)
  {
    return parser_->update(buffer, length);
  }
}
