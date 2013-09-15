// Copyright (C) 2013, Aldrin D'Souza.
// License: http://opensource.org/licenses/BSD-2-Clause

#include <coro/shell.hpp>

void pipeline()
{
  shell({
    cat("/tmp/access.log")
  });
}

void rigid()
{
  std::string line;
  std::ifstream input("/tmp/access.log");
  int count = 0;
  while(std::getline(input, line)) {
    count++;
  }
  std::cout << count << std::endl;
}

int main(int argc, char **argv)
{
  if(argc > 1) { rigid(); }
  else { pipeline(); }
}
