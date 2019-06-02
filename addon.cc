#include <napi.h>
#include "generator.h"

Napi::Object CreateGenerator(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() < 1)
    {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return Napi::Object::New(env);
    }
    if (!info[0].IsNumber())
    {
        Napi::TypeError::New(env, "id must be number").ThrowAsJavaScriptException();
        return Napi::Object::New(env);
    }
    Napi::Number par0 = info[0].As<Napi::Number>();
    int id = par0.Int32Value();
    if (id < 0 || id > 511)
    {
        Napi::RangeError::New(env, "id must be >=0 or <= 511").ThrowAsJavaScriptException();
        return Napi::Object::New(env);
    }
    return Generator::NewInstance(env, info[0]);
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports)
{
    Napi::Object new_exports = Napi::Function::New(env, CreateGenerator, "CreateGenerator");
    return Generator::Init(env, new_exports);
}

NODE_API_MODULE(addon, InitAll)