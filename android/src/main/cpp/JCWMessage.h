#include <jni.h>
#include <fbjni/fbjni.h>
#include <string>

namespace rnjsi {

    using namespace facebook;
    using namespace jni;

    struct JCWMessage : public JavaClass<JCWMessage> {
        static constexpr auto kJavaDescriptor = "Lcom/rnjsisecond/CWMessage;";

        public:
        local_ref<jstring> getId();
        local_ref<jstring> getAuthor();
        local_ref<jstring> getBody();
        local_ref<jstring> getConversation();
    };

} 
