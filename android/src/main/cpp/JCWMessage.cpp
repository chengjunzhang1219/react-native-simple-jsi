
#include "JCWMessage.h"

#include <jni.h>
#include <fbjni/fbjni.h>

namespace rnjsi {

using namespace facebook;
using namespace jni;

    local_ref<jstring> JCWMessage::getId() {
        static const auto getPropertyMethod = getClass()->getMethod<jstring()>("getId");
        return getPropertyMethod(self());
    }

    local_ref<jstring> JCWMessage::getAuthor() {
        static const auto getPropertyMethod = getClass()->getMethod<jstring()>("getAuthor");
        return getPropertyMethod(self());
    }

    local_ref<jstring> JCWMessage::getBody() {
        static const auto getPropertyMethod = getClass()->getMethod<jstring()>("getBody");
        return getPropertyMethod(self());
    }

    local_ref<jstring> JCWMessage::getConversation() {
        static const auto getPropertyMethod = getClass()->getMethod<jstring()>("getConversation");
        return getPropertyMethod(self());
    }
} // namespace rnjsi
