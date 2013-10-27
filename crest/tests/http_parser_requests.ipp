// The following is extracted from
// https://github.com/joyent/http-parser/blob/master/test.c

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpedantic"

#undef TRUE
#define TRUE 1
#undef FALSE
#define FALSE 0

#define MAX_HEADERS 13
#define MAX_ELEMENT_SIZE 2048

struct message
{
  const char *name; // for debugging purposes
  const char *raw;
  enum http_parser_type type;
  enum http_method method;
  int status_code;
  char request_path[MAX_ELEMENT_SIZE];
  char request_url[MAX_ELEMENT_SIZE];
  char fragment[MAX_ELEMENT_SIZE];
  char query_string[MAX_ELEMENT_SIZE];
  char body[MAX_ELEMENT_SIZE];
  size_t body_size;
  const char *host;
  const char *userinfo;
  uint16_t port;
  int num_headers;
  enum { NONE=0, FIELD, VALUE } last_header_element;
  char headers [MAX_HEADERS][2][MAX_ELEMENT_SIZE];
  int should_keep_alive;

  const char *upgrade; // upgraded body

  unsigned short http_major;
  unsigned short http_minor;

  int message_begin_cb_called;
  int headers_complete_cb_called;
  int message_complete_cb_called;
  int message_complete_on_eof;
  int body_is_final;
};

