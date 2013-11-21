// Copyright (C) 2013, Aldrin D'Souza.
// License: http://opensource.org/licenses/BSD-2-Clause

#include "messages.hpp"
#include <boost/format.hpp>

namespace crest {
// All user facing messages are cataloged in the messages
// namespace.
 namespace messages {
// The format strings of the messages are specified in one
// place, here. These can be scraped for
  namespace format {
   const boost::format process_request("processing request %1%");
  }

  const std::string process_request(const request& r)
  {
    boost::format f(process_request);
    f % r.url();
    return f.str()
  }
 }
}
