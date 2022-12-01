// Copyright [2022] <zhanghao&Dominik>
// odk.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "./odk.h"
CMN_ERROR Error;
DWORD m_dwTAID = 0;
TCHAR m_szProjectFile[260];
Napi::Function* g_callbackFunction;
Napi::Env* g_environment;
// change unicode to uft8 for getting odk text8/16 value
char* UnicodeToUtf8(wchar_t* unicode) {
    int len;
    len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);
    char *szUtf8 = reinterpret_cast<char *>(malloc(len + 1));
    memset(szUtf8, 0, len + 1);
    WideCharToMultiByte(CP_UTF8, 0, unicode, -1, szUtf8, len, NULL, NULL);
    return szUtf8;
}
// change uft8 to unicode for setting odk text8/16 value
wchar_t * utf_8ToUnicode(char* szu8) {
  if (NULL == szu8)

  return NULL;

  size_t sSize = strlen(szu8);

  const unsigned char* p = (const unsigned char*) szu8;

  wchar_t* dBuf = new wchar_t[sSize + 1];

  unsigned char* des = (unsigned char*) dBuf;

  memset(des, 0, sizeof(wchar_t) * (sSize + 1));

  while (*p != NULL) {
  if ((UINT16) *p <= 0x7f) {
  *des++ = *p;

  *des++ = 0;

  p++;

  } else if ((UINT16)*p <= 0xcf) {
    unsigned char ch1 = *p++;
    unsigned char ch2 = *p++;
    *des++ = ch1 << 6 | (ch2 & 0x3f);
    *des++ = ch1 >> 2 | 0x07;
    } else if ((UINT16)*p <= 0xef) {
    unsigned char ch1 = *p++;
    unsigned char ch2 = *p++;
    unsigned char ch22 = ch2;
    unsigned char ch3 = *p++;
    *des++ = ch22 << 6 | (ch3 & 0x3f);
    *des++ = ch1 << 4 | (ch2 >> 2 & 0x0f);
    } else {
      break;
    }
  }
  return dBuf;
}
// write string into BSTR
BSTR GetBSTRFromString(char* inputStr) {
  if (inputStr == NULL)
  return NULL;
  wchar_t * pwidstr = utf_8ToUnicode(inputStr);
  BSTR ret = ::SysAllocString(reinterpret_cast<OLECHAR *>(pwidstr));
  delete[] pwidstr;
  return ret;
}
// transform ODK date into string
std::string DoubleTime2Str(double time) {
  // time_t is a timestamps(secs) counted from epoch 1970-1-1(UTL),typeof=int32
  time_t t = time * 24 * 3600 - 2209190400;
  // time is date value in double which intger part stands for the days counted from epoch 1899-12-30(TDatetime);
  // 2209190400 :difference of  timestamps(secs) form epoch 1970-1-1 to 1899-12-30
  // the timestamps overcounted when transferming double date to time_t ,mins the difference timestamp becomes UTL.
  // use tm structure in time.h to store the date and time
  struct tm tm1;
  // set time_t value to tm structure
  localtime_s(&tm1, &t);
  // set a arry to store stringfy date
  char sz[64];
  // initialize memory
  memset(sz, 0, 64);

  // set the string format and fill out the array sz
  sprintf_s(sz, "%04d-%02d-%02d %02d:%02d:%02d",
            tm1.tm_year + 1900,
            tm1.tm_mon + 1,
            tm1.tm_mday,
            tm1.tm_hour,
            tm1.tm_min,
            tm1.tm_sec);
  // retrun the sting sz;
  return std::string(sz);
}
// revers converte string to double date
double StrTime2Double(char *strtime) {
  struct tm tm1;
  time_t time;
  double Date;
  int result = sscanf(strtime, "%4d-%2d-%2d %2d:%2d:%2d",
         &tm1.tm_year,
         &tm1.tm_mon,
         &tm1.tm_mday,
         &tm1.tm_hour,
         &tm1.tm_min,
         &tm1.tm_sec);
  if (result != 6) {
    Date = 0;
  } else if ((tm1.tm_year < 1900) || (tm1.tm_mon > 12) || (tm1.tm_mday > 30) || (tm1.tm_hour > 24) || (tm1.tm_min > 60) ||(tm1.tm_sec > 60)) {
    Date = 1;
  } else {
    tm1.tm_year -= 1900;
    tm1.tm_mon--;
    tm1.tm_isdst = -1;
    time = mktime(&tm1);
    Date = static_cast<double>(time + 2209190400) / 3600 / 24;
  }
  return Date;
}

