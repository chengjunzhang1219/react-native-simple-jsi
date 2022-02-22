#include "MessageHostObject.h"
#include <android/log.h>
#include <fbjni/fbjni.h>
#include <jni.h>
#include <vector>
#include <string>
#include "JSIJNIConversion.h"

namespace rnjsi {

    using namespace facebook;

    MessageHostObject::MessageHostObject(jni::alias_ref<JCWMessage::javaobject> cwmessage): cwmessage(make_global(cwmessage)) { }

    MessageHostObject::~MessageHostObject() {
        jni::ThreadScope::WithClassLoader([&] { cwmessage.reset(); });
    }

    std::vector<jsi::PropNameID> MessageHostObject::getPropertyNames(jsi::Runtime& rt) {
        std::vector<jsi::PropNameID> result;
        result.push_back(jsi::PropNameID::forUtf8(rt, std::string("id")));
        result.push_back(jsi::PropNameID::forUtf8(rt, std::string("author")));
        result.push_back(jsi::PropNameID::forUtf8(rt, std::string("body")));
        result.push_back(jsi::PropNameID::forUtf8(rt, std::string("conversation")));
        return result;
    }

    jsi::Value MessageHostObject::get(jsi::Runtime& runtime, const jsi::PropNameID& propNameId) {
        auto name = propNameId.utf8(runtime);

        if (name == "id") {
            return JSIJNIConversion::convertJNIObjectToJSIValue(runtime, cwmessage->getId());
        }
        if (name == "author") {
            return JSIJNIConversion::convertJNIObjectToJSIValue(runtime, cwmessage->getAuthor());
        }
        if (name == "body") {
            return JSIJNIConversion::convertJNIObjectToJSIValue(runtime, cwmessage->getBody());
        }
        if (name == "conversation") {
            return JSIJNIConversion::convertJNIObjectToJSIValue(runtime, cwmessage->getConversation());
        }
        return jsi::Value::undefined();
    }
}