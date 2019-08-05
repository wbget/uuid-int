#ifndef TEST_ADDONS_NAPI_7_FACTORY_WRAP_MYOBJECT_H_
#define TEST_ADDONS_NAPI_7_FACTORY_WRAP_MYOBJECT_H_

#include <node_api.h>

class MyObject
{
public:
  static napi_status Init(napi_env env);
  static void Destructor(napi_env env, void *nativeObject, void *finalize_hint);
  static napi_status NewInstance(napi_env env,
                                 napi_value id,
                                 napi_value seed,
                                 napi_value *instance);

private:
  MyObject();
  ~MyObject();

  static napi_ref constructor;
  static napi_value New(napi_env env, napi_callback_info info);
  napi_env env_;
  napi_ref wrapper_;

  static napi_value UUID(napi_env env, napi_callback_info info);
  int id_;
  int counter_;
  int nextTime_;
  int seed_;
  void next();
  int now();
};

#endif // TEST_ADDONS_NAPI_7_FACTORY_WRAP_MYOBJECT_H_