/**
 * @brief conncet function for odk
 *
 * @return std::string=""
 */
std::string ConnectFunction() {
    BOOL bRet = FALSE;

    CMN_ERROR Error;

    memset(&Error, 0, sizeof(Error));

    // callback DMConnect
    bRet = DMConnect("ODK", NULL, NULL, &Error);

    memset(&Error, 0, sizeof(Error));

    bRet = FALSE;

    DWORD openProjekts = 0;

    bRet = DMEnumOpenedProjects(&openProjekts, NULL, m_szProjectFile, &Error);

    return "";
}
/**
 * @brief ODK set valuefunction
 * 
 * @param TagNames ['testSigned32','testFloat64'...]
 * @param nNum length of TagNames
 * @param TagValues ['123','2.45'...]
 * @param inValues output
 * @param env napi env
 * @return int 
 */
int MyDMSetValue(TCHAR** TagNames, const int nNum, char** TagValues, napi_value inValues, Napi::Env *env) {
    USES_CONVERSION;
    BOOL typeflag = true;
    int length = 0;
    uint32_t index = 0;
    BOOL ret = FALSE;
    napi_value *v = new napi_value;
    const char* t = "true";
    const char* f = "false";
    TCHAR szProjFile[260 + 1];
    DWORD dwSize = _MAX_PATH;
    int* Datatype = new int[nNum];
    std::string err = "";
    std::string date;
    const char *errinfo = "";
    char *pstr = NULL;
    double db;
    int intger;
    DM_VARKEY *VarKey = reinterpret_cast<DM_VARKEY *>(malloc((sizeof(DM_VARKEY) * nNum)));
    VARIANT *VarVal = reinterpret_cast<VARIANT *>(malloc((sizeof(VARIANT) * nNum)));
    DWORD *VarSta = reinterpret_cast<DWORD *>(malloc((sizeof(DWORD) * nNum)));
    LPDM_TYPEREF lpdmTypeRef = new DM_TYPEREF[nNum];
    memset(lpdmTypeRef, 0, sizeof(DM_TYPEREF) * nNum);

    CMN_ERROR Error;
    memset(&Error, 0, sizeof(Error));
    ret = TRUE;

    if (FALSE != ret) {
      memset(&Error, 0, sizeof(Error));
      // RunTime project
      ret = DMGetRuntimeProject(szProjFile, dwSize, &Error);
      if (FALSE != ret) {
        // read tag //fill out DM_VARKEY
        // runtime is done
        for (int i = 0; i < nNum; i++) {
          VarKey[i].dwKeyType = DM_VARKEY_NAME;
          VarKey[i].dwID = 0;
          lstrcpy(VarKey[i].szName, TagNames[i]);
          VarKey[i].lpvUserData = reinterpret_cast<VOID *>(i);
        }
        // get the datyatype
        if (!::DMGetVarType(szProjFile, VarKey, nNum, lpdmTypeRef, &Error)) {
          // DMGetVarType failed
          err = "Get Variable name failed in winccV7.5";
          errinfo = err.c_str();
          length = strlen(errinfo);
          napi_create_string_utf8(*env, errinfo, length, v);
          napi_set_element(*env, inValues, index, *v);
          return 1;
        } else {
          // printf("DMGetVarType successed!");
          for (int iOut = 0; iOut <nNum; iOut++) {
            ret = false;
            // transfer DWORD dwType to int Datatype
            Datatype[iOut] = lpdmTypeRef[iOut].dwType;
            switch (Datatype[iOut]) {
            // BIT
            case 1:
            {
              VarVal[iOut].vt = VT_UI1;
              // tell whether tagvalue is true or false
              if (strcmp(t, TagValues[iOut]) == 0) {
                VarVal[iOut].boolVal = 1;
              } else if (strcmp(f, TagValues[iOut]) == 0) {
                VarVal[iOut].boolVal = 0;
              } else {
                err = "value type is not Bool";
                errinfo = err.c_str();
                length = strlen(errinfo);
                napi_create_string_utf8(*env, errinfo, length, v);
                napi_set_element(*env, inValues, iOut+1, *v);
              }
              break;
            }
            // BYTE
            case 3:
            {
              VarVal[iOut].vt = VT_UI1;
              // transfer char** TagValues to long
              intger = _tcstol(TagValues[iOut], &pstr, 10);
              // check the value type is correct or not
              if (pstr != nullptr && pstr[0] == '\0') {
                // if pstr is NULL
                VarVal[iOut].lVal = intger;
              } else {
                // there are char in Tagvalues,return errormessage
                err = "valuetype is not unsigned short";
                errinfo = err.c_str();
                length = strlen(errinfo);
                napi_create_string_utf8(*env, errinfo, length, v);
                napi_set_element(*env, inValues, iOut + 1, *v);
              }
              break;
            }
            // SBYTE //SWORD
            case 2:
            case 4:
            {
              VarVal[iOut].vt = VT_I2;
              // transfer char** TagValues to long
              intger = _tcstol(TagValues[iOut], &pstr, 10);
              // check the value type is correct or not
              if (pstr != nullptr && pstr[0] == '\0') {
                // if pstr is NULL
                VarVal[iOut].lVal = intger;
                } else {
                  // there are char in Tagvalues,return errormessage
                  err = "valuetype is signed short";
                  errinfo = err.c_str();
                  length = strlen(errinfo);
                  napi_create_string_utf8(*env, errinfo, length, v);
                  napi_set_element(*env, inValues, iOut+1, *v);
                }
                break;
            }
            // SDWORD
            case 5:
            case 6: {
                // transfer char** TagValues to long
                VarVal[iOut].vt = VT_I4;
                intger = _tcstol(TagValues[iOut], &pstr, 10);
                // check the value type is correct or not
                if (pstr != nullptr && pstr[0] == '\0') {
                  if (INT_MIN < intger && intger < INT_MAX) {
                    VarVal[iOut].lVal = intger;
                  } else {
                    err = "value over limited";
                    VarVal[iOut].lVal = 0;
                    errinfo = err.c_str();
                    length = strlen(errinfo);
                    napi_create_string_utf8(*env, errinfo, length, v);
                    napi_set_element(*env, inValues, iOut + 1, *v);
                  }
                } else {
                  // there are char in Tagvalues,return errormessage
                  // reset int to 0;
                  VarVal[iOut].lVal = 0;
                  err = "valuetype is not signed long";
                  errinfo = err.c_str();
                  length = strlen(errinfo);
                  napi_create_string_utf8(*env, errinfo, length, v);
                  napi_set_element(*env, inValues, iOut+1, *v);
                }
                break;
            }
            // DWORD
            case 7: {
              VarVal[iOut].vt = VT_UI4;
              // transfer char** TagValues to long
              intger = _tcstol(TagValues[iOut], &pstr, 10);
              // check the value type is correct or not
              if (pstr != nullptr && pstr[0] == '\0') {
                // if pstr is NULL
                VarVal[iOut].lVal = intger;
              } else {
                // there are char in Tagvalues,return errormessage
                err = "valuetype is not unsigned long";
                errinfo = err.c_str();
                length = strlen(errinfo);
                napi_create_string_utf8(*env, errinfo, length, v);
                napi_set_element(*env, inValues, iOut+1, *v);
              }
              break;
            }
            // FLOAT
            case 8: {
                VarVal[iOut].vt = VT_R4;
                // transfer char** TagValues to double/float
                db = strtod(TagValues[iOut], &pstr);
                // check the value type is correct or not
                if (pstr != nullptr && pstr[0] == '\0') {
                  // if pstr is NULL
                  VarVal[iOut].dblVal = db;
                } else {
                  // there are char in Tagvalues,return errormessage
                  err = "valuetype is not float";
                  errinfo = err.c_str();
                  length = strlen(errinfo);
                  napi_create_string_utf8(*env, errinfo, length, v);
                  napi_set_element(*env, inValues, iOut+1, *v);
                }
                break;
            }
            // DOUBLE
            case 9: {
                VarVal[iOut].vt = VT_R8;
                // transfer char** TagValues to double/float
                db = strtod(TagValues[iOut], &pstr);
                // check the value type is correct or not
                if (pstr != nullptr && pstr[0] == '\0') {
                  VarVal[iOut].dblVal = db;
                } else {
                  // there are char in Tagvalues,return errormessage
                  err = "valuetype is not double";
                  errinfo = err.c_str();
                  length = strlen(errinfo);
                  napi_create_string_utf8(*env, errinfo, length, v);
                  napi_set_element(*env, inValues, iOut+1, *v);
                }
                break;
            }
            // TEXT_8
            case 10: {
              VarVal[iOut].vt = VT_BSTR;
              // transfer utf8 char* to unicode bstr
              VarVal[iOut].bstrVal = GetBSTRFromString(TagValues[iOut]);
              break;
            }
            // TEXT_16
            case 11: {
              VarVal[iOut].vt = VT_BSTR;
              VarVal[iOut].bstrVal = GetBSTRFromString(TagValues[iOut]);
              break;
            }
            // Datetime
            case 19: {
              VarVal[iOut].vt = VT_DATE;
              // converter stringtime to double
              db = StrTime2Double(TagValues[iOut]);
              // tell output double value
              if (db == 0) {
                err = "incorrect Date format";
              } else if (db == 1) {
                err = "Date value unnormal";
              } else {
                VarVal[iOut].date = db;
                break;
              }
              errinfo = err.c_str();
              length = strlen(errinfo);
              napi_create_string_utf8(*env, errinfo, length, v);
              napi_set_element(*env, inValues, iOut + 1, *v);
              break;
            }
            default:
              break;
            }  // end switch case
          }   // end for (all tag setted value)
        }     // end else
        // set tag
        memset(&Error, 0, sizeof(CMN_ERROR));

        ret = DMSetValue(VarKey, nNum, VarVal, VarSta, &Error);
        } else {
          // connection error
           err = "wincc Runtime connection failed";
           errinfo = err.c_str();
           length = strlen(errinfo);
           napi_value *v = new napi_value;
           napi_create_string_utf8(*env, errinfo, length, v);
           napi_set_element(*env, inValues, index, *v);
           return 1;
        }
        }
        delete[] lpdmTypeRef;
        delete[] VarKey;
        return 1;
}
/**
 * @brief ODK get value function
 *
 * @param TagNames ['testSigned32','testFloat64'...]
 * @param nNum length of TagNames
 * @param outValues output
 * @param env napi env
 * @return int
 */