const struct message requests[] =
#define CURL_GET 0
{{.name = "curl get"
  , .type = HTTP_REQUEST
  , .raw = "GET /test HTTP/1.1\r\n"
           "User-Agent: curl/7.18.0 (i486-pc-linux-gnu) libcurl/7.18.0 OpenSSL/0.9.8g zlib/1.2.3.3 libidn/1.1\r\n"
           "Host: 0.0.0.0=5000\r\n"
           "Accept: */*\r\n"
           "\r\n"
  , .should_keep_alive = TRUE
  , .message_complete_on_eof = FALSE
  , .http_major = 1
  , .http_minor = 1
  , .method = HTTP_GET
  , .query_string = ""
  , .fragment = ""
  , .request_path = "/test"
  , .request_url = "/test"
  , .num_headers = 3
  , .headers =
  {{"User-Agent", "curl/7.18.0 (i486-pc-linux-gnu) libcurl/7.18.0 OpenSSL/0.9.8g zlib/1.2.3.3 libidn/1.1"}
   , {"Host", "0.0.0.0=5000"}
   , {"Accept", "*/*"}}

  , .body = ""}
#define FIREFOX_GET 1
 , {.name = "firefox get"
    , .type = HTTP_REQUEST
    , .raw = "GET /favicon.ico HTTP/1.1\r\n"
             "Host: 0.0.0.0=5000\r\n"
             "User-Agent: Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.9) Gecko/2008061015 Firefox/3.0\r\n"
             "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
             "Accept-Language: en-us,en;q=0.5\r\n"
             "Accept-Encoding: gzip,deflate\r\n"
             "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n"
             "Keep-Alive: 300\r\n"
             "Connection: keep-alive\r\n"
             "\r\n"
    , .should_keep_alive = TRUE
    , .message_complete_on_eof = FALSE
    , .http_major = 1
    , .http_minor = 1
    , .method = HTTP_GET
    , .query_string = ""
    , .fragment = ""
    , .request_path = "/favicon.ico"
    , .request_url = "/favicon.ico"
    , .num_headers = 8
    , .headers =
    {{"Host", "0.0.0.0=5000"}
     , {"User-Agent", "Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.9) Gecko/2008061015 Firefox/3.0"}
     , {"Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"}
     , {"Accept-Language", "en-us,en;q=0.5"}
     , {"Accept-Encoding", "gzip,deflate"}
     , {"Accept-Charset", "ISO-8859-1,utf-8;q=0.7,*;q=0.7"}
     , {"Keep-Alive", "300"}
     , {"Connection", "keep-alive"}}

    , .body = ""}

#define DUMBFUCK 2
 , {.name = "dumbfuck"
    , .type = HTTP_REQUEST
    , .raw = "GET /dumbfuck HTTP/1.1\r\n"
             "aaaaaaaaaaaaa:++++++++++\r\n"
             "\r\n"
    , .should_keep_alive = TRUE
    , .message_complete_on_eof = FALSE
    , .http_major = 1
    , .http_minor = 1
    , .method = HTTP_GET
    , .query_string = ""
    , .fragment = ""
    , .request_path = "/dumbfuck"
    , .request_url = "/dumbfuck"
    , .num_headers = 1
    , .headers =
    {{"aaaaaaaaaaaaa", "++++++++++"}}
    , .body = ""}

#define FRAGMENT_IN_URI 3
 , {.name = "fragment in url"
    , .type = HTTP_REQUEST
    , .raw = "GET /forums/1/topics/2375?page=1#posts-17408 HTTP/1.1\r\n"
             "\r\n"
    , .should_keep_alive = TRUE
    , .message_complete_on_eof = FALSE
    , .http_major = 1
    , .http_minor = 1
    , .method = HTTP_GET
    , .query_string = "page=1"
    , .fragment = "posts-17408"
    , .request_path = "/forums/1/topics/2375"
                      /* XXX request url does include
                         fragment? */
    , .request_url = "/forums/1/topics/2375?page=1#posts-17408"
    , .num_headers = 0
    , .body = ""}

#define GET_NO_HEADERS_NO_BODY 4
 , {.name = "get no headers no body"
    , .type = HTTP_REQUEST
    , .raw = "GET /get_no_headers_no_body/world HTTP/1.1\r\n"
             "\r\n"
    , .should_keep_alive = TRUE
    , .message_complete_on_eof = FALSE    /* would need
                                             Connection:
                                             close */
    , .http_major = 1
    , .http_minor = 1
    , .method = HTTP_GET
    , .query_string = ""
    , .fragment = ""
    , .request_path = "/get_no_headers_no_body/world"
    , .request_url = "/get_no_headers_no_body/world"
    , .num_headers = 0
    , .body = ""}

#define GET_ONE_HEADER_NO_BODY 5
 , {.name = "get one header no body"
    , .type = HTTP_REQUEST
    , .raw = "GET /get_one_header_no_body HTTP/1.1\r\n"
             "Accept: */*\r\n"
             "\r\n"
    , .should_keep_alive = TRUE
    , .message_complete_on_eof = FALSE    /* would need
                                             Connection:
                                             close */
    , .http_major = 1
    , .http_minor = 1
    , .method = HTTP_GET
    , .query_string = ""
    , .fragment = ""
    , .request_path = "/get_one_header_no_body"
    , .request_url = "/get_one_header_no_body"
    , .num_headers = 1
    , .headers =
    {{"Accept", "*/*"}}
    , .body = ""}

#define GET_FUNKY_CONTENT_LENGTH 6
 , {.name = "get funky content length body hello"
    , .type = HTTP_REQUEST
    , .raw = "GET /get_funky_content_length_body_hello HTTP/1.0\r\n"
             "conTENT-Length: 5\r\n"
             "\r\n"
             "HELLO"
    , .should_keep_alive = FALSE
    , .message_complete_on_eof = FALSE
    , .http_major = 1
    , .http_minor = 0
    , .method = HTTP_GET
    , .query_string = ""
    , .fragment = ""
    , .request_path = "/get_funky_content_length_body_hello"
    , .request_url = "/get_funky_content_length_body_hello"
    , .num_headers = 1
    , .headers =
    {{"conTENT-Length", "5"}}
    , .body = "HELLO"}

#define POST_IDENTITY_BODY_WORLD 7
 , {.name = "post identity body world"
    , .type = HTTP_REQUEST
    , .raw = "POST /post_identity_body_world?q=search#hey HTTP/1.1\r\n"
             "Accept: */*\r\n"
             "Transfer-Encoding: identity\r\n"
             "Content-Length: 5\r\n"
             "\r\n"
             "World"
    , .should_keep_alive = TRUE
    , .message_complete_on_eof = FALSE
    , .http_major = 1
    , .http_minor = 1
    , .method = HTTP_POST
    , .query_string = "q=search"
    , .fragment = "hey"
    , .request_path = "/post_identity_body_world"
    , .request_url = "/post_identity_body_world?q=search#hey"
    , .num_headers = 3
    , .headers =
    {{"Accept", "*/*"}
     , {"Transfer-Encoding", "identity"}
     , {"Content-Length", "5"}}

    , .body = "World"}

#define POST_CHUNKED_ALL_YOUR_BASE 8
 , {.name = "post - chunked body: all your base are belong to us"
    , .type = HTTP_REQUEST
    , .raw = "POST /post_chunked_all_your_base HTTP/1.1\r\n"
             "Transfer-Encoding: chunked\r\n"
             "\r\n"
             "1e\r\nall your base are belong to us\r\n"
             "0\r\n"
             "\r\n"
    , .should_keep_alive = TRUE
    , .message_complete_on_eof = FALSE
    , .http_major = 1
    , .http_minor = 1
    , .method = HTTP_POST
    , .query_string = ""
    , .fragment = ""
    , .request_path = "/post_chunked_all_your_base"
    , .request_url = "/post_chunked_all_your_base"
    , .num_headers = 1
    , .headers =
    {{"Transfer-Encoding", "chunked"}}
    , .body = "all your base are belong to us"}

#define TWO_CHUNKS_MULT_ZERO_END 9
 , {.name = "two chunks ; triple zero ending"
    , .type = HTTP_REQUEST
    , .raw = "POST /two_chunks_mult_zero_end HTTP/1.1\r\n"
             "Transfer-Encoding: chunked\r\n"
             "\r\n"
             "5\r\nhello\r\n"
             "6\r\n world\r\n"
             "000\r\n"
             "\r\n"
    , .should_keep_alive = TRUE
    , .message_complete_on_eof = FALSE
    , .http_major = 1
    , .http_minor = 1
    , .method = HTTP_POST
    , .query_string = ""
    , .fragment = ""
    , .request_path = "/two_chunks_mult_zero_end"
    , .request_url = "/two_chunks_mult_zero_end"
    , .num_headers = 1
    , .headers =
    {{"Transfer-Encoding", "chunked"}}
    , .body = "hello world"}

#define CHUNKED_W_TRAILING_HEADERS 10
 , {.name = "chunked with trailing headers. blech."
    , .type = HTTP_REQUEST
    , .raw = "POST /chunked_w_trailing_headers HTTP/1.1\r\n"
             "Transfer-Encoding: chunked\r\n"
             "\r\n"
             "5\r\nhello\r\n"
             "6\r\n world\r\n"
             "0\r\n"
             "Vary: *\r\n"
             "Content-Type: text/plain\r\n"
             "\r\n"
    , .should_keep_alive = TRUE
    , .message_complete_on_eof = FALSE
    , .http_major = 1
    , .http_minor = 1
    , .method = HTTP_POST
    , .query_string = ""
    , .fragment = ""
    , .request_path = "/chunked_w_trailing_headers"
    , .request_url = "/chunked_w_trailing_headers"
    , .num_headers = 3
    , .headers =
    {{"Transfer-Encoding", "chunked"}
     , {"Vary", "*"}
     , {"Content-Type", "text/plain"}}

    , .body = "hello world"}

#define CHUNKED_W_BULLSHIT_AFTER_LENGTH 11
 , {.name = "with bullshit after the length"
    , .type = HTTP_REQUEST
    , .raw = "POST /chunked_w_bullshit_after_length HTTP/1.1\r\n"
             "Transfer-Encoding: chunked\r\n"
             "\r\n"
             "5; ihatew3;whatthefuck=aretheseparametersfor\r\nhello\r\n"
             "6; blahblah; blah\r\n world\r\n"
             "0\r\n"
             "\r\n"
    , .should_keep_alive = TRUE
    , .message_complete_on_eof = FALSE
    , .http_major = 1
    , .http_minor = 1
    , .method = HTTP_POST
    , .query_string = ""
    , .fragment = ""
    , .request_path = "/chunked_w_bullshit_after_length"
    , .request_url = "/chunked_w_bullshit_after_length"
    , .num_headers = 1
    , .headers =
    {{"Transfer-Encoding", "chunked"}}
    , .body = "hello world"}

#define WITH_QUOTES 12
 , {.name = "with quotes"
    , .type = HTTP_REQUEST
    , .raw = "GET /with_\"stupid\"_quotes?foo=\"bar\" HTTP/1.1\r\n\r\n"
    , .should_keep_alive = TRUE
    , .message_complete_on_eof = FALSE
    , .http_major = 1
    , .http_minor = 1
    , .method = HTTP_GET
    , .query_string = "foo=\"bar\""
    , .fragment = ""
    , .request_path = "/with_\"stupid\"_quotes"
    , .request_url = "/with_\"stupid\"_quotes?foo=\"bar\""
    , .num_headers = 0
    , .headers = {}
    , .body = ""}

#define APACHEBENCH_GET 13
/* The server receiving this request SHOULD NOT wait for EOF
 * to know that content-length == 0.
 * How to represent this in a unit test?
 * message_complete_on_eof
 * Compare with NO_CONTENT_LENGTH_RESPONSE.
 */
 , {.name = "apachebench get"
    , .type = HTTP_REQUEST
    , .raw = "GET /test HTTP/1.0\r\n"
             "Host: 0.0.0.0:5000\r\n"
             "User-Agent: ApacheBench/2.3\r\n"
             "Accept: */*\r\n\r\n"
    , .should_keep_alive = FALSE
    , .message_complete_on_eof = FALSE
    , .http_major = 1
    , .http_minor = 0
    , .method = HTTP_GET
    , .query_string = ""
    , .fragment = ""
    , .request_path = "/test"
    , .request_url = "/test"
    , .num_headers = 3
    , .headers = {{"Host", "0.0.0.0:5000"}
                  , {"User-Agent", "ApacheBench/2.3"}
                  , {"Accept", "*/*"}}

    , .body = ""}

#define QUERY_URL_WITH_QUESTION_MARK_GET 14
/* Some clients include '?' characters in query strings.
 */
 , {.name = "query url with question mark"
    , .type = HTTP_REQUEST
    , .raw = "GET /test.cgi?foo=bar?baz HTTP/1.1\r\n\r\n"
    , .should_keep_alive = TRUE
    , .message_complete_on_eof = FALSE
    , .http_major = 1
    , .http_minor = 1
    , .method = HTTP_GET
    , .query_string = "foo=bar?baz"
    , .fragment = ""
    , .request_path = "/test.cgi"
    , .request_url = "/test.cgi?foo=bar?baz"
    , .num_headers = 0
    , .headers = {}
    , .body = ""}

#define PREFIX_NEWLINE_GET 15
/* Some clients, especially after a POST in a keep-alive
   connection,
 * will send an extra CRLF before the next request
 */
 , {.name = "newline prefix get"
    , .type = HTTP_REQUEST
    , .raw = "\r\nGET /test HTTP/1.1\r\n\r\n"
    , .should_keep_alive = TRUE
    , .message_complete_on_eof = FALSE
    , .http_major = 1
    , .http_minor = 1
    , .method = HTTP_GET
    , .query_string = ""
    , .fragment = ""
    , .request_path = "/test"
    , .request_url = "/test"
    , .num_headers = 0
    , .headers = {}
    , .body = ""}

#define UPGRADE_REQUEST 16
 , {.name = "upgrade request"
    , .type = HTTP_REQUEST
    , .raw = "GET /demo HTTP/1.1\r\n"
             "Host: example.com\r\n"
             "Connection: Upgrade\r\n"
             "Sec-WebSocket-Key2: 12998 5 Y3 1  .P00\r\n"
             "Sec-WebSocket-Protocol: sample\r\n"
             "Upgrade: WebSocket\r\n"
             "Sec-WebSocket-Key1: 4 @1  46546xW%0l 1 5\r\n"
             "Origin: http://example.com\r\n"
             "\r\n"
             "Hot diggity dogg"
    , .should_keep_alive = TRUE
    , .message_complete_on_eof = FALSE
    , .http_major = 1
    , .http_minor = 1
    , .method = HTTP_GET
    , .query_string = ""
    , .fragment = ""
    , .request_path = "/demo"
    , .request_url = "/demo"
    , .num_headers = 7
    , .upgrade = "Hot diggity dogg"
    , .headers = {{"Host", "example.com"}
                  , {"Connection", "Upgrade"}
                  , {"Sec-WebSocket-Key2", "12998 5 Y3 1  .P00"}
                  , {"Sec-WebSocket-Protocol", "sample"}
                  , {"Upgrade", "WebSocket"}
                  , {"Sec-WebSocket-Key1", "4 @1  46546xW%0l 1 5"}
                  , {"Origin", "http://example.com"}}

    , .body = ""}

#define CONNECT_REQUEST 17
 , {.name = "connect request"
    , .type = HTTP_REQUEST
    , .raw = "CONNECT 0-home0.netscape.com:443 HTTP/1.0\r\n"
             "User-agent: Mozilla/1.1N\r\n"
             "Proxy-authorization: basic aGVsbG86d29ybGQ=\r\n"
             "\r\n"
             "some data\r\n"
             "and yet even more data"
    , .should_keep_alive = FALSE
    , .message_complete_on_eof = FALSE
    , .http_major = 1
    , .http_minor = 0
    , .method = HTTP_CONNECT
    , .query_string = ""
    , .fragment = ""
    , .request_path = ""
    , .request_url = "0-home0.netscape.com:443"
    , .num_headers = 2
    , .upgrade = "some data\r\nand yet even more data"
    , .headers = {{"User-agent", "Mozilla/1.1N"}
                  , {"Proxy-authorization", "basic aGVsbG86d29ybGQ="}}

    , .body = ""}

#define REPORT_REQ 18
 , {.name = "report request"
    , .type = HTTP_REQUEST
    , .raw = "REPORT /test HTTP/1.1\r\n"
             "\r\n"
    , .should_keep_alive = TRUE
    , .message_complete_on_eof = FALSE
    , .http_major = 1
    , .http_minor = 1
    , .method = HTTP_REPORT
    , .query_string = ""
    , .fragment = ""
    , .request_path = "/test"
    , .request_url = "/test"
    , .num_headers = 0
    , .headers = {}
    , .body = ""}

#define NO_HTTP_VERSION 19
 , {.name = "request with no http version"
    , .type = HTTP_REQUEST
    , .raw = "GET /\r\n"
             "\r\n"
    , .should_keep_alive = FALSE
    , .message_complete_on_eof = FALSE
    , .http_major = 0
    , .http_minor = 9
    , .method = HTTP_GET
    , .query_string = ""
    , .fragment = ""
    , .request_path = "/"
    , .request_url = "/"
    , .num_headers = 0
    , .headers = {}
    , .body = ""}

#define MSEARCH_REQ 20
 , {.name = "m-search request"
    , .type = HTTP_REQUEST
    , .raw = "M-SEARCH * HTTP/1.1\r\n"
             "HOST: 239.255.255.250:1900\r\n"
             "MAN: \"ssdp:discover\"\r\n"
             "ST: \"ssdp:all\"\r\n"
             "\r\n"
    , .should_keep_alive = TRUE
    , .message_complete_on_eof = FALSE
    , .http_major = 1
    , .http_minor = 1
    , .method = HTTP_MSEARCH
    , .query_string = ""
    , .fragment = ""
    , .request_path = "*"
    , .request_url = "*"
    , .num_headers = 3
    , .headers = {{"HOST", "239.255.255.250:1900"}
                  , {"MAN", "\"ssdp:discover\""}
                  , {"ST", "\"ssdp:all\""}}

    , .body = ""}

#define LINE_FOLDING_IN_HEADER 21
 , {.name = "line folding in header value"
    , .type = HTTP_REQUEST
    , .raw = "GET / HTTP/1.1\r\n"
             "Line1:   abc\r\n"
             "\tdef\r\n"
             " ghi\r\n"
             "\t\tjkl\r\n"
             "  mno \r\n"
             "\t \tqrs\r\n"
             "Line2: \t line2\t\r\n"
             "\r\n"
    , .should_keep_alive = TRUE
    , .message_complete_on_eof = FALSE
    , .http_major = 1
    , .http_minor = 1
    , .method = HTTP_GET
    , .query_string = ""
    , .fragment = ""
    , .request_path = "/"
    , .request_url = "/"
    , .num_headers = 2
    , .headers = {{"Line1", "abcdefghijklmno qrs"}
                  , {"Line2", "line2\t"}}

    , .body = ""}


#define QUERY_TERMINATED_HOST 22
 , {.name = "host terminated by a query string"
    , .type = HTTP_REQUEST
    , .raw = "GET http://hypnotoad.org?hail=all HTTP/1.1\r\n"
             "\r\n"
    , .should_keep_alive = TRUE
    , .message_complete_on_eof = FALSE
    , .http_major = 1
    , .http_minor = 1
    , .method = HTTP_GET
    , .query_string = "hail=all"
    , .fragment = ""
    , .request_path = ""
    , .request_url = "http://hypnotoad.org?hail=all"
    , .host = "hypnotoad.org"
    , .num_headers = 0
    , .headers = {}
    , .body = ""}

#define QUERY_TERMINATED_HOSTPORT 23
 , {.name = "host:port terminated by a query string"
    , .type = HTTP_REQUEST
    , .raw = "GET http://hypnotoad.org:1234?hail=all HTTP/1.1\r\n"
             "\r\n"
    , .should_keep_alive = TRUE
    , .message_complete_on_eof = FALSE
    , .http_major = 1
    , .http_minor = 1
    , .method = HTTP_GET
    , .query_string = "hail=all"
    , .fragment = ""
    , .request_path = ""
    , .request_url = "http://hypnotoad.org:1234?hail=all"
    , .host = "hypnotoad.org"
    , .port = 1234
    , .num_headers = 0
    , .headers = {}
    , .body = ""}

#define SPACE_TERMINATED_HOSTPORT 24
 , {.name = "host:port terminated by a space"
    , .type = HTTP_REQUEST
    , .raw = "GET http://hypnotoad.org:1234 HTTP/1.1\r\n"
             "\r\n"
    , .should_keep_alive = TRUE
    , .message_complete_on_eof = FALSE
    , .http_major = 1
    , .http_minor = 1
    , .method = HTTP_GET
    , .query_string = ""
    , .fragment = ""
    , .request_path = ""
    , .request_url = "http://hypnotoad.org:1234"
    , .host = "hypnotoad.org"
    , .port = 1234
    , .num_headers = 0
    , .headers = {}
    , .body = ""}

#define PATCH_REQ 25
 , {.name = "PATCH request"
    , .type = HTTP_REQUEST
    , .raw = "PATCH /file.txt HTTP/1.1\r\n"
             "Host: www.example.com\r\n"
             "Content-Type: application/example\r\n"
             "If-Match: \"e0023aa4e\"\r\n"
             "Content-Length: 10\r\n"
             "\r\n"
             "cccccccccc"
    , .should_keep_alive = TRUE
    , .message_complete_on_eof = FALSE
    , .http_major = 1
    , .http_minor = 1
    , .method = HTTP_PATCH
    , .query_string = ""
    , .fragment = ""
    , .request_path = "/file.txt"
    , .request_url = "/file.txt"
    , .num_headers = 4
    , .headers = {{"Host", "www.example.com"}
                  , {"Content-Type", "application/example"}
                  , {"If-Match", "\"e0023aa4e\""}
                  , {"Content-Length", "10"}}

    , .body = "cccccccccc"}

#define CONNECT_CAPS_REQUEST 26
 , {.name = "connect caps request"
    , .type = HTTP_REQUEST
    , .raw = "CONNECT HOME0.NETSCAPE.COM:443 HTTP/1.0\r\n"
             "User-agent: Mozilla/1.1N\r\n"
             "Proxy-authorization: basic aGVsbG86d29ybGQ=\r\n"
             "\r\n"
    , .should_keep_alive = FALSE
    , .message_complete_on_eof = FALSE
    , .http_major = 1
    , .http_minor = 0
    , .method = HTTP_CONNECT
    , .query_string = ""
    , .fragment = ""
    , .request_path = ""
    , .request_url = "HOME0.NETSCAPE.COM:443"
    , .num_headers = 2
    , .upgrade = ""
    , .headers = {{"User-agent", "Mozilla/1.1N"}
                  , {"Proxy-authorization", "basic aGVsbG86d29ybGQ="}}

    , .body = ""}

#if !HTTP_PARSER_STRICT
#define UTF8_PATH_REQ 27
 , {.name = "utf-8 path request"
    , .type = HTTP_REQUEST
    , .raw = "GET /δ¶/δt/pope?q=1#narf HTTP/1.1\r\n"
             "Host: github.com\r\n"
             "\r\n"
    , .should_keep_alive = TRUE
    , .message_complete_on_eof = FALSE
    , .http_major = 1
    , .http_minor = 1
    , .method = HTTP_GET
    , .query_string = "q=1"
    , .fragment = "narf"
    , .request_path = "/δ¶/δt/pope"
    , .request_url = "/δ¶/δt/pope?q=1#narf"
    , .num_headers = 1
    , .headers = {{"Host", "github.com"}}
    , .body = ""}

#define HOSTNAME_UNDERSCORE 28
 , {.name = "hostname underscore"
    , .type = HTTP_REQUEST
    , .raw = "CONNECT home_0.netscape.com:443 HTTP/1.0\r\n"
             "User-agent: Mozilla/1.1N\r\n"
             "Proxy-authorization: basic aGVsbG86d29ybGQ=\r\n"
             "\r\n"
    , .should_keep_alive = FALSE
    , .message_complete_on_eof = FALSE
    , .http_major = 1
    , .http_minor = 0
    , .method = HTTP_CONNECT
    , .query_string = ""
    , .fragment = ""
    , .request_path = ""
    , .request_url = "home_0.netscape.com:443"
    , .num_headers = 2
    , .upgrade = ""
    , .headers = {{"User-agent", "Mozilla/1.1N"}
                  , {"Proxy-authorization", "basic aGVsbG86d29ybGQ="}}

    , .body = ""}
#endif  /* !HTTP_PARSER_STRICT */

/* see https://github.com/ry/http-parser/issues/47 */
#define EAT_TRAILING_CRLF_NO_CONNECTION_CLOSE 29
 , {.name = "eat CRLF between requests, no \"Connection: close\" header"
    , .raw = "POST / HTTP/1.1\r\n"
             "Host: www.example.com\r\n"
             "Content-Type: application/x-www-form-urlencoded\r\n"
             "Content-Length: 4\r\n"
             "\r\n"
             "q=42\r\n"    /* note the trailing CRLF */
    , .should_keep_alive = TRUE
    , .message_complete_on_eof = FALSE
    , .http_major = 1
    , .http_minor = 1
    , .method = HTTP_POST
    , .query_string = ""
    , .fragment = ""
    , .request_path = "/"
    , .request_url = "/"
    , .num_headers = 3
    , .upgrade = 0
    , .headers = {{"Host", "www.example.com"}
                  , {"Content-Type", "application/x-www-form-urlencoded"}
                  , {"Content-Length", "4"}}

    , .body = "q=42"}

/* see https://github.com/ry/http-parser/issues/47 */
#define EAT_TRAILING_CRLF_WITH_CONNECTION_CLOSE 30
 , {.name = "eat CRLF between requests even if \"Connection: close\" is set"
    , .raw = "POST / HTTP/1.1\r\n"
             "Host: www.example.com\r\n"
             "Content-Type: application/x-www-form-urlencoded\r\n"
             "Content-Length: 4\r\n"
             "Connection: close\r\n"
             "\r\n"
             "q=42\r\n"    /* note the trailing CRLF */
    , .should_keep_alive = FALSE
    , .message_complete_on_eof = FALSE    /* input buffer
                                             isn't empty
                                                when
                                             on_message_complete
                                             is called */
    , .http_major = 1
    , .http_minor = 1
    , .method = HTTP_POST
    , .query_string = ""
    , .fragment = ""
    , .request_path = "/"
    , .request_url = "/"
    , .num_headers = 4
    , .upgrade = 0
    , .headers = {{"Host", "www.example.com"}
                  , {"Content-Type", "application/x-www-form-urlencoded"}
                  , {"Content-Length", "4"}
                  , {"Connection", "close"}}

    , .body = "q=42"}

#define PURGE_REQ 31
 , {.name = "PURGE request"
    , .type = HTTP_REQUEST
    , .raw = "PURGE /file.txt HTTP/1.1\r\n"
             "Host: www.example.com\r\n"
             "\r\n"
    , .should_keep_alive = TRUE
    , .message_complete_on_eof = FALSE
    , .http_major = 1
    , .http_minor = 1
    , .method = HTTP_PURGE
    , .query_string = ""
    , .fragment = ""
    , .request_path = "/file.txt"
    , .request_url = "/file.txt"
    , .num_headers = 1
    , .headers = {{"Host", "www.example.com"}}
    , .body = ""}

#define SEARCH_REQ 32
 , {.name = "SEARCH request"
    , .type = HTTP_REQUEST
    , .raw = "SEARCH / HTTP/1.1\r\n"
             "Host: www.example.com\r\n"
             "\r\n"
    , .should_keep_alive = TRUE
    , .message_complete_on_eof = FALSE
    , .http_major = 1
    , .http_minor = 1
    , .method = HTTP_SEARCH
    , .query_string = ""
    , .fragment = ""
    , .request_path = "/"
    , .request_url = "/"
    , .num_headers = 1
    , .headers = {{"Host", "www.example.com"}}
    , .body = ""}

#define PROXY_WITH_BASIC_AUTH 33
 , {.name = "host:port and basic_auth"
    , .type = HTTP_REQUEST
    , .raw = "GET http://a%12:b!&*$@hypnotoad.org:1234/toto HTTP/1.1\r\n"
             "\r\n"
    , .should_keep_alive = TRUE
    , .message_complete_on_eof = FALSE
    , .http_major = 1
    , .http_minor = 1
    , .method = HTTP_GET
    , .fragment = ""
    , .request_path = "/toto"
    , .request_url = "http://a%12:b!&*$@hypnotoad.org:1234/toto"
    , .host = "hypnotoad.org"
    , .userinfo = "a%12:b!&*$"
    , .port = 1234
    , .num_headers = 0
    , .headers = {}
    , .body = ""}


 , {.name = NULL}     /* sentinel */
};

#pragma clang diagnostic pop
