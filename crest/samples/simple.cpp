// Copyright (C) 2013, Aldrin D'Souza.
// Licensing Terms:
// http://opensource.org/licenses/BSD-2-Clause

#include <crest/server.hpp>
#include <crest/impl/messages.hpp>
#include <iostream>

void hello(const crest::request& request, crest::response& response)
{
  std::cout << crest::messages::process_request(request) << std::endl;
  response.content.append("Hello");
  if(request.has_parameter("name"))
  {
    response.content.append(" ");
    response.content.append(request.parameter("name"));
  }

  response.status = crest::http_status::ok;
};

int main()
{
  crest::server server;
  server.add_route("/", hello);
  server.run();
}
