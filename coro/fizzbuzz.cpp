#include <iostream>
#include <coro/pipeline.hpp>

int main()
{
  typedef basic_pipeline<int> fizzbuzz;

  auto one2hundred = [](fizzbuzz::in&, fizzbuzz::out & yield) {
    for(int i = 1; i <= 100; i++) {
      yield(i);
    }
  };

  auto fizz = [](fizzbuzz::in & source, fizzbuzz::out & yield) {
    for(; source; source()) {
      int i = source.get();
      if(i % 3 == 0) {
        std::cout << "fizz";
        if(i % 5 != 0) {
          std::cout << std::endl;
          continue;
        }
      }
      yield(i);
    }
  };

  auto buzz = [](fizzbuzz::in & source, fizzbuzz::out & yield) {
    for(; source; source()) {
      int i = source.get();
      if(i % 5 == 0) {
        std::cout << "buzz" << std::endl;
      } else {
        yield(i);
      }
    }
  };

  auto rest = [](fizzbuzz::in & source, fizzbuzz::out&) {
    for(; source; source()) {
      std::cout << source.get() << std::endl;
    }
  };

  fizzbuzz({one2hundred, fizz, buzz, rest});
}
