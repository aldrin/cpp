// Copyright (C) 2013, Aldrin D'Souza.
// License: http://opensource.org/licenses/BSD-2-Clause

#include <set>
#include <regex>
#include <string>
#include <fstream>
#include <iostream>

#include <boost/tokenizer.hpp>
#include <boost/filesystem.hpp>

#include <coro/pipeline.hpp>

typedef basic_pipeline<std::string> shell;

shell::link cat(const std::string& filename)
{
  return [filename](shell::in&, shell::out & yield) {
    std::string line;
    std::ifstream input(filename);
    while(std::getline(input, line)) {
      yield(line);
    }
  };
}

shell::link cat()
{
  return [](shell::in & source, shell::out & yield) {
    std::string line;
    for(; source; source()) {
      std::ifstream input(source.get());
      while(std::getline(input, line)) {
        yield(line);
      }
    }
  };
}

shell::link ls(const std::string& dir)
{
  namespace fs = boost::filesystem;
  return [dir](shell::in & source, shell::out & yield) {
    fs::path p(dir);
    if(fs::exists(p) && fs::is_directory(p)) {
      for(auto f = fs::directory_iterator(p); f != fs::directory_iterator(); ++f) {
        if(fs::is_regular_file(f->path())) {
          yield(f->path().string());
        }
      }
    }
  };
}

shell::link grep(const std::string& pattern)
{
  return [pattern](shell::in & source, shell::out & yield) {
    const std::regex regex(pattern);
    for(; source; source()) {
      const std::string& line(source.get());
      if(std::regex_search(line, regex)) {
        yield(line);
      }
    }
  };
}

shell::link substr(const std::string& in)
{
  return [in](shell::in & source, shell::out & yield) {
    for(; source; source()) {
      const std::string& line(source.get());
      if(line.find(in) != std::string::npos) {
        yield(line);
      }
    }
  };
}

shell::link uniq()
{
  return [](shell::in & source, shell::out & yield) {
    std::set<std::string> unique;
    for(; source; source()) {
      unique.insert(source.get());
    }
    for(const auto & s : unique) {
      yield(s);
    }
  };
}

shell::link uniq(std::set<std::string>& unique)
{
  return [&unique](shell::in & source, shell::out&) {
    for(; source; source()) {
      unique.insert(source.get());
    }
  };
}

shell::link cut(const char *delim, int field)
{
  return [delim, field](shell::in & source, shell::out & yield) {
    typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
    for(; source; source()) {
      int i = 1;
      for(auto & t : tokenizer(source.get(), boost::char_separator<char>(delim))) {
        if(i++ == field) {
          yield(t);
          break;
        }
      }
    }
  };
}

shell::link echo()
{
  return [](shell::in & source, shell::out&) {
    for(; source; source()) {
      std::cout << source.get() << std::endl;
    }
  };
}
