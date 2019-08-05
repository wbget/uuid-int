#include "myobject.h"
#include <assert.h>
#include <iostream>
#include <ctime>

using namespace std;

MyObject::MyObject() : env_(nullptr), wrapper_(nullptr) {}

MyObject::~MyObject() { napi_delete_reference(env_, wrapper_); }

void MyObject::Destructor(napi_env env, void *nativeObject, void * /*finalize_hint*/)
{
    reinterpret_cast<MyObject *>(nativeObject)->~MyObject();
}

#define DECLARE_NAPI_METHOD(name, func)         \
    {                                           \
        name, 0, func, 0, 0, 0, napi_default, 0 \
    }

napi_ref MyObject::constructor;

napi_status MyObject::Init(napi_env env)
{
    napi_status status;
    napi_property_descriptor properties[] = {
        DECLARE_NAPI_METHOD("uuid", UUID),
    };

    napi_value cons;
    status =
        napi_define_class(env, "MyObject", NAPI_AUTO_LENGTH, New, nullptr, 1, properties, &cons);
    if (status != napi_ok)
        return status;

    status = napi_create_reference(env, cons, 1, &constructor);
    if (status != napi_ok)
        return status;

    return napi_ok;
}

napi_value MyObject::New(napi_env env, napi_callback_info info)
{
    napi_status status;

    size_t argc = 2;
    napi_value args[2];
    napi_value jsthis;
    status = napi_get_cb_info(env, info, &argc, args, &jsthis, nullptr);
    assert(status == napi_ok);

    MyObject *obj = new MyObject();

    status = napi_get_value_int32(env, args[0], &obj->id_);
    assert(status == napi_ok);
    cout << obj->id_ << endl;

    status = napi_get_value_int32(env, args[1], &obj->seed_);
    assert(status == napi_ok);
    cout << obj->seed_ << endl;
    obj->next();

    obj->env_ = env;
    status = napi_wrap(env,
                       jsthis,
                       reinterpret_cast<void *>(obj),
                       MyObject::Destructor,
                       nullptr, /* finalize_hint */
                       &obj->wrapper_);
    assert(status == napi_ok);

    return jsthis;
}

napi_status MyObject::NewInstance(napi_env env,
                                  napi_value id,
                                  napi_value seed,
                                  napi_value *instance)
{
    napi_status status;

    const int argc = 2;
    napi_value argv[argc] = {id, seed};

    napi_value cons;
    status = napi_get_reference_value(env, constructor, &cons);
    if (status != napi_ok)
        return status;

    status = napi_new_instance(env, cons, argc, argv, instance);
    if (status != napi_ok)
        return status;

    return napi_ok;
}

napi_value MyObject::UUID(napi_env env, napi_callback_info info)
{
    napi_status status;

    napi_value jsthis;
    status = napi_get_cb_info(env, info, nullptr, nullptr, &jsthis, nullptr);
    assert(status == napi_ok);

    MyObject *obj;
    status = napi_unwrap(env, jsthis, reinterpret_cast<void **>(&obj));
    assert(status == napi_ok);

    time_t now = obj->now();
    if (now < obj->nextTime_)
    {
        if (obj->counter_ > 4095)
        {
            napi_throw_range_error(env, "1", "uuid out of range");
        }
    }
    else
    {
        obj->next();
    }

    uint64_t time = (obj->nextTime_ & 0x1FFFFFFFF) << 21;
    uint64_t id = (obj->id_ & 0x1FF) << 12;
    uint64_t count = obj->counter_ & 0xFFF;
    uint64_t uuid = time + id + count;

    obj->counter_++;

    napi_value num;
    status = napi_create_double(env, uuid, &num);
    if (status != napi_ok)
    {
        napi_throw_range_error(env, "2", "uuid error");
    }

    return num;
}

void MyObject::next()
{
    this->counter_ = 0;
    this->nextTime_ = this->now() + 1;
}
int MyObject::now()
{
    return time(0) - this->seed_;
}