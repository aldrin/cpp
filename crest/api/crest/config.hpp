// Copyright (C) 2013, Aldrin D'Souza.
// License: http://opensource.org/licenses/BSD-2-Clause

#include <map>
#include <tuple>
#include <string>

#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>

namespace crest {

  class config {

  public:
    enum class key {
      port,
      pre_processors,
      post_processors,
    };

    template<typename T> T get(key k) const {
      auto i = store_.find(name(k));
      return i != store_.not_found() ? i->second.get_value<T>() : default_value<T>(k);
    }

    template<typename T> void set(key k, T value) {
      store_.put(name(k), value);
    }

    static const char *name(key k) {
      return get_spec<0>(k);
    }

    static const char *description(key k) {
      return get_spec<1>(k);
    }

    template<typename T> static T default_value(key k) {
      return boost::lexical_cast<T>(get_spec<2>(k));
    }

  private:
    typedef std::tuple<const char *, const char *, const char *> spec;

    template<int I> static const char *get_spec(key k) {
      auto entry = key_spec_.find(k);
      assert(entry != std::end(key_spec_));
      return std::get<I>(entry->second);
    }

    boost::property_tree::ptree store_;
    static const std::map<key, spec> key_spec_;
  };
}
