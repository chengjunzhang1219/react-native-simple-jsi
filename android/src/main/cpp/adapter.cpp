#include <jni.h>
#include <sys/types.h>
#include "example.h"
#include "pthread.h"
#include <jsi/jsi.h>
#include <jsi/JSIDynamic.h>
#include <folly/dynamic.h>
#include <react/jni/NativeMap.h>
#include <react/jni/ReadableNativeMap.h>
#include <react/jni/WritableNativeMap.h>
#include "MessageHostObject.h"
#include "JSIJNIConversion.h"

using namespace facebook;
using namespace std;
using namespace rnjsi;

JavaVM *java_vm;
jclass java_class;
jobject java_object;

/**
 * A simple callback function that allows us to detach current JNI Environment
 * when the thread
 * See https://stackoverflow.com/a/30026231 for detailed explanation
 */

void DeferThreadDetach(JNIEnv *env) {
    static pthread_key_t thread_key;

    // Set up a Thread Specific Data key, and a callback that
    // will be executed when a thread is destroyed.
    // This is only done once, across all threads, and the value
    // associated with the key for any given thread will initially
    // be NULL.
    static auto run_once = [] {
        const auto err = pthread_key_create(&thread_key, [](void *ts_env) {
            if (ts_env) {
                java_vm->DetachCurrentThread();
            }
        });
        if (err) {
            // Failed to create TSD key. Throw an exception if you want to.
        }
        return 0;
    }();

    // For the callback to actually be executed when a thread exits
    // we need to associate a non-NULL value with the key on that thread.
    // We can use the JNIEnv* as that value.
    const auto ts_env = pthread_getspecific(thread_key);
    if (!ts_env) {
        if (pthread_setspecific(thread_key, env)) {
            // Failed to set thread-specific value for key. Throw an exception if you want to.
        }
    }
}

/**
 * Get a JNIEnv* valid for this thread, regardless of whether
 * we're on a native thread or a Java thread.
 * If the calling thread is not currently attached to the JVM
 * it will be attached, and then automatically detached when the
 * thread is destroyed.
 *
 * See https://stackoverflow.com/a/30026231 for detailed explanation
 */
JNIEnv *GetJniEnv() {
    JNIEnv *env = nullptr;
    // We still call GetEnv first to detect if the thread already
    // is attached. This is done to avoid setting up a DetachCurrentThread
    // call on a Java thread.

    // g_vm is a global.
    auto get_env_result = java_vm->GetEnv((void **) &env, JNI_VERSION_1_6);
    if (get_env_result == JNI_EDETACHED) {
        if (java_vm->AttachCurrentThread(&env, NULL) == JNI_OK) {
            DeferThreadDetach(env);
        } else {
            // Failed to attach thread. Throw an exception if you want to.
        }
    } else if (get_env_result == JNI_EVERSION) {
        // Unsupported JNI version. Throw an exception if you want to.
    }
    return env;
}


static jstring string2jstring(JNIEnv *env, const string &str) {
    return (*env).NewStringUTF(str.c_str());
}


