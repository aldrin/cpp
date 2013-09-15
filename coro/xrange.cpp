// Copyright (C) 2013, Aldrin D'Souza.
// License: http://opensource.org/licenses/BSD-2-Clause

#include <boost/bind.hpp>
#include <boost/coroutine/coroutine.hpp>

// The coroutine type.
typedef boost::coroutines::coroutine<int()> coro;

// The implementation routine of the coroutine.
void xrange_impl(coro::caller_type& yield, int limit)
{
  for(int i = 0; i < limit; i++) {
    yield(i); // Return results back to the caller
  }
}

int main()
{
  // Construct the coroutine instance
  coro xrange(boost::bind(xrange_impl, _1, 10000));

  int sum = 0;
  while(xrange) { // Check completion status
    sum += xrange.get(); // Extract yielded result
    xrange(); // Fire it again.
  }
  assert(sum == 49995000);
}
