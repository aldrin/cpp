// Copyright (C) 2013, Aldrin D'Souza.
// License: http://opensource.org/licenses/BSD-2-Clause

#include <cassert>

#include <crest/request.hpp>
#include <crest/impl/http_parser.h>

void test_simple_parse()
{
  assert(!crest::request().update("hello world"));
  assert(!crest::request().update("GET / HTP/1.1\r\n\r\n"));
  assert(!crest::request().update("ASDF / HTTP/1.1\r\n\r\n"));
  assert(!crest::request().update("PROPPATCHA / HTTP/1.1\r\n\r\n"));
  assert(!crest::request().update("GETA / HTTP/1.1\r\n\r\n"));
  assert(crest::request().update("GET / HTTP/1.1\r\nContent-Type: text/plain\r\n\r\n"));
}

void test_parse()
{
  crest::request request;
  assert(request.update
         (
           "GET http://aldrin.co/blah?q=test HTTP/1.1\r\n"
           "Host: 0.0.0.0=5000\r\n"
           "User-Agent: Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.9)\r\n"
           "Accept: text/html,application/xhtml+xml,application/xml;q=0.8\r\n"
           "Accept-Language: en-us,en;q=0.5\r\n"
           "Accept-Charset: ISO-8859-1,utf-8;q=0.7\r\n"
           "Accept-Encoding: gzip,deflate\r\n"
           "Accept-Charset: *;q=0.7\r\n"
           "Keep-Alive: 300\r\n"
           "Connection: keep-alive\r\n"
           "Content-Length: 5\r\n"
           "\r\n"
           "World"
         ));
  assert(request.url() == "http://aldrin.co/blah?q=test");
  assert(request.has_parameter("url_host"));
  assert(request.has_parameter("url_query"));
  assert(request.has_parameter("url_path"));
  assert(request.has_header("Host"));
  assert(request.has_header("Keep-Alive"));
  assert(request.has_header("Accept-Language"));
  assert(request.parameter("url_host") == "aldrin.co");
  assert(request.parameter("url_query") == "q=test");
  assert(request.parameter("url_path") == "/blah");
  assert(request.header_as<int>("Keep-Alive") == 300);
  assert(request.header("Accept-Language") == "en-us,en;q=0.5");
  assert(request.header("Accept-Charset") == "ISO-8859-1,utf-8;q=0.7,*;q=0.7");
  assert(request.content() == "World");
}

#ifdef __clang__

#include "http_parser_requests.ipp"

void test_http_parser_cases()
{
  for(int i = 0; requests[i].name; i++)
  {
    crest::request request;
    assert(request.update(requests[i].raw));
    assert(request.url() == requests[i].request_url);
    assert(request.method() == http_method_str(http_method(requests[i].method)));
    for(int j = 0; j < requests[i].num_headers; j++)
    {
      std::string name(requests[i].headers[j][0]);
      std::string value(requests[i].headers[j][1]);
      assert(request.has_header(name));
      assert(request.header(name) == value);
    }
    assert(request.content() == requests[i].body);
    if(requests[i].host)
    {
      assert(request.has_parameter("url_host"));
      assert(request.parameter("url_host") == requests[i].host);
    }

    if(requests[i].request_path && strlen(requests[i].request_path))
    {
      assert(request.has_parameter("url_path"));
      assert(request.parameter("url_path") == requests[i].request_path);
    }

    if(requests[i].query_string && strlen(requests[i].query_string))
    {
      assert(request.has_parameter("url_query"));
      assert(request.parameter("url_query") == requests[i].query_string);
    }
  }
}

#else
void test_http_parser_cases()
{
  std::cout << "ignoring http_parser test cases" << std::endl;
}

#endif

int main()
{
  test_parse();
  test_simple_parse();
  test_http_parser_cases();
  return 0;
}