void install(jsi::Runtime &jsiRuntime) {
    auto getDeviceName = jsi::Function::createFromHostFunction(jsiRuntime,
                                                          jsi::PropNameID::forAscii(jsiRuntime,
                                                                               "getDeviceName"),
                                                          0,
                                                          [](jsi::Runtime &runtime,
                                                             const jsi::Value &thisValue,
                                                             const jsi::Value *arguments,
                                                             size_t count) -> jsi::Value {

                                                              JNIEnv *jniEnv = GetJniEnv();

                                                              java_class = jniEnv->GetObjectClass(
                                                                      java_object);
                                                              jmethodID getModel = jniEnv->GetMethodID(
                                                                      java_class, "getModel",
                                                                      "()Ljava/lang/String;");
                                                              jobject result = jniEnv->CallObjectMethod(
                                                                      java_object, getModel);
                                                              const char *str = jniEnv->GetStringUTFChars(
                                                                      (jstring) result, NULL);

                                                              return jsi::Value(runtime,
                                                                           jsi::String::createFromUtf8(
                                                                                   runtime, str));

                                                          });

    jsiRuntime.global().setProperty(jsiRuntime, "getDeviceName", move(getDeviceName));


    auto setItem = jsi::Function::createFromHostFunction(jsiRuntime,
                                                    jsi::PropNameID::forAscii(jsiRuntime,
                                                                         "setItem"),
                                                    2,
                                                    [](jsi::Runtime &runtime,
                                                       const jsi::Value &thisValue,
                                                       const jsi::Value *arguments,
                                                       size_t count) -> jsi::Value {

                                                        string key = arguments[0].getString(
                                                                runtime).utf8(runtime);
                                                        string value = arguments[1].getString(
                                                                runtime).utf8(runtime);

                                                        JNIEnv *jniEnv = GetJniEnv();

                                                        java_class = jniEnv->GetObjectClass(
                                                                java_object);


                                                        jmethodID set = jniEnv->GetMethodID(
                                                                java_class, "setItem",
                                                                "(Ljava/lang/String;Ljava/lang/String;)V");

                                                        jstring jstr1 = string2jstring(jniEnv,
                                                                                       key);
                                                        jstring jstr2 = string2jstring(jniEnv,
                                                                                       value);
                                                        jvalue params[2];
                                                        params[0].l = jstr1;
                                                        params[1].l = jstr2;

                                                        jniEnv->CallVoidMethodA(
                                                                java_object, set, params);

                                                        return jsi::Value(true);

                                                    });

    jsiRuntime.global().setProperty(jsiRuntime, "setItem", move(setItem));

    auto setMessage = jsi::Function::createFromHostFunction(jsiRuntime,
                                                            jsi::PropNameID::forAscii(jsiRuntime,
                                                                                      "setMessage"),
                                                            1,
                                                            [](jsi::Runtime &runtime,
                                                               const jsi::Value &thisValue,
                                                               const jsi::Value *arguments,
                                                               size_t count) -> jsi::Value
                                                            {
                                                                auto messageReadableMap = arguments[0].asObject(runtime);
                                                                auto messageValue = jsi::Value(runtime, messageReadableMap);
                                                                auto dynamic = jsi::dynamicFromValue(runtime, messageValue);
                                                                auto map = react::ReadableNativeArray::newObjectCxxArgs(std::move(dynamic));

                                                                JNIEnv *jniEnv = GetJniEnv();

                                                                java_class = jniEnv->GetObjectClass(
                                                                    java_object);

                                                                jmethodID set = jniEnv->GetMethodID(
                                                                    java_class, "setMessage",
                                                                    "(Lcom/facebook/react/bridge/ReadableArray;)V");

                                                                jvalue params[1];
                                                                params[0].l = map.release();

                                                                jniEnv->CallVoidMethodA(
                                                                    java_object, set, params);

                                                                return jsi::Value(true);
                                                            });

    jsiRuntime.global().setProperty(jsiRuntime, "setMessage", move(setMessage));


    auto getItem = jsi::Function::createFromHostFunction(jsiRuntime,
                                                    jsi::PropNameID::forAscii(jsiRuntime,
                                                                         "getItem"),
                                                    1,
                                                    [](jsi::Runtime &runtime,
                                                       const jsi::Value &thisValue,
                                                       const jsi::Value *arguments,
                                                       size_t count) -> jsi::Value {

                                                        string key = arguments[0].getString(
                                                                        runtime)
                                                                .utf8(
                                                                        runtime);

                                                        JNIEnv *jniEnv = GetJniEnv();

                                                        java_class = jniEnv->GetObjectClass(
                                                                java_object);
                                                        jmethodID get = jniEnv->GetMethodID(
                                                                java_class, "getItem",
                                                                "(Ljava/lang/String;)Ljava/lang/String;");

                                                        jstring jstr1 = string2jstring(jniEnv,
                                                                                       key);
                                                        jvalue params[2];
                                                        params[0].l = jstr1;

                                                        jobject result = jniEnv->CallObjectMethodA(
                                                                java_object, get, params);
                                                        const char *str = jniEnv->GetStringUTFChars(
                                                                (jstring) result, NULL);

                                                        return jsi::Value(runtime,
                                                                     jsi::String::createFromUtf8(
                                                                             runtime, str));

                                                    });

    jsiRuntime.global().setProperty(jsiRuntime, "getItem", move(getItem));

}

extern "C"
JNIEXPORT void JNICALL
Java_com_rnjsisecond_RnJsiSecondModule_nativeInstall(JNIEnv *env, jobject thiz, jlong jsi) {
    auto runtime = reinterpret_cast<jsi::Runtime *>(jsi);
    if (runtime) {
        installExampleBridges(*runtime);
        install(*runtime);
    }
    env->GetJavaVM(&java_vm);
    java_object = env->NewGlobalRef(thiz);
}