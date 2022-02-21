#include "example.h"
#include <jsi/jsi.h>

using namespace facebook;
using namespace std;

void installExampleBridges(jsi::Runtime & jsiRuntime) {
    auto helloWorld = jsi::Function::createFromHostFunction(jsiRuntime,
                                                    jsi::PropNameID::forAscii(jsiRuntime,
                                                                            "helloWorld"),
                                                    0,
                                                    [](jsi::Runtime &runtime,
                                                        const jsi::Value &thisValue,
                                                        const jsi::Value *arguments,
                                                        size_t count) -> jsi::Value {
        string helloworld = "helloworld";
        
        
        return jsi::Value(runtime,
                    jsi::String::createFromUtf8(
                                            runtime,
                                            helloworld));
        
    });
    
    jsiRuntime.global().setProperty(jsiRuntime, "helloWorld", move(helloWorld));
    
    auto multiply = jsi::Function::createFromHostFunction(jsiRuntime,
                                                    jsi::PropNameID::forAscii(jsiRuntime,
                                                                        "multiply"),
                                                    2,
                                                    [](jsi::Runtime &runtime,
                                                        const jsi::Value &thisValue,
                                                        const jsi::Value *arguments,
                                                        size_t count) -> jsi::Value {
        int x = arguments[0].getNumber();
        int y = arguments[1].getNumber();
        
        return jsi::Value(x * y);
        
    });
    
    jsiRuntime.global().setProperty(jsiRuntime, "multiply", move(multiply));
    
    auto multiplyWithCallback = jsi::Function::createFromHostFunction(jsiRuntime,
                                                                jsi::PropNameID::forAscii(jsiRuntime,
                                                                                    "multiplyWithCallback"),
                                                                3,
                                                                [](jsi::Runtime &runtime,
                                                                    const jsi::Value &thisValue,
                                                                    const jsi::Value *arguments,
                                                                    size_t count) -> jsi::Value {
        int x = arguments[0].getNumber();
        int y = arguments[1].getNumber();
        
        arguments[2].getObject(runtime).getFunction(runtime).call(runtime, x * y);
        
        return jsi::Value();
        
    });
    
    jsiRuntime.global().setProperty(jsiRuntime, "multiplyWithCallback", move(multiplyWithCallback));
}
