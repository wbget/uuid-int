#include <napi.h>
#include <ctime>
// #include <iostream>

#include "generator.h"

using namespace Napi;
// using namespace std;

Napi::FunctionReference Generator::constructor;

Napi::Object Generator::Init(Napi::Env env, Napi::Object exports)
{
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "Generator", {InstanceMethod("uuid", &Generator::UUID)});

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("Generator", func);
    return exports;
}

Generator::Generator(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Generator>(info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    this->id_ = info[0].As<Napi::Number>().Int32Value();
    this->next();
};

void Generator::next()
{
    this->counter_ = 0;
    this->nextTime_ = this->now() + 1;
}
int Generator::now()
{
    return time(0);
}

Napi::Object Generator::NewInstance(Napi::Env env, Napi::Value arg)
{
    Napi::EscapableHandleScope scope(env);
    Napi::Object obj = constructor.New({arg});
    obj.Set(Napi::String::New(env, "id"), arg);
    return scope.Escape(napi_value(obj)).ToObject();
}
Napi::Value Generator::UUID(const Napi::CallbackInfo &info)
{
    // 32位时间+9位标示+12位自增 = 53位
    Napi::Env env = info.Env();
    time_t now = this->now();
    if (now < this->nextTime_)
    {
        if (this->counter_ > 4095)
        {
            Napi::RangeError::New(env, "ids MAX, 4096/s").ThrowAsJavaScriptException();
            return Napi::Number::New(env, -1);
        }
    }
    else
    {
        this->next();
    }

    uint64_t time = (this->nextTime_ & 0x1FFFFFFFF) << 21;
    uint64_t id = (this->id_ & 0x1FF) << 12;
    uint64_t count = this->counter_ & 0xFFF;
    uint64_t uuid = time + id + count;

    // cout << this->nextTime_ << ',' << time << ',' << id << ',' << count << ',' << endl;
    this->counter_++;
    Napi::Number result = Napi::Number::New(env, uuid);
    return result;
}