int MyDMGetValue(TCHAR** TagNames, const int nNum, napi_value outValues, Napi::Env *env) {
    // declaer parameter in function mbrtoc16() to transfer char to char16_t
    mbstate_t state;

    USES_CONVERSION;

    CMN_ERROR Error;

    BOOL ret = FALSE;

    DWORD dwSize = _MAX_PATH;


    TCHAR szProjFile[260 + 1];
    int length = 0;
    uint32_t index = 0;
    std::string err = "";
    const char *errinfo = "";
    DM_VARKEY *VarKey = reinterpret_cast<DM_VARKEY *>(malloc((sizeof(DM_VARKEY) * nNum)));
    DM_VAR_UPDATE_STRUCT *VarUp = reinterpret_cast<DM_VAR_UPDATE_STRUCT *>(malloc((sizeof(DM_VAR_UPDATE_STRUCT) * nNum)));
    // the included VARIANT's in the DM_VAR_UPDATE_STRUCT's then initialized to VT_EMPTY with the memset,

    // don't do this later again, because VT_BSTR's can be present after DMGetValue(...)!

    memset(&Error, 0, sizeof(Error));

    /*ret = MyDMGetConnectionState();*/
    // because function not included before
    ret = TRUE;

    if (FALSE != ret) {
        memset(&Error, 0, sizeof(Error));

        // RunTime project

        ret = DMGetRuntimeProject(szProjFile, dwSize, &Error);

        if (FALSE != ret) {
         // fill out DM_VARKEY

            for (int iRead = 0; iRead < nNum; iRead++) {
                VarKey[iRead].dwKeyType = DM_VARKEY_NAME;

                VarKey[iRead].dwID = 0;

                lstrcpy(VarKey[iRead].szName, &TagNames[iRead][0]);

                VarKey[iRead].lpvUserData = reinterpret_cast<VOID *>(iRead);
            }

            memset(&Error, 0, sizeof(Error));

            ret = DMGetValue(VarKey, nNum, VarUp, &Error);

            if (FALSE != ret) {
                for (int iOut = 0; iOut < nNum; iOut++) {
                    switch (VarUp[iOut].dmTypeRef.dwType) {
                    // vt = 3 VT_I4 = 3
                    case DM_VARTYPE_BIT: {
                      napi_value v;
                      // napi_get_boolean is a golbal instance function support c-node/node-c
                      napi_get_boolean(*env, VarUp[iOut].dmValue.boolVal, &v);
                      napi_set_element(*env, outValues, iOut+1, v);
                        break;
                    }
                    // vt = 17 VT_UI1 = 17
                    case DM_VARTYPE_BYTE:
                    // vt = 3 VT_I4 = 3 //WORD=unsigned short=int16
                    case DM_VARTYPE_WORD:

                    // vt = 5 VT_R8 = 5
                    case DM_VARTYPE_DWORD: {
                        // DWORD=unsigned long=uint32
                        napi_value* v = new napi_value;
                        napi_create_uint32(*env, VarUp[iOut].dmValue.ulVal, v);
                        napi_set_element(*env, outValues, iOut+1, *v);
                        break;
                    }
                    // vt = 2 VT_I2 = 2 //SWORD=signed short=int16
                    case DM_VARTYPE_SBYTE:
                    case DM_VARTYPE_SWORD:
                    // vt = 3 VT_I4 = 3 //SDWORD=signed long=int32
                    case DM_VARTYPE_SDWORD: {
                         napi_value* v = new napi_value;
                         // int64 is not apply to long, use int32 instead
                        napi_create_int32(*env, VarUp[iOut].dmValue.lVal, v);
                        napi_set_element(*env, outValues, iOut+1, *v);
                        break;
                    }
                      // vt = 4 VT_R4 = 4
                    case DM_VARTYPE_FLOAT:
                      // vt = 5 VT_R8 = 5
                    case DM_VARTYPE_DOUBLE: {
                         napi_value* v = new napi_value;
                         napi_create_double(*env, VarUp[iOut].dmValue.dblVal, v);
                         napi_set_element(*env, outValues, iOut+1, *v);
                        break;
                    }
                      // vt = 8 VT_BSTR = 8
                    case DM_VARTYPE_TEXT_8: {
                        // the functiion transfer Unicode-wchar* to utf8-char,when read bstr,wchar*=bstr
                        char* cCharUtf = UnicodeToUtf8(VarUp[iOut].dmValue.bstrVal);
                        size_t length = strlen(cCharUtf);
                        // set char[] to napi_value and pass to index.cpp as output
                        napi_value* v = new napi_value;

                        napi_create_string_utf8(*env, cCharUtf, length, v);

                        napi_set_element(*env, outValues, iOut+1, *v);

                        break;
                    }
                    // vt = 8 VT_BSTR = 8
                    case DM_VARTYPE_TEXT_16: {
                        // the functiion transfer Unicode-wchar* to utf8-char,when read bstr,wchar*=bstr
                        char* cCharUtf = UnicodeToUtf8(VarUp[iOut].dmValue.bstrVal);
                        size_t length = strlen(cCharUtf);
                        // number of char[]
                        const size_t in_sz = sizeof cCharUtf / sizeof * cCharUtf;
                        // length of stringchar
                        // start to transfer char to char16_t !!
                        char16_t out[in_sz + 128];
                        char *p_in = cCharUtf;
                        char16_t* p_out = out;
                        int rc;
                        // function of transform from char to char16_t
                        while ((rc = mbrtoc16(p_out, p_in, length, &state))) {
                          if (rc == -3) {
                            p_out += 1;
                          } else if (rc > 0) {
                            p_in += rc;
                            p_out += 1;
                          } else {
                            break;
                          }
                        }
                        // the length of Char16_T string
                        size_t out_sz = p_out - out;
                        // set char16_t[] to napi_value and pass to napi.cpp as output
                        napi_value* v = new napi_value;

                        napi_create_string_utf16(*env, out, out_sz, v);

                        napi_set_element(*env, outValues, iOut+1, *v);

                        break;
                    }
                    // double date is a TDatetime value in Delphi since December 30, 1899
                    case DM_VARTYPE_DATETIME: {
                        double t0 = VarUp[iOut].dmValue.date;
                        // call oleTime2Str(),encoding the Doule and retrun a string format"yyyy/mm/dd hh//mm//ss"
                        std::string datestring = DoubleTime2Str(t0);
                        // convert string datestring to const char* date
                        const char* date = reinterpret_cast<const char* >(datestring.data());

                        int length = strlen(date);

                        // set const char* to napi_value and pass to napi.cpp as output
                        napi_value* v = new napi_value;

                        napi_create_string_utf8(*env, date, length, v);

                        napi_set_element(*env, outValues, iOut+1, *v);

                         break;
                    }
                    default:
                    // printf(" unidenfified ODK Datatype");
                        break;
                    }  // end switch case

                    VariantClear(&VarUp[iOut].dmValue);
                }  // end for
            } else {
                printf("Error in DMGetValue\n");
            }

        } else {
            printf("Error in DMGetRuntimeProject\n");
             err = "wincc Runtime connection failed";
           errinfo = err.c_str();
           length = strlen(errinfo);
           napi_value *v = new napi_value;
           napi_create_string_utf8(*env, errinfo, length, v);
           napi_set_element(*env, outValues, index, *v);
        }

    } else {
        printf("Error in MyDMGetConnectionState\n");
    }
    return 1;
}
/**
 * @brief DM_NOTIFY_VARIABLE_PROC callback function for data transfer through the data manager.
 *
 * @param dwTAID Transaction ID assigned to the calling function by the data manager.
 * @param lpdmvus Pointer to the first structure of the DM_VAR_UPDATE_STRUCT type containing the values of the requested tags.
 * @param dwItems Number of structures passed in lpdmvus (identical with the number of tag values).
 * @param lpvUser Pointer to application-specific data. This pointer is made available again within the callback function
 * @return BOOL true or false
 */
