#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <jsi/jsi.h>

namespace example {
  void install(facebook::jsi::Runtime &jsiRuntime);
}

namespace facebook {
    namespace jsi {
        class Runtime;
    }
}

#endif /* EXAMPLE_H */
