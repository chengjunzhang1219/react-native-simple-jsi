#include <jsi/jsi.h>
#include <jni.h>
#include <fbjni/fbjni.h>
#include <vector>
#include <string>

#include "JCWMessage.h"

namespace rnjsi {

    using namespace facebook;

    class JSI_EXPORT MessageHostObject : public jsi::HostObject {
        public:
            explicit MessageHostObject(jni::alias_ref<JCWMessage::javaobject> cwmessage);
            ~MessageHostObject();

        public:
            jsi::Value get(jsi::Runtime &, const jsi::PropNameID &name) override;
            std::vector<jsi::PropNameID> getPropertyNames(jsi::Runtime &rt) override;

        public:
            jni::global_ref<JCWMessage> cwmessage; 

        private:
        static auto constexpr TAG = "CWMessage";
    };

} // namespace rnjsi
