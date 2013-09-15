// Copyright (C) 2013, Aldrin D'Souza.
// License: http://opensource.org/licenses/BSD-2-Clause

#include <vector>
#include <memory>
#include <vector>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/coroutine/coroutine.hpp>

template<typename T>
class basic_pipeline {
private:
  typedef boost::coroutines::coroutine<const T&()> coro;
  typedef std::unique_ptr<coro> coroptr;

public:
  typedef coro in;
  typedef typename coro::caller_type out;
  typedef boost::function<void(in&, out&)> link;

  basic_pipeline(const std::vector<link>& links) {
    std::vector<coroptr> coros;
    coros.push_back(coroptr(new coro([](out&) {})));
    for(auto & lnk : links) {
      coros.push_back(coroptr(new coro(boost::bind(lnk, boost::ref(*coros.back().get()), _1))));
    }
  }
};
