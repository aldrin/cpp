#include <iostream>
#include <coro/pipeline.hpp>
#include <valgrind/callgrind.h>

typedef basic_pipeline<int> pipeline;

int rigidly()
{
  int sum = 0;
  for(int i = 0; i < 100; i++) {
    sum += i;
  }
  return sum;
}

int switches = 0;

int flexibly()
{
  int sum = 0;
  pipeline({
    [](pipeline::in&, pipeline::out & yield) {
      for(int i = 0; i < 100; i++) {
        yield(i);
      }
    },
    [](pipeline::in & source, pipeline::out & yield) {
      for(; source; source()) {
        yield(source.get());
      }
    },
    [&sum](pipeline::in & source, pipeline::out&) {
      for(; source; source()) {
        sum += source.get();
      }
    }
  });
  return sum;
}

int main(int argc, char **argv)
{
  auto func = flexibly;
  if(argc > 1) { func = rigidly; }

  CALLGRIND_START_INSTRUMENTATION;
  std::cout << func() << std::endl;
  CALLGRIND_STOP_INSTRUMENTATION;
}