BOOL NotifyVariableProc(DWORD dwTAID, LPDM_VAR_UPDATE_STRUCT lpdmvus, DWORD dwItems, LPVOID lpvUser) {
  std::string changedName = "";
  napi_value v;
  napi_value newtag;
  for (DWORD i = 0; i < dwItems; i++) {
    LPDM_VAR_UPDATE_STRUCT lpdmvus2 = &lpdmvus[i];

    if (lpdmvus2->dmValue.vt == (VT_ARRAY | VT_UI1)) {
      LPBYTE pArray;
      HRESULT hr;

          hr = SafeArrayAccessData(lpdmvus2->dmValue.parray, reinterpret_cast<VOID **>(&pArray));
      if (!FAILED(hr)) {
        printf("Var:%s: Type:Rohdatum\nValues:%02x %02x %02x %02x\n",
                lpdmvus2->dmVarKey.szName,
                pArray[0],
                pArray[1],
                pArray[2],
                pArray[3]);
        SafeArrayUnaccessData(lpdmvus2->dmValue.parray);
      } else {
        printf("SafeArrayAccessData failed\n");
      }
    } else {
      if (lpdmvus2->dmValue.vt == 3) {
        // get tag name as callback
        changedName = lpdmvus2->dmVarKey.szName;
        const char *newName = reinterpret_cast<const char *>(changedName.data());
        int length = strlen(newName);
        // transfer tagname and tagvalue into napi value
        napi_create_string_utf8(*g_environment, newName, length, &newtag);
        napi_create_int64(*g_environment, lpdmvus2->dmValue.iVal, &v);
      }
      if (lpdmvus2->dmValue.vt == 5) {
        changedName = lpdmvus2->dmVarKey.szName;
        const char *newName = reinterpret_cast<const char *>(changedName.data());
        int length = strlen(newName);
        napi_create_string_utf8(*g_environment, newName, length, &newtag);
        napi_create_double(*g_environment, lpdmvus2->dmValue.dblVal, &v);
      }
      if (lpdmvus2->dmValue.vt == 8) {
        changedName = lpdmvus2->dmVarKey.szName;
        const char *newName = reinterpret_cast<const char *>(changedName.data());
        int length = strlen(newName);
        napi_create_string_utf8(*g_environment, newName, length, &newtag);
        char *cCharUtf = UnicodeToUtf8(lpdmvus2->dmValue.bstrVal);
        int length2 = strlen(cCharUtf);
        napi_create_string_utf8(*g_environment, cCharUtf, length2, &v);
      }
      if (lpdmvus2->dmValue.vt == 7) {
        changedName = lpdmvus2->dmVarKey.szName;
        const char *newName = reinterpret_cast<const char *>(changedName.data());
        int length = strlen(newName);
        napi_create_string_utf8(*g_environment, newName, length, &newtag);
        std::string datestring = DoubleTime2Str(lpdmvus2->dmValue.date);
        const char *date = reinterpret_cast<const char *>(datestring.data());
        int length3 = strlen(date);
        napi_create_string_utf8(*g_environment, date, length3, &v);
      }
      if (lpdmvus2->dmValue.vt == 11) {
        changedName = lpdmvus2->dmVarKey.szName;
        const char *newName = reinterpret_cast<const char *>(changedName.data());
        int length = strlen(newName);
        napi_create_string_utf8(*g_environment, newName, length, &newtag);
        napi_get_boolean(*g_environment, lpdmvus2->dmValue.boolVal, &v);
      }
     }   // end else
    }  // end for
    // set callback content(uptag, upvalue)
    g_callbackFunction->Call({newtag, v});
    return  TRUE;
}
/**
 * @brief odk subscribe funtion do while to 
 * 
 * @param nameToSubscribe ['testSignede32','testFloat64'..]
 * @param nNum length of nameToSubscribe
 * @param callbackFunction napi callbackfuntion
 * @param env napi env
 * @return std::string 
 */
