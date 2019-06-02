#ifndef GENERATOR_H
#define GENERATOR_H

#include <napi.h>

class Generator : public Napi::ObjectWrap<Generator>
{
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object NewInstance(Napi::Env env, Napi::Value arg);
    Generator(const Napi::CallbackInfo &info);

private:
    static Napi::FunctionReference constructor;
    Napi::Value UUID(const Napi::CallbackInfo &info);
    void next();
    int now();
    int id_;
    int counter_;
    int nextTime_;
};

#endif