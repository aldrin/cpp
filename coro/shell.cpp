// Copyright (C) 2013, Aldrin D'Souza.
// License: http://opensource.org/licenses/BSD-2-Clause
#include <coro/shell.hpp>

void all_in_dir()
{
  std::set<std::string> lines;
  shell({
    ls("/tmp"),
    grep("*.log"),
    cat(),
    substr(" 403 "),
    cut(" ", 1),
    uniq(lines)
  });
  std::cout << lines.size() << std::endl;
}

int main()
{
  shell({
    cat("/tmp/access.log"),
    substr(" 403 "),
    cut(" ", 1),
    uniq(),
    echo()
  });
}