std::string Subscribe(TCHAR** nameToSubscribe, const int nNum , Napi::Function* callbackFunction, Napi::Env *env) {
    g_callbackFunction = callbackFunction;
    g_environment = env;
    memset(&Error, 0, sizeof(CMN_ERROR));
    if (!DMBeginStartVarUpdate(&m_dwTAID, &Error)) {
        printf("error\n");
    } else {
        // printf("success BeginStartVarUpdate\n");
    }

    LPDM_VARKEY lpdmVarKey = new DM_VARKEY[nNum];
    for (int i = 0; i < nNum; i++) {
        lpdmVarKey[i].dwKeyType = DM_VARKEY_NAME;
        lpdmVarKey[i].dwID = 0;   // is always 0
        lstrcpy(lpdmVarKey[i].szName, nameToSubscribe[i]);
        lpdmVarKey[i].lpvUserData = reinterpret_cast<VOID *>(i);
    }
    DWORD dwCycle = 0;
    memset(&Error, 0, sizeof(CMN_ERROR));

    // call DMStartVarUpdate function in ODK
    if (!::DMStartVarUpdate(m_dwTAID, lpdmVarKey, nNum, dwCycle,
        NotifyVariableProc, NULL, &Error)) {
        printf("failed\n");
    } else {
        // printf("DMStartVarUpdate ok.\n");
    }
    if (!DMEndStartVarUpdate(m_dwTAID, &Error)) {
        printf("error\n");
    } else {
        // printf("end startVarUpdate succeeded\n");
    }
    return "";
}


