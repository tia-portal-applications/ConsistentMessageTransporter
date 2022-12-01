// Copyright [2022] <zhanghao&Dominik>
#pragma once
#include <napi.h>
#include <dmclient.h>
#include "atlstr.h"
#include <string>
#include <uchar.h>

std::string ConnectFunction();
int  MyDMSetValue(TCHAR** TagNames, const int nNum, char** TagValues, napi_value inValues, Napi::Env *env);
int MyDMGetValue(TCHAR** TagNames, const int nNum, napi_value outValues, Napi::Env* env);
std::string Subscribe(TCHAR** nameToSubscribe, const int nNum, Napi::Function* callbackFunction, Napi::Env* env);
static BOOL NotifyVariableProc(DWORD dwTAID, LPDM_VAR_UPDATE_STRUCT lpdmvus, DWORD dwItems, LPVOID lpvUser);
