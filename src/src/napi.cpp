// Copyright [2022] <zhanghao&Dominik>
#include <napi.h>
#include <odk.h>

// global flag set false to subscribing
bool g_cb_Ready = false;
// napi connect function
Napi::String connectFunc(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    // call odk connection in odk.cpp
    std::string result = ConnectFunction();
    return Napi::String::New(env, result);
}
// napi write function
napi_value setValue(const Napi::CallbackInfo &info) {
  // get the parameters infos,array a for tagname, b for tagvalue
  Napi::Env env = info.Env();
  Napi::Array a = info[0].As<Napi::Array>();
  Napi::Array b = info[1].As<Napi::Array>();

  uint32_t arraylength = a.Length();

  // declear the formal parameters and dynamiclly allocate the memory
  TCHAR **TagNames = reinterpret_cast<TCHAR **>(malloc((sizeof(TCHAR *) * arraylength)));
  // declear TagValue as char but not the tchar!
  char **TagValues = reinterpret_cast<char **>(malloc((sizeof(char *) * arraylength)));

  char buffer1[128];
  char buffer2[128];
  size_t buffer1_size = 128;
  size_t buffer2_size = 128;
  size_t copied;

  // create 2 buffers and pass the infs to formal parameters
  for (uint32_t i = 0; i < arraylength; i++) {
    Napi::Value v1 = a[i];
    Napi::Value v2 = b[i];
    // get napi value into buffer
    napi_get_value_string_utf8(env, v1.ToString(), buffer1, buffer1_size, &copied);
    napi_get_value_string_utf8(env, v2.ToString(), buffer2, buffer2_size, &copied);

    // fill out each parameters from buffer
    TagNames[i] = reinterpret_cast<TCHAR *>(malloc((sizeof(TCHAR) * buffer1_size)));
    _tcscpy_s(TagNames[i], buffer1_size, buffer1);

    TagValues[i] = reinterpret_cast<char *>(malloc((sizeof(char) * buffer2_size)));
    // in order to use strcpy_s() third parameter must be const char*!
    const char *buffer4 = buffer2;
    strcpy_s(TagValues[i], buffer2_size, buffer4);
    }
    // set retrun sring null
    std::string result1 = "";
    // const inValues = [];
    napi_value inValues;
    napi_create_array(env, &inValues);
    result1 = MyDMSetValue(TagNames, arraylength, TagValues, inValues, &env);

  // memory free
  for (uint32_t i = 0; i < arraylength; i++) {
      free(TagNames[i]);
      free(TagValues[i]);
     }
    free(TagNames);
    free(TagValues);
    return inValues;
}
// napi read function returns an array
napi_value getValue(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();

    Napi::Array a = info[0].As<Napi::Array>();
    uint32_t arraylength = a.Length();

    TCHAR **TagNames = reinterpret_cast<TCHAR **>(malloc((sizeof(TCHAR *) * arraylength)));

    for (uint32_t i = 0; i < arraylength; i++) {
        Napi::Value v = a[i];
        char buffer[128];
        size_t buffer_size = 128;
        size_t copied;
        napi_get_value_string_utf8(env, v.ToString(), buffer, buffer_size, &copied);
        TagNames[i] = reinterpret_cast<TCHAR *>(malloc((sizeof(TCHAR) * buffer_size)));
        _tcscpy_s(TagNames[i], buffer_size, buffer);
    }
    std::string result = "";
    // const outValues = [];
    napi_value outValues;
    napi_create_array(env, &outValues);
    result = MyDMGetValue(TagNames, arraylength, outValues, &env);
    
        /* free resources */
        for (uint32_t i = 0; i < arraylength; i++)
    {
      free(TagNames[i]);
    }
    free(TagNames);
    // outValues/res=[result1,resutl2,.....result3];
    return outValues;
}
// napi subscribe function returns callback info
Napi::String subscribeValue(const Napi::CallbackInfo &info) {
  std::string result = "";
  Napi::Env env = info.Env();
  Napi::Array a = info[0].As<Napi::Array>();
  uint32_t arraylength = a.Length();
  TCHAR **nameToSubscribe = reinterpret_cast<TCHAR **>(malloc((sizeof(TCHAR *) * arraylength)));
  for (uint32_t i = 0; i < arraylength; i++) {
    Napi::Value v = a[i];
    char buffer[128];
    size_t buffer_size = 128;
    size_t copied;
    napi_get_value_string_utf8(env, v.ToString(), buffer, buffer_size, &copied);
    nameToSubscribe[i] = reinterpret_cast<TCHAR *>(malloc((sizeof(TCHAR) * buffer_size)));
    _tcscpy_s(nameToSubscribe[i], buffer_size, buffer);
    }
    // callbackFunction
    Napi::Function callbackFunction = info[1].As<Napi::Function>();

    bool first = true;
    MSG msg;
    do {
        Sleep(200);

        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
          // translates virtual-key messages into character messages
          TranslateMessage(&msg);
          // dispatches a message to a window procedure
          DispatchMessage(&msg);
        }

        if (first) {
            first = false;
            Subscribe(nameToSubscribe, arraylength, &callbackFunction, &env);
        }
    } while (!g_cb_Ready);  // has to be set to true to finish cleanly
    return Napi::String::New(env, result);
}
// napi init config
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(
        Napi::String::New(env, "connectFunc"),
        Napi::Function::New(env, connectFunc));

    exports.Set(
        Napi::String::New(env, "subscribeValue"),
        Napi::Function::New(env, subscribeValue));
    exports.Set(
        Napi::String::New(env, "setValue"),
        Napi::Function::New(env, setValue));
    exports.Set(
        Napi::String::New(env, "getValue"),
        Napi::Function::New(env, getValue));
    return exports;
}
// napi module config
NODE_API_MODULE(odk, Init)
