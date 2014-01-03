// Copyright (C) 2013, Aldrin D'Souza.
// License: http://opensource.org/licenses/BSD-2-Clause
#include <map>
#include <string>
#include <iostream>
#include <easio/samples.hpp>

int main(int argc, const char **argv) {
  using namespace std;
  typedef function<void()> sample;

  // A map of known samples we have.
  map<string, pair<sample, string>> samples;

  // Sample registration functor (mostly to remove the "make_pair" clutter)
  auto register_sample = [&samples](sample r, string name, string description) {
    samples.insert(make_pair(name, make_pair(r, description)));
  };

  // Register the samples we know of
  register_sample(simplest_server, "simplest-server",
                  "The simplest possible server (mostly useless)");

  // A valid sample name must be specified - else show usage.
  if (argc != 2 || samples.count(argv[1]) == 0) {
    cerr << "choose a sample: " << endl;
    for (auto i = samples.begin(); i != samples.end(); ++i) {
      cerr << '\t' << i->first << ":\t" << i->second.second << endl;
    }
    // No point continuing.
    exit(1);
  }

  samples.find(argv[1])->second.first();
}
