//-------------------------------------------------------------------------------
//
//  DMCLIENT.H
//
//  Copyright (c) Siemens AG 1994 - 2011
//
//-------------------------------------------------------------------------------

#ifndef __DMCLIENT_H__
#define __DMCLIENT_H__


#ifndef _WINDOWS_
  #include <WINDOWS.H>
#endif

#ifndef OHIOAPI_H
  #include "OHIOAPI.H"
#endif

#ifndef __DMDEFS_H__
  #include "DMDEFS.H"
#endif

#ifndef __LICENSE_H__
  #include "LICENSE.H"
#endif

#ifdef _MSC_VER
  #pragma pack (push, DMCLIENT_Packing_Alignment)
#endif

#pragma pack (1)


// Definition von __OUTSIDE_CISS__ fuer alle callback-Funktionen
// wird fuer AP gebraucht

#ifndef __OUTSIDE_CISS__
    #ifdef __CISS__
        #define __OUTSIDE_CISS__ __outside
    #else
        #define __OUTSIDE_CISS__
    #endif
#endif


/*-----------------------------------------------------------------------
 * Operating-Systems
 */
#define DM_OS_UNKNOWN           0
#define DM_OS_NT                1       /* Windows NT */
#define DM_OS_32S               2       /* Windows 3.x mit Win 32s */
#define DM_OS_CHICAGO           3       /* Windows 95 */
#define DM_OS_2000              4       /* Windows 2000 */
#define DM_OS_XP                5       /* Windows XP */
#define DM_OS_2003              6       /* Windows 2003 */

// V7.3.1 newer OS'es
#define DM_OS_VISTA_32             0x06001   /* Windows Vista 32Bit */
#define DM_OS_VISTA_SP1_32         0x06011   /* Windows Vista SP1 32Bit */
#define DM_OS_VISTA_SP2_32         0x06021   /* Windows Vista SP2 32Bit */
#define DM_OS_VISTA_64             0x96001   /* Windows Vista 64Bit */
#define DM_OS_VISTA_SP1_64         0x96011   /* Windows Vista SP1 64Bit */
#define DM_OS_VISTA_SP2_64         0x96021   /* Windows Vista SP2 64Bit */

#define DM_OS_S2008_32             0x06003   /* Windows Server 2008 32Bit */
#define DM_OS_S2008_SP1_32         0x06013   /* Windows Server 2008 SP1 32Bit */
#define DM_OS_S2008_SP2_32         0x06023   /* Windows Server 2008 SP2 32Bit */
#define DM_OS_S2008_64             0x96003   /* Windows Server 2008 64Bit */
#define DM_OS_S2008_SP1_64         0x96013   /* Windows Server 2008 SP1 64Bit */
#define DM_OS_S2008_SP2_64         0x96023   /* Windows Server 2008 SP2 64Bit */

#define DM_OS_W7_32                0x06101   /* Windows 7 32Bit */
#define DM_OS_W7_SP1_32            0x06111   /* Windows 7 SP1 32Bit */
#define DM_OS_W7_64                0x96101   /* Windows 7 64Bit */
#define DM_OS_W7_SP1_64            0x96111   /* Windows 7 SP1 64Bit */

#define DM_OS_S2008R2_32           0x06103   /* Windows Server 2008 R2 32Bit */
#define DM_OS_S2008R2_SP1_32       0x06113   /* Windows Server 2008 R2 SP1 32Bit */
#define DM_OS_S2008R2_64           0x96103   /* Windows Server 2008 R2 64Bit */
#define DM_OS_S2008R2_SP1_64       0x96113   /* Windows Server 2008 R2 SP1 64Bit */

#define DM_OS_W8_32                0x06201   /* Windows 8 32Bit */
#define DM_OS_W8_64                0x96201   /* Windows 8 64Bit */

#define DM_OS_S2012_32             0x06203   /* Windows Server 2012 32Bit */
#define DM_OS_S2012_64             0x96203   /* Windows Server 2012 64Bit */

#define DM_OS_W81_32               0x06301   /* Windows 8.1 32Bit */
#define DM_OS_W81_64               0x96301   /* Windows 8.1 64Bit */

#define DM_OS_S2012R2_32           0x06303   /* Windows Server 2012 R2 32Bit */
#define DM_OS_S2012R2_64           0x96303   /* Windows Server 2012 R2 64Bit */

#define DM_OS_W10_64               0x96401   /* Windows 10 64Bit */

#define DM_OS_S2016_64             0x96403   /* Windows Server 2016 64Bit */

// if the returned value of DMGetOSVersionW() is not defined here (see since 7.3.1) can also be like:
// 0x0,0,0,"ProcessorArchitecture(32/64Bit)","MajorVersion","MinorVersion","ServicePackMajor","ProductType"
// take also a look to the definitions of OSVERSIONINFOEX and SYSTEM_INFO structure for the given parts and coded in nibbles

/*
 * Name of the DLL
 */
#define NAME_DMCLIENTDLL        _T("DMCLIENT.DLL")


/*
 * VARIANT
 */
typedef VARIANT*       PVARIANT;
typedef VARIANT NEAR*  NPVARIANT;
typedef VARIANT FAR*   LPVARIANT;

/*
 * Description of a variable's type
 */
typedef struct tagDM_TYPEREFA
{
    DWORD       dwType;         /* See constants DM_TYPEREF_xxx */
    DWORD       dwSize;         /* size of type in byte */
    char        szTypeName[ MAX_DM_TYPE_NAME + 1 ];     /* name of type */
} DM_TYPEREFA, FAR *LPDM_TYPEREFA;

typedef struct tagDM_TYPEREFW
{
    DWORD       dwType;         /* See constants DM_TYPEREF_xxx */
    DWORD       dwSize;         /* size of type in byte */
    WCHAR       szTypeName[ MAX_DM_TYPE_NAME + 1 ];     /* name of type */
} DM_TYPEREFW, FAR *LPDM_TYPEREFW;


#ifdef UNICODE
typedef DM_TYPEREFW     DM_TYPEREF;
typedef LPDM_TYPEREFW   LPDM_TYPEREF;
#else
typedef DM_TYPEREFA     DM_TYPEREF;
typedef LPDM_TYPEREFA   LPDM_TYPEREF;
#endif

/*
 * Description of a variable
 */
typedef struct tagDM_VARKEYA
{
    DWORD       dwKeyType;           /* which key is valid */
    DWORD       dwID;                /* Key: Varaible-ID */
    char        szName[ MAX_DM_VAR_NAME + 1 ];       /* Key: Variable-Name */
    LPVOID      lpvUserData;         /* User-specfic field */
} DM_VARKEYA, FAR *LPDM_VARKEYA;
typedef struct tagDM_VARKEYW
{
    DWORD       dwKeyType;           /* which key is valid */
    DWORD       dwID;                /* Key: Varaible-ID */
    WCHAR       szName[ MAX_DM_VAR_NAME + 1 ];       /* Key: Variable-Name */
    LPVOID      lpvUserData;         /* User-specfic field */
} DM_VARKEYW, FAR *LPDM_VARKEYW;


#ifdef UNICODE
typedef DM_VARKEYW      DM_VARKEY;
typedef LPDM_VARKEYW    LPDM_VARKEY;
#else
typedef DM_VARKEYA      DM_VARKEY;
typedef LPDM_VARKEYA    LPDM_VARKEY;
#endif

/*
 * Description of Update-data
 */
typedef struct tagDM_VAR_UPDATE_STRUCTA
{
    DM_TYPEREFA         dmTypeRef;      /* Type of variable-data */
    DM_VARKEYA          dmVarKey;       /* Variable-Access-Key */
    VARIANT             dmValue;        /* Update-Value */
    DWORD               dwState;        /* State of Variable and value */
} DM_VAR_UPDATE_STRUCTA, FAR *LPDM_VAR_UPDATE_STRUCTA;

typedef struct tagDM_VAR_UPDATE_STRUCTW
{
    DM_TYPEREFW         dmTypeRef;      /* Type of variable-data */
    DM_VARKEYW          dmVarKey;       /* Variable-Access-Key */
    VARIANT             dmValue;        /* Update-Value */
    DWORD               dwState;        /* State of Variable and value */
} DM_VAR_UPDATE_STRUCTW, FAR *LPDM_VAR_UPDATE_STRUCTW;

#ifdef UNICODE
typedef DM_VAR_UPDATE_STRUCTW   DM_VAR_UPDATE_STRUCT;
typedef LPDM_VAR_UPDATE_STRUCTW LPDM_VAR_UPDATE_STRUCT;
#else
typedef DM_VAR_UPDATE_STRUCTA   DM_VAR_UPDATE_STRUCT;
typedef LPDM_VAR_UPDATE_STRUCTA LPDM_VAR_UPDATE_STRUCT;
#endif

/*
 * Description of Update-data
 */
typedef struct tagDM_VAR_UPDATE_STRUCTEXA
{
    DM_TYPEREFA         dmTypeRef;      /* Type of variable-data */
    DM_VARKEYA          dmVarKey;       /* Variable-Access-Key */
    VARIANT             dmValue;        /* Update-Value */
    DWORD               dwState;        /* State of Variable and value */
    DWORD               dwQualityCode;  /* Quality code of Variable and value */
} DM_VAR_UPDATE_STRUCTEXA, FAR *LPDM_VAR_UPDATE_STRUCTEXA;

typedef struct tagDM_VAR_UPDATE_STRUCTEXW
{
    DM_TYPEREFW         dmTypeRef;      /* Type of variable-data */
    DM_VARKEYW          dmVarKey;       /* Variable-Access-Key */
    VARIANT             dmValue;        /* Update-Value */
    DWORD               dwState;        /* State of Variable and value */
    DWORD               dwQualityCode;  /* Quality code of Variable and value */
} DM_VAR_UPDATE_STRUCTEXW, FAR *LPDM_VAR_UPDATE_STRUCTEXW;

#ifdef UNICODE
typedef DM_VAR_UPDATE_STRUCTEXW   DM_VAR_UPDATE_STRUCTEX;
typedef LPDM_VAR_UPDATE_STRUCTEXW LPDM_VAR_UPDATE_STRUCTEX;
#else
typedef DM_VAR_UPDATE_STRUCTEXA   DM_VAR_UPDATE_STRUCTEX;
typedef LPDM_VAR_UPDATE_STRUCTEXA LPDM_VAR_UPDATE_STRUCTEX;
#endif

/*
 * DM_DIRECTORY_INFO
 */
typedef struct tagDM_DIRECTORY_INFOA
{
    char        szProjectDir[ _MAX_PATH + 1 ];          /* Path of global data */
    char        szProjectAppDir[ _MAX_PATH + 1 ];       /* Application-Path of project data */
    char        szGlobalLibDir[ _MAX_PATH + 1 ];        /* Application-Path of global libs */
    char        szProjectLibDir[ _MAX_PATH + 1 ];       /* Application-Path of project libs */
    char        szLokalProjectAppDir[ _MAX_PATH + 1 ];  /* Application-Path of station data */
} DM_DIRECTORY_INFOA, FAR *LPDM_DIRECTORY_INFOA;
typedef struct tagDM_DIRECTORY_INFOW
{
    WCHAR       szProjectDir[ _MAX_PATH + 1 ];          /* Path of global data */
    WCHAR       szProjectAppDir[ _MAX_PATH + 1 ];       /* Application-Path of project data */
    WCHAR       szGlobalLibDir[ _MAX_PATH + 1 ];        /* Application-Path of global libs */
    WCHAR       szProjectLibDir[ _MAX_PATH + 1 ];       /* Application-Path of project libs */
    WCHAR       szLokalProjectAppDir[ _MAX_PATH + 1 ];  /* Application-Path of station data */
} DM_DIRECTORY_INFOW, FAR *LPDM_DIRECTORY_INFOW;

#ifdef UNICODE
typedef DM_DIRECTORY_INFOW      DM_DIRECTORY_INFO;
typedef LPDM_DIRECTORY_INFOW    LPDM_DIRECTORY_INFO;
#else
typedef DM_DIRECTORY_INFOA      DM_DIRECTORY_INFO;
typedef LPDM_DIRECTORY_INFOA    LPDM_DIRECTORY_INFO;
#endif

/*
 * DM_PROJECT_INFO
 */
typedef struct tagDM_PROJECT_INFOA
{
    char        szProjectFile[ _MAX_PATH + 1 ];         /* Pfad / Dateiname der Projektdatei */
    char        szDSNName[ MAX_DM_DSN_NAME + 1 ];       /* Data Source Name für Datenbank (DB.DLL) */
    DWORD       dwDataLocale;                           /* Locale ID für Projektierungsdaten */
} DM_PROJECT_INFOA, FAR *LPDM_PROJECT_INFOA;
typedef struct tagDM_PROJECT_INFOW
{
    WCHAR       szProjectFile[ _MAX_PATH + 1 ];         /* Pfad / Dateiname der Projektdatei */
    WCHAR       szDSNName[ MAX_DM_DSN_NAME + 1 ];       /* Data Source Name für Datenbank (DB.DLL) */
    DWORD       dwDataLocale;                           /* Locale ID für Projektierungsdaten */
} DM_PROJECT_INFOW, FAR *LPDM_PROJECT_INFOW;

#ifdef UNICODE
typedef DM_PROJECT_INFOW        DM_PROJECT_INFO;
typedef LPDM_PROJECT_INFOW      LPDM_PROJECT_INFO;
#else
typedef DM_PROJECT_INFOA        DM_PROJECT_INFO;
typedef LPDM_PROJECT_INFOA      LPDM_PROJECT_INFO;
#endif

/*
 * DM_VARLIMIT
 */
typedef struct tagDM_VARLIMIT
{
    VARIANT  dmMaxRange;        /* Maximum value of datatype */
    VARIANT  dmMinRange;        /* Minimum value of datatype */
    VARIANT  dmMaxLimit;        /* Upper limit of variable value */
    VARIANT  dmMinLimit;        /* Lower limit of variable value */
} DM_VARLIMIT;

typedef DM_VARLIMIT*      PDM_VARLIMIT;
typedef DM_VARLIMIT FAR*  LPDM_VARLIMIT;


/*
 * DM_CYCLE_INFO
 */
typedef struct tagDM_CYCLE_INFOA
{
    DWORD       dwCycleTime;    /* Cycletime in ms */
    DWORD       dwCycleIndex;   /* Index of cycle */
    char        szDescription[ MAX_DM_CYCLE_NAME + 1 ]; /* Description as text */
} DM_CYCLE_INFOA, FAR *LPDM_CYCLE_INFOA;
typedef struct tagDM_CYCLE_INFOW
{
    DWORD       dwCycleTime;    /* Cycletime in ms */
    DWORD       dwCycleIndex;   /* Index of cycle */
    WCHAR       szDescription[ MAX_DM_CYCLE_NAME + 1 ]; /* Description as text */
} DM_CYCLE_INFOW, FAR *LPDM_CYCLE_INFOW;

#ifdef UNICODE
typedef DM_CYCLE_INFOW          DM_CYCLE_INFO;
typedef LPDM_CYCLE_INFOW        LPDM_CYCLE_INFO;
#else
typedef DM_CYCLE_INFOA          DM_CYCLE_INFO;
typedef LPDM_CYCLE_INFOA        LPDM_CYCLE_INFO;
#endif

/*
 * DM_SHIFT_INFO
 */
typedef struct tagDM_SHIFT_INFOA
{
    char        szShiftName[ MAX_DM_SHIFT_NAME_LEN + 1 ];
    SYSTEMTIME  stStart;
    SYSTEMTIME  stStop;
} DM_SHIFT_INFOA, FAR *LPDM_SHIFT_INFOA;
typedef struct tagDM_SHIFT_INFOW
{
    WCHAR       szShiftName[ MAX_DM_SHIFT_NAME_LEN + 1 ];
    SYSTEMTIME  stStart;
    SYSTEMTIME  stStop;
} DM_SHIFT_INFOW, FAR *LPDM_SHIFT_INFOW;

#ifdef UNICODE
typedef DM_SHIFT_INFOW          DM_SHIFT_INFO;
typedef LPDM_SHIFT_INFOW        LPDM_SHIFT_INFO;
#else
typedef DM_SHIFT_INFOA          DM_SHIFT_INFO;
typedef LPDM_SHIFT_INFOA        LPDM_SHIFT_INFO;
#endif


/*
 * Description of shift holyday
 */
typedef struct tagDM_SHIFT_HOLYDAYA
{
   TCHAR      szName[ MAX_DM_SHIFT_HOLYNAME + 1];
   SYSTEMTIME stTime;
} DM_SHIFT_HOLYDAY, FAR *LPDM_SHIFT_HOLYDAY;


/*
 * Description of shift
 */
typedef struct tagDM_SHIFT
{
    TCHAR       szName[ MAX_DM_SHIFT_NAME_LEN + 1 ];
    SYSTEMTIME  stStart;          // start time
    SYSTEMTIME  stStop;
    COLORREF    color;
    BOOL        bDisablePrint;    // TRUE, if shift should not be printed
} DM_SHIFT, FAR *LPDM_SHIFT;


/*
 * Description of shift day
 */
typedef struct tagDM_SHIFTDAY
{
   WORD       wMask;                // bitmask for shifts
} DM_SHIFTDAY, FAR *LPDM_SHIFTDAY;


/*
 * Description of shift month
 */
typedef struct tagDM_SHIFTMON
{
   TCHAR       szFullName[MAX_DM_SHIFT_NAME_LEN + 1];
   TCHAR       szAbrName[MAX_DM_SHIFT_NAME_LEN + 1];
   WORD        wDays;                 // number of days
   DM_SHIFTDAY dmDays[31];
} DM_SHIFTMON, FAR *LPDM_SHIFTMON;


/*
 * Description of shift map
 */
typedef struct tagDM_SHIFT_MAP
{
   TCHAR       szFullName[MAX_DM_SHIFT_NAME_LEN + 1];   // Key: name of shift map
   TCHAR       szAbrName[MAX_DM_SHIFT_NAME_LEN + 1];
   WORD        wYear;             // year of the map
   DM_SHIFTMON dwMonth[12];       //
} DM_SHIFT_MAP, FAR *LPDM_SHIFT_MAP;


/*
 * Description of shift data
 */
typedef struct tagDM_SHIFT_DATA
{
   DM_SHIFT_MAP     dmShiftMap;         // data of shift map
   DM_SHIFT         dmShift[MAX_DM_SHIFTS];  // data of shift list
   WORD             wShifts;            // nuber of shifts
   DM_SHIFT_HOLYDAY dmHolyday[MAX_DM_SHIFT_HOLYDAYS];  // holydays for shift map
   WORD             wHolydays;          // number of holydays
   BOOL             bActive;            // shift map is active for this year
   BOOL             bTimeTextInput;     // TRUE, if time input should be done by normal edit control
} DM_SHIFT_DATA, FAR *LPDM_SHIFT_DATA;



/*
 * DM_FORMAT_INFO
 */
typedef struct tagDM_FORMAT_INFOA
{
    DWORD       dwID;           /* Kennung der Konvertierungsroutine */
    char        szName[MAX_DM_FORMAT_NAME + 1]; /* Name der Konvertierungsroutine */
} DM_FORMAT_INFOA, FAR *LPDM_FORMAT_INFOA;
typedef struct tagDM_FORMAT_INFOW
{
    DWORD       dwID;           /* Kennung der Konvertierungsroutine */
    WCHAR       szName[MAX_DM_FORMAT_NAME + 1]; /* Name der Konvertierungsroutine */
} DM_FORMAT_INFOW, FAR *LPDM_FORMAT_INFOW;

#ifdef UNICODE
typedef DM_FORMAT_INFOW         DM_FORMAT_INFO;
typedef LPDM_FORMAT_INFOW       LPDM_FORMAT_INFO;
#else
typedef DM_FORMAT_INFOA         DM_FORMAT_INFO;
typedef LPDM_FORMAT_INFOA       LPDM_FORMAT_INFO;
#endif


/*
 * DM_VAR_SCALE_PARAMS
 */
typedef struct tagDM_VAR_SCALE_PARAMA
{
    double      dValue;         /* Wert des Parameters */
    char        szName[MAX_DM_SCALE_PARAM_NAME + 1];    /* Parametername */
} DM_VAR_SCALE_PARAMA, FAR *LPDM_VAR_SCALE_PARAMA;
typedef struct tagDM_VAR_SCALE_PARAMW
{
    double      dValue;         /* Wert des Parameters */
    WCHAR       szName[MAX_DM_SCALE_PARAM_NAME + 1];    /* Parametername */
} DM_VAR_SCALE_PARAMW, FAR *LPDM_VAR_SCALE_PARAMW;

#ifdef UNICODE
typedef DM_VAR_SCALE_PARAMW     DM_VAR_SCALE_PARAM;
typedef LPDM_VAR_SCALE_PARAMW   LPDM_VAR_SCALE_PARAM;
#else
typedef DM_VAR_SCALE_PARAMA     DM_VAR_SCALE_PARAM;
typedef LPDM_VAR_SCALE_PARAMA   LPDM_VAR_SCALE_PARAM;
#endif

/*
 * DM_SCALE_PARAM
 */
typedef struct tagDM_SCALE_PARAMA
{
    DWORD       dwPos;                  /* Position des Parameters */
    char        szName[MAX_DM_SCALE_PARAM_NAME + 1];    /* Parametername */
} DM_SCALE_PARAMA, FAR *LPDM_SCALE_PARAMA;
typedef struct tagDM_SCALE_PARAMW
{
    DWORD       dwPos;                  /* Position des Parameters */
    WCHAR       szName[MAX_DM_SCALE_PARAM_NAME + 1];    /* Parametername */
} DM_SCALE_PARAMW, FAR *LPDM_SCALE_PARAMW;

#ifdef UNICODE
typedef DM_SCALE_PARAMW         DM_SCALE_PARAM;
typedef LPDM_SCALE_PARAMW       LPDM_SCALE_PARAM;
#else
typedef DM_SCALE_PARAMA         DM_SCALE_PARAM;
typedef LPDM_SCALE_PARAMA       LPDM_SCALE_PARAM;
#endif


/*
 * DM_SCALE_INFO
 */
typedef struct tagDM_SCALE_INFOA
{
    DWORD       dwID;                           /* Kennung der Normierungsroutine */
    char        szName[MAX_DM_SCALE_NAME + 1];  /* Name der Normierungsroutine */
    DWORD       dwNumParams;                    /* Anzahl der benötigten Parameter */
} DM_SCALE_INFOA, FAR * LPDM_SCALE_INFOA;
typedef struct tagDM_SCALE_INFOW
{
    DWORD       dwID;                           /* Kennung der Normierungsroutine */
    WCHAR       szName[MAX_DM_SCALE_NAME + 1];  /* Name der Normierungsroutine */
    DWORD       dwNumParams;                    /* Anzahl der benötigten Parameter */
} DM_SCALE_INFOW, FAR * LPDM_SCALE_INFOW;

#ifdef UNICODE
typedef DM_SCALE_INFOW          DM_SCALE_INFO;
typedef LPDM_SCALE_INFOW        LPDM_SCALE_INFO;
#else
typedef DM_SCALE_INFOA          DM_SCALE_INFO;
typedef LPDM_SCALE_INFOA        LPDM_SCALE_INFO;
#endif


/*
 * DM_BITFIELD_BIT_DESCRIPTOR
 */
typedef struct tagDM_BITFIELD_BIT_DESCRIPTORA
{
    DWORD       dwBitPos;                       /* Bitpostion innerhalb des Bitfelds */
    char        szName[MAX_DM_MEMBER_NAME + 1]; /* Name des Bits */
} DM_BITFIELD_BIT_DESCRIPTORA, FAR * LPDM_BITFIELD_BIT_DESCRIPTORA;
typedef struct tagDM_BITFIELD_BIT_DESCRIPTORW
{
    DWORD       dwBitPos;                       /* Bitpostion innerhalb des Bitfelds */
    WCHAR       szName[MAX_DM_MEMBER_NAME + 1]; /* Name des Bits */
} DM_BITFIELD_BIT_DESCRIPTORW, FAR * LPDM_BITFIELD_BIT_DESCRIPTORW;

#ifdef UNICODE
typedef DM_BITFIELD_BIT_DESCRIPTORW     DM_BITFIELD_BIT_DESCRIPTOR;
typedef LPDM_BITFIELD_BIT_DESCRIPTORW   LPDM_BITFIELD_BIT_DESCRIPTOR;
#else
typedef DM_BITFIELD_BIT_DESCRIPTORA     DM_BITFIELD_BIT_DESCRIPTOR;
typedef LPDM_BITFIELD_BIT_DESCRIPTORA   LPDM_BITFIELD_BIT_DESCRIPTOR;
#endif


/*
 * DM_BITFIELD_DESCRIPTOR
 */
typedef struct tagDM_BITFIELD_DESCRIPTORA
{
    DWORD  dwNumBits;           /* Anzahl der belegten Bits */
    DWORD  dwBits;              /* Belegte Bitpositionen == 1 */
} DM_BITFIELD_DESCRIPTORA, FAR *LPDM_BITFIELD_DESCRIPTORA;
typedef struct tagDM_BITFIELD_DESCRIPTORW
{
    DWORD  dwNumBits;           /* Anzahl der belegten Bits */
    DWORD  dwBits;              /* Belegte Bitpositionen == 1 */
} DM_BITFIELD_DESCRIPTORW, FAR *LPDM_BITFIELD_DESCRIPTORW;

#ifdef UNICODE
typedef DM_BITFIELD_DESCRIPTORW         DM_BITFIELD_DESCRIPTOR;
typedef LPDM_BITFIELD_DESCRIPTORW       LPDM_BITFIELD_DESCRIPTOR;
#else
typedef DM_BITFIELD_DESCRIPTORA         DM_BITFIELD_DESCRIPTOR;
typedef LPDM_BITFIELD_DESCRIPTORA       LPDM_BITFIELD_DESCRIPTOR;
#endif

/*
 * DM_ARRAY_DESCRIPTOR
 */
typedef struct tagDM_ARRAY_DESCRIPTORA
{
    DWORD               dwNumElements;  /* Anzahl der Arrayelemente */
    DWORD               dwASElemSize;   /* Elementgröße AS */
    DWORD               dwASElemOffset; /* Offset innerhalb eines AS ELements */
    DM_FORMAT_INFOA     dmFormat;       /* Zahlenformatinformation */
    DM_SCALE_INFOA      dmScale;        /* Skalierungsinformation */
} DM_ARRAY_DESCRIPTORA, FAR *LPDM_ARRAY_DESCRIPTORA;
typedef struct tagDM_ARRAY_DESCRIPTORW
{
    DWORD               dwNumElements;  /* Anzahl der Arrayelemente */
    DWORD               dwASElemSize;   /* Elementgröße AS */
    DWORD               dwASElemOffset; /* Offset innerhalb eines AS ELements */
    DM_FORMAT_INFOW     dmFormat;       /* Zahlenformatinformation */
    DM_SCALE_INFOW      dmScale;        /* Skalierungsinformation */
} DM_ARRAY_DESCRIPTORW, FAR *LPDM_ARRAY_DESCRIPTORW;

#ifdef UNICODE
typedef DM_ARRAY_DESCRIPTORW            DM_ARRAY_DESCRIPTOR;
typedef LPDM_ARRAY_DESCRIPTORW          LPDM_ARRAY_DESCRIPTOR;
#else
typedef DM_ARRAY_DESCRIPTORA            DM_ARRAY_DESCRIPTOR;
typedef LPDM_ARRAY_DESCRIPTORA          LPDM_ARRAY_DESCRIPTOR;
#endif

/*
 * DM_STRUCT_MEMBER_DESCRIPTOR
 */
typedef struct tagDM_STRUCT_MEMBER_DESCRIPTORA
{
    BYTE                szName[MAX_DM_MEMBER_NAME];     /* Name des Members */
    DWORD               dwOSOffset;     /* Offset im Wertepuffer der Struktur */
    DWORD               dwASOffset;     /* Offset im AS Puffer */
    DM_FORMAT_INFOA     dmFormat;       /* Zahlenformatinformation */
    DM_SCALE_INFOA      dmScale;        /* Skalierungsinformation */
} DM_STRUCT_MEMBER_DESCRIPTORA, FAR * LPDM_STRUCT_MEMBER_DESCRIPTORA;
typedef struct tagDM_STRUCT_MEMBER_DESCRIPTORW
{
    WCHAR               szName[MAX_DM_MEMBER_NAME];     /* Name des Members */
    DWORD               dwOSOffset;     /* Offset im Wertepuffer der Struktur */
    DWORD               dwASOffset;     /* Offset im AS Puffer */
    DM_FORMAT_INFOA     dmFormat;       /* Zahlenformatinformation */
    DM_SCALE_INFOA      dmScale;        /* Skalierungsinformation */
} DM_STRUCT_MEMBER_DESCRIPTORW, FAR * LPDM_STRUCT_MEMBER_DESCRIPTORW;

#ifdef UNICODE
typedef DM_STRUCT_MEMBER_DESCRIPTORW    DM_STRUCT_MEMBER_DESCRIPTOR;
typedef LPDM_STRUCT_MEMBER_DESCRIPTORW  LPDM_STRUCT_MEMBER_DESCRIPTOR;
#else
typedef DM_STRUCT_MEMBER_DESCRIPTORA    DM_STRUCT_MEMBER_DESCRIPTOR;
typedef LPDM_STRUCT_MEMBER_DESCRIPTORA  LPDM_STRUCT_MEMBER_DESCRIPTOR;
#endif


/*
 * DM_STRUCT_DESCRIPTOR
 */
typedef struct tagDM_STRUCT_DESCRIPTORA
{
    DWORD       dwNumMembers;
} DM_STRUCT_DESCRIPTORA, FAR *LPDM_STRUCT_DESCRIPTORA;
typedef struct tagDM_STRUCT_DESCRIPTORW
{
    DWORD       dwNumMembers;
} DM_STRUCT_DESCRIPTORW, FAR *LPDM_STRUCT_DESCRIPTORW;

#ifdef UNICODE
typedef DM_STRUCT_DESCRIPTORW   DM_STRUCT_DESCRIPTOR;
typedef LPDM_STRUCT_DESCRIPTORW LPDM_STRUCT_DESCRIPTOR;
#else
typedef DM_STRUCT_DESCRIPTORA   DM_STRUCT_DESCRIPTOR;
typedef LPDM_STRUCT_DESCRIPTORA LPDM_STRUCT_DESCRIPTOR;
#endif

/*
 * DM_TYPE_UNION
 */
typedef union tagDM_TYPE_UNIONA
{
    DM_BITFIELD_DESCRIPTORA     dmBitField;
    DM_ARRAY_DESCRIPTORA        dmArray;
    DM_STRUCT_DESCRIPTORA       dmStruct;
} DM_TYPE_UNIONA, FAR *LPDM_TYPE_UNIONA;
typedef union tagDM_TYPE_UNIONW
{
    DM_BITFIELD_DESCRIPTORW     dmBitField;
    DM_ARRAY_DESCRIPTORW        dmArray;
    DM_STRUCT_DESCRIPTORW       dmStruct;
} DM_TYPE_UNIONW, FAR *LPDM_TYPE_UNIONW;

#ifdef UNICODE
typedef DM_TYPE_UNIONW          DM_TYPE_UNION;
typedef LPDM_TYPE_UNIONW        LPDM_TYPE_UNION;
#else
typedef DM_TYPE_UNIONA          DM_TYPE_UNION;
typedef LPDM_TYPE_UNIONA        LPDM_TYPE_UNION;
#endif


/*
 * DM_TYPE_DESCRIPTOR
 */
typedef struct tagDM_TYPE_DESCRIPTORA
{
    DM_TYPEREFA         dmTypeRef;      /* Typreferenz Beschreiber */
    DM_TYPE_UNIONA      dmType;         /* Typaufbau Beschreiber */
} DM_TYPE_DESCRIPTORA, FAR * LPDM_TYPE_DESCRIPTORA;
typedef struct tagDM_TYPE_DESCRIPTORW
{
    DM_TYPEREFW         dmTypeRef;      /* Typreferenz Beschreiber */
    DM_TYPE_UNIONW      dmType;         /* Typaufbau Beschreiber */
} DM_TYPE_DESCRIPTORW, FAR * LPDM_TYPE_DESCRIPTORW;

#ifdef UNICODE
typedef DM_TYPE_DESCRIPTORW     DM_TYPE_DESCRIPTOR;
typedef LPDM_TYPE_DESCRIPTORW   LPDM_TYPE_DESCRIPTOR;
#else
typedef DM_TYPE_DESCRIPTORA     DM_TYPE_DESCRIPTOR;
typedef LPDM_TYPE_DESCRIPTORA   LPDM_TYPE_DESCRIPTOR;
#endif


/*
 * DM_VARFILTER
 */
typedef struct tagDM_VARFILTERA
{
    DWORD       dwFlags;        /* DM_VARFILTER_XXX | DM_VARFILTER_YYY */
    DWORD       dwNumTypes;     /* Anzahl Typen in pdwTypes */
    LPDWORD     pdwTypes;       /* auszufilternde Datentypen (DM_VARTYPE_XXX) */
    LPSTR       lpszGroup;      /* auszufilternder Gruppenname */
    LPSTR       lpszName;       /* auszufilternder Variablenname */
    LPSTR       lpszConn;       /* auszufilternde log. Verbindung */
} DM_VARFILTERA, FAR *LPDM_VARFILTERA;
typedef struct tagDM_VARFILTERW
{
    DWORD       dwFlags;        /* DM_VARFILTER_XXX | DM_VARFILTER_YYY */
    DWORD       dwNumTypes;     /* Anzahl Typen in pdwTypes */
    LPDWORD     pdwTypes;       /* auszufilternde Datentypen (DM_VARTYPE_XXX) */
    LPWSTR      lpszGroup;      /* auszufilternder Gruppenname */
    LPWSTR      lpszName;       /* auszufilternder Variablenname */
    LPWSTR      lpszConn;       /* auszufilternde log. Verbindung */
} DM_VARFILTERW, FAR *LPDM_VARFILTERW;

#ifdef UNICODE
typedef DM_VARFILTERW           DM_VARFILTER;
typedef LPDM_VARFILTERW         LPDM_VARFILTER;
#else
typedef DM_VARFILTERA           DM_VARFILTER;
typedef LPDM_VARFILTERA         LPDM_VARFILTER;
#endif

/*
 * DM_UNITFILTER
 */
typedef struct tagDM_UNITFILTERA
{
    DWORD       dwFlags;                    /* DM_UNITFILTER_XXX | DM_UNITFILTER_YYY */
    char        szDriverName[_MAX_PATH+1];  /* auszufilternder Variablenname */
} DM_UNITFILTERA, FAR *LPDM_UNITFILTERA;
typedef struct tagDM_UNITFILTERW
{
    DWORD       dwFlags;                    /* DM_UNITFILTER_XXX | DM_UNITFILTER_YYY */
    WCHAR       szDriverName[_MAX_PATH+1];  /* auszufilternder Variablenname */
} DM_UNITFILTERW, FAR *LPDM_UNITFILTERW;

#ifdef UNICODE
typedef DM_UNITFILTERW           DM_UNITFILTER;
typedef LPDM_UNITFILTERW         LPDM_UNITFILTER;
#else
typedef DM_UNITFILTERA           DM_UNITFILTER;
typedef LPDM_UNITFILTERA         LPDM_UNITFILTER;
#endif

/*
 * DM_CONNECTIONFILTER
 */
typedef struct tagDM_CONNECTIONFILTERA
{
    DWORD       dwFlags;                         /* DM_CONNECTIONFILTER_XXX | DM_CONNECTIONFILTER_YYY */
    char        szUnitName[MAX_DM_UNIT_NAME+1];  /* auszufilternder Variablenname */
    char        szDriverName[_MAX_PATH+1];       /* auszufilternder Variablenname */
} DM_CONNECTIONFILTERA, FAR *LPDM_CONNECTIONFILTERA;
typedef struct tagDM_CONNECTIONFILTERW
{
    DWORD       dwFlags;                         /* DM_CONNECTIONFILTER_XXX | DM_CONNECTIONFILTER_YYY */
    WCHAR       szUnitName[MAX_DM_UNIT_NAME+1];  /* auszufilternder Variablenname */
    WCHAR       szDriverName[_MAX_PATH+1];       /* auszufilternder Variablenname */
} DM_CONNECTIONFILTERW, FAR *LPDM_CONNECTIONFILTERW;

#ifdef UNICODE
typedef DM_CONNECTIONFILTERW           DM_CONNECTIONFILTER;
typedef LPDM_CONNECTIONFILTERW         LPDM_CONNECTIONFILTER;
#else
typedef DM_CONNECTIONFILTERA           DM_CONNECTIONFILTER;
typedef LPDM_CONNECTIONFILTERA         LPDM_CONNECTIONFILTER;
#endif

/*
 * DM_GROUPFILTER
 */
typedef struct tagDM_GROUPFILTERA
{
    DWORD       dwFlags;                                    /* DM_GROUPFILTER_XXX | DM_GROUPFILTER_YYY */
    char        szConnectionName[MAX_DM_CONNECTION_NAME+1]; /* auszufilternder Variablenname */
    char        szUnitName[MAX_DM_UNIT_NAME+1];             /* auszufilternder Variablenname */
    char        szDriverName[_MAX_PATH+1];                  /* auszufilternder Variablenname */
} DM_GROUPFILTERA, FAR *LPDM_GROUPFILTERA;
typedef struct tagGROUPFILTERW
{
    DWORD       dwFlags;                                    /* DM_GROUPFILTER_XXX | DM_GROUPFILTER_YYY */
    WCHAR       szConnectionName[MAX_DM_CONNECTION_NAME+1]; /* auszufilternder Variablenname */
    WCHAR       szUnitName[MAX_DM_UNIT_NAME+1];             /* auszufilternder Variablenname */
    WCHAR       szDriverName[_MAX_PATH+1];                  /* auszufilternder Variablenname */
} DM_GROUPFILTERW, FAR *LPDM_GROUPFILTERW;

#ifdef UNICODE
typedef DM_GROUPFILTERW           DM_GROUPFILTER;
typedef LPDM_GROUPFILTERW         LPDM_GROUPFILTER;
#else
typedef DM_GROUPFILTERA           DM_GROUPFILTER;
typedef LPDM_GROUPFILTERA         LPDM_GROUPFILTER;
#endif

/*
 * DM_DATA_SERVICE
 */
typedef struct tagDM_DATA_SERVICEA
{
    DWORD       dwTeleType;     /* Telegramm-Klasse (EDIT / RUNTIME, ...) */
    char        szService[MAX_DM_SERVICE_NAME + 1];     /* Servicename */
    char        szSendingApp[MAX_DM_APP_NAME + 1];      /* logischer Name des Absenders (der bei DMConnect angegeben wurde) */
    DWORD       dwSendingMachine;                       /* Rechnernummer, von dem das Paket verschickt wurde 0..63 */
    DWORD       dwDataSize;                             /* Größe des Datenpakets in Byte (byData[0] ... byData[ dwDataSize - 1 ]) */
    BYTE        byData[1];                              /* Zeiger auf die Daten */
} DM_DATA_SERVICEA, FAR *LPDM_DATA_SERVICEA;
typedef struct tagDM_DATA_SERVICEW
{
    DWORD       dwTeleType;     /* Telegramm-Klasse (EDIT / RUNTIME, ...) */
    WCHAR       szService[MAX_DM_SERVICE_NAME + 1];     /* Servicename */
    WCHAR       szSendingApp[MAX_DM_APP_NAME + 1];      /* logischer Name des Absenders (der bei DMConnect angegeben wurde) */
    DWORD       dwSendingMachine;                       /* Rechnernummer, von dem das Paket verschickt wurde 0..63 */
    DWORD       dwDataSize;                             /* Größe des Datenpakets in Byte (byData[0] ... byData[ dwDataSize - 1 ]) */
    BYTE        byData[1];                              /* Zeiger auf die Daten */
} DM_DATA_SERVICEW, FAR *LPDM_DATA_SERVICEW;

#ifdef UNICODE
typedef DM_DATA_SERVICEW        DM_DATA_SERVICE;
typedef LPDM_DATA_SERVICEW      LPDM_DATA_SERVICE;
#else
typedef DM_DATA_SERVICEA        DM_DATA_SERVICE;
typedef LPDM_DATA_SERVICEA      LPDM_DATA_SERVICE;
#endif


/*
 * DM_SD_TARGET_APP
 */
typedef struct tagDM_SD_TARGET_APPA
{
    char        szAppName[MAX_DM_APP_NAME + 1]; /* logischer Applikationsname */
} DM_SD_TARGET_APPA, FAR *LPDM_SD_TARGET_APPA;
typedef struct tagDM_SD_TARGET_APPW
{
    WCHAR       szAppName[MAX_DM_APP_NAME + 1]; /* logischer Applikationsname */
} DM_SD_TARGET_APPW, FAR *LPDM_SD_TARGET_APPW;

#ifdef UNICODE
typedef DM_SD_TARGET_APPW       DM_SD_TARGET_APP;
typedef LPDM_SD_TARGET_APPW     LPDM_SD_TARGET_APP;
#else
typedef DM_SD_TARGET_APPA       DM_SD_TARGET_APP;
typedef LPDM_SD_TARGET_APPA     LPDM_SD_TARGET_APP;
#endif


/*
 * DM_SD_TARGET_MACHINE
 */
typedef struct tagDM_SD_TARGET_MACHINEA
{
    BOOL        fServer;        /* Server oder Client */
    BOOL        fLocal;         /* Localer oder remote Rechner */
    char        szMachineName[MAX_COMPUTERNAME_LENGTH + 1];     /* Rechnername */
} DM_SD_TARGET_MACHINEA, FAR *LPDM_SD_TARGET_MACHINEA;
typedef struct tagDM_SD_TARGET_MACHINEW
{
    BOOL        fServer;        /* Server oder Client */
    BOOL        fLocal;         /* Localer oder remote Rechner */
    WCHAR       szMachineName[MAX_COMPUTERNAME_LENGTH + 1];     /* Rechnername */
} DM_SD_TARGET_MACHINEW, FAR *LPDM_SD_TARGET_MACHINEW;

#ifdef UNICODE
typedef DM_SD_TARGET_MACHINEW   DM_SD_TARGET_MACHINE;
typedef LPDM_SD_TARGET_MACHINEW LPDM_SD_TARGET_MACHINE;
#else
typedef DM_SD_TARGET_MACHINEA   DM_SD_TARGET_MACHINE;
typedef LPDM_SD_TARGET_MACHINEA LPDM_SD_TARGET_MACHINE;
#endif


/*
 * DM_SEND_DATA_STRUCT
 */
typedef struct tagDM_SEND_DATA_STRUCTA
{
    BOOL                  fHighPriority;                         /* Auftrag mit höherer Priorität */
    char                  szService[MAX_DM_SERVICE_NAME + 1];    /* Servicename */
    DWORD                 dwTargetMachineFlags;                  /* DM_SD_XXX defines */
    DWORD                 dwTargetMachines;                      /* Anzahl der ausgefüllten Strukturen in dmTargetMachine */
    DM_SD_TARGET_MACHINEA dmTargetMachine[MAX_DM_OHIO_MACHINES]; /* Maschinenbeschreiber (fServer, fLocal uninteressant) */
    DWORD                 dwTargetApps;                          /* Anzahl der ausgefüllten Strukturen in dmTargetApp */
    DM_SD_TARGET_APPA     dmTargetApp[MAX_DM_OHIO_APPLICATIONS]; /* Applikationsbeschreiber */
    DWORD                 dwDataSize;                            /* Datengröße in Byte */
    BYTE                  byData[1];                             /* Array, das die Daten enthält */
} DM_SEND_DATA_STRUCTA, FAR *LPDM_SEND_DATA_STRUCTA;
typedef struct tagDM_SEND_DATA_STRUCTW
{
    BOOL                  fHighPriority;                         /* Auftrag mit höherer Priorität */
    WCHAR                 szService[MAX_DM_SERVICE_NAME + 1];    /* Servicename */
    DWORD                 dwTargetMachineFlags;                  /* DM_SD_XXX defines */
    DWORD                 dwTargetMachines;                      /* Anzahl der ausgefüllten Strukturen in dmTargetMachine */
    DM_SD_TARGET_MACHINEW dmTargetMachine[MAX_DM_OHIO_MACHINES]; /* Maschinenbeschreiber (fServer, fLocal uninteressant) */
    DWORD                 dwTargetApps;                          /* Anzahl der ausgefüllten Strukturen in dmTargetApp */
    DM_SD_TARGET_APPW     dmTargetApp[MAX_DM_OHIO_APPLICATIONS]; /* Applikationsbeschreiber */
    DWORD                 dwDataSize;                            /* Datengröße in Byte */
    BYTE                  byData[1];                             /* Array, das die Daten enthält */
} DM_SEND_DATA_STRUCTW, FAR *LPDM_SEND_DATA_STRUCTW;

#ifdef UNICODE
typedef DM_SEND_DATA_STRUCTW    DM_SEND_DATA_STRUCT;
typedef LPDM_SEND_DATA_STRUCTW  LPDM_SEND_DATA_STRUCT;
#else
typedef DM_SEND_DATA_STRUCTA    DM_SEND_DATA_STRUCT;
typedef LPDM_SEND_DATA_STRUCTA  LPDM_SEND_DATA_STRUCT;
#endif


/*
 * DM_MACHINE_TABLE
 */
typedef struct tagDM_MACHINE_TABLEA
{
    LONG                  nNumMachines;             /* Anzahl der Rechner im Projekt */
    LONG                  nLocalMachine;            /* Index des Eintrags der lokalen Machine */
    DM_SD_TARGET_MACHINEA tm[MAX_DM_OHIO_MACHINES]; /* Rechnerbeschreiber 0..( nNumMachines - 1 ) */
} DM_MACHINE_TABLEA, FAR *LPDM_MACHINE_TABLEA;
typedef struct tagDM_MACHINE_TABLEW
{
    LONG                  nNumMachines;             /* Anzahl der Rechner im Projekt */
    LONG                  nLocalMachine;            /* Index des Eintrags der lokalen Machine */
    DM_SD_TARGET_MACHINEW tm[MAX_DM_OHIO_MACHINES]; /* Rechnerbeschreiber 0..( nNumMachines - 1 ) */
} DM_MACHINE_TABLEW, FAR *LPDM_MACHINE_TABLEW;

#ifdef UNICODE
typedef DM_MACHINE_TABLEW       DM_MACHINE_TABLE;
typedef LPDM_MACHINE_TABLEW     LPDM_MACHINE_TABLE;
#else
typedef DM_MACHINE_TABLEA       DM_MACHINE_TABLE;
typedef LPDM_MACHINE_TABLEA     LPDM_MACHINE_TABLE;
#endif


/*
 * DM_DROP_TARGET_PROC
 */
typedef BOOL (* __OUTSIDE_CISS__ DM_DROP_TARGET_PROCA)(
    POINT        ptScreen,      /* Punkt in Bildschirmkoordinaten */
    LPDM_VARKEYA lpdmKey,       /* Zeiger auf DM_VARKEY Strukturen */
    DWORD        dwItems,       /* Anzahl der Strukturen */
    LPVOID       lpvUser);      /* Zeiger der durchgereicht wird */
typedef BOOL (* __OUTSIDE_CISS__ DM_DROP_TARGET_PROCW)(
    POINT        ptScreen,      /* Punkt in Bildschirmkoordinaten */
    LPDM_VARKEYW lpdmKey,       /* Zeiger auf DM_VARKEY Strukturen */
    DWORD        dwItems,       /* Anzahl der Strukturen */
    LPVOID       lpvUser);      /* Zeiger der durchgereicht wird */

#ifdef UNICODE
typedef DM_DROP_TARGET_PROCW    DM_DROP_TARGET_PROC;
#else
typedef DM_DROP_TARGET_PROCA    DM_DROP_TARGET_PROC;
#endif


/*
 * DM_TEST_DROP_TARGET_PROC
 */
typedef BOOL (* __OUTSIDE_CISS__ DM_TEST_DROP_TARGET_PROCA)(
    POINT        ptScreen,      /* Punkt in Bildschirmkoordinaten */
    LPDM_VARKEYA lpdmKey,       /* Zeiger auf DM_VARKEY Strukturen */
    DWORD        dwItems,       /* Anzahl der Strukturen */
    LPVOID       lpvUser );     /* Zeiger der durchgereicht wird */
typedef BOOL (* __OUTSIDE_CISS__ DM_TEST_DROP_TARGET_PROCW)(
    POINT        ptScreen,      /* Punkt in Bildschirmkoordinaten */
    LPDM_VARKEYW lpdmKey,       /* Zeiger auf DM_VARKEY Strukturen */
    DWORD        dwItems,       /* Anzahl der Strukturen */
    LPVOID       lpvUser );     /* Zeiger der durchgereicht wird */

#ifdef UNICODE
typedef DM_TEST_DROP_TARGET_PROCW       DM_TEST_DROP_TARGET_PROC;
#else
typedef DM_TEST_DROP_TARGET_PROCA       DM_TEST_DROP_TARGET_PROC;
#endif


/*
 * LPDM_DLGOPTIONS
 */
typedef struct tagDM_DLGOPTIONSA
{
    DWORD                     dwFlags;            /* DM_DLG_XXX */
    LPRECT                    lprcPreference;     /* Rechteck, in dem der Dialog erscheinen soll (NULL=>zentriert) */
    DM_TEST_DROP_TARGET_PROCA lpfnTestDropTarget; /* Testfunktion für Drag & Drop */
    DM_DROP_TARGET_PROCA      lpfnDropTarget;     /* Importfunktion für Drag & Drop  */
    LPVOID                    lpvUser;            /* Zeiger, der an lpfnDropTarget durchgereicht wird */
} DM_DLGOPTIONSA, FAR *LPDM_DLGOPTIONSA;
typedef struct tagDM_DLGOPTIONSW
{
    DWORD                     dwFlags;            /* DM_DLG_XXX */
    LPRECT                    lprcPreference;     /* Rechteck, in dem der Dialog erscheinen soll (NULL=>zentriert) */
    DM_TEST_DROP_TARGET_PROCW lpfnTestDropTarget; /* Testfunktion für Drag & Drop */
    DM_DROP_TARGET_PROCW      lpfnDropTarget;     /* Importfunktion für Drag & Drop  */
    LPVOID                    lpvUser;            /* Zeiger, der an lpfnDropTarget durchgereicht wird */
} DM_DLGOPTIONSW, FAR *LPDM_DLGOPTIONSW;

#ifdef UNICODE
typedef DM_DLGOPTIONSW          DM_DLGOPTIONS;
typedef LPDM_DLGOPTIONSW        LPDM_DLGOPTIONS;
#else
typedef DM_DLGOPTIONSA          DM_DLGOPTIONS;
typedef LPDM_DLGOPTIONSA        LPDM_DLGOPTIONS;
#endif

/*
 * NEWPROJECT
 */
typedef struct tagMCP_NEWPROJECT_DATAA
{
    DWORD   dwFlags;                            /* MCP_NPRO_FLAGs */
    char    szProjectFile[_MAX_PATH +1];        /* Pfad und Name des Projekts */
    char    szProducer[MAX_DM_VAR_NAME +1];     /* Ersteller */
    char    szCreationDate[MAX_DM_VAR_NAME +1]; /* ErstellDatum */
    char    szEditor[MAX_DM_VAR_NAME +1];       /* Bearbeiter */
    char    szLastMod[MAX_DM_VAR_NAME +1];      /* letzte Änderung */
    char    szVersion[MAX_DM_VAR_NAME +1];      /* Version */
    char    szComment[MAX_DM_VAR_NAME +1];      /* Kommentar */
} MCP_NEWPROJECT_DATAA, FAR *LPMCP_NEWPROJECT_DATAA;
typedef struct tagMCP_NEWPROJECT_DATAW
{
    DWORD   dwFlags;                            /* MCP_NPRO_FLAGs */
    WCHAR   szProjectFile[_MAX_PATH +1];        /* Pfad und Name des Projekts */
    WCHAR   szProducer[MAX_DM_VAR_NAME +1];     /* Ersteller */
    WCHAR   szCreationDate[MAX_DM_VAR_NAME +1]; /* ErstellDatum */
    WCHAR   szEditor[MAX_DM_VAR_NAME +1];       /* Bearbeiter */
    WCHAR   szLastMod[MAX_DM_VAR_NAME +1];      /* letzte Änderung */
    WCHAR   szVersion[MAX_DM_VAR_NAME +1];      /* Version */
    WCHAR   szComment[MAX_DM_VAR_NAME +1];      /* Kommentar */
} MCP_NEWPROJECT_DATAW, FAR *LPMCP_NEWPROJECT_DATAW;

#ifdef UNICODE
typedef MCP_NEWPROJECT_DATAW    MCP_NEWPROJECT_DATA;
typedef LPMCP_NEWPROJECT_DATAW  LPMCP_NEWPROJECT_DATA;
#else
typedef MCP_NEWPROJECT_DATAA    MCP_NEWPROJECT_DATA;
typedef LPMCP_NEWPROJECT_DATAA  LPMCP_NEWPROJECT_DATA;
#endif


/*
 * NEWMACHINE
 */
enum MACHINE_TYPE
{
    Server,                 // Server
    Client,                 // Client
    Engineering_Station    // Engineering Station
};

typedef struct tagMCP_NEWMACHINE_DATAA
{
    DWORD           dwFlags;                            // MCP_NMAC_FLAGs
    char            szProjectFile[_MAX_PATH +1];        // Pfad und Name des Projekts
    char            szName[MAX_COMPUTERNAME_LENGTH +1]; // Rechnername
    enum MACHINE_TYPE   eType;                              // Rechnertyp
} MCP_NEWMACHINE_DATAA, FAR *LPMCP_NEWMACHINE_DATAA;
typedef struct tagMCP_NEWMACHINE_DATAW
{
    DWORD           dwFlags;                            // MCP_NMAC_FLAGs
    WCHAR           szProjectFile[_MAX_PATH +1];        // Pfad und Name des Projekts
    WCHAR           szName[MAX_COMPUTERNAME_LENGTH +1]; // Rechnername
    enum MACHINE_TYPE   eType;                              // Rechnertyp
} MCP_NEWMACHINE_DATAW, FAR *LPMCP_NEWMACHINE_DATAW;

#ifdef UNICODE
typedef MCP_NEWMACHINE_DATAW    MCP_NEWMACHINE_DATA;
typedef LPMCP_NEWMACHINE_DATAW  LPMCP_NEWMACHINE_DATA;
#else
typedef MCP_NEWMACHINE_DATAA    MCP_NEWMACHINE_DATA;
typedef LPMCP_NEWMACHINE_DATAA  LPMCP_NEWMACHINE_DATA;
#endif


/*
 * NEWCONNECTION
 */
typedef struct tagMCP_NEWCONNECTION_DATAA
{
    DWORD   dwFlags ;                                 // MCP_NCON_FLAGs
    char    szProjectFile [_MAX_PATH +1] ;
    char    szUnitName    [MAX_DM_UNIT_NAME +1] ;
    char    szConnection  [MAX_DM_CONNECTION_NAME +3] ;
    char    szCommon      [MAX_DM_CON_COMMON +1] ;
    char    szSpecific    [MAX_DM_CON_SPECIFIC +1] ;
} MCP_NEWCONNECTION_DATAA, FAR *LPMCP_NEWCONNECTION_DATAA;
typedef struct tagMCP_NEWCONNECTION_DATAW
{
    DWORD   dwFlags ;                                 // MCP_NCON_FLAGs
    WCHAR   szProjectFile [_MAX_PATH +1] ;
    WCHAR   szUnitName    [MAX_DM_UNIT_NAME +1] ;
    WCHAR   szConnection  [MAX_DM_CONNECTION_NAME +3] ;
    WCHAR   szCommon      [MAX_DM_CON_COMMON +1] ;
    WCHAR   szSpecific    [MAX_DM_CON_SPECIFIC +1] ;
} MCP_NEWCONNECTION_DATAW, FAR *LPMCP_NEWCONNECTION_DATAW;

#ifdef UNICODE
typedef MCP_NEWCONNECTION_DATAW         MCP_NEWCONNECTION_DATA;
typedef LPMCP_NEWCONNECTION_DATAW       LPMCP_NEWCONNECTION_DATA;
#else
typedef MCP_NEWCONNECTION_DATAA         MCP_NEWCONNECTION_DATA;
typedef LPMCP_NEWCONNECTION_DATAA       LPMCP_NEWCONNECTION_DATA;
#endif


/*
 * DELETECONNECTION
 */
typedef struct tagMCP_DELETECONNECTION_DATAA
{
    DWORD   dwFlags ;                                 // MCP_NCON_FLAGs
    char    szProjectFile [_MAX_PATH +1] ;
    char    szConnection  [MAX_DM_CONNECTION_NAME +3] ;
} MCP_DELETECONNECTION_DATAA, FAR *LPMCP_DELETECONNECTION_DATAA;
typedef struct tagMCP_DELETECONNECTION_DATAW
{
    DWORD   dwFlags ;                                 // MCP_NCON_FLAGs
    WCHAR   szProjectFile [_MAX_PATH +1] ;
    WCHAR   szConnection  [MAX_DM_CONNECTION_NAME +3] ;
} MCP_DELETECONNECTION_DATAW, FAR *LPMCP_DELETECONNECTION_DATAW;

#ifdef UNICODE
typedef MCP_DELETECONNECTION_DATAW         MCP_DELETECONNECTION_DATA;
typedef LPMCP_DELETECONNECTION_DATAW       LPMCP_DELETECONNECTION_DATA;
#else
typedef MCP_DELETECONNECTION_DATAA         MCP_DELETECONNECTION_DATA;
typedef LPMCP_DELETECONNECTION_DATAA       LPMCP_DELETECONNECTION_DATA;
#endif

/*
 * NEWSTARTLISTENTRY
 */
enum OPEN_MODE
{
    SHOWNORMAL    = SW_SHOWNORMAL,
    SHOWMINIMIZED = SW_SHOWMINIMIZED,
    SHOWMAXIMIZED = SW_SHOWMAXIMIZED
};

typedef struct tagMCP_STARTLIST_DATAA
{
    char            szProjectFile[_MAX_PATH +1];               // Pfad und Name des Projekts
    char            szMachineName[MAX_COMPUTERNAME_LENGTH +1]; // Rechnername
    char            szApplication[MAX_PATH +1];
    char            szParams[MAX_PATH +1];
    char            szWorkingDir[MAX_PATH +1];
    BOOL            fUseProjectDir;
    enum OPEN_MODE  nOpen;
} MCP_STARTLIST_DATAA, FAR *LPMCP_STARTLIST_DATAA;

typedef struct tagMCP_STARTLIST_DATAW
{
    WCHAR           szProjectFile[_MAX_PATH +1];               // Pfad und Name des Projekts
    WCHAR           szMachineName[MAX_COMPUTERNAME_LENGTH +1]; // Rechnername
    WCHAR           szApplication[MAX_PATH +1];
    WCHAR           szParams[MAX_PATH +1];
    WCHAR           szWorkingDir[MAX_PATH +1];
    BOOL            fUseProjectDir;
    enum OPEN_MODE  nOpen;
} MCP_STARTLIST_DATAW, FAR *LPMCP_STARTLIST_DATAW;

#ifdef UNICODE
typedef MCP_STARTLIST_DATAW    MCP_STARTLIST_DATA;
typedef LPMCP_STARTLIST_DATAW  LPMCP_STARTLIST_DATA;
#else
typedef MCP_STARTLIST_DATAA    MCP_STARTLIST_DATA;
typedef LPMCP_STARTLIST_DATAA  LPMCP_STARTLIST_DATA;
#endif

/*
 * VARIABLE_COMMON
 */


typedef struct tagMCP_VARIABLE_COMMONA
{
    DWORD   dwVarType;      // Variablentyp
    DWORD   dwVarLength;    // Variablenlänge
    DWORD   dwVarProperty;  // Variablen Eigenschaften (Interne / Externe Variable)
    DWORD   dwFormat;       // Formatanpassung
} MCP_VARIABLE_COMMONA, FAR *LPMCP_VARIABLE_COMMONA ;
typedef struct tagMCP_VARIABLE_COMMONW
{
    DWORD   dwVarType;      // Variablentyp
    DWORD   dwVarLength;    // Variablenlänge
    DWORD   dwVarProperty;  // VariablenFlags (Interne / Externe Variable)
    DWORD   dwFormat;       // Formatanpassung
} MCP_VARIABLE_COMMONW, FAR *LPMCP_VARIABLE_COMMONW;

#ifdef UNICODE
typedef MCP_VARIABLE_COMMONW    MCP_VARIABLE_COMMON;
typedef LPMCP_VARIABLE_COMMONW  LPMCP_VARIABLE_COMMON;
#else
typedef MCP_VARIABLE_COMMONA    MCP_VARIABLE_COMMON;
typedef LPMCP_VARIABLE_COMMONA  LPMCP_VARIABLE_COMMON;
#endif


/*
 * VARIABLE_PROTOCOL
 */
typedef struct tagMCP_VARIABLE_PROTOCOLA
{
    BOOL    bTopLimitErr;           // Verletzung Obergrenze
    BOOL    bBottomLimitErr;        // Verletzung Untergrenze
    BOOL    bTransformationErr;     // Wandlungsfehler
    BOOL    bWriteErr;              // Unzulässigem Schreibzugriff
    BOOL    bWriteErrApplication;   // Schribzugriff Applikation
    BOOL    bWriteErrProzess;       // Schreibzugriff Prozess
} MCP_VARIABLE_PROTOCOLA, FAR *LPMCP_VARIABLE_PROTOCOLA;
typedef struct tagMCP_VARIABLE_PROTOCOLW
{
    BOOL    bTopLimitErr;           // Verletzung Obergrenze
    BOOL    bBottomLimitErr;        // Verletzung Untergrenze
    BOOL    bTransformationErr;     // Wandlungsfehler
    BOOL    bWriteErr;              // Unzulässigem Schreibzugriff
    BOOL    bWriteErrApplication;   // Schribzugriff Applikation
    BOOL    bWriteErrProzess;       // Schreibzugriff Prozess
} MCP_VARIABLE_PROTOCOLW, FAR *LPMCP_VARIABLE_PROTOCOLW;

#ifdef UNICODE
typedef MCP_VARIABLE_PROTOCOLW          MCP_VARIABLE_PROTOCOL;
typedef LPMCP_VARIABLE_PROTOCOLW        LPMCP_VARIABLE_PROTOCOL;
#else
typedef MCP_VARIABLE_PROTOCOLA          MCP_VARIABLE_PROTOCOL;
typedef LPMCP_VARIABLE_PROTOCOLA        LPMCP_VARIABLE_PROTOCOL;
#endif


/*
 * VARIABLE_LIMITS
 */
typedef struct tagMCP_VARIABLE_LIMITSA
{
    double  dTopLimit;          // Obergrenze
    double  dBottomLimit;       // Untergrenze
    double  dStartValue;        // Startwert
    double  dSubstituteValue;   // Ersatzwert
    BOOL    bTopLimit;          // Ersatzwert verwenden bei Obergrenze
    BOOL    bBottomLimit;       // Ersatzwert verwenden bei Untergrenze
    BOOL    bStartValue;        // Ersatzwert verwenden als Startwert
    BOOL    bConnectionErr;     // Ersatzwert verwenden bei Verbindungsfehlern
    BOOL    bTopLimitValid;     // Wert Obergrenze gültig
    BOOL    bBottomLimitValid;  // Wert Untergrenze gültig
    BOOL    bStartValueValid;   // Startwert gültig
    BOOL    bSubstValueValid;   // Ersatzwert gültig
} MCP_VARIABLE_LIMITSA, FAR *LPMCP_VARIABLE_LIMITSA;
typedef struct tagMCP_VARIABLE_LIMITSW
{
    double  dTopLimit;          // Obergrenze
    double  dBottomLimit;       // Untergrenze
    double  dStartValue;        // Startwert
    double  dSubstituteValue;   // Ersatzwert
    BOOL    bTopLimit;          // Ersatzwert verwenden bei Obergrenze
    BOOL    bBottomLimit;       // Ersatzwert verwenden bei Untergrenze
    BOOL    bStartValue;        // Ersatzwert verwenden als Startwert
    BOOL    bConnectionErr;     // Ersatzwert verwenden bei Verbindungsfehlern
    BOOL    bTopLimitValid;     // Wert Obergrenze gültig
    BOOL    bBottomLimitValid;  // Wert Untergrenze gültig
    BOOL    bStartValueValid;   // Startwert gültig
    BOOL    bSubstValueValid;   // Ersatzwert gültig
} MCP_VARIABLE_LIMITSW, FAR *LPMCP_VARIABLE_LIMITSW;

#ifdef UNICODE
typedef MCP_VARIABLE_LIMITSW    MCP_VARIABLE_LIMITS;
typedef LPMCP_VARIABLE_LIMITSW  LPMCP_VARIABLE_LIMITS;
#else
typedef MCP_VARIABLE_LIMITSA    MCP_VARIABLE_LIMITS;
typedef LPMCP_VARIABLE_LIMITSA  LPMCP_VARIABLE_LIMITS;
#endif


/*
 * NEWVARIABLE
 */
typedef struct tagMCP_NEWVARIABLE_DATAA
{
    DWORD                   dwFlags;                                 /* MCP_NVAR_FLAGs  */
    char                    szProjectFile[_MAX_PATH +1];             /* ProjektPfad + Name */
    char                    szConnection[MAX_DM_CONNECTION_NAME +3]; /* VerbindungsName */
    char                    szVarName[MAX_DM_VAR_NAME +1];           /* VariablenName   */
    char                    szGroupName[MAX_DM_GROUP_NAME +1];       /* GruppenName     */
    MCP_VARIABLE_COMMONA    Common;                                  /* MCP_VARIABLE_COMMON Struktur */
    MCP_VARIABLE_PROTOCOLA  Protocol;                                /* MCP_VARIABLE_PROTOCOL Struktur */
    MCP_VARIABLE_LIMITSA    Limits;                                  /* MCP_VARIABLE_LIMITS Struktur */
    char                    szSpecific[MAX_DM_VAR_SPECIFIC +1];      /* Kanal / Verbindungsabhängige Parameter */
} MCP_NEWVARIABLE_DATAA, FAR *LPMCP_NEWVARIABLE_DATAA;
typedef struct tagMCP_NEWVARIABLE_DATAW
{
    DWORD                   dwFlags;                                 /* MCP_NVAR_FLAGs  */
    WCHAR                   szProjectFile[_MAX_PATH +1];             /* ProjektPfad + Name */
    WCHAR                   szConnection[MAX_DM_CONNECTION_NAME +3]; /* VerbindungsName */
    WCHAR                   szVarName[MAX_DM_VAR_NAME +1];           /* VariablenName   */
    WCHAR                   szGroupName[MAX_DM_GROUP_NAME +1];       /* GruppenName     */
    MCP_VARIABLE_COMMONW    Common;                                  /* MCP_VARIABLE_COMMON Struktur */
    MCP_VARIABLE_PROTOCOLW  Protocol;                                /* MCP_VARIABLE_PROTOCOL Struktur */
    MCP_VARIABLE_LIMITSW    Limits;                                  /* MCP_VARIABLE_LIMITS Struktur */
    WCHAR                   szSpecific[MAX_DM_VAR_SPECIFIC +1];      /* Kanal / Verbindungsabhängige Parameter */
} MCP_NEWVARIABLE_DATAW, FAR *LPMCP_NEWVARIABLE_DATAW;

#ifdef UNICODE
typedef MCP_NEWVARIABLE_DATAW   MCP_NEWVARIABLE_DATA;
typedef LPMCP_NEWVARIABLE_DATAW LPMCP_NEWVARIABLE_DATA;
#else
typedef MCP_NEWVARIABLE_DATAA   MCP_NEWVARIABLE_DATA;
typedef LPMCP_NEWVARIABLE_DATAA LPMCP_NEWVARIABLE_DATA;
#endif


/*
 * DELETEVARIABLE
 */
typedef struct tagMCP_DELETEVARIABLE_DATAA
{
    DWORD                   dwFlags;                                 /* MCP_NVAR_FLAGs  */
    char                    szProjectFile[_MAX_PATH +1];             /* ProjektPfad + Name */
    char                    szVarName[MAX_DM_VAR_NAME +1];           /* VariablenName   */
} MCP_DELETEVARIABLE_DATAA, FAR *LPMCP_DELETEVARIABLE_DATAA;
typedef struct tagMCP_DELETEVARIABLE_DATAW
{
    DWORD                   dwFlags;                                 /* MCP_NVAR_FLAGs  */
    WCHAR                   szProjectFile[_MAX_PATH +1];             /* ProjektPfad + Name */
    WCHAR                   szVarName[MAX_DM_VAR_NAME +1];           /* VariablenName   */
} MCP_DELETEVARIABLE_DATAW, FAR *LPMCP_DELETEVARIABLE_DATAW;

#ifdef UNICODE
typedef MCP_DELETEVARIABLE_DATAW   MCP_DELETEVARIABLE_DATA;
typedef LPMCP_DELETEVARIABLE_DATAW LPMCP_DELETEVARIABLE_DATA;
#else
typedef MCP_DELETEVARIABLE_DATAA   MCP_DELETEVARIABLE_DATA;
typedef LPMCP_DELETEVARIABLE_DATAA LPMCP_DELETEVARIABLE_DATA;
#endif


/*
 * DM_SERVERSTATE
 */
typedef struct tagDM_SERVERSTATEA
{
    char                    szServerName[MAX_COMPUTERNAME_LENGTH + 1];
} DM_SERVERSTATEA, FAR *LPDM_SERVERSTATEA;
typedef struct tagDM_SERVERSTATEW
{
    WCHAR                   szServerName[MAX_COMPUTERNAME_LENGTH + 1];
} DM_SERVERSTATEW, FAR *LPDM_SERVERSTATEW;

#ifdef UNICODE
typedef DM_SERVERSTATEW             DM_SERVERSTATE;
typedef LPDM_SERVERSTATEW           LPDM_SERVERSTATE;
#else
typedef DM_SERVERSTATEA             DM_SERVERSTATE;
typedef LPDM_SERVERSTATEA           LPDM_SERVERSTATE;
#endif


/*
 * DM_ENUM_OPENED_PROJECTS_PROC
 */
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_OPENED_PROJECTS_PROCA)(
    LPDM_PROJECT_INFOA lpInfo,
    LPVOID lpvUser );
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_OPENED_PROJECTS_PROCW)(
    LPDM_PROJECT_INFOW lpInfo,
    LPVOID lpvUser );

#ifdef UNICODE
typedef DM_ENUM_OPENED_PROJECTS_PROCW   DM_ENUM_OPENED_PROJECTS_PROC;
#else
typedef DM_ENUM_OPENED_PROJECTS_PROCA   DM_ENUM_OPENED_PROJECTS_PROC;
#endif


/*
 * DM_NOTIFY_PROC
 */
typedef BOOL (* __OUTSIDE_CISS__ DM_NOTIFY_PROCA)(
    DWORD dwNotifyClass,
    DWORD  dwNotifyCode,
    LPBYTE lpbyData,
    DWORD  dwItems,
    LPVOID lpvUser);
typedef BOOL (* __OUTSIDE_CISS__ DM_NOTIFY_PROCW)(
    DWORD dwNotifyClass,
    DWORD  dwNotifyCode,
    LPBYTE lpbyData,
    DWORD  dwItems,
    LPVOID lpvUser);

#ifdef UNICODE
typedef DM_NOTIFY_PROCW DM_NOTIFY_PROC;
#else
typedef DM_NOTIFY_PROCA DM_NOTIFY_PROC;
#endif


/*
 * DM_NOTIFY_VARIABLE_PROC
 */
typedef BOOL (* __OUTSIDE_CISS__ DM_NOTIFY_VARIABLE_PROCA)(
    DWORD                   dwTAID,
    LPDM_VAR_UPDATE_STRUCTA lpdmvus,
    DWORD                   dwItems,
    LPVOID                  lpvUser);
typedef BOOL (* __OUTSIDE_CISS__ DM_NOTIFY_VARIABLE_PROCW)(
    DWORD                   dwTAID,
    LPDM_VAR_UPDATE_STRUCTW lpdmvus,
    DWORD                   dwItems,
    LPVOID                  lpvUser);

#ifdef UNICODE
typedef DM_NOTIFY_VARIABLE_PROCW        DM_NOTIFY_VARIABLE_PROC;
#else
typedef DM_NOTIFY_VARIABLE_PROCA        DM_NOTIFY_VARIABLE_PROC;
#endif

/*
 * DM_NOTIFY_VARIABLEEX_PROC
 */
typedef BOOL (* __OUTSIDE_CISS__ DM_NOTIFY_VARIABLEEX_PROCA)(
    DWORD                   dwTAID,
    LPDM_VAR_UPDATE_STRUCTEXA lpdmvus,
    DWORD                   dwItems,
    LPVOID                  lpvUser);
typedef BOOL (* __OUTSIDE_CISS__ DM_NOTIFY_VARIABLEEX_PROCW)(
    DWORD                   dwTAID,
    LPDM_VAR_UPDATE_STRUCTEXW lpdmvus,
    DWORD                   dwItems,
    LPVOID                  lpvUser);

#ifdef UNICODE
typedef DM_NOTIFY_VARIABLEEX_PROCW        DM_NOTIFY_VARIABLEEX_PROC;
#else
typedef DM_NOTIFY_VARIABLEEX_PROCA        DM_NOTIFY_VARIABLEEX_PROC;
#endif

/*
 * DM_COMPLETITION_PROC
 */
typedef BOOL (* __OUTSIDE_CISS__ DM_COMPLETITION_PROCA)(
    DWORD              dwTAID,
    LPDWORD            lpdmVarState,
    DWORD              dwItems,
    LPVOID             lpvUser);
typedef BOOL (* __OUTSIDE_CISS__ DM_COMPLETITION_PROCW)(
    DWORD              dwTAID,
    LPDWORD            lpdmVarState,
    DWORD              dwItems,
    LPVOID             lpvUser);

#ifdef UNICODE
typedef DM_COMPLETITION_PROCW   DM_COMPLETITION_PROC;
#else
typedef DM_COMPLETITION_PROCA   DM_COMPLETITION_PROC;
#endif


/*
 * DM_ENUM_CYCLES_PROC
 */
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_CYCLES_PROCA)(
    LPDM_CYCLE_INFOA lpdmCycle,
    DWORD            dwItem,
    LPVOID           lpvUser);
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_CYCLES_PROCW)(
    LPDM_CYCLE_INFOW lpdmCycle,
    DWORD            dwItem,
    LPVOID           lpvUser);

#ifdef UNICODE
typedef DM_ENUM_CYCLES_PROCW    DM_ENUM_CYCLES_PROC;
#else
typedef DM_ENUM_CYCLES_PROCA    DM_ENUM_CYCLES_PROC;
#endif


/*
 * DM_NOTIFY_SELECT_VAR_PROC
 */
typedef BOOL (* __OUTSIDE_CISS__ DM_NOTIFY_SELECT_VAR_PROCA)(
    LPDM_VARKEYA lpdmVarKey,
    DWORD        dwItem,
    LPVOID       lpvUser );
typedef BOOL (* __OUTSIDE_CISS__ DM_NOTIFY_SELECT_VAR_PROCW)(
    LPDM_VARKEYW lpdmVarKey,
    DWORD        dwItem,
    LPVOID       lpvUser );

#ifdef UNICODE
typedef DM_NOTIFY_SELECT_VAR_PROCW      DM_NOTIFY_SELECT_VAR_PROC;
#else
typedef DM_NOTIFY_SELECT_VAR_PROCA      DM_NOTIFY_SELECT_VAR_PROC;
#endif


/*
 * DM_ENUM_FORMATS_PROC
 */
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_FORMATS_PROCA)(
    LPDM_FORMAT_INFOA lpdmFormat,
    DWORD             dwItem,
    LPVOID            lpvUser );
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_FORMATS_PROCW)(
    LPDM_FORMAT_INFOW lpdmFormat,
    DWORD             dwItem,
    LPVOID            lpvUser );

#ifdef UNICODE
typedef DM_ENUM_FORMATS_PROCW   DM_ENUM_FORMATS_PROC;
#else
typedef DM_ENUM_FORMATS_PROCA   DM_ENUM_FORMATS_PROC;
#endif


/*
 * DM_ENUM_SCALINGS_PROC
 */
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_SCALINGS_PROCA)(
    LPDM_SCALE_INFOA lpdmScale,
    DWORD            dwItem,
    LPVOID           lpvUser );
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_SCALINGS_PROCW)(
    LPDM_SCALE_INFOW lpdmScale,
    DWORD            dwItem,
    LPVOID           lpvUser );

#ifdef UNICODE
typedef DM_ENUM_SCALINGS_PROCW  DM_ENUM_SCALINGS_PROC;
#else
typedef DM_ENUM_SCALINGS_PROCA  DM_ENUM_SCALINGS_PROC;
#endif


/*
 * DM_ENUM_SCALE_PARAMS_PROC
 */
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_SCALE_PARAMS_PROCA)(
    LPDM_SCALE_PARAMA lpdmParam,
    DWORD             dwItem,
    LPVOID            lpvUser);
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_SCALE_PARAMS_PROCW)(
    LPDM_SCALE_PARAMW lpdmParam,
    DWORD             dwItem,
    LPVOID            lpvUser);

#ifdef UNICODE
typedef DM_ENUM_SCALE_PARAMS_PROCW      DM_ENUM_SCALE_PARAMS_PROC;
#else
typedef DM_ENUM_SCALE_PARAMS_PROCA      DM_ENUM_SCALE_PARAMS_PROC;
#endif


/*
 * DM_ENUM_VAR_SCALE_PARAMS_PROC
 */
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_VAR_SCALE_PARAMS_PROCA)(
    LPDM_VAR_SCALE_PARAMA lpdmScale,
    DWORD                 dwItem,
    LPVOID                lpvUser);
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_VAR_SCALE_PARAMS_PROCW)(
    LPDM_VAR_SCALE_PARAMW lpdmScale,
    DWORD                 dwItem,
    LPVOID                lpvUser);

#ifdef UNICODE
typedef DM_ENUM_VAR_SCALE_PARAMS_PROCW  DM_ENUM_VAR_SCALE_PARAMS_PROC;
#else
typedef DM_ENUM_VAR_SCALE_PARAMS_PROCA  DM_ENUM_VAR_SCALE_PARAMS_PROC;
#endif


/*
 * DM_ENUM_STRUCT_MEMBERS_PROC
 */
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_STRUCT_MEMBERS_PROCA)(
    LPDM_STRUCT_MEMBER_DESCRIPTORA lpdmMember,
    DWORD                          dwItem,
    LPVOID                         lpvUser);
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_STRUCT_MEMBERS_PROCW)(
    LPDM_STRUCT_MEMBER_DESCRIPTORW lpdmMember,
    DWORD                          dwItem,
    LPVOID                         lpvUser);

#ifdef UNICODE
typedef DM_ENUM_STRUCT_MEMBERS_PROCW    DM_ENUM_STRUCT_MEMBERS_PROC;
#else
typedef DM_ENUM_STRUCT_MEMBERS_PROCA    DM_ENUM_STRUCT_MEMBERS_PROC;
#endif



/*
 * DM_ENUM_BITFIELD_BITS_PROC
 */
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_BITFIELD_BITS_PROCA)(
    DM_BITFIELD_BIT_DESCRIPTORA lpdmBit,
    DWORD                       dwItem,
    LPVOID                      lpvUser);
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_BITFIELD_BITS_PROCW)(
    DM_BITFIELD_BIT_DESCRIPTORW lpdmBit,
    DWORD                       dwItem,
    LPVOID                      lpvUser);

#ifdef UNICODE
typedef DM_ENUM_BITFIELD_BITS_PROCW     DM_ENUM_BITFIELD_BITS_PROC;
#else
typedef DM_ENUM_BITFIELD_BITS_PROCA     DM_ENUM_BITFIELD_BITS_PROC;
#endif


/*
 * DM_ENUM_SHIFTS_PROC
 */
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_SHIFTS_PROCA)(
    LPDM_SHIFT_INFOA lpdmInfo,
    LPVOID           lpvUser );
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_SHIFTS_PROCW)(
    LPDM_SHIFT_INFOW lpdmInfo,
    LPVOID           lpvUser );

#ifdef UNICODE
typedef DM_ENUM_SHIFTS_PROCW    DM_ENUM_SHIFTS_PROC;
#else
typedef DM_ENUM_SHIFTS_PROCA    DM_ENUM_SHIFTS_PROC;
#endif


/*
 * DM_DATA_SERVICE_PROC
 */
typedef BOOL (* __OUTSIDE_CISS__ DM_DATA_SERVICE_PROCA)(
    LPDM_DATA_SERVICEA lpds,
    LPVOID             lpvUser);
typedef BOOL (* __OUTSIDE_CISS__ DM_DATA_SERVICE_PROCW)(
    LPDM_DATA_SERVICEW lpds,
    LPVOID             lpvUser);

#ifdef UNICODE
typedef DM_DATA_SERVICE_PROCW   DM_DATA_SERVICE_PROC;
#else
typedef DM_DATA_SERVICE_PROCA   DM_DATA_SERVICE_PROC;
#endif


/*
 * DM_ENUM_DATA_SERVICE_PROC
 */
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_DATA_SERVICE_PROCA)(
    LPCSTR                lpszService,
    DM_DATA_SERVICE_PROCA lpfnService,
    LPVOID                lpvUser);
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_DATA_SERVICE_PROCW)(
    LPCWSTR               lpszService,
    DM_DATA_SERVICE_PROCW lpfnService,
    LPVOID                lpvUser);

#ifdef UNICODE
typedef DM_ENUM_DATA_SERVICE_PROCW      DM_ENUM_DATA_SERVICE_PROC;
#else
typedef DM_ENUM_DATA_SERVICE_PROCA      DM_ENUM_DATA_SERVICE_PROC;
#endif

// Verbindungen
/*
 * Description of a connection
 */
typedef struct tagDM_CONNKEYA
{
   CHAR        szName[ MAX_DM_CONNECTION_NAME + 1 ];   /* Key: connection name */
   LPVOID      lpvUserData;         /* User-specfic field */
} DM_CONNKEYA, FAR *LPDM_CONNKEYA;
typedef struct tagDM_CONNKEYW
{
    WCHAR       szName[ MAX_DM_CONNECTION_NAME + 1 ];  /* Key: connection name */
    LPVOID      lpvUserData;         /* User-specfic field */
} DM_CONNKEYW, FAR *LPDM_CONNKEYW;


#ifdef UNICODE
typedef DM_CONNKEYW      DM_CONNKEY;
typedef LPDM_CONNKEYW    LPDM_CONNKEY;
#else
typedef DM_CONNKEYA      DM_CONNKEY;
typedef LPDM_CONNKEYA    LPDM_CONNKEY;
#endif



typedef struct tagDM_CONNECTION_DATAA
{
    CHAR    szConnection  [MAX_DM_CONNECTION_NAME +3] ;
    CHAR    szUnitName    [MAX_DM_UNIT_NAME +1] ;
    CHAR    szCommon      [MAX_DM_CON_COMMON +1] ;
    CHAR    szSpecific    [MAX_DM_CON_SPECIFIC +1] ;
    DWORD   dwVarNum;                              /* number of variables */
} DM_CONNECTION_DATAA, FAR *LPDM_CONNECTION_DATAA;
typedef struct tagDM_CONNECTION_DATAW
{
    WCHAR   szConnection  [MAX_DM_CONNECTION_NAME +3] ;
    WCHAR   szUnitName    [MAX_DM_UNIT_NAME +1] ;
    WCHAR   szCommon      [MAX_DM_CON_COMMON +1] ;
    WCHAR   szSpecific    [MAX_DM_CON_SPECIFIC +1] ;
    DWORD   dwVarNum;                              /* number of variables */
} DM_CONNECTION_DATAW, FAR *LPDM_CONNECTION_DATAW;

#ifdef UNICODE
typedef DM_CONNECTION_DATAW         DM_CONNECTION_DATA;
typedef LPDM_CONNECTION_DATAW       LPDM_CONNECTION_DATA;
#else
typedef DM_CONNECTION_DATAA         DM_CONNECTION_DATA;
typedef LPDM_CONNECTION_DATAA       LPDM_CONNECTION_DATA;
#endif


/*
 * DM_ENUM_CONNECTION_PROC
 */
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_CONNECTION_PROCA)(
    LPDM_CONNECTION_DATAA lpdmConData,
    LPVOID                 lpvUser);
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_CONNECTION_PROCW)(
    LPDM_CONNECTION_DATAW lpdmConData,
    LPVOID                 lpvUser);

#ifdef UNICODE
typedef DM_ENUM_CONNECTION_PROCW      DM_ENUM_CONNECTION_PROC;
#else
typedef DM_ENUM_CONNECTION_PROCA      DM_ENUM_CONNECTION_PROC;
#endif


// Variablen
/*
 *
 */
typedef struct tagDM_VARIABLE_DATAA {
    DM_TYPEREFA dmTypeRef;                                /* Type of variable-data */
    DM_VARLIMIT dmVarLimit;                               /* limits of variables */
    VARIANT     dmStart;                                  /* start value */
    VARIANT     dmDefault;                                /* default value */
    DWORD       dwNotify;                                 /* Protokolleinträge generieren für...*/
    DWORD       dwFlags;                                  /* Ersatzwert verwenden */
    CHAR        szSpecific[MAX_DM_VAR_SPECIFIC +1];       /* parameter */
    CHAR        szGroup[MAX_DM_GROUP_NAME +1];            /* Variablengruppen */
    CHAR        szConnection[MAX_DM_CONNECTION_NAME +1];  /* Verbindung */
    CHAR        szChannel[_MAX_PATH +1];                  /* Name des Kanals */
    CHAR        szUnit[MAX_DM_UNIT_NAME +1];              /* Name der Unit */
} DM_VARIABLE_DATAA, FAR *LPDM_VARIABLE_DATAA;

typedef struct tagDM_VARIABLE_DATAW {
    DM_TYPEREFW dmTypeRef;                                /* Type of variable-data */
    DM_VARLIMIT dmVarLimit;                               /* limits of variables */
    VARIANT     dmStart;                                  /* start value */
    VARIANT     dmDefault;                                /* default value */
    DWORD       dwNotify;                                 /* Protokolleinträge generieren für...*/
    DWORD       dwFlags;                                  /* Ersatzwert verwenden */
    WCHAR       szSpecific[MAX_DM_VAR_SPECIFIC +1];       /* parameter */
    WCHAR       szGroup[MAX_DM_GROUP_NAME +1];            /* Variablengruppen */
    WCHAR       szConnection[MAX_DM_CONNECTION_NAME +1];  /* Verbindung */
    WCHAR       szChannel[_MAX_PATH +1];                  /* Name des Kanals */
    WCHAR       szUnit[MAX_DM_UNIT_NAME +1];              /* Name der Unit */
} DM_VARIABLE_DATAW, FAR *LPDM_VARIABLE_DATAW;

#ifdef UNICODE
typedef DM_VARIABLE_DATAW         DM_VARIABLE_DATA;
typedef LPDM_VARIABLE_DATAW       LPDM_VARIABLE_DATA;
#else
typedef DM_VARIABLE_DATAA         DM_VARIABLE_DATA;
typedef LPDM_VARIABLE_DATAA       LPDM_VARIABLE_DATA;
#endif


/*
 * DM_ENUM_VARIABLE_PROC
 */
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_VARIABLE_PROCA)(
    LPDM_VARKEYA        lpdmVarKey,
    LPDM_VARIABLE_DATAA lpdmVarData,
    LPVOID              lpvUser);
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_VARIABLE_PROCW)(
    LPDM_VARKEYW        lpdmVarKey,
    LPDM_VARIABLE_DATAW lpdmVarData,
    LPVOID              lpvUser);

#ifdef UNICODE
typedef DM_ENUM_VARIABLE_PROCW      DM_ENUM_VARIABLE_PROC;
#else
typedef DM_ENUM_VARIABLE_PROCA      DM_ENUM_VARIABLE_PROC;
#endif

/*
 * Description of a variable group
 */
typedef struct tagDM_VARGRPKEYA
{
   CHAR        szName[ MAX_DM_VAR_NAME + 1 ];   /* Key: Variablegroup-Name */
   LPVOID      lpvUserData;         /* User-specfic field */
} DM_VARGRPKEYA, FAR *LPDM_VARGRPKEYA;
typedef struct tagDM_VARGRPKEYW
{
    WCHAR       szName[ MAX_DM_VAR_NAME + 1 ];  /* Key: Variablegroup-Name */
    LPVOID      lpvUserData;         /* User-specfic field */
} DM_VARGRPKEYW, FAR *LPDM_VARGRPKEYW;


#ifdef UNICODE
typedef DM_VARGRPKEYW      DM_VARGRPKEY;
typedef LPDM_VARGRPKEYW    LPDM_VARGRPKEY;
#else
typedef DM_VARGRPKEYA      DM_VARGRPKEY;
typedef LPDM_VARGRPKEYA    LPDM_VARGRPKEY;
#endif



/*
 * Data of a variable group
 */
typedef struct tagDM_VARGRP_DATAA
{
    CHAR   szName[ MAX_DM_VAR_NAME + 1 ];  /* Variablegroup-Name */
    DWORD  dwCreatorID;
    WORD   dwVarNum;                       /* number of variables */
    LPVOID lpvUserData;         /* User-specfic field */
} DM_VARGRP_DATAA, FAR *LPDM_VARGRP_DATAA;
typedef struct tagDM_VARGRP_DATAW
{
    WCHAR  szName[ MAX_DM_VAR_NAME + 1 ];  /* Variablegroup-Name */
    DWORD  dwCreatorID;
    WORD   dwVarNum;                       /* number of variables */
    LPVOID lpvUserData;         /* User-specfic field */
} DM_VARGRP_DATAW, FAR *LPDM_VARGRP_DATAW;


#ifdef UNICODE
typedef DM_VARGRP_DATAW      DM_VARGRP_DATA;
typedef LPDM_VARGRP_DATAW    LPDM_VARGRP_DATA;
#else
typedef DM_VARGRP_DATAA      DM_VARGRP_DATA;
typedef LPDM_VARGRP_DATAA    LPDM_VARGRP_DATA;
#endif


/*
 * DM_ENUM_VARGRP_PROC
 */
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_VARGRP_PROCA)(
    LPDM_VARGRP_DATAA lpdmVarGrpData,
    LPVOID            lpvUser);
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_VARGRP_PROCW)(
    LPDM_VARGRP_DATAW lpdmConData,
    LPVOID            lpvUser);

#ifdef UNICODE
typedef DM_ENUM_VARGRP_PROCW      DM_ENUM_VARGRP_PROC;
#else
typedef DM_ENUM_VARGRP_PROCA      DM_ENUM_VARGRP_PROC;
#endif

#ifdef __cplusplus
extern "C" {
#endif


/*-----------------------------------------------------------------------
 * DMEnumVarData
 */
BOOL WINAPI DMEnumVarDataA(
    LPCSTR lpszProjectFile,
    LPDM_VARKEYA lpdmVarKey,
    DWORD dwItems,
    DM_ENUM_VARIABLE_PROCA lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);

BOOL WINAPI DMEnumVarDataW(
    LPCWSTR lpszProjectFile,
    LPDM_VARKEYW lpdmVarKey,
    DWORD dwItems,
    DM_ENUM_VARIABLE_PROCW lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);

typedef BOOL (WINAPI *DMENUMVARDATAFCTA)(
    LPCSTR lpszProjectFile,
    LPDM_VARKEYA lpdmVarKey,
    DWORD dwItems,
    DM_ENUM_VARIABLE_PROCA lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMENUMVARDATAFCTW)(
    LPCWSTR lpszProjectFile,
    LPDM_VARKEYW lpdmVarKey,
    DWORD dwItems,
    DM_ENUM_VARIABLE_PROCW lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMENUMVARDATA      "DMEnumVarDataW"
#define DMEnumVarData           DMEnumVarDataW
typedef DMENUMVARDATAFCTW       DMENUMVARDATAFCT;
#else
#define NAME_DMENUMVARDATA      "DMEnumVarDataA"
#define DMEnumVarData           DMEnumVarDataA
typedef DMENUMVARDATAFCTA       DMENUMVARDATAFCT;
#endif

/*-----------------------------------------------------------------------
 * DMEnumVariables
 */

/*
 * DM_ENUM_VAR_PROC
 */
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_VAR_PROCA)(
    LPDM_VARKEYA        lpdmVarKey,
    LPVOID              lpvUser);
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_VAR_PROCW)(
    LPDM_VARKEYW        lpdmVarKey,
    LPVOID              lpvUser);

#ifdef UNICODE
#define DM_ENUM_VAR_PROC        DM_ENUM_VAR_PROCW
#else
#define DM_ENUM_VAR_PROC        DM_ENUM_VAR_PROCA
#endif

BOOL WINAPI DMEnumVariablesA(
    LPCSTR lpszProjectFile,
    LPDM_VARFILTERA lpdmVarFilter,
    DM_ENUM_VAR_PROCA lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError
);

BOOL WINAPI DMEnumVariablesW(
    LPCWSTR lpszProjectFile,
    LPDM_VARFILTERW lpdmVarFilter,
    DM_ENUM_VAR_PROCW lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError
);

typedef BOOL (WINAPI *DMENUMVARIABLESA)(
    LPCSTR lpszProjectFile,
    LPDM_VARFILTERA lpdmVarFilter,
    DM_ENUM_VAR_PROCA lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMENUMVARIABLESW)(
    LPCWSTR lpszProjectFile,
    LPDM_VARFILTERW lpdmVarFilter,
    DM_ENUM_VAR_PROCW lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMENUMVARIABLES       "DMEnumVariablesW"
#define DMEnumVariables            DMEnumVariablesW
typedef DMENUMVARIABLESW           DMENUMVARIABLES;
#else
#define NAME_DMENUMVARIABLES       "DMEnumVariablesA"
#define DMEnumVariables            DMEnumVariablesA
typedef DMENUMVARIABLESA           DMENUMVARIABLES;
#endif


/*-----------------------------------------------------------------------
 * DMEnumConnectionData
 */
BOOL WINAPI DMEnumConnectionDataA(
    LPCSTR lpszProjectFile,
    LPDM_CONNKEYA lpdmConnKey,
    DWORD dwItems,
    DM_ENUM_CONNECTION_PROCA lpfnCallback,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);

BOOL WINAPI DMEnumConnectionDataW(
    LPCWSTR lpszProjectFile,
    LPDM_CONNKEYW lpdmConnKey,
    DWORD dwItems,
    DM_ENUM_CONNECTION_PROCW lpfnCallback,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);

typedef BOOL (WINAPI *DMENUMCONNECTIONDATAFCTA)(
    LPCSTR lpszProjectFile,
    LPDM_CONNKEYA lpdmConnKey,
    DWORD dwItems,
    DM_ENUM_CONNECTION_PROCA lpfnCallback,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMENUMCONNECTIONDATAFCTW)(
    LPCWSTR lpszProjectFile,
    LPDM_CONNKEYW lpdmConnKey,
    DWORD dwItems,
    DM_ENUM_CONNECTION_PROCW lpfnCallback,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMENUMCONNECTIONDATA       "DMEnumConnectionDataW"
#define DMEnumConnectionData            DMEnumConnectionDataW
typedef DMENUMCONNECTIONDATAFCTW        DMENUMCONNECTIONDATAFCT;
#else
#define NAME_DMENUMCONNECTIONDATA       "DMEnumConnectionDataA"
#define DMEnumConnectionData            DMEnumConnectionDataA
typedef DMENUMCONNECTIONDATAFCTA        DMENUMCONNECTIONDATAFCT;
#endif


/*-----------------------------------------------------------------------
 * DMEnumVarGrpData
 */
BOOL WINAPI DMEnumVarGrpDataA(
    LPSTR lpszProjectFile,
    LPDM_VARGRPKEYA lpdmVarGrpKey,
    DWORD dwItems,
    DM_ENUM_VARGRP_PROCA lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);

BOOL WINAPI DMEnumVarGrpDataW(
    LPWSTR lpszProjectFile,
    LPDM_VARGRPKEYW lpdmVarGrpKey,
    DWORD dwItems,
    DM_ENUM_VARGRP_PROCW lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);

typedef BOOL (WINAPI *DMENUMVARGRPDATAFCTA)(
    LPSTR lpszProjectFile,
    LPDM_VARGRPKEYA lpdmVarGrpKey,
    DWORD dwItems,
    DM_ENUM_VARGRP_PROCA lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMENUMVARGRPDATAFCTW)(
    LPWSTR lpszProjectFile,
    LPDM_VARGRPKEYW lpdmVarGrpKey,
    DWORD dwItems,
    DM_ENUM_VARGRP_PROCW lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMENUMVARGRPDATA           "DMEnumVarGrpDataW"
#define DMEnumVarGrpData                DMEnumVarGrpDataW
typedef DMENUMVARGRPDATAFCTW            DMENUMVARGRPDATAFCT;
#else
#define NAME_DMENUMVARGRPDATA           "DMEnumVarGrpDataA"
#define DMEnumVarGrpData                DMEnumVarGrpDataA
typedef DMENUMVARGRPDATAFCTA            DMENUMVARGRPDATAFCT;
#endif


/*-----------------------------------------------------------------------
 * DMCreateNewProject
 */
BOOL WINAPI DMCreateNewProjectA(
    HWND hwndParent,
    LPSTR lpszProjectFile,
    DWORD dwBufSize,
    LPCMN_ERRORA lpdmError);

BOOL WINAPI DMCreateNewProjectW(
    HWND hwndParent,
    LPWSTR lpszProjectFile,
    DWORD dwBufSize,
    LPCMN_ERRORW lpdmError);

typedef BOOL (WINAPI *DMCREATENEWPROJECTFCTA)(
    HWND hwndParent,
    LPSTR lpszProjectFile,
    DWORD dwBufSize,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMCREATENEWPROJECTFCTW)(
    HWND hwndParent,
    LPWSTR lpszProjectFile,
    DWORD dwBufSize,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMCREATENEWPROJECT         "DMCreateNewProjectW"
#define DMCreateNewProject              DMCreateNewProjectW
typedef DMCREATENEWPROJECTFCTW          DMCREATENEWPROJECTFCT;
#else
#define NAME_DMCREATENEWPROJECT         "DMCreateNewProjectA"
#define DMCreateNewProject              DMCreateNewProjectA
typedef DMCREATENEWPROJECTFCTA          DMCREATENEWPROJECTFCT;
#endif


/*-----------------------------------------------------------------------
 * DMOpenProject
 */
BOOL WINAPI DMOpenProjectA(
    HWND hwndParent,
    LPSTR lpszProjectFile,
    DWORD dwBufSize,
    LPCMN_ERRORA lpdmError );

BOOL WINAPI DMOpenProjectW(
    HWND hwndParent,
    LPWSTR lpszProjectFile,
    DWORD dwBufSize,
    LPCMN_ERRORW lpdmError );

typedef BOOL (WINAPI *DMOPENPROJECTFCTA)(
    HWND hwndParent,
    LPSTR lpszProjectFile,
    DWORD dwBufSize,
    LPCMN_ERRORA lpdmError );
typedef BOOL (WINAPI *DMOPENPROJECTFCTW)(
    HWND hwndParent,
    LPWSTR lpszProjectFile,
    DWORD dwBufSize,
    LPCMN_ERRORW lpdmError );

#ifdef UNICODE
#define NAME_DMOPENPROJECT              "DMOpenProjectW"
#define DMOpenProject                   DMOpenProjectW
typedef DMOPENPROJECTFCTW               DMOPENPROJECTFCT;
#else
#define NAME_DMOPENPROJECT              "DMOpenProjectA"
#define DMOpenProject                   DMOpenProjectA
typedef DMOPENPROJECTFCTA               DMOPENPROJECTFCT;
#endif


/*-----------------------------------------------------------------------
 * DMOpenProjectDoc
 */
BOOL WINAPI DMOpenProjectDocA(
    LPSTR lpszProjectFile,
    LPCMN_ERRORA lpdmError);

BOOL WINAPI DMOpenProjectDocW(
    LPWSTR lpszProjectFile,
    LPCMN_ERRORW lpdmError);

typedef BOOL (WINAPI *DMOPENPROJECTDOCFCTA)(
    LPSTR lpszProjectFile,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMOPENPROJECTDOCFCTW)(
    LPWSTR lpszProjectFile,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMOPENPROJECTDOC           "DMOpenProjectDocW"
#define DMOpenProjectDoc                DMOpenProjectDocW
typedef DMOPENPROJECTDOCFCTW            DMOPENPROJECTDOCFCT;
#else
#define NAME_DMOPENPROJECTDOC           "DMOpenProjectDocA"
#define DMOpenProjectDoc                DMOpenProjectDocA
typedef DMOPENPROJECTDOCFCTA            DMOPENPROJECTDOCFCT;
#endif


/*-----------------------------------------------------------------------
 * DMEditProjectSettings
 */
BOOL WINAPI DMEditProjectSettingsA(
    HWND hwndParent,
    LPCSTR lpszProjectFile,
    LPCMN_ERRORA lpdmError );

BOOL WINAPI DMEditProjectSettingsW(
    HWND hwndParent,
    LPCWSTR lpszProjectFile,
    LPCMN_ERRORW lpdmError );

typedef BOOL (WINAPI *DMEDITPROJECTSETTINGSFCTA)(
    HWND hwndParent,
    LPCSTR lpszProjectFile,
    LPCMN_ERRORA lpdmError );
typedef BOOL (WINAPI *DMEDITPROJECTSETTINGSFCTW)(
    HWND hwndParent,
    LPCWSTR lpszProjectFile,
    LPCMN_ERRORW lpdmError );

#ifdef UNICODE
#define NAME_DMEDITPROJECTSETTINGS      "DMEditProjectSettingsW"
#define DMEditProjectSettings           DMEditProjectSettingsW
typedef DMEDITPROJECTSETTINGSFCTW       DMEDITPROJECTSETTINGSFCT;
#else
#define NAME_DMEDITPROJECTSETTINGS      "DMEditProjectSettingsA"
#define DMEditProjectSettings           DMEditProjectSettingsA
typedef DMEDITPROJECTSETTINGSFCTA       DMEDITPROJECTSETTINGSFCT;
#endif


/*-----------------------------------------------------------------------
 * DMGetRuntimeProject
 */
BOOL WINAPI DMGetRuntimeProjectA(
    LPSTR lpszProjectFile,
    DWORD dwBufSize,
    LPCMN_ERRORA lpdmError);

BOOL WINAPI DMGetRuntimeProjectW(
    LPWSTR lpszProjectFile,
    DWORD dwBufSize,
    LPCMN_ERRORW lpdmError);

typedef BOOL (WINAPI *DMGETRUNTIMEPROJECTFCTA)(
    LPSTR lpszProjectFile,
    DWORD dwBufSize,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMGETRUNTIMEPROJECTFCTW)(
    LPWSTR lpszProjectFile,
    DWORD dwBufSize,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMGETRUNTIMEPROJECT        "DMGetRuntimeProjectW"
#define DMGetRuntimeProject             DMGetRuntimeProjectW
typedef DMGETRUNTIMEPROJECTFCTW         DMGETRUNTIMEPROJECTFCT;
#else
#define NAME_DMGETRUNTIMEPROJECT        "DMGetRuntimeProjectA"
#define DMGetRuntimeProject             DMGetRuntimeProjectA
typedef DMGETRUNTIMEPROJECTFCTA         DMGETRUNTIMEPROJECTFCT;
#endif


/*-----------------------------------------------------------------------
 * DMGetProjectDirectory
 */
BOOL WINAPI DMGetProjectDirectoryA(
    LPCSTR lpszAppName,
    LPCSTR lpszProjectFile,
    LPDM_DIRECTORY_INFOA lpdmDirInfo,
    LPCMN_ERRORA lpdmError );

BOOL WINAPI DMGetProjectDirectoryW(
    LPCWSTR lpszAppName,
    LPCWSTR lpszProjectFile,
    LPDM_DIRECTORY_INFOW lpdmDirInfo,
    LPCMN_ERRORW lpdmError );

typedef BOOL (WINAPI *DMGETPROJECTDIRECTORYFCTA)(
    LPCSTR lpszAppName,
    LPCSTR lpszProjectFile,
    LPDM_DIRECTORY_INFOA lpdmDirInfo,
    LPCMN_ERRORA lpdmError );
typedef BOOL (WINAPI *DMGETPROJECTDIRECTORYFCTW)(
    LPCWSTR lpszAppName,
    LPCWSTR lpszProjectFile,
    LPDM_DIRECTORY_INFOW lpdmDirInfo,
    LPCMN_ERRORW lpdmError );

#ifdef UNICODE
#define NAME_DMGETPROJECTDIRECTORY      "DMGetProjectDirectoryW"
#define DMGetProjectDirectory           DMGetProjectDirectoryW
typedef DMGETPROJECTDIRECTORYFCTW       DMGETPROJECTDIRECTORYFCT;
#else
#define NAME_DMGETPROJECTDIRECTORY      "DMGetProjectDirectoryA"
#define DMGetProjectDirectory           DMGetProjectDirectoryA
typedef DMGETPROJECTDIRECTORYFCTA       DMGETPROJECTDIRECTORYFCT;
#endif


/*-----------------------------------------------------------------------
 * DMGetProjectInformation
 */
BOOL WINAPI DMGetProjectInformationA(
    LPCSTR lpszProjectFile,
    LPDM_PROJECT_INFOA lpProjectInfo,
    LPCMN_ERRORA lpdmError);

BOOL WINAPI DMGetProjectInformationW(
    LPCWSTR lpszProjectFile,
    LPDM_PROJECT_INFOW lpProjectInfo,
    LPCMN_ERRORW lpdmError);

typedef BOOL (WINAPI *DMGETPROJECTINFORMATIONFCTA)(
    LPCSTR lpszProjectFile,
    LPDM_PROJECT_INFOA lpProjectInfo,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMGETPROJECTINFORMATIONFCTW)(
    LPCWSTR lpszProjectFile,
    LPDM_PROJECT_INFOW lpProjectInfo,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMGETPROJECTINFORMATION    "DMGetProjectInformationW"
#define DMGetProjectInformation         DMGetProjectInformationW
typedef DMGETPROJECTINFORMATIONFCTW     DMGETPROJECTINFORMATIONFCT;
#else
#define NAME_DMGETPROJECTINFORMATION    "DMGetProjectInformationA"
#define DMGetProjectInformation         DMGetProjectInformationA
typedef DMGETPROJECTINFORMATIONFCTA     DMGETPROJECTINFORMATIONFCT;
#endif


/*-----------------------------------------------------------------------
 * DMEnumOpenedProjects
 */
BOOL WINAPI DMEnumOpenedProjectsA(
    LPDWORD lpdwItems,
    DM_ENUM_OPENED_PROJECTS_PROCA lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);

BOOL WINAPI DMEnumOpenedProjectsW(
    LPDWORD lpdwItems,
    DM_ENUM_OPENED_PROJECTS_PROCW lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);

typedef BOOL (WINAPI *DMENUMOPENEDPROJECTSFCTA)(
    LPDWORD lpdwItems,
    DM_ENUM_OPENED_PROJECTS_PROCA lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMENUMOPENEDPROJECTSFCTW)(
    LPDWORD lpdwItems,
    DM_ENUM_OPENED_PROJECTS_PROCW lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMENUMOPENEDPROJECTS       "DMEnumOpenedProjectsW"
#define DMEnumOpenedProjects            DMEnumOpenedProjectsW
typedef DMENUMOPENEDPROJECTSFCTW        DMENUMOPENEDPROJECTSFCT;
#else
#define NAME_DMENUMOPENEDPROJECTS       "DMEnumOpenedProjectsA"
#define DMEnumOpenedProjects            DMEnumOpenedProjectsA
typedef DMENUMOPENEDPROJECTSFCTA        DMENUMOPENEDPROJECTSFCT;
#endif


/*-----------------------------------------------------------------------
 * DMSetLanguage
 */
BOOL WINAPI DMSetLanguageA(
    DWORD dwLocaleID,
    LPCMN_ERRORA lpdmError );

BOOL WINAPI DMSetLanguageW(
    DWORD dwLocaleID,
    LPCMN_ERRORW lpdmError );

typedef BOOL (WINAPI *DMSETLANGUAGEFCTA)(
    DWORD dwLocaleID,
    LPCMN_ERRORA lpdmError );

typedef BOOL (WINAPI *DMSETLANGUAGEFCTW)(
    DWORD dwLocaleID,
    LPCMN_ERRORW lpdmError );

#ifdef UNICODE
#define NAME_DMSETLANGUAGE              "DMSetLanguageW"
#define DMSetLanguage                   DMSetLanguageW
typedef DMSETLANGUAGEFCTW               DMSETLANGUAGEFCT;
#else
#define NAME_DMSETLANGUAGE              "DMSetLanguageA"
#define DMSetLanguage                   DMSetLanguageA
typedef DMSETLANGUAGEFCTA               DMSETLANGUAGEFCT;
#endif


/*-----------------------------------------------------------------------
 * DMConnect
 */
BOOL WINAPI DMConnectA(
    LPCSTR lpszAppName,
    DM_NOTIFY_PROCA lpfnNotify,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError );

BOOL WINAPI DMConnectW(
    LPCWSTR lpszAppName,
    DM_NOTIFY_PROCW lpfnNotify,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError );

typedef BOOL (WINAPI *DMCONNECTFCTA)(
    LPCSTR lpszAppName,
    DM_NOTIFY_PROCA lpfnNotify,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError );
typedef BOOL (WINAPI *DMCONNECTFCTW)(
    LPCWSTR lpszAppName,
    DM_NOTIFY_PROCW lpfnNotify,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError );

#ifdef UNICODE
#define NAME_DMCONNECT                  "DMConnectW"
#define DMConnect                       DMConnectW
typedef DMCONNECTFCTW                   DMCONNECTFCT;
#else
#define NAME_DMCONNECT                  "DMConnectA"
#define DMConnect                       DMConnectA
typedef DMCONNECTFCTA                   DMCONNECTFCT;
#endif


/*-----------------------------------------------------------------------
 * DMDisConnect
 */
BOOL WINAPI DMDisConnectA(LPCMN_ERRORA lpdmError);

BOOL WINAPI DMDisConnectW(LPCMN_ERRORW lpdmError);

typedef BOOL (WINAPI *DMDISCONNECTFCTA)(LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMDISCONNECTFCTW)(LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMDISCONNECT               "DMDisConnectW"
#define DMDisConnect                    DMDisConnectW
typedef DMDISCONNECTFCTW                DMDISCONNECTFCT;
#else
#define NAME_DMDISCONNECT               "DMDisConnectA"
#define DMDisConnect                    DMDisConnectA
typedef DMDISCONNECTFCTA                DMDISCONNECTFCT;
#endif



/*-----------------------------------------------------------------------
 * DMGetConnectionState
 */
BOOL WINAPI DMGetConnectionStateA(LPCMN_ERRORA lpdmError);

BOOL WINAPI DMGetConnectionStateW(LPCMN_ERRORW lpdmError);

typedef BOOL (WINAPI *DMGETCONNECTIONSTATEFCTA)(LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMGETCONNECTIONSTATEFCTW)(LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMGETCONNECTIONSTATE       "DMGetConnectionStateW"
#define DMGetConnectionState            DMGetConnectionStateW
typedef DMGETCONNECTIONSTATEFCTW        DMGETCONNECTIONSTATEFCT;
#else
#define NAME_DMGETCONNECTIONSTATE       "DMGetConnectionStateA"
#define DMGetConnectionState            DMGetConnectionStateA
typedef DMGETCONNECTIONSTATEFCTA        DMGETCONNECTIONSTATEFCT;
#endif


/*-----------------------------------------------------------------------
 * DMGetNumPendingBlocks
 */
BOOL WINAPI DMGetNumPendingBlocksA(
    LONG *plEntries,
    LPCMN_ERRORA lpdmError);

BOOL WINAPI DMGetNumPendingBlocksW(
    LONG *plEntries,
    LPCMN_ERRORW lpdmError);

typedef BOOL (WINAPI *DMGETNUMPENDINGBLOCKSFCTA)(
    LONG *plEntries,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMGETNUMPENDINGBLOCKSFCTW)(
    LONG *plEntries,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMGETNUMPENDINGBLOCKS      "DMGetNumPendingBlocksW"
#define DMGetNumPendingBlocks           DMGetNumPendingBlocksW
typedef DMGETNUMPENDINGBLOCKSFCTW       DMGETNUMPENDINGBLOCKSFCT;
#else
#define NAME_DMGETNUMPENDINGBLOCKS      "DMGetNumPendingBlocksA"
#define DMGetNumPendingBlocks           DMGetNumPendingBlocksA
typedef DMGETNUMPENDINGBLOCKSFCTA       DMGETNUMPENDINGBLOCKSFCT;
#endif


/*-----------------------------------------------------------------------
 * DMGetNumPendingBlocks
 */
BOOL WINAPI DMSetBlockQueueSizeA(
    LONG  nCount,
    LPCMN_ERRORA lpdmError);

BOOL WINAPI DMSetBlockQueueSizeW(
    LONG  nCount,
    LPCMN_ERRORW lpdmError);

typedef BOOL (WINAPI *DMSETBLOCKQUEUESIZEFCTA)(
    LONG  nCount,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMSETBLOCKQUEUESIZEFCTW)(
    LONG  nCount,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMSETBLOCKQUEUESIZE        "DMSetBlockQueueSizeW"
#define DMSetBlockQueueSize             DMSetBlockQueueSizeW
typedef DMSETBLOCKQUEUESIZEFCTW         DMSETBLOCKQUEUESIZEFCT;
#else
#define NAME_DMSETBLOCKQUEUESIZE        "DMSetBlockQueueSizeA"
#define DMSetBlockQueueSize             DMSetBlockQueueSizeA
typedef DMSETBLOCKQUEUESIZEFCTA         DMSETBLOCKQUEUESIZEFCT;
#endif


/*-----------------------------------------------------------------------
 * DMClearBlockQueue
 */
BOOL WINAPI DMClearBlockQueueA(LPCMN_ERRORA lpdmError);

BOOL WINAPI DMClearBlockQueueW(LPCMN_ERRORW lpdmError);

typedef BOOL (WINAPI *DMCLEARBLOCKQUEUEFCTA)(LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMCLEARBLOCKQUEUEFCTW)(LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMCLEARBLOCKQUEUE          "DMClearBlockQueueW"
#define DMClearBlockQueue               DMClearBlockQueueW
typedef DMCLEARBLOCKQUEUEFCTW           DMCLEARBLOCKQUEUEFCT;
#else
#define NAME_DMCLEARBLOCKQUEUE          "DMClearBlockQueueA"
#define DMClearBlockQueue               DMClearBlockQueueA
typedef DMCLEARBLOCKQUEUEFCTA           DMCLEARBLOCKQUEUEFCT;
#endif


/*-----------------------------------------------------------------------
 * DMCreateLogEntry
 */
BOOL WINAPI DMCreateLogEntryA(
    LPCSTR lpzszAction,
    LPCSTR lpszReason,
    LPCMN_ERRORA lpdmError);

BOOL WINAPI DMCreateLogEntryW(
    LPCWSTR lpzszAction,
    LPCWSTR lpszReason,
    LPCMN_ERRORW lpdmError);

typedef BOOL (WINAPI *DMCREATELOGENTRYFCTA)(
    LPCSTR lpzszAction,
    LPCSTR lpszReason,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMCREATELOGENTRYFCTW)(
    LPCWSTR lpzszAction,
    LPCWSTR lpszReason,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMCREATELOGENTRY           "DMCreateLogEntryW"
#define DMCreateLogEntry                DMCreateLogEntryW
typedef DMCREATELOGENTRYFCTW            DMCREATELOGENTRYFCT;
#else
#define NAME_DMCREATELOGENTRY           "DMCreateLogEntryA"
#define DMCreateLogEntry                DMCreateLogEntryA
typedef DMCREATELOGENTRYFCTA            DMCREATELOGENTRYFCT;
#endif


/*-----------------------------------------------------------------------
 * DMBeginStartVarUpdate
 */
BOOL WINAPI DMBeginStartVarUpdateA(
    LPDWORD     pdwTAID,
    LPCMN_ERRORA lpdmError);

BOOL WINAPI DMBeginStartVarUpdateW(
    LPDWORD     pdwTAID,
    LPCMN_ERRORW lpdmError);

typedef BOOL (WINAPI *DMBEGINSTARTVARUPDATEFCTA)(
    LPDWORD pdwTAID,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMBEGINSTARTVARUPDATEFCTW)(
    LPDWORD pdwTAID,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMBEGINSTARTVARUPDATE      "DMBeginStartVarUpdateW"
#define DMBeginStartVarUpdate           DMBeginStartVarUpdateW
typedef DMBEGINSTARTVARUPDATEFCTW       DMBEGINSTARTVARUPDATEFCT;
#else
#define NAME_DMBEGINSTARTVARUPDATE      "DMBeginStartVarUpdateA"
#define DMBeginStartVarUpdate           DMBeginStartVarUpdateA
typedef DMBEGINSTARTVARUPDATEFCTA       DMBEGINSTARTVARUPDATEFCT;
#endif


/*-----------------------------------------------------------------------
 * DMStartVarUpdate
 */
BOOL WINAPI DMStartVarUpdateA(
    DWORD dwTAID,
    LPDM_VARKEYA lpdmVarKey,
    DWORD dwItems,
    DWORD dwCycle,
    DM_NOTIFY_VARIABLE_PROCA lpfnVariable,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);

BOOL WINAPI DMStartVarUpdateW(
    DWORD dwTAID,
    LPDM_VARKEYW lpdmVarKey,
    DWORD dwItems,
    DWORD dwCycle,
    DM_NOTIFY_VARIABLE_PROCW lpfnVariable,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);

typedef BOOL (WINAPI *DMSTARTVARUPDATEFCTA)(
    DWORD dwTAID,
    LPDM_VARKEYA lpdmVarKey,
    DWORD dwItems,
    DWORD dwCycle,
    DM_NOTIFY_VARIABLE_PROCA lpfnVariable,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMSTARTVARUPDATEFCTW)(
    DWORD dwTAID,
    LPDM_VARKEYW lpdmVarKey,
    DWORD dwItems,
    DWORD dwCycle,
    DM_NOTIFY_VARIABLE_PROCW lpfnVariable,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMSTARTVARUPDATE           "DMStartVarUpdateW"
#define DMStartVarUpdate                DMStartVarUpdateW
typedef DMSTARTVARUPDATEFCTW            DMSTARTVARUPDATEFCT;
#else
#define NAME_DMSTARTVARUPDATE           "DMStartVarUpdateA"
#define DMStartVarUpdate                DMStartVarUpdateA
typedef DMSTARTVARUPDATEFCTA            DMSTARTVARUPDATEFCT;
#endif

/*-----------------------------------------------------------------------
 * DMStartVarUpdateEx
 */
BOOL WINAPI DMStartVarUpdateExA(
    DWORD dwTAID,
    LPDM_VARKEYA lpdmVarKey,
    DWORD dwItems,
    DWORD dwCycle,
    DM_NOTIFY_VARIABLEEX_PROCA lpfnVariableEx,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);

BOOL WINAPI DMStartVarUpdateExW(
    DWORD dwTAID,
    LPDM_VARKEYW lpdmVarKey,
    DWORD dwItems,
    DWORD dwCycle,
    DM_NOTIFY_VARIABLEEX_PROCW lpfnVariableEx,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);

typedef BOOL (WINAPI *DMSTARTVARUPDATEEXFCTA)(
    DWORD dwTAID,
    LPDM_VARKEYA lpdmVarKey,
    DWORD dwItems,
    DWORD dwCycle,
    DM_NOTIFY_VARIABLEEX_PROCA lpfnVariableEx,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMSTARTVARUPDATEEXFCTW)(
    DWORD dwTAID,
    LPDM_VARKEYW lpdmVarKey,
    DWORD dwItems,
    DWORD dwCycle,
    DM_NOTIFY_VARIABLEEX_PROCW lpfnVariableEx,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMSTARTVARUPDATEEX           "DMStartVarUpdateExW"
#define DMStartVarUpdateEx                DMStartVarUpdateExW
typedef DMSTARTVARUPDATEEXFCTW            DMSTARTVARUPDATEEXFCT;
#else
#define NAME_DMSTARTVARUPDATEEX           "DMStartVarUpdateExA"
#define DMStartVarUpdateEx                DMStartVarUpdateExA
typedef DMSTARTVARUPDATEEXFCTA            DMSTARTVARUPDATEEXFCT;
#endif


/*-----------------------------------------------------------------------
 * DMEndStartVarUpdate
 */
BOOL WINAPI DMEndStartVarUpdateA(
    DWORD dwTAID,
    LPCMN_ERRORA lpdmError);

BOOL WINAPI DMEndStartVarUpdateW(
    DWORD dwTAID,
    LPCMN_ERRORW lpdmError);

typedef BOOL (WINAPI *DMENDSTARTVARUPDATEFCTA)(
    DWORD dwTAID,
    LPCMN_ERRORA lpdmError );
typedef BOOL (WINAPI *DMENDSTARTVARUPDATEFCTW)(
    DWORD dwTAID,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMENDSTARTVARUPDATE        "DMEndStartVarUpdateW"
#define DMEndStartVarUpdate             DMEndStartVarUpdateW
typedef DMENDSTARTVARUPDATEFCTW         DMENDSTARTVARUPDATEFCT;
#else
#define NAME_DMENDSTARTVARUPDATE        "DMEndStartVarUpdateA"
#define DMEndStartVarUpdate             DMEndStartVarUpdateA
typedef DMENDSTARTVARUPDATEFCTA         DMENDSTARTVARUPDATEFCT;
#endif


/*-----------------------------------------------------------------------
 * DMStopVarUpdate
 */
BOOL WINAPI DMStopVarUpdateA(
    DWORD dwTAID,
    LPCMN_ERRORA lpdmError);

BOOL WINAPI DMStopVarUpdateW(
    DWORD dwTAID,
    LPCMN_ERRORW lpdmError);

typedef BOOL (WINAPI *DMSTOPVARUPDATEFCTA)(
    DWORD dwTAID,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMSTOPVARUPDATEFCTW)(
    DWORD dwTAID,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMSTOPVARUPDATE            "DMStopVarUpdateW"
#define DMStopVarUpdate                 DMStopVarUpdateW
typedef DMSTOPVARUPDATEFCTW             DMSTOPVARUPDATEFCT;
#else
#define NAME_DMSTOPVARUPDATE            "DMStopVarUpdateA"
#define DMStopVarUpdate                 DMStopVarUpdateA
typedef DMSTOPVARUPDATEFCTA             DMSTOPVARUPDATEFCT;
#endif


/*-----------------------------------------------------------------------
 * DMStopAllUpdate
 */
BOOL WINAPI DMStopAllUpdatesA(LPCMN_ERRORA lpdmError);

BOOL WINAPI DMStopAllUpdatesW(LPCMN_ERRORW lpdmError);

typedef BOOL (WINAPI *DMSTOPALLUPDATESFCTA)(LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMSTOPALLUPDATESFCTW)(LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMSTOPALLUPDATES           "DMStopAllUpdatesW"
#define DMStopAllUpdates                DMStopAllUpdatesW
typedef DMSTOPALLUPDATESFCTW            DMSTOPALLUPDATEFSFCT;
#else
#define NAME_DMSTOPALLUPDATES           "DMStopAllUpdatesA"
#define DMStopAllUpdates                DMStopAllUpdatesA
typedef DMSTOPALLUPDATESFCTA            DMSTOPALLUPDATEFSFCT;
#endif

/*-----------------------------------------------------------------------
 * DMSuspendVarUpdate
 */
BOOL WINAPI DMSuspendVarUpdateA(
    DWORD dwTAID,
    LPCMN_ERRORA lpdmError);

BOOL WINAPI DMSuspendVarUpdateW(
    DWORD dwTAID,
    LPCMN_ERRORW lpdmError);

typedef BOOL (WINAPI *DMSUSPENDVARUPDATEFCTA)(
    DWORD dwTAID,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMSUSPENDVARUPDATEFCTW)(
    DWORD dwTAID,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMSUSPENDVARUPDATE         "DMSuspendVarUpdateW"
#define DMSuspendVarUpdate              DMSuspendVarUpdateW
typedef DMSUSPENDVARUPDATEFCTW          DMSUSPENDVARUPDATEFCT;
#else
#define NAME_DMSUSPENDVARUPDATE         "DMSuspendVarUpdateA"
#define DMSuspendVarUpdate              DMSuspendVarUpdateA
typedef DMSUSPENDVARUPDATEFCTA          DMSUSPENDVARUPDATEFCT;
#endif


/*-----------------------------------------------------------------------
 * DMResumeVarUpdate
 */
BOOL WINAPI DMResumeVarUpdateA(
    DWORD dwTAID,
    LPCMN_ERRORA lpdmError);

BOOL WINAPI DMResumeVarUpdateW(
    DWORD dwTAID,
    LPCMN_ERRORW lpdmError);

typedef BOOL (WINAPI *DMRESUMEVARUPDATEFCTA)(
    DWORD dwTAID,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMRESUMEVARUPDATEFCTW)(
    DWORD dwTAID,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMRESUMEVARUPDATE          "DMResumeVarUpdateW"
#define DMResumeVarUpdate               DMResumeVarUpdateW
typedef DMRESUMEVARUPDATEFCTW           DMRESUMEVARUPDATEFCT;
#else
#define NAME_DMRESUMEVARUPDATE          "DMResumeVarUpdateA"
#define DMResumeVarUpdate               DMResumeVarUpdateA
typedef DMRESUMEVARUPDATEFCTA           DMRESUMEVARUPDATEFCT;
#endif


/*-----------------------------------------------------------------------
 * DMGetValue
 */
BOOL WINAPI DMGetValueA(
    LPDM_VARKEYA            lpdmVarKey,
    DWORD                   dwItems,
    LPDM_VAR_UPDATE_STRUCTA lpdmvus,
    LPCMN_ERRORA            lpdmError);

BOOL WINAPI DMGetValueW(
    LPDM_VARKEYW            lpdmVarKey,
    DWORD                   dwItems,
    LPDM_VAR_UPDATE_STRUCTW lpdmvus,
    LPCMN_ERRORW            lpdmError);

typedef BOOL (WINAPI *DMGETVALUEFCTA)(
    LPDM_VARKEYA lpdmVarKey,
    DWORD dwItems,
    LPDM_VAR_UPDATE_STRUCTA lpdmvus,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMGETVALUEFCTW)(
    LPDM_VARKEYW lpdmVarKey,
    DWORD dwItems,
    LPDM_VAR_UPDATE_STRUCTW lpdmvus,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMGETVALUE                 "DMGetValueW"
#define DMGetValue												DMGetValueW
typedef DMGETVALUEFCTW                    DMGETVALUEFCT;
#else
#define NAME_DMGETVALUE                 "DMGetValueA"
#define DMGetValue                      DMGetValueA
typedef DMGETVALUEFCTA                  DMGETVALUEFCT;
#endif

/*-----------------------------------------------------------------------
 * DMGetValueEx
 */
BOOL WINAPI DMGetValueExA(
    LPDM_VARKEYA            lpdmVarKey,
    DWORD                   dwItems,
    LPDM_VAR_UPDATE_STRUCTEXA lpdmvus,
    LPCMN_ERRORA            lpdmError);
BOOL WINAPI DMGetValueExW(
    LPDM_VARKEYW            lpdmVarKey,
    DWORD                   dwItems,
    LPDM_VAR_UPDATE_STRUCTEXW lpdmvus,
    LPCMN_ERRORW            lpdmError);

typedef BOOL (WINAPI *DMGETVALUEEXFCTA)(
    LPDM_VARKEYA lpdmVarKey,
    DWORD dwItems,
    LPDM_VAR_UPDATE_STRUCTEXA lpdmvus,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMGETVALUEEXFCTW)(
    LPDM_VARKEYW lpdmVarKey,
    DWORD dwItems,
    LPDM_VAR_UPDATE_STRUCTEXW lpdmvus,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMGETVALUEEX                 "DMGetValueExW"
#define DMGetValueEx                      DMGetValueExW
typedef DMGETVALUEEXFCTW                  DMGETVALUEEXFCT;
#else
#define NAME_DMGETVALUEEX                 "DMGetValueExA"
#define DMGetValueEx                      DMGetValueExA
typedef DMGETVALUEEXFCTA                  DMGETVALUEEXFCT;
#endif

/*-----------------------------------------------------------------------
 * DMGetValueWait
 */
BOOL WINAPI DMGetValueWaitA(
    LPDWORD                  pdwTAID,
    LPDM_VARKEYA             lpdmVarKey,
    DWORD                    dwItems,
    BOOL                     fWaitForCompletition,
    DWORD                    dwTimeOut,
    DM_NOTIFY_VARIABLE_PROCA lpfnVariable,
    LPVOID                   lpvUser,
    LPCMN_ERRORA             lpdmError);

BOOL WINAPI DMGetValueWaitW(
    LPDWORD                  pdwTAID,
    LPDM_VARKEYW             lpdmVarKey,
    DWORD                    dwItems,
    BOOL                     fWaitForCompletition,
    DWORD                    dwTimeOut,
    DM_NOTIFY_VARIABLE_PROCW lpfnVariable,
    LPVOID                   lpvUser,
    LPCMN_ERRORW             lpdmError);

typedef BOOL (WINAPI *DMGETVALUEWAITFCTA)(
    LPDWORD pdwTAID,
    LPDM_VARKEYA lpdmVarKey,
    DWORD dwItems,
    BOOL fWaitForCompletition,
    DWORD dwTimeOut,
    DM_NOTIFY_VARIABLE_PROCA lpfnVariable,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMGETVALUEWAITFCTW)(
    LPDWORD pdwTAID,
    LPDM_VARKEYW lpdmVarKey,
    DWORD dwItems,
    BOOL fWaitForCompletition,
    DWORD dwTimeOut,
    DM_NOTIFY_VARIABLE_PROCW lpfnVariable,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMGETVALUEWAIT             "DMGetValueWaitW"
#define DMGetValueWait                  DMGetValueWaitW
typedef DMGETVALUEWAITFCTW              DMGETVALUEWAITFCT;
#else
#define NAME_DMGETVALUEWAIT             "DMGetValueWaitA"
#define DMGetValueWait                  DMGetValueWaitA
typedef DMGETVALUEWAITFCTA              DMGETVALUEWAITFCT;
#endif

/*-----------------------------------------------------------------------
 * DMGetValueWaitEx
 */
BOOL WINAPI DMGetValueWaitExA(
    LPDWORD                  pdwTAID,
    LPDM_VARKEYA             lpdmVarKey,
    DWORD                    dwItems,
    BOOL                     fWaitForCompletition,
    DWORD                    dwTimeOut,
    DM_NOTIFY_VARIABLEEX_PROCA lpfnVariable,
    LPVOID                   lpvUser,
    LPCMN_ERRORA             lpdmError);

BOOL WINAPI DMGetValueWaitExW(
    LPDWORD                  pdwTAID,
    LPDM_VARKEYW             lpdmVarKey,
    DWORD                    dwItems,
    BOOL                     fWaitForCompletition,
    DWORD                    dwTimeOut,
    DM_NOTIFY_VARIABLEEX_PROCW lpfnVariable,
    LPVOID                   lpvUser,
    LPCMN_ERRORW             lpdmError);

typedef BOOL (WINAPI *DMGETVALUEWAITEXFCTA)(
    LPDWORD pdwTAID,
    LPDM_VARKEYA lpdmVarKey,
    DWORD dwItems,
    BOOL fWaitForCompletition,
    DWORD dwTimeOut,
    DM_NOTIFY_VARIABLEEX_PROCA lpfnVariable,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMGETVALUEWAITEXFCTW)(
    LPDWORD pdwTAID,
    LPDM_VARKEYW lpdmVarKey,
    DWORD dwItems,
    BOOL fWaitForCompletition,
    DWORD dwTimeOut,
    DM_NOTIFY_VARIABLEEX_PROCW lpfnVariable,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMGETVALUEWAITEX             "DMGetValueWaitExW"
#define DMGetValueWaitEx                  DMGetValueWaitExW
typedef DMGETVALUEWAITEXFCTW              DMGETVALUEWAITEXFCT;
#else
#define NAME_DMGETVALUEWAITEX             "DMGetValueWaitExA"
#define DMGetValueWaitEx                  DMGetValueWaitExA
typedef DMGETVALUEWAITEXFCTA              DMGETVALUEWAITEXFCT;
#endif


/*-----------------------------------------------------------------------
 * DMSetValue
 */
BOOL WINAPI DMSetValueA(
    LPDM_VARKEYA lpdmVarKey,
    DWORD dwItems,
    LPVARIANT lpdmValue,
    LPDWORD lpdmVarState,
    LPCMN_ERRORA lpdmError);

BOOL WINAPI DMSetValueW(
    LPDM_VARKEYW lpdmVarKey,
    DWORD dwItems,
    LPVARIANT lpdmValue,
    LPDWORD lpdmVarState,
    LPCMN_ERRORW lpdmError);

typedef BOOL (WINAPI *DMSETVALUEFCTA)(
    LPDM_VARKEYA lpdmVarKey,
    DWORD dwItems,
    LPVARIANT lpdmValue,
    LPDWORD lpdmVarState,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMSETVALUEFCTW)(
    LPDM_VARKEYW lpdmVarKey,
    DWORD dwItems,
    LPVARIANT lpdmValue,
    LPDWORD lpdmVarState,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMSETVALUE                 "DMSetValueW"
#define DMSetValue                      DMSetValueW
typedef DMSETVALUEFCTW                  DMSETVALUEFCT;
#else
#define NAME_DMSETVALUE                 "DMSetValueA"
#define DMSetValue                      DMSetValueA
typedef DMSETVALUEFCTA                  DMSETVALUEFCT;
#endif


/*-----------------------------------------------------------------------
 * DMSetValueMessage
 */
#define DMSVM_OPERATIONREPORT   0x0001
#define DMSVM_OPERATIONMESSAGE  0x0002

BOOL WINAPI DMSetValueMessageA(
    LPDM_VARKEYA lpdmVarKey,
    LPVARIANT lpdmValue,
    DWORD fFlags,
    LPSTR lpszMessage,
    LPCMN_ERRORA lpdmError);

BOOL WINAPI DMSetValueMessageW(
    LPDM_VARKEYW lpdmVarKey,
    LPVARIANT lpdmValue,
    DWORD fFlags,
    LPWSTR lpszMessage,
    LPCMN_ERRORW lpdmError);

typedef BOOL (WINAPI *DMSETVALUEMESSAGEFCTA)(
    LPDM_VARKEYA lpdmVarKey,
    LPVARIANT lpdmValue,
    DWORD fFlags,
    LPSTR lpszMessage,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMSETVALUEMESSAGEFCTW)(
    LPDM_VARKEYW lpdmVarKey,
    LPVARIANT lpdmValue,
    DWORD fFlags,
    LPWSTR lpszMessage,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMSETVALUEMESSAGE          "DMSetValueMessageW"
#define DMSetValueMessage               DMSetValueMessageW
typedef DMSETVALUEMESSAGEFCTW           DMSETVALUEMESSAGEEFCT;
#else
#define NAME_DMSETVALUEMESSAGE          "DMSetValueMessageA"
#define DMSetValueMessage               DMSetValueMessageA
typedef DMSETVALUEMESSAGEFCTA           DMSETVALUEMESSAGEEFCT;
#endif


/*-----------------------------------------------------------------------
 * DMSetValueWait
 */
BOOL WINAPI DMSetValueWaitA(
    LPDWORD pdwTAID,
    LPDM_VARKEYA lpdmVarKey,
    DWORD dwItems,
    LPVARIANT lpdmValue,
    DWORD dwTimeOut,
    DM_COMPLETITION_PROCA lpfnCompletition,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);

BOOL WINAPI DMSetValueWaitW(
    LPDWORD pdwTAID,
    LPDM_VARKEYW lpdmVarKey,
    DWORD dwItems,
    LPVARIANT lpdmValue,
    DWORD dwTimeOut,
    DM_COMPLETITION_PROCW lpfnCompletition,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);

typedef BOOL (WINAPI *DMSETVALUEWAITFCTA)(
    LPDWORD pdwTAID,
    LPDM_VARKEYA lpdmVarKey,
    DWORD dwItems,
    LPVARIANT lpdmValue,
    DWORD dwTimeOut,
    DM_COMPLETITION_PROCA lpfnCompletition,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMSETVALUEWAITFCTW)(
    LPDWORD pdwTAID,
    LPDM_VARKEYW lpdmVarKey,
    DWORD dwItems,
    LPVARIANT lpdmValue,
    DWORD dwTimeOut,
    DM_COMPLETITION_PROCW lpfnCompletition,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMSETVALUEWAIT             "DMSetValueWaitW"
#define DMSetValueWait                  DMSetValueWaitW
typedef DMSETVALUEWAITFCTW              DMSETVALUEWAITFCT;
#else
#define NAME_DMSETVALUEWAIT             "DMSetValueWaitA"
#define DMSetValueWait                  DMSetValueWaitA
typedef DMSETVALUEWAITFCTA              DMSETVALUEWAITFCT;
#endif


/*-----------------------------------------------------------------------
 * DMSetValueWaitMessage
 */
BOOL WINAPI DMSetValueWaitMessageA(
    LPDWORD pdwTAID,
    LPDM_VARKEYA lpdmVarKey,
    LPVARIANT lpdmValue,
    DWORD dwTimeOut,
    DM_COMPLETITION_PROCA lpfnCompletition,
    DWORD fFlags,
    LPSTR lpszMessage,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);

BOOL WINAPI DMSetValueWaitMessageW(
    LPDWORD pdwTAID,
    LPDM_VARKEYW lpdmVarKey,
    LPVARIANT lpdmValue,
    DWORD dwTimeOut,
    DM_COMPLETITION_PROCW lpfnCompletition,
    DWORD fFlags,
    LPWSTR lpszMessage,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);

typedef BOOL (WINAPI *DMSETVALUEWAITMESSAGEFCTA)(
    LPDWORD pdwTAID,
    LPDM_VARKEYA lpdmVarKey,
    LPVARIANT lpdmValue,
    DWORD dwTimeOut,
    DM_COMPLETITION_PROCA lpfnCompletition,
    DWORD fFlags,
    LPSTR lpszMessage,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMSETVALUEWAITMESSAGEFCTW)(
    LPDWORD pdwTAID,
    LPDM_VARKEYW lpdmVarKey,
    LPVARIANT lpdmValue,
    DWORD dwTimeOut,
    DM_COMPLETITION_PROCW lpfnCompletition,
    DWORD fFlags,
    LPWSTR lpszMessage,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMSETVALUEWAITMESSAGE      "DMSetValueWaitMessageW"
#define DMSetValueWaitMessage           DMSetValueWaitMessageW
typedef DMSETVALUEWAITMESSAGEFCTW       DMSETVALUEWAITMESSAGEFCT;
#else
#define NAME_DMSETVALUEWAITMESSAGE      "DMSetValueWaitMessageA"
#define DMSetValueWaitMessage           DMSetValueWaitMessageA
typedef DMSETVALUEWAITMESSAGEFCTA       DMSETVALUEWAITMESSAGEFCT;
#endif


/*-----------------------------------------------------------------------
 * DMGetVarLimits
 */
BOOL WINAPI DMGetVarLimitsA(
    LPCSTR lpszProjectFile,
    LPDM_VARKEYA lpdmVarKey,
    DWORD dwItems,
    LPDM_VARLIMIT lpdmVarLimit,
    LPCMN_ERRORA lpdmError );

BOOL WINAPI DMGetVarLimitsW(
    LPCWSTR lpszProjectFile,
    LPDM_VARKEYW lpdmVarKey,
    DWORD dwItems,
    LPDM_VARLIMIT lpdmVarLimit,
    LPCMN_ERRORW lpdmError );

typedef BOOL (WINAPI *DMGETVARLIMITSFCTA)(
    LPCSTR lpszProjectFile,
    LPDM_VARKEYA lpdmVarKey,
    DWORD dwItems,
    LPDM_VARLIMIT lpdmVarLimit,
    LPCMN_ERRORA lpdmError );
typedef BOOL (WINAPI *DMGETVARLIMITSFCTW)(
    LPCWSTR lpszProjectFile,
    LPDM_VARKEYW lpdmVarKey,
    DWORD dwItems,
    LPDM_VARLIMIT lpdmVarLimit,
    LPCMN_ERRORW lpdmError );

#ifdef UNICODE
#define NAME_DMGETVARLIMITS             "DMGetVarLimitsW"
#define DMGetVarLimits                  DMGetVarLimitsW
typedef DMGETVARLIMITSFCTW              DMGETVARLIMITSFCT;
#else
#define NAME_DMGETVARLIMITS             "DMGetVarLimitsA"
#define DMGetVarLimits                  DMGetVarLimitsA
typedef DMGETVARLIMITSFCTA              DMGETVARLIMITSFCT;
#endif


/*-----------------------------------------------------------------------
 * DMGetVarType
 */
BOOL WINAPI DMGetVarTypeA(
    LPCSTR lpszProjectFile,
    LPDM_VARKEYA lpdmVarKey,
    DWORD dwItems,
    LPDM_TYPEREFA lpdmTypeRef,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMGetVarTypeW(
    LPCWSTR lpszProjectFile,
    LPDM_VARKEYW lpdmVarKey,
    DWORD dwItems,
    LPDM_TYPEREFW lpdmTypeRef,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMGETVARTYPEFCTA)(
    LPCSTR lpszProjectFile,
    LPDM_VARKEYA lpdmVarKey,
    DWORD dwItems,
    LPDM_TYPEREFA lpdmTypeRef,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMGETVARTYPEFCTW)(
    LPCWSTR lpszProjectFile,
    LPDM_VARKEYW lpdmVarKey,
    DWORD dwItems,
    LPDM_TYPEREFW lpdmTypeRef,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMGETVARTYPE               "DMGetVarTypeW"
#define DMGetVarType                    DMGetVarTypeW
typedef DMGETVARTYPEFCTW                DMGETVARTYPEFCT;
#else
#define NAME_DMGETVARTYPE               "DMGetVarTypeA"
#define DMGetVarType                    DMGetVarTypeA
typedef DMGETVARTYPEFCTA                DMGETVARTYPEFCT;
#endif


/*-----------------------------------------------------------------------
 * DMGetVarInfo
 */
BOOL WINAPI DMGetVarInfoA(
    LPCSTR lpszProjectFile,
    LPDM_VARKEYA lpdmVarKey,
    DWORD dwItems,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMGetVarInfoW(
    LPCWSTR lpszProjectFile,
    LPDM_VARKEYW lpdmVarKey,
    DWORD dwItems,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMGETVARINFOFCTA)(
    LPCSTR lpszProjectFile,
    LPDM_VARKEYA lpdmVarKey,
    DWORD dwItems,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMGETVARINFOFCTW)(
    LPCWSTR lpszProjectFile,
    LPDM_VARKEYW lpdmVarKey,
    DWORD dwItems,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMGETVARINFO               "DMGetVarInfoW"
#define DMGetVarInfo                    DMGetVarInfoW
typedef DMGETVARINFOFCTW                DMGETVARINFOFCT;
#else
#define NAME_DMGETVARINFO               "DMGetVarInfoA"
#define DMGetVarInfo                    DMGetVarInfoA
typedef DMGETVARINFOFCTA                DMGETVARINFOFCT;
#endif

/*-----------------------------------------------------------------------
 * DMEditUpdateCycles
 */
BOOL WINAPI DMEditUpdateCyclesA(
    LPCSTR lpszProjectFile,
    HWND hwndParent,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMEditUpdateCyclesW(
    LPCWSTR lpszProjectFile,
    HWND hwndParent,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMEDITUPDATECYCLESFCTA)(
    LPCSTR lpszProjectFile,
    HWND hwndParent,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMEDITUPDATECYCLESFCTW)(
    LPCWSTR lpszProjectFile,
    HWND hwndParent,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMEDITUPDATECYCLES         "DMEditUpdateCyclesW"
#define DMEditUpdateCycles              DMEditUpdateCyclesW
typedef DMEDITUPDATECYCLESFCTW          DMEDITUPDATECYCLESFCT;
#else
#define NAME_DMEDITUPDATECYCLES         "DMEditUpdateCyclesA"
#define DMEditUpdateCycles              DMEditUpdateCyclesA
typedef DMEDITUPDATECYCLESFCTA          DMEDITUPDATECYCLESFCT;
#endif


/*-----------------------------------------------------------------------
 * DMEnumUpdateCycles
 */
BOOL WINAPI DMEnumUpdateCyclesA(
    LPCSTR lpszProjectFile,
    LPDWORD lpdwItems,
    DM_ENUM_CYCLES_PROCA lpfnCycle,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMEnumUpdateCyclesW(
    LPCWSTR lpszProjectFile,
    LPDWORD lpdwItems,
    DM_ENUM_CYCLES_PROCW lpfnCycle,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMENUMUPDATECYCLESFCTA)(
    LPCSTR lpszProjectFile,
    LPDWORD lpdwItems,
    DM_ENUM_CYCLES_PROCA lpfnCycle,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMENUMUPDATECYCLESFCTW)(
    LPCWSTR lpszProjectFile,
    LPDWORD lpdwItems,
    DM_ENUM_CYCLES_PROCW lpfnCycle,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMENUMUPDATECYCLES         "DMEnumUpdateCyclesW"
#define DMEnumUpdateCycles              DMEnumUpdateCyclesW
typedef DMENUMUPDATECYCLESFCTW          DMENUMUPDATECYCLESFCT;
#else
#define NAME_DMENUMUPDATECYCLES         "DMEnumUpdateCyclesA"
#define DMEnumUpdateCycles              DMEnumUpdateCyclesA
typedef DMENUMUPDATECYCLESFCTA          DMENUMUPDATECYCLESFCT;
#endif

/*-----------------------------------------------------------------------
 * DMEnumNumberFormats
 */
BOOL WINAPI DMEnumNumberFormatsA(
    LPDWORD lpdwItems,
    DM_ENUM_FORMATS_PROCA lpfnFormat,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMEnumNumberFormatsW(
    LPDWORD lpdwItems,
    DM_ENUM_FORMATS_PROCW lpfnFormat,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMENUMNUMBERFORMATSFCTA)(
    LPDWORD lpdwItems,
    DM_ENUM_FORMATS_PROCA lpfnFormat,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMENUMNUMBERFORMATSFCTW)(
    LPDWORD lpdwItems,
    DM_ENUM_FORMATS_PROCW lpfnFormat,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMENUMNUMBERFORMATS        "DMEnumNumberFormatsW"
#define DMEnumNumberFormats             DMEnumNumberFormatsW
typedef DMENUMNUMBERFORMATSFCTW         DMENUMNUMBERFORMATSFCT;
#else
#define NAME_DMENUMNUMBERFORMATS        "DMEnumNumberFormatsA"
#define DMEnumNumberFormats             DMEnumNumberFormatsA
typedef DMENUMNUMBERFORMATSFCTA         DMENUMNUMBERFORMATSFCT;
#endif


/*-----------------------------------------------------------------------
 * DMEnumNumberFormats
 */
BOOL WINAPI DMEnumNumberScalingsA(
    LPDWORD lpdwItems,
    DM_ENUM_SCALINGS_PROCA lpfnScale,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMEnumNumberScalingsW(
    LPDWORD lpdwItems,
    DM_ENUM_SCALINGS_PROCW lpfnScale,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMENUMNUMBERSCALINGSFCTA)(
    LPDWORD lpdwItems,
    DM_ENUM_SCALINGS_PROCA lpfnScale,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMENUMNUMBERSCALINGSFCTW)(
    LPDWORD lpdwItems,
    DM_ENUM_SCALINGS_PROCW lpfnScale,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);


#ifdef UNICODE
#define NAME_DMENUMNUMBERSCALINGS       "DMEnumNumberScalingsW"
#define DMEnumNumberScalings            DMEnumNumberScalingsW
typedef DMENUMNUMBERSCALINGSFCTW        DMENUMNUMBERSCALINGSFCT;
#else
#define NAME_DMENUMNUMBERSCALINGS       "DMEnumNumberScalingsA"
#define DMEnumNumberScalings            DMEnumNumberScalingsA
typedef DMENUMNUMBERSCALINGSFCTA        DMENUMNUMBERSCALINGSFCT;
#endif


/*-----------------------------------------------------------------------
 * DMEnumScaleParams
 */
BOOL WINAPI DMEnumScaleParamsA(
    LPDWORD lpdwItems,
    LPDM_SCALE_INFOA lpScaleInfo,
    DM_ENUM_SCALE_PARAMS_PROCA lpfnParams,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMEnumScaleParamsW(
    LPDWORD lpdwItems,
    LPDM_SCALE_INFOW lpScaleInfo,
    DM_ENUM_SCALE_PARAMS_PROCW lpfnParams,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMENUMSCALEPARAMSFCTA)(
    LPDWORD lpdwItems,
    LPDM_SCALE_INFOA lpScaleInfo,
    DM_ENUM_SCALE_PARAMS_PROCA lpfnParams,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMENUMSCALEPARAMSFCTW)(
    LPDWORD lpdwItems,
    LPDM_SCALE_INFOW lpScaleInfo,
    DM_ENUM_SCALE_PARAMS_PROCW lpfnParams,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMENUMSCALEPARAMS          "DMEnumScaleParamsW"
#define DMEnumScaleParams               DMEnumScaleParamsW
typedef DMENUMSCALEPARAMSFCTW           DMENUMSCALEPARAMSFCT;
#else
#define NAME_DMENUMSCALEPARAMS          "DMEnumScaleParamsA"
#define DMEnumScaleParams               DMEnumScaleParamsA
typedef DMENUMSCALEPARAMSFCTA           DMENUMSCALEPARAMSFCT;
#endif


/*-----------------------------------------------------------------------
 * DMEnumScaleParams
 */
BOOL WINAPI DMEnumVarScaleParamsA(
    LPCSTR lpszProjectFile,
    LPDWORD lpdwItems,
    LPDM_VARKEYA lpdmVarKey,
    DM_ENUM_VAR_SCALE_PARAMS_PROCA lpfnParams,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMEnumVarScaleParamsW(
    LPCWSTR lpszProjectFile,
    LPDWORD lpdwItems,
    LPDM_VARKEYW lpdmVarKey,
    DM_ENUM_VAR_SCALE_PARAMS_PROCW lpfnParams,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMENUMVARSCALEPARAMSFCTA)(
    LPCSTR lpszProjectFile,
    LPDWORD lpdwItems,
    LPDM_VARKEYA lpdmVarKey,
    DM_ENUM_VAR_SCALE_PARAMS_PROCA lpfnParams,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMENUMVARSCALEPARAMSFCTW)(
    LPCWSTR lpszProjectFile,
    LPDWORD lpdwItems,
    LPDM_VARKEYW lpdmVarKey,
    DM_ENUM_VAR_SCALE_PARAMS_PROCW lpfnParams,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMENUMVARSCALEPARAMS       "DMEnumVarScaleParamsW"
#define DMEnumVarScaleParams            DMEnumVarScaleParamsW
typedef DMENUMVARSCALEPARAMSFCTW        DMENUMVARSCALEPARAMSFCT;
#else
#define NAME_DMENUMVARSCALEPARAMS       "DMEnumVarScaleParamsA"
#define DMEnumVarScaleParams            DMEnumVarScaleParamsA
typedef DMENUMVARSCALEPARAMSFCTA        DMENUMVARSCALEPARAMSFCT;
#endif


/*-----------------------------------------------------------------------
 * DMEnumBitFieldBits
 */
BOOL WINAPI DMEnumBitFieldBitsA(
    LPCSTR lpszProjectFile,
    LPDWORD lpdwItems,
    LPDM_TYPEREFA lpdmTypeRef,
    DM_ENUM_BITFIELD_BITS_PROCA lpfnBits,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMEnumBitFieldBitsW(
    LPCWSTR lpszProjectFile,
    LPDWORD lpdwItems,
    LPDM_TYPEREFW lpdmTypeRef,
    DM_ENUM_BITFIELD_BITS_PROCW lpfnBits,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMENUMBITFIELDBITSFCTA)(
    LPCSTR lpszProjectFile,
    LPDWORD lpdwItems,
    LPDM_TYPEREFA lpdmTypeRef,
    DM_ENUM_BITFIELD_BITS_PROCA lpfnBits,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMENUMBITFIELDBITSFCTW)(
    LPCWSTR lpszProjectFile,
    LPDWORD lpdwItems,
    LPDM_TYPEREFW lpdmTypeRef,
    DM_ENUM_BITFIELD_BITS_PROCW lpfnBits,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMENUMBITFIELDBITS         "DMEnumBitFieldBitsW"
#define DMEnumBitFieldBits              DMEnumBitFieldBitsW
typedef DMENUMBITFIELDBITSFCTW          DMENUMBITFIELDBITSFCT;
#else
#define NAME_DMENUMBITFIELDBITS         "DMEnumBitFieldBitsA"
#define DMEnumBitFieldBits              DMEnumBitFieldBitsA
typedef DMENUMBITFIELDBITSFCTA          DMENUMBITFIELDBITSFCT;
#endif


/*-----------------------------------------------------------------------
 * DMCreateBitFieldType
 */
BOOL WINAPI DMCreateBitFieldTypeA(
    LPCSTR lpszProjectFile,
    LPDM_TYPE_DESCRIPTORA lpdmType,
    DWORD dwNumBits,
    LPDM_BITFIELD_BIT_DESCRIPTORA lpdmBits,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMCreateBitFieldTypeW(
    LPCWSTR lpszProjectFile,
    LPDM_TYPE_DESCRIPTORW lpdmType,
    DWORD dwNumBits,
    LPDM_BITFIELD_BIT_DESCRIPTORW lpdmBits,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMCREATEBITFIELDTYPEFCTA)(
    LPCSTR lpszProjectFile,
    LPDM_TYPE_DESCRIPTORA lpdmType,
    DWORD dwNumBits,
    LPDM_BITFIELD_BIT_DESCRIPTORA lpdmBits,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMCREATEBITFIELDTYPEFCTW)(
    LPCWSTR lpszProjectFile,
    LPDM_TYPE_DESCRIPTORW lpdmType,
    DWORD dwNumBits,
    LPDM_BITFIELD_BIT_DESCRIPTORW lpdmBits,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMCREATEBITFIELDTYPE       "DMCreateBitFieldTypeW"
#define DMCreateBitFieldType            DMCreateBitFieldTypeW
typedef DMCREATEBITFIELDTYPEFCTW        DMCREATEBITFIELDTYPEFCT;
#else
#define NAME_DMCREATEBITFIELDTYPE       "DMCreateBitFieldTypeA"
#define DMCreateBitFieldType            DMCreateBitFieldTypeA
typedef DMCREATEBITFIELDTYPEFCTA        DMCREATEBITFIELDTYPEFCT;
#endif


/*-----------------------------------------------------------------------
 * DMCreateArrayType
 */
BOOL WINAPI DMCreateArrayTypeA(
    LPCSTR lpszProjectFile,
    LPDM_TYPE_DESCRIPTORA lpdmType,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMCreateArrayTypeW(
    LPCWSTR lpszProjectFile,
    LPDM_TYPE_DESCRIPTORW lpdmType,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMCREATEARRAYTYPEFCTA)(
    LPCSTR lpszProjectFile,
    LPDM_TYPE_DESCRIPTORA lpdmType,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMCREATEARRAYTYPEFCTW)(
    LPCWSTR lpszProjectFile,
    LPDM_TYPE_DESCRIPTORW lpdmType,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMCREATEARRAYTYPE          "DMCreateArrayTypeW"
#define DMCreateArrayType               DMCreateArrayTypeW
typedef DMCREATEARRAYTYPEFCTW           DMCREATEARRAYTYPEFCT;
#else
#define NAME_DMCREATEARRAYTYPE          "DMCreateArrayTypeA"
#define DMCreateArrayType               DMCreateArrayTypeA
typedef DMCREATEARRAYTYPEFCTA           DMCREATEARRAYTYPEFCT;
#endif


/*-----------------------------------------------------------------------
 * DMCreateStructType
 */
BOOL WINAPI DMCreateStructTypeA(
    LPCSTR lpszProjectFile,
    LPDM_TYPE_DESCRIPTORA lpdmType,
    DWORD dwNumMembers,
    LPDM_STRUCT_MEMBER_DESCRIPTORA lpdmMembers,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMCreateStructTypeW(
    LPCWSTR lpszProjectFile,
    LPDM_TYPE_DESCRIPTORW lpdmType,
    DWORD dwNumMembers,
    LPDM_STRUCT_MEMBER_DESCRIPTORW lpdmMembers,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMCREATESTRUCTTYPEFCTA)(
    LPCSTR lpszProjectFile,
    LPDM_TYPE_DESCRIPTORA lpdmType,
    DWORD dwNumMembers,
    LPDM_STRUCT_MEMBER_DESCRIPTORA lpdmMembers,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMCREATESTRUCTTYPEFCTW)(
    LPCWSTR lpszProjectFile,
    LPDM_TYPE_DESCRIPTORW lpdmType,
    DWORD dwNumMembers,
    LPDM_STRUCT_MEMBER_DESCRIPTORW lpdmMembers,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMCREATESTRUCTTYPE         "DMCreateStructTypeW"
#define DMCreateStructType              DMCreateStructTypeW
typedef DMCREATESTRUCTTYPEFCTW          DMCREATESTRUCTTYPEFCT;
#else
#define NAME_DMCREATESTRUCTTYPE         "DMCreateStructTypeA"
#define DMCreateStructType              DMCreateStructTypeA
typedef DMCREATESTRUCTTYPEFCTA          DMCREATESTRUCTTYPEFCT;
#endif


/*-----------------------------------------------------------------------
 * DMShowVarProperties
 */
BOOL WINAPI DMShowVarPropertiesA(
    LPCSTR lpszProjectFile,
    HWND hwndParent,
    LPDM_VARKEYA lpdmVarKey,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMShowVarPropertiesW(
    LPCWSTR lpszProjectFile,
    HWND hwndParent,
    LPDM_VARKEYW lpdmVarKey,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMSHOWVARPROPERTIESFCTA)(
    LPCSTR lpszProjectFile,
    HWND hwndParent,
    LPDM_VARKEYA lpdmVarKey,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMSHOWVARPROPERTIESFCTW)(
    LPCWSTR lpszProjectFile,
    HWND hwndParent,
    LPDM_VARKEYW lpdmVarKey,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMSHOWVARPROPERTIES        "DMShowVarPropertiesW"
#define DMShowVarProperties             DMShowVarPropertiesW
typedef DMSHOWVARPROPERTIESFCTW         DMSHOWVARPROPERTIESFCT;
#else
#define NAME_DMSHOWVARPROPERTIES        "DMShowVarPropertiesA"
#define DMShowVarProperties             DMShowVarPropertiesA
typedef DMSHOWVARPROPERTIESFCTA         DMSHOWVARPROPERTIESFCT;
#endif


/*-----------------------------------------------------------------------
 * DMShowVarDatabase
 */
BOOL WINAPI DMShowVarDatabaseA(
    LPCSTR lpszProjectFile,
    HWND hwndParent,
    LPDM_DLGOPTIONSA lpdmOptions,
    LPDM_VARFILTERA lpdmFilter,
    LPDM_VARKEYA lpdmVarKey,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMShowVarDatabaseW(
    LPCWSTR lpszProjectFile,
    HWND hwndParent,
    LPDM_DLGOPTIONSW lpdmOptions,
    LPDM_VARFILTERW lpdmFilter,
    LPDM_VARKEYW lpdmVarKey,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMSHOWVARDATABASEFCTA)(
    LPCSTR lpszProjectFile,
    HWND hwndParent,
    LPDM_DLGOPTIONSA lpdmOptions,
    LPDM_VARFILTERA lpdmFilter,
    LPDM_VARKEYA lpdmVarKey,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMSHOWVARDATABASEFCTW)(
    LPCWSTR lpszProjectFile,
    HWND hwndParent,
    LPDM_DLGOPTIONSW lpdmOptions,
    LPDM_VARFILTERW lpdmFilter,
    LPDM_VARKEYW lpdmVarKey,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMSHOWVARDATABASE          "DMShowVarDatabaseW"
#define DMShowVarDatabase               DMShowVarDatabaseW
typedef DMSHOWVARDATABASEFCTW           DMSHOWVARDATABASEFCT;
#else
#define NAME_DMSHOWVARDATABASE          "DMShowVarDatabaseA"
#define DMShowVarDatabase               DMShowVarDatabaseA
typedef DMSHOWVARDATABASEFCTA           DMSHOWVARDATABASEFCT;
#endif


/*-----------------------------------------------------------------------
 * DMShowVarDatabaseMulti
 */
BOOL WINAPI DMShowVarDatabaseMultiA(
    LPCSTR lpszProjectFile,
    HWND hwndParent,
    LPDM_DLGOPTIONSA lpdmOptions,
    LPDM_VARFILTERA lpdmFilter,
    LPDWORD lpdwItems,
    DM_NOTIFY_SELECT_VAR_PROCA lpfnVariables,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMShowVarDatabaseMultiW(
    LPCWSTR lpszProjectFile,
    HWND hwndParent,
    LPDM_DLGOPTIONSW lpdmOptions,
    LPDM_VARFILTERW lpdmFilter,
    LPDWORD lpdwItems,
    DM_NOTIFY_SELECT_VAR_PROCW lpfnVariables,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMSHOWVARDATABASEMULTIFCTA)(
    LPCSTR lpszProjectFile,
    HWND hwndParent,
    LPDM_DLGOPTIONSA lpdmOptions,
    LPDM_VARFILTERA lpdmFilter,
    LPDWORD lpdwItems,
    DM_NOTIFY_SELECT_VAR_PROCA lpfnVariables,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMSHOWVARDATABASEMULTIFCTW)(
    LPCWSTR lpszProjectFile,
    HWND hwndParent,
    LPDM_DLGOPTIONSW lpdmOptions,
    LPDM_VARFILTERW lpdmFilter,
    LPDWORD lpdwItems,
    DM_NOTIFY_SELECT_VAR_PROCW lpfnVariables,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMSHOWVARDATABASEMULTI     "DMShowVarDatabaseMultiW"
#define DMShowVarDatabaseMulti          DMShowVarDatabaseMultiW
typedef DMSHOWVARDATABASEMULTIFCTW      DMSHOWVARDATABASEMULTIFCT;
#else
#define NAME_DMSHOWVARDATABASEMULTI     "DMShowVarDatabaseMultiA"
#define DMShowVarDatabaseMulti          DMShowVarDatabaseMultiA
typedef DMSHOWVARDATABASEMULTIFCTA      DMSHOWVARDATABASEMULTIFCT;
#endif


/*-----------------------------------------------------------------------
 * DMShiftDialog
 */
BOOL WINAPI DMShiftDialogA(
    LPCSTR lpszProjectFile,
    HWND hwndParent,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMShiftDialogW(
    LPCWSTR lpszProjectFile,
    HWND hwndParent,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMSHIFTDIALOGFCTA)(
    LPCSTR lpszProjectFile,
    HWND hwndParent,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMSHIFTDIALOGFCTW)(
    LPCWSTR lpszProjectFile,
    HWND hwndParent,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMSHIFTDIALOG              "DMShiftDialogW"
#define DMShiftDialog                   DMShiftDialogW
typedef DMSHIFTDIALOGFCTW               DMSHIFTDIALOGFCT;
#else
#define NAME_DMSHIFTDIALOG              "DMShiftDialogA"
#define DMShiftDialog                   DMShiftDialogA
typedef DMSHIFTDIALOGFCTA               DMSHIFTDIALOGFCT;
#endif


/*-----------------------------------------------------------------------
 * DMEnumShifts
 */
BOOL WINAPI DMEnumShiftsA(
    LPCSTR lpszProjectFile,
    LPDWORD lpdwItems,
    DM_ENUM_SHIFTS_PROCA  lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMEnumShiftsW(
    LPCWSTR lpszProjectFile,
    LPDWORD lpdwItems,
    DM_ENUM_SHIFTS_PROCW  lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMENUMSHIFTSFCTA)(
    LPCSTR lpszProjectFile,
    LPDWORD lpdwItems,
    DM_ENUM_SHIFTS_PROCA  lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMENUMSHIFTSFCTW)(
    LPCWSTR lpszProjectFile,
    LPDWORD lpdwItems,
    DM_ENUM_SHIFTS_PROCW  lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMENUMSHIFTS               "DMEnumShiftsW"
#define DMEnumShifts                    DMEnumShiftsW
typedef DMENUMSHIFTSFCTW                DMENUMSHIFTSFCT;
#else
#define NAME_DMENUMSHIFTS               "DMEnumShiftsA"
#define DMEnumShifts                    DMEnumShiftsA
typedef DMENUMSHIFTSFCTA                DMENUMSHIFTSFCT;
#endif


/*-----------------------------------------------------------------------
 * DMGetShiftMap
 */
BOOL WINAPI DMGetShiftMapA(
    LPCSTR lpszProjectFile,
    LPCSTR lpszName,
    LPDM_SHIFT_DATA lpdmShiftData,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMGetShiftMapW(
    LPCWSTR lpszProjectFile,
    LPCWSTR lpszName,
    LPDM_SHIFT_DATA lpdmShiftData,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMGETSHIFTMAPFCTA)(
    LPCSTR lpszProjectFile,
    LPCSTR lpszName,
    LPDM_SHIFT_DATA lpdmShiftData,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMGETSHIFTMAPFCTW)(
    LPCWSTR lpszProjectFile,
    LPCWSTR lpszName,
    LPDM_SHIFT_DATA lpdmShiftData,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMGETSHIFTMAP              "DMGetShiftMapW"
#define DMGetShiftMap                   DMGetShiftMapW
typedef DMGETSHIFTMAPFCTW               DMGETSHIFTMAPFCT;
#else
#define NAME_DMGETSHIFTMAP              "DMGetShiftMapA"
#define DMGetShiftMap                   DMGetShiftMapA
typedef DMGETSHIFTMAPFCTA               DMGETSHIFTMAPFCT;
#endif


/*-----------------------------------------------------------------------
 * DMInstallDataService
 */
BOOL WINAPI DMInstallDataServiceA(
    LPCSTR lpszService,
    DM_DATA_SERVICE_PROCA lpfnService,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMInstallDataServiceW(
    LPCWSTR lpszService,
    DM_DATA_SERVICE_PROCW lpfnService,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMINSTALLDATASERVICEFCTA)(
    LPCSTR lpszService,
    DM_DATA_SERVICE_PROCA lpfnService,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMINSTALLDATASERVICEFCTW)(
    LPCWSTR lpszService,
    DM_DATA_SERVICE_PROCW lpfnService,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMINSTALLDATASERVICE       "DMInstallDataServiceW"
#define DMInstallDataService            DMInstallDataServiceW
typedef DMINSTALLDATASERVICEFCTW        DMINSTALLDATASERVICEFCT;
#else
#define NAME_DMINSTALLDATASERVICE       "DMInstallDataServiceA"
#define DMInstallDataService            DMInstallDataServiceA
typedef DMINSTALLDATASERVICEFCTA        DMINSTALLDATASERVICEFCT;
#endif


/*-----------------------------------------------------------------------
 * DMEnumDataServices
 */
DWORD WINAPI DMEnumDataServicesA(
    LPDWORD lpdwItems,
    DM_ENUM_DATA_SERVICE_PROCA lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);


DWORD WINAPI DMEnumDataServicesW(
    LPDWORD lpdwItems,
    DM_ENUM_DATA_SERVICE_PROCW lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMENUMDATASERVICESFCTA)(
    LPDWORD lpdwItems,
    DM_ENUM_DATA_SERVICE_PROCA lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMENUMDATASERVICESFCTW)(
    LPDWORD lpdwItems,
    DM_ENUM_DATA_SERVICE_PROCW lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMENUMDATASERVICES         "DMEnumDataServicesW"
#define DMEnumDataServices              DMEnumDataServicesW
typedef DMENUMDATASERVICESFCTW          DMENUMDATASERVICESFCT;
#else
#define NAME_DMENUMDATASERVICE          "DMEnumDataServicesA"
#define DMEnumDataServices              DMEnumDataServicesA
typedef DMENUMDATASERVICESFCTA          DMENUMDATASERVICESFCT;
#endif


/*-----------------------------------------------------------------------
 * DMendApplicationData
 */
BOOL WINAPI DMSendApplicationDataA(
    LPDM_SEND_DATA_STRUCTA lpdmSendData,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMSendApplicationDataW(
    LPDM_SEND_DATA_STRUCTW lpdmSendData,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMSENDAPPLICATIONDATAFCTA)(
    LPDM_SEND_DATA_STRUCTA lpdmSendData,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMSENDAPPLICATIONDATAFCTW)(
    LPDM_SEND_DATA_STRUCTW lpdmSendData,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMSENDAPPLICATIONDATA      "DMSendApplicationDataW"
#define DMSendApplicationData           DMSendApplicationDataW
typedef DMSENDAPPLICATIONDATAFCTW       DMSENDAPPLICATIONDATAFCT;
#else
#define NAME_DMSENDAPPLICATIONDATA      "DMSendApplicationDataA"
#define DMSendApplicationData           DMSendApplicationDataA
typedef DMSENDAPPLICATIONDATAFCTA       DMSENDAPPLICATIONDATAFCT;
#endif


/*-----------------------------------------------------------------------
 * DMGetMachineTable
 */
BOOL WINAPI DMGetMachineTableA(
    LPCSTR lpszProjectFile,
    LPDM_MACHINE_TABLEA lpdmMachineTable,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMGetMachineTableW(
    LPCWSTR lpszProjectFile,
    LPDM_MACHINE_TABLEW lpdmMachineTable,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMGETMACHINETABLEFCTA)(
    LPCSTR lpszProjectFile,
    LPDM_MACHINE_TABLEA lpdmMachineTable,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMGETMACHINETABLEFCTW)(
    LPCWSTR lpszProjectFile,
    LPDM_MACHINE_TABLEW lpdmMachineTable,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMGETMACHINETABLE      "DMGetMachineTableW"
#define DMGetMachineTable           DMGetMachineTableW
typedef DMGETMACHINETABLEFCTW       DMGETMACHINETABLEFCT;
#else
#define NAME_DMGETMACHINETABLE      "DMGetMachineTableA"
#define DMGetMachineTable           DMGetMachineTableA
typedef DMGETMACHINETABLEFCTA       DMGETMACHINETABLEFCT;
#endif


/*-----------------------------------------------------------------------
 * DMGetOSVersion
 */
DWORD WINAPI DMGetOSVersionA(VOID);

DWORD WINAPI DMGetOSVersionW(VOID);

typedef BOOL (WINAPI *DMGETOSVERSIONFCTA)(VOID);
typedef BOOL (WINAPI *DMGETOSVERSIONFCTW)(VOID);

#ifdef UNICODE
#define NAME_DMGETOSVERSION             "DMGetOSVersionW"
#define DMGetOSVersion                  DMGetOSVersionW
typedef DMGETOSVERSIONFCTW              DMGETOSVERSIONFCT;
#else
#define NAME_DMGETOSVERSION             "DMGetOSVersionA"
#define DMGetOSVersion                  DMGetOSVersionA
typedef DMGETOSVERSIONFCTA              DMGETOSVERSIONFCT;
#endif


/*-----------------------------------------------------------------------
 * DMShowLanguageDialog
 */
BOOL WINAPI DMShowLanguageDialogA(
    HWND hwndParent,
    DWORD dwFlags,
    DWORD dwSetLocaleIDs[],
    UINT uSetIDArraySize,
    LPDWORD lpdwGetLocaleID,
    LPCMN_ERRORA lpdmError);

BOOL WINAPI DMShowLanguageDialogW(
    HWND hwndParent,
    DWORD dwFlags,
    DWORD dwSetLocaleIDs[],
    UINT uSetIDArraySize,
    LPDWORD lpdwGetLocaleID,
    LPCMN_ERRORW lpdmError);

typedef BOOL (WINAPI *DMSHOWLANGUAGEDIALOGFCTA)(
    HWND hwndParent,
    DWORD dwFlags,
    DWORD dwSetLocaleIDs[],
    UINT uSetIDArraySize,
    LPDWORD lpdwGetLocaleID,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMSHOWLANGUAGEDIALOGFCTW)(
    HWND hwndParent,
    DWORD dwFlags,
    DWORD dwSetLocaleIDs[],
    UINT uSetIDArraySize,
    LPDWORD lpdwGetLocaleID,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMSHOWLANGUAGEDIALOG       "DMShowLanguageDialogW"
#define DMShowLanguageDialog            DMShowLanguageDialogW
typedef DMSHOWLANGUAGEDIALOGFCTW        DMSHOWLANGUAGEDIALOGFCT;
#else
#define NAME_DMSHOWLANGUAGEDIALOG       "DMShowLanguageDialogA"
#define DMShowLanguageDialog            DMShowLanguageDialogA
typedef DMSHOWLANGUAGEDIALOGFCTA        DMSHOWLANGUAGEDIALOGFCT;
#endif


/*-----------------------------------------------------------------------
 * DMChangeDataLocale
 */
BOOL WINAPI DMChangeDataLocaleA(
    LPCSTR lpszProjectFile,
    DWORD dwLocaleID,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMChangeDataLocaleW(
    LPCWSTR lpszProjectFile,
    DWORD dwLocaleID,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMCHANGEDATALOCALEFCTA)(
    LPCSTR lpszProjectFile,
    DWORD dwLocaleID,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMCHANGEDATALOCALEFCTW)(
    LPCWSTR lpszProjectFile,
    DWORD dwLocaleID,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMCHANGEDATALOCALE         "DMChangeDataLocaleW"
#define DMChangeDataLocale              DMChangeDataLocaleW
typedef DMCHANGEDATALOCALEFCTW          DMCHANGEDATALOCALEFCT;
#else
#define NAME_DMCHANGEDATALOCALE         "DMChangeDataLocaleA"
#define DMChangeDataLocale              DMChangeDataLocaleA
typedef DMCHANGEDATALOCALEFCTA          DMCHANGEDATALOCALEFCT;
#endif


/*-----------------------------------------------------------------------
 * DMRefreshExplorerTree
 */
BOOL WINAPI DMRefreshExplorerTreeA(
    LPCSTR lpszProjectFile,
    LPCSTR lpszExtDLL,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMRefreshExplorerTreeW(
    LPCWSTR lpszProjectFile,
    LPCWSTR lpszExtDLL,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMREFRESHEXPLORERTREEFCTA)(
    LPCSTR lpszProjectFile,
    LPCSTR lpszExtDLL,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMREFRESHEXPLORERTREEFCTW)(
    LPCWSTR lpszProjectFile,
    LPCWSTR lpszExtDLL,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMREFRESHEXPLORERTREE      "DMRefreshExplorerTreeW"
#define DMRefreshExplorerTree           DMRefreshExplorerTreeW
typedef DMREFRESHEXPLORERTREEFCTW       DMREFRESHEXPLORERTREEFCT;
#else
#define NAME_DMREFRESHEXPLORERTREE      "DMRefreshExplorerTreeA"
#define DMRefreshExplorerTree           DMRefreshExplorerTreeA
typedef DMREFRESHEXPLORERTREEFCTA       DMREFRESHEXPLORERTREEFCT;
#endif


/*-----------------------------------------------------------------------
 * DMGetSystemLocale
 */
BOOL WINAPI DMGetSystemLocaleA(
    LPDWORD lpdwLocaleID,
    LPCMN_ERRORA lpdmError);

BOOL WINAPI DMGetSystemLocaleW(
    LPDWORD lpdwLocaleID,
    LPCMN_ERRORW lpdmError);

typedef BOOL (WINAPI *DMGETSYSTEMLOCALEFCTA)(
    LPDWORD lpdwLocaleID,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMGETSYSTEMLOCALEFCTW)(
    LPDWORD lpdwLocaleID,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMGETSYSTEMLOCALE          "DMGetSystemLocaleW"
#define DMGetSystemLocale               DMGetSystemLocaleW
typedef DMGETSYSTEMLOCALEFCTW           DMGETSYSTEMLOCALEFCT;
#else
#define NAME_DMGETSYSTEMLOCALE          "DMGetSystemLocaleA"
#define DMGetSystemLocale               DMGetSystemLocaleA
typedef DMGETSYSTEMLOCALEFCTA           DMGETSYSTEMLOCALEFCT;
#endif


/*-----------------------------------------------------------------------
 * DMGetDataLocale
 */
BOOL WINAPI DMGetDataLocaleA(
    LPDWORD lpdwLocaleID,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMGetDataLocaleW(
    LPDWORD lpdwLocaleID,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMGETDATALOCALEFCTA)(
    LPDWORD lpdwLocaleID,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMGETDATALOCALEFCTW)(
    LPDWORD lpdwLocaleID,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMGETDATALOCALE            "DMGetDataLocaleW"
#define DMGetDataLocale                 DMGetDataLocaleW
typedef DMGETDATALOCALEFCTW             DMGETDATALOCALEFCT;
#else
#define NAME_DMGETDATALOCALE            "DMGetDataLocaleA"
#define DMGetDataLocale                 DMGetDataLocaleA
typedef DMGETDATALOCALEFCTA             DMGETDATALOCALEFCT;
#endif


/*-----------------------------------------------------------------------
 * DMCreateObjectA
 */
/*
 * Use this structure to insert a new driver into a project
 */
typedef struct
{
    DWORD       dwSize;                 /* Size of structure */
    char        szPath[_MAX_PATH + 1];      /* Path of driver to load */
} MCP_NEWDRIVER_DATAA, *LPMCP_NEWDRIVER_DATAA;
typedef struct
{
    DWORD       dwSize;                 /* Size of structure */
    WCHAR       szPath[_MAX_PATH + 1];      /* Path of driver to load */
} MCP_NEWDRIVER_DATAW, *LPMCP_NEWDRIVER_DATAW;

#ifdef UNICODE
typedef MCP_NEWDRIVER_DATAW     MCP_NEWDRIVER_DATA;
typedef LPMCP_NEWDRIVER_DATAW   LPMCP_NEWDRIVER_DATA;
#else
typedef MCP_NEWDRIVER_DATAA     MCP_NEWDRIVER_DATA;
typedef LPMCP_NEWDRIVER_DATAA   LPMCP_NEWDRIVER_DATA;
#endif

typedef struct tagMCP_NEWGROUP_DATAA
{
    DWORD   dwSize;
    char    szGroupName   [MAX_DM_GROUP_NAME +1] ;
    char    szConnection  [MAX_DM_CONNECTION_NAME +1] ;
} MCP_NEWGROUP_DATAA, FAR *LPMCP_NEWGROUP_DATAA;
typedef struct tagMCP_NEWGROUP_DATAW
{
    DWORD   dwSize;
    WCHAR   szGroupName   [MAX_DM_GROUP_NAME +1] ;
    WCHAR   szConnection  [MAX_DM_CONNECTION_NAME +1] ;
} MCP_NEWGROUP_DATAW, FAR *LPMCP_NEWGROUP_DATAW;

#ifdef UNICODE
typedef MCP_NEWGROUP_DATAW         MCP_NEWGROUP_DATA;
typedef LPMCP_NEWGROUP_DATAW       LPMCP_NEWGROUP_DATA;
#else
typedef MCP_NEWGROUP_DATAA         MCP_NEWGROUP_DATA;
typedef LPMCP_NEWGROUP_DATAA       LPMCP_NEWGROUP_DATA;
#endif


BOOL WINAPI DMCreateObjectA(
    DWORD dwObjectType,
    LPCSTR lpszProjectFile,
    LPVOID lpvObjectData,
    DWORD dwFlags,
    DWORD dwCreatorID,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMCreateObjectW(
    DWORD dwObjectType,
    LPCWSTR lpszProjectFile,
    LPVOID lpvObjectData,
    DWORD dwFlags,
    DWORD dwCreatorID,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMCREATEOBJECTFCTA)(
    DWORD dwObjectType,
    LPCSTR lpszProjectFile,
    LPVOID lpvObjectData,
    DWORD dwFlags,
    DWORD dwCreatorID,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMCREATEOBJECTFCTW)(
    DWORD dwObjectType,
    LPCWSTR lpszProjectFile,
    LPVOID lpvObjectData,
    DWORD dwFlags,
    DWORD dwCreatorID,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMCREATEOBJECT             "DMCreateObjectW"
#define DMCreateObject                  DMCreateObjectW
typedef DMCREATEOBJECTFCTW              DMCREATEOBJECTFCT;
#else
#define NAME_DMCREATEOBJECT             "DMCreateObjectA"
#define DMCreateObject                  DMCreateObjectA
typedef DMCREATEOBJECTFCTA              DMCREATEOBJECTFCT;
#endif


/*-----------------------------------------------------------------------
 * DMRenameObjectA
 */
BOOL WINAPI DMRenameObjectA(
    DWORD dwObjectType,
    LPCSTR lpszProjectFile,
    LPCSTR lpszOldName,
    LPCSTR lpszNewName,
    DWORD dwFlags,
    DWORD dwCreatorID,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMRenameObjectW(
    DWORD dwObjectType,
    LPCWSTR lpszProjectFile,
    LPCWSTR lpszOldName,
    LPCWSTR lpszNewName,
    DWORD dwFlags,
    DWORD dwCreatorID,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMRENAMEOBJECTFCTA)(
    DWORD dwObjectType,
    LPCSTR lpszProjectFile,
    LPCSTR lpszOldName,
    LPCSTR lpszNewName,
    DWORD dwFlags,
    DWORD dwCreatorID,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMRENAMEOBJECTFCTW)(
    DWORD dwObjectType,
    LPWSTR lpszProjectFile,
    LPWSTR lpszOldName,
    LPWSTR lpszNewName,
    DWORD dwFlags,
    DWORD dwCreatorID,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMRENAMEOBJECT             "DMRenameObjectW"
#define DMRenameObject                  DMRenameObjectW
typedef DMRENAMEOBJECTFCTW              DMRENAMEOBJECTFCT;
#else
#define NAME_DMRENAMEOBJECT             "DMRenameObjectA"
#define DMRenameObject                  DMRenameObjectA
typedef DMRENAMEOBJECTFCTA              DMRENAMEOBJECTFCT;
#endif


/*-----------------------------------------------------------------------
 * DMDeleteAllObjectsA
 */
BOOL WINAPI DMDeleteAllObjectsA(
    DWORD dwObjectType,
    LPCSTR lpszProjectFile,
    LPCSTR lpszName,
    DWORD dwFlags,
    DWORD dwCreatorID,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMDeleteAllObjectsW(
    DWORD dwObjectType,
    LPCWSTR lpszProjectFile,
    LPCWSTR lpszName,
    DWORD dwFlags,
    DWORD dwCreatorID,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMDELETEALLOBJECTSFCTA)(
    DWORD dwObjectType,
    LPCSTR lpszProjectFile,
    LPCSTR lpszName,
    DWORD dwFlags,
    DWORD dwCreatorID,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMDELETEALLOBJECTSFCTW)(
    DWORD dwObjectType,
    LPWSTR lpszProjectFile,
    LPWSTR lpszName,
    DWORD dwFlags,
    DWORD dwCreatorID,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMDELETEALLOBJECTS         "DMDeleteAllObjectsW"
#define DMDeleteAllObjects              DMDeleteAllObjectsW
typedef DMDELETEALLOBJECTSFCTW          DMDELETEALLOBJECTSFCT;
#else
#define NAME_DMDELETEALLOBJECTS         "DMDeleteAllObjectsA"
#define DMDeleteAllObjects              DMDeleteAllObjectsA
typedef DMDELETEALLOBJECTSFCTA          DMDELETEALLOBJECTSFCT;
#endif


/*-----------------------------------------------------------------------
 * DMDeleteObjectA
 */
BOOL WINAPI DMDeleteObjectA(
    DWORD dwObjectType,
    LPCSTR lpszProjectFile,
    LPCSTR lpszName,
    DWORD dwFlags,
    DWORD dwCreatorID,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMDeleteObjectW(
    DWORD dwObjectType,
    LPCWSTR lpszProjectFile,
    LPCWSTR lpszName,
    DWORD dwFlags,
    DWORD dwCreatorID,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMDELETEOBJECTFCTA)(
    DWORD dwObjectType,
    LPCSTR lpszProjectFile,
    LPCSTR lpszName,
    DWORD dwFlags,
    DWORD dwCreatorID,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMDELETEOBJECTFCTW)(
    DWORD dwObjectType,
    LPWSTR lpszProjectFile,
    LPWSTR lpszName,
    DWORD dwFlags,
    DWORD dwCreatorID,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMDELETEOBJECT       "DMDeleteObjectW"
#define DMDeleteObject            DMDeleteObjectW
typedef DMDELETEOBJECTFCTW        DMDELETEOBJECTFCT;
#else
#define NAME_DMDELETEOBJECT       "DMDeleteObjectA"
#define DMDeleteObject            DMDeleteObjectA
typedef DMDELETEOBJECTFCTA        DMDELETEOBJECTFCT;
#endif

/*-----------------------------------------------------------------------
 * DMDeleteObjectsForConnectionA
 */
BOOL WINAPI DMDeleteObjectsForConnectionA(
    DWORD dwObjectType,
    LPCSTR lpszProjectFile,
    LPCSTR lpszName,
    DWORD dwFlags,
    DWORD dwCreatorID,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMDeleteObjectsForConnectionW(
    DWORD dwObjectType,
    LPCWSTR lpszProjectFile,
    LPCWSTR lpszName,
    DWORD dwFlags,
    DWORD dwCreatorID,
    LPCMN_ERRORW lpdmError);



typedef BOOL (WINAPI *DMDELETEOBJECTFORCONNECTIONFCTA)(
    DWORD dwObjectType,
    LPCSTR lpszProjectFile,
    LPCSTR lpszName,
    DWORD dwFlags,
    DWORD dwCreatorID,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMDELETEOBJECTFORCONNECTIONFCTW)(
    DWORD dwObjectType,
    LPWSTR lpszProjectFile,
    LPWSTR lpszName,
    DWORD dwFlags,
    DWORD dwCreatorID,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMDELETEOBJECTFORCONNECTION            "DMDeleteObjectsForConnectionW"
#define DMDeleteObjectsForConnection                DMDeleteObjectsForConnectionW
typedef DMDELETEOBJECTFCTW                          MDELETEOBJECTFORCONNECTIONFCT;
#else
#define NAME_DMDELETEOBJECTFORCONNECTION            "DMDeleteObjectsForConnectionA"
#define DMDeleteObjectsForConnection                DMDeleteObjectsForConnectionA
typedef DMDELETEOBJECTFORCONNECTIONFCTA             DMDELETEOBJECTFORCONNECTIONFCT;
#endif

/*-----------------------------------------------------------------------
 * DMEnumObjectA
 */
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_OBJECT_PROCA)(
    DWORD dwObjectType,
    LPVOID lpvData,
    LPVOID lpvUser);
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_OBJECT_PROCW)(
    DWORD dwObjectType,
    LPVOID lpvData,
    LPVOID lpvUser);

//
// Use this structure in the Enum-Callback if objects of type "driver"
// are enumerated.
//
typedef struct
{
    DWORD       dwSize;
    char        szPath[_MAX_PATH + 1];      // Path of driver
} DM_ENUM_DRIVER_DATAA, *LPDM_ENUM_DRIVER_DATAA;
typedef struct
{
    DWORD       dwSize;
    WCHAR       szPath[_MAX_PATH + 1];      // Path of driver
} DM_ENUM_DRIVER_DATAW, *LPDM_ENUM_DRIVER_DATAW;

#ifdef UNICODE
typedef DM_ENUM_DRIVER_DATAW        DM_ENUM_DRIVER_DATA;
typedef LPDM_ENUM_DRIVER_DATAW  LPDM_ENUM_DRIVER_DATA;
#else
typedef DM_ENUM_DRIVER_DATAA        DM_ENUM_DRIVER_DATA;
typedef LPDM_ENUM_DRIVER_DATAA  LPDM_ENUM_DRIVER_DATA;
#endif

//
// Use this structure in the Enum-Callback if objects of type "driver"
// are enumerated.
//
typedef struct tagDM_ENUM_CONNECTION_DATAA
{
    DWORD   dwSize;
    CHAR    szConnection  [MAX_DM_CONNECTION_NAME +3] ;
    CHAR    szUnitName    [MAX_DM_UNIT_NAME +1] ;
    CHAR    szPath        [_MAX_PATH + 1];      // Path of driver
    CHAR    szSpecific    [MAX_DM_CON_SPECIFIC +1] ;
    DWORD   dwVarNum;
    DWORD   dwCreatorID;
} DM_ENUM_CONNECTION_DATAA, FAR *LPDM_ENUM_CONNECTION_DATAA;
typedef struct tagDM_ENUM_CONNECTION_DATAW
{
    DWORD   dwSize;
    WCHAR   szConnection  [MAX_DM_CONNECTION_NAME +3] ;
    WCHAR   szUnitName    [MAX_DM_UNIT_NAME +1] ;
    WCHAR   szPath        [_MAX_PATH + 1];      // Path of driver
    WCHAR   szSpecific    [MAX_DM_CON_SPECIFIC +1] ;
    DWORD   dwVarNum;
    DWORD   dwCreatorID;
} DM_ENUM_CONNECTION_DATAW, FAR *LPDM_ENUM_CONNECTION_DATAW;

#ifdef UNICODE
typedef DM_ENUM_CONNECTION_DATAW        DM_ENUM_CONNECTION_DATA;
typedef LPDM_ENUM_CONNECTION_DATAW      LPDM_ENUM_CONNECTION_DATA;
#else
typedef DM_ENUM_CONNECTION_DATAA        DM_ENUM_CONNECTION_DATA;
typedef LPDM_ENUM_CONNECTION_DATAA      LPDM_ENUM_CONNECTION_DATA;
#endif

//
// Use this structure in the Enum-Callback if objects of type "driver-
// unit" are enumerated.
//
typedef struct
{
    DWORD       dwSize;
    DWORD       dwCreatorID;
    char        szUnitName[MAX_DM_UNIT_NAME + 1];
    char        szPath[_MAX_PATH + 1];      // Path of driver
} DM_ENUM_DRIVERUNIT_DATAA, *LPDM_ENUM_DRIVERUNIT_DATAA;
typedef struct
{
    DWORD       dwSize;
    DWORD       dwCreatorID;
    WCHAR       szUnitName[MAX_DM_UNIT_NAME + 1];
    WCHAR       szPath[_MAX_PATH + 1];      // Path of driver
} DM_ENUM_DRIVERUNIT_DATAW, *LPDM_ENUM_DRIVERUNIT_DATAW;

#ifdef UNICODE
typedef DM_ENUM_DRIVERUNIT_DATAW        DM_ENUM_DRIVERUNIT_DATA;
typedef LPDM_ENUM_DRIVERUNIT_DATAW  LPDM_ENUM_DRIVERUNIT_DATA;
#else
typedef DM_ENUM_DRIVERUNIT_DATAA        DM_ENUM_DRIVERUNIT_DATA;
typedef LPDM_ENUM_DRIVERUNIT_DATAA  LPDM_ENUM_DRIVERUNIT_DATA;
#endif


//
// Use this structure in the Enum-Callback if objects of type "´tag-group"
// are enumerated.
//
typedef struct
{
    DWORD       dwSize;
    DWORD       dwCreatorID;
    char        szGroupName[MAX_DM_GROUP_NAME + 1]; // Name of Group
    //
    // The following fields are containing valid data if the tag-group
    // contains external tags.
    // The fields are containing '\0' if the group contains internal tags
    //
    char        szConnectionName[MAX_DM_CONNECTION_NAME + 1];
    char        szUnitName[MAX_DM_UNIT_NAME + 1];
    char        szDriverName[_MAX_PATH + 1];
} DM_ENUM_GROUP_DATAA, *LPDM_ENUM_GROUP_DATAA;
typedef struct
{
    DWORD       dwSize;
    DWORD       dwCreatorID;
    WCHAR       szGroupName[MAX_DM_GROUP_NAME + 1]; // Name of Group
    //
    // The following fields are containing valid data if the tag-group
    // contains external tags.
    // The fields are containing '\0' if the group contains internal tags
    //
    WCHAR       szConnectionName[MAX_DM_CONNECTION_NAME + 1];
    WCHAR       szUnitName[MAX_DM_UNIT_NAME + 1];
    WCHAR       szDriverName[_MAX_PATH + 1];
} DM_ENUM_GROUP_DATAW, *LPDM_ENUM_GROUP_DATAW;
#ifdef UNICODE
typedef DM_ENUM_GROUP_DATAW     DM_ENUM_GROUP_DATA;
typedef LPDM_ENUM_GROUP_DATAW   LPDM_ENUM_GROUP_DATA;
#else
typedef DM_ENUM_GROUP_DATAA     DM_ENUM_GROUP_DATA;
typedef LPDM_ENUM_GROUP_DATAA   LPDM_ENUM_GROUP_DATA;
#endif


BOOL WINAPI DMEnumObjectA(
    DWORD dwObjectType,
    LPCSTR lpszProjectFile,
    LPVOID lpvFilter,
    DM_ENUM_OBJECT_PROCA lpfnEnum,
    LPVOID lpvUser,
    DWORD dwCreatorID,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMEnumObjectW(
    DWORD dwObjectType,
    LPCWSTR lpszProjectFile,
    LPVOID lpvFilter,
    DM_ENUM_OBJECT_PROCW lpfnEnum,
    LPVOID lpvUser,
    DWORD dwCreatorID,
    LPCMN_ERRORW lpdmError);



typedef BOOL (WINAPI *DMENUMOBJECTFCTA)(
    DWORD dwObjectType,
    LPCSTR lpszProjectFile,
    LPVOID lpvFilter,
    DM_ENUM_OBJECT_PROCA lpfnEnum,
    LPVOID lpvUser,
    DWORD dwCreatorID,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMENUMOBJECTFCTW)(
    DWORD dwObjectType,
    LPCWSTR lpszProjectFile,
    LPVOID lpvFilter,
    DM_ENUM_OBJECT_PROCW lpfnEnum,
    LPVOID lpvUser,
    DWORD dwCreatorID,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMENUMOBJECT               "DMEnumObjectW"
#define DMEnumObject                    DMEnumObjectW
typedef DMENUMOBJECTFCTW                DMENUMOBJECTFCT;
#else
#define NAME_DMENUMOBJECT               "DMEnumObjectA"
#define DMEnumObject                    DMEnumObjectA
typedef DMENUMOBJECTFCTA                DMENUMOBJECTFCT;
#endif



/*-----------------------------------------------------------------------
 * GAPICreateNewProject
 */
BOOL WINAPI GAPICreateNewProjectA(
    LPMCP_NEWPROJECT_DATAA pData,
    LPCMN_ERRORA lpdmError) ;


BOOL WINAPI GAPICreateNewProjectW(
    LPMCP_NEWPROJECT_DATAW pData,
    LPCMN_ERRORW lpdmError) ;


typedef BOOL (WINAPI *GAPICREATENEWPROJECTFCTA)(
    LPMCP_NEWPROJECT_DATAA pData,
    LPCMN_ERRORA lpdmError) ;
typedef BOOL (WINAPI *GAPICREATENEWPROJECTFCTW)(
    LPMCP_NEWPROJECT_DATAW pData,
    LPCMN_ERRORW lpdmError) ;


#ifdef UNICODE
#define NAME_GAPICREATENEWPROJECT       "GAPICreateNewProjectW"
#define GAPICreateNewProject            GAPICreateNewProjectW
typedef GAPICREATENEWPROJECTFCTW        GAPICREATENEWPROJECTFCT;
#else
#define NAME_GAPICREATENEWPROJECT       "GAPICreateNewProjectA"
#define GAPICreateNewProject            GAPICreateNewProjectA
typedef GAPICREATENEWPROJECTFCTA        GAPICREATENEWPROJECTFCT;
#endif


/*-----------------------------------------------------------------------
 * GAPICreateNewProject
 */
BOOL WINAPI GAPICreateNewMachineA(
    LPMCP_NEWMACHINE_DATAA pData,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI GAPICreateNewMachineW(
    LPMCP_NEWMACHINE_DATAW pData,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *GAPICREATENEWMACHINEFCTA)(
    LPMCP_NEWMACHINE_DATAA pData,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *GAPICREATENEWMACHINEFCTW)(
    LPMCP_NEWMACHINE_DATAW pData,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_GAPICREATENEWMACHINE       "GAPICreateNewMachineW"
#define GAPICreateNewMachine            GAPICreateNewMachineW
typedef GAPICREATENEWMACHINEFCTW        GAPICREATENEWMACHINEFCT;
#else
#define NAME_GAPICREATENEWMACHINE       "GAPICreateNewMachineA"
#define GAPICreateNewMachine            GAPICreateNewMachineA
typedef GAPICREATENEWMACHINEFCTA        GAPICREATENEWMACHINEFCT;
#endif


/*-----------------------------------------------------------------------
 * GAPICreateNewConnection
 */
BOOL WINAPI GAPICreateNewConnectionA(
    LPMCP_NEWCONNECTION_DATAA pData,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI GAPICreateNewConnectionW(
    LPMCP_NEWCONNECTION_DATAW pData,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *GAPICREATENEWCONNECTIONFCTA)(
    LPMCP_NEWCONNECTION_DATAA pData,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *GAPICREATENEWCONNECTIONFCTW)(
    LPMCP_NEWCONNECTION_DATAW pData,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_GAPICREATENEWCONNECTION    "GAPICreateNewConnectionW"
#define GAPICreateNewConnection         GAPICreateNewConnectionW
typedef GAPICREATENEWCONNECTIONFCTW     GAPICREATENEWCONNECTIONFCT;
#else
#define NAME_GAPICREATENEWCONNECTION    "GAPICreateNewConnectionA"
#define GAPICreateNewConnection         GAPICreateNewConnectionA
typedef GAPICREATENEWCONNECTIONFCTA     GAPICREATENEWCONNECTIONFCT;
#endif


/*-----------------------------------------------------------------------
 * GAPIDeleteConnection
 */
BOOL WINAPI GAPIDeleteConnectionA(
    LPMCP_DELETECONNECTION_DATAA pData,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI GAPIDeleteConnectionW(
    LPMCP_DELETECONNECTION_DATAW pData,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *GAPIDELETECONNECTIONFCTA)(
    LPMCP_DELETECONNECTION_DATAA pData,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *GAPIDELETECONNECTIONFCTW)(
    LPMCP_DELETECONNECTION_DATAW pData,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_GAPIDELETECONNECTION       "GAPIDeleteConnectionW"
#define GAPIDeleteConnection            GAPIDeleteConnectionW
typedef GAPIDELETECONNECTIONFCTW        GAPIDELETECONNECTIONFCT;
#else
#define NAME_GAPIDELETECONNECTION       "GAPIDeleteConnectionA"
#define GAPIDeleteConnection            GAPIDeleteConnectionA
typedef GAPIDELETECONNECTIONFCTA        GAPIDELETECONNECTIONFCT;
#endif


/*-----------------------------------------------------------------------
 * GAPICreateNewVariable
 */
BOOL WINAPI GAPICreateNewVariableA(
    LPMCP_NEWVARIABLE_DATAA pData,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI GAPICreateNewVariableW(
    LPMCP_NEWVARIABLE_DATAW pData,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *GAPICREATENEWVARIABLEFCTA)(
    LPMCP_NEWVARIABLE_DATAA pData,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *GAPICREATENEWVARIABLEFCTW)(
    LPMCP_NEWVARIABLE_DATAW pData,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_GAPICREATENEWVARIABLE      "GAPICreateNewVariableW"
#define GAPICreateNewVariable           GAPICreateNewVariableW
typedef GAPICREATENEWVARIABLEFCTW       GAPICREATENEWVARIABLEFCT;
#else
#define NAME_GAPICREATENEWVARIABLE      "GAPICreateNewVariableA"
#define GAPICreateNewVariable           GAPICreateNewVariableA
typedef GAPICREATENEWVARIABLEFCTA       GAPICREATENEWVARIABLEFCT;
#endif


/*-----------------------------------------------------------------------
 * GAPIDeleteVariable
 */
BOOL WINAPI GAPIDeleteVariableA(
    LPMCP_DELETEVARIABLE_DATAA pData,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI GAPIDeleteVariableW(
    LPMCP_DELETEVARIABLE_DATAW pData,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *GAPIDELETEVARIABLEFCTA)(
    LPMCP_DELETEVARIABLE_DATAA pData,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *GAPIDELETEVARIABLEFCTW)(
    LPMCP_DELETEVARIABLE_DATAW pData,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_GAPIDELETEVARIABLE         "GAPIDeleteVariableW"
#define GAPIDeleteVariable              GAPIDeleteVariableW
typedef GAPIDELETEVARIABLEFCTW          GAPIDELETEVARIABLEFCT;
#else
#define NAME_GAPIDELETEVARIABLE         "GAPIDeleteVariableA"
#define GAPIDeleteVariable              GAPIDeleteVariableA
typedef GAPIDELETEVARIABLEFCTA          GAPIDELETEVARIABLEFCT;
#endif

/*-----------------------------------------------------------------------
 * GAPICreateStartListEntry
 */
BOOL WINAPI GAPICreateStartListEntryA(
    LPMCP_STARTLIST_DATAA pData,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI GAPICreateStartListEntryW(
    LPMCP_STARTLIST_DATAW pData,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *GAPICREATESTARTLISTENTRYFCTA)(
    LPMCP_STARTLIST_DATAA pData,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *GAPICREATESTARTLISTENTRYFCTW)(
    LPMCP_STARTLIST_DATAW pData,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_GAPICREATENEWSTARTLISTENTRY      "GAPICreateStartListEntryW"
#define GAPICreateStartListEntry              GAPICreateStartListEntryW
typedef GAPICREATESTARTLISTENTRYFCTW          GAPICREATESTARTLISTENTRYFCT;
#else
#define NAME_GAPICREATENEWSTARTLISTENTRY      "GAPICreateStartListEntryA"
#define GAPICreateStartListEntry              GAPICreateStartListEntryA
typedef GAPICREATESTARTLISTENTRYFCTA          GAPICREATESTARTLISTENTRYFCT;
#endif

/*-----------------------------------------------------------------------
 * DMGetMachineInfo
 */
BOOL WINAPI DMGetMachineInfoA(
    LPCSTR lpszLogicalName,
    LPVOID lpvData,
    LPDWORD lpdwSize,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMGetMachineInfoW(
    LPCWSTR lpszLogicalName,
    LPVOID lpvData,
    LPDWORD lpdwSize,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMGETMACHINEINFOFCTA)(
    LPCSTR lpszLogicalName,
    LPVOID lpvData,
    LPDWORD lpdwSize,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMGETMACHINEINFOFCTW)(
    LPCWSTR lpszLogicalName,
    LPVOID lpvData,
    LPDWORD lpdwSize,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMGETMACHINEINFO           "DMGetMachineInfoW"
#define DMGetMachineInfo                DMGetMachineInfoW
typedef DMGETMACHINEINFOFCTW            DMGETMACHINEINFOFCT;
#else
#define NAME_DMGETMACHINEINFO           "DMGetMachineInfoA"
#define DMGetMachineInfo                DMGetMachineInfoA
typedef DMGETMACHINEINFOFCTA            DMGETMACHINEINFOFCT;
#endif


/*-----------------------------------------------------------------------
 * DMGetHotKey
 */
#define DM_HOTKEY_LOGON         0       /* Logon to WinCC */
#define DM_HOTKEY_LOGOFF        1       /* Logoff from WinCC */
#define DM_HOTKEY_HARDCOPY      2       /* Hardcopy */
#define DM_HOTKEY_SYSMENU		3
#define DM_HOTKEY_LAST          (DM_HOTKEY_SYSMENU + 1)
#define DM_HOTKEY_MAX         128

BOOL WINAPI DMGetHotKeyA(
    DWORD dwHotKeyAction,
    LPDWORD lpdwHotKey,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMGetHotKeyW(
    DWORD dwHotKeyAction,
    LPDWORD lpdwHotKey,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMGETHOTKEYFCTA)(
    DWORD dwHotKeyAction,
    LPDWORD lpdwHotKey,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMGETHOTKEYFCTW)(
    DWORD dwHotKeyAction,
    LPDWORD lpdwHotKey,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMGETHOTKEY                "DMGetHotKeyW"
#define DMGetHotKey                     DMGetHotKeyW
typedef DMGETHOTKEYFCTW                 DMGETHOTKEYFCT;
#else
#define NAME_DMGETHOTKEY                "DMGetHotKeyA"
#define DMGetHotKey                     DMGetHotKeyA
typedef DMGETHOTKEYFCTA                 DMGETHOTKEYFCT;
#endif


/*-----------------------------------------------------------------------
 * DMDeactivateRTProject
 */
BOOL WINAPI DMDeactivateRTProjectA(LPCMN_ERRORA lpdmError);
BOOL WINAPI DMActivateRTProjectA(LPCMN_ERRORA lpdmError);
BOOL WINAPI DMDeactivateRTProjectW(LPCMN_ERRORW lpdmError);
BOOL WINAPI DMActivateRTProjectW(LPCMN_ERRORW lpdmError);

typedef BOOL (WINAPI *DMDEACTIVATERTPROJECTFCTA)(LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMDEACTIVATERTPROJECTFCTW)(LPCMN_ERRORW lpdmError);
typedef BOOL (WINAPI *DMACTIVATERTPROJECTFCTA)(LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMACTIVATERTPROJECTFCTW)(LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMDEACTIVATERTPROJECT      "DMDeactivateRTProjectW"
#define DMDeactivateRTProject           DMDeactivateRTProjectW
typedef DMDEACTIVATERTPROJECTFCTW       DMDEACTIVATERTPROJECTFCT;
#define NAME_DMACTIVATERTPROJECT      "DMActivateRTProjectW"
#define DMActivateRTProject           DMActivateRTProjectW
typedef DMACTIVATERTPROJECTFCTW       DMACTIVATERTPROJECTFCT;
#else
#define NAME_DMDEACTIVATERTPROJECT      "DMDeactivateRTProjectA"
#define DMDeactivateRTProject           DMDeactivateRTProjectA
typedef DMDEACTIVATERTPROJECTFCTA       DMDEACTIVATERTPROJECTFCT;
#define NAME_DMACTIVATERTPROJECT      "DMActivateRTProjectA"
#define DMActivateRTProject           DMActivateRTProjectA
typedef DMACTIVATERTPROJECTFCTA       DMACTIVATERTPROJECTFCT;
#endif


/*-----------------------------------------------------------------------
 * DMWinCCExit and DMWinCCExitEx
 */

#define DM_SDMODE_SYSTEM   0x00000000   /* Shutdown Windows without Reboot */
#define DM_SDMODE_WINCC    0x00000001   /* Terminate WinCC */
#define DM_SDMODE_LOGOFF   0x00000002   /* Terminate WinCC and Log off */
#define DM_SDMODE_REBOOT   0x00000003   /* Terminate WinCC and reboot Windows */
#define DM_SDMODE_POWEROFF 0x00000004   /* Shutdown Windows and power off RQ287443 */
#define DM_SDMODE_FORCE_POWEROFF	0x00000005	/*Shutdown windows and power off by force - RQ AP01291487*/


BOOL WINAPI DMExitWinCCA(VOID);

BOOL WINAPI DMExitWinCCW(VOID);

typedef BOOL (WINAPI *DMEXITWINCCFCTA)(VOID);
typedef BOOL (WINAPI *DMEXITWINCCFCTW)(VOID);

#ifdef UNICODE
#define NAME_DMEXITWINCC        "DMExitWinCCW"
#define DMExitWinCC             DMExitWinCCW
typedef DMEXITWINCCFCTW         DMEXITWINCCFCT;
#else
#define NAME_DMEXITWINCC        "DMExitWinCCA"
#define DMExitWinCC             DMExitWinCCA
typedef DMEXITWINCCFCTA         DMEXITWINCCFCT;
#endif


BOOL WINAPI DMExitWinCCExA(DWORD dwMode);

BOOL WINAPI DMExitWinCCExW(DWORD dwMode);

typedef BOOL (WINAPI *DMEXITWINCCEXFCTA)(DWORD);
typedef BOOL (WINAPI *DMEXITWINCCEXFCTW)(DWORD);

#ifdef UNICODE
#define NAME_DMEXITWINCCEX      "DMExitWinCCExW"
#define DMExitWinCCEx           DMExitWinCCExW
typedef DMEXITWINCCEXFCTW       DMEXITWINCCEXFCT;
#else
#define NAME_DMEXITWINCCEX      "DMExitWinCCExA"
#define DMExitWinCCEx           DMExitWinCCExA
typedef DMEXITWINCCEXFCTA       DMEXITWINCCEXFCT;
#endif



/*-----------------------------------------------------------------------
 * DMInitLic
 */
BOOL WINAPI DMInitLicA( CHAR* pAppName, PSL_INITA pInit, DWORD* pHandle, PCMN_ERRORA pError);
BOOL WINAPI DMInitLicW( WCHAR* pAppName, PSL_INITW pInit, DWORD* pHandle, PCMN_ERRORW pError);

typedef BOOL (WINAPI *DMINITLICFCTA)(CHAR* pAppName, PSL_INITA pInit, DWORD* pHandle, PCMN_ERRORA pError);
typedef BOOL (WINAPI *DMINITLICFCTW)(WCHAR* pAppName, PSL_INITW pInit, DWORD* pHandle, PCMN_ERRORW pError);

#ifdef UNICODE
#define NAME_DMINITLIC    "DMInitLicW"
#define DMInitLic         DMInitLicW
typedef DMINITLICFCTW     DMINITLICFCT;
#else
#define NAME_DMINITLIC    "DMInitLicA"
#define DMInitLic         DMInitLicA
typedef DMINITLICFCTA     DMINITLICFCT;
#endif

/*-----------------------------------------------------------------------
 * DMExitLic
 */
BOOL WINAPI DMExitLicA( DWORD* pHandle, PCMN_ERRORA pError);
BOOL WINAPI DMExitLicW( DWORD* pHandle, PCMN_ERRORW pError);

typedef BOOL (WINAPI *DMEXITLICFCTA)(DWORD* pHandle, PCMN_ERRORA pError) ;
typedef BOOL (WINAPI *DMEXITLICFCTW)(DWORD* pHandle, PCMN_ERRORW pError) ;

#ifdef UNICODE
#define NAME_DMEXITLIC    "DMExitLicW"
#define DMExitLic         DMExitLicW
typedef DMEXITLICFCTW     DMEXITLICFCT;
#else
#define NAME_DMEXITLIC    "DMExitLicA"
#define DMExitLic         DMExitLicA
typedef DMEXITLICFCTA     DMEXITLICFCT;
#endif

/*-----------------------------------------------------------------------
 * DMCheckLic
 */
BOOL WINAPI DMCheckLicA( DWORD* pHandle, PSL_LICENSE pLicense, PCMN_ERRORA pError);
BOOL WINAPI DMCheckLicW( DWORD* pHandle, PSL_LICENSE pLicense, PCMN_ERRORW pError);

typedef BOOL (WINAPI *DMCHECKLICFCTA)( DWORD* pHandle, PSL_LICENSE pLicense, PCMN_ERRORA pError);
typedef BOOL (WINAPI *DMCHECKLICFCTW)( DWORD* pHandle, PSL_LICENSE pLicense, PCMN_ERRORW pError);

#ifdef UNICODE
#define NAME_DMCHECKLIC   "DMCheckLicW"
#define DMCheckLic         DMCheckLicW
typedef DMCHECKLICFCTW     DMCHECKLICFCT;
#else
#define NAME_DMCHECKLIC    "DMCheckLicA"
#define DMCheckLic         DMCheckLicA
typedef DMCHECKLICFCTA     DMCHECKLICFCT;
#endif

/*-----------------------------------------------------------------------
 * DMCheckLimit
 */
BOOL WINAPI DMCheckLimitA( DWORD* pHandle, PSL_LIMIT pLimit, PCMN_ERRORA pError);
BOOL WINAPI DMCheckLimitW( DWORD* pHandle, PSL_LIMIT pLimit, PCMN_ERRORW pError);

typedef BOOL (WINAPI *DMCHECKLIMITFCTA)( DWORD* pHandle, PSL_LIMIT pLimit, PCMN_ERRORA pError);
typedef BOOL (WINAPI *DMCHECKLIMITFCTW)( DWORD* pHandle, PSL_LIMIT pLimit, PCMN_ERRORW pError);

#ifdef UNICODE
#define NAME_DMCHECKLIMIT    "DMCheckLimitW"
#define DMCheckLimit         DMCheckLimitW
typedef DMCHECKLIMITFCTW     DMCHECKLIMITFCT;
#else
#define NAME_DMCHECKLIMIT    "DMCheckLimitA"
#define DMCheckLimit         DMCheckLimitA
typedef DMCHECKLIMITFCTA     DMCHECKLIMITFCT;
#endif

/*-----------------------------------------------------------------------
 * DMShowLicViolation
 */
int WINAPI DMShowLicViolationA( HWND hWnd, LPCSTR lpszCaption );
int WINAPI DMShowLicViolationW( HWND hWnd, LPCWSTR lpszCaption );

typedef int (WINAPI *DMSHOWLICVIOLATIONFCTA)( HWND hWnd, LPCSTR lpszCaption );
typedef int (WINAPI *DMSHOWLICVIOLATIONFCTW)( HWND hWnd, LPCWSTR lpszCaption );

#ifdef UNICODE
#define NAME_DMSHOWLICVIOLATION    "DMShowLicViolationW"
#define DMShowLicViolation         DMShowLicViolationW
typedef DMSHOWLICVIOLATIONFCTW     DMSHOWLICVIOLATIONFCT;
#else
#define NAME_DMSHOWLICVIOLATION    "DMShowLicViolationA"
#define DMShowLicViolation         DMShowLicViolationA
typedef DMSHOWLICVIOLATIONFCTA     DMSHOWLICVIOLATIONFCT;
#endif

/*-----------------------------------------------------------------------
 * DMGetLicVersion
 */
BOOL WINAPI DMGetLicVersion(DWORD* pSLVersion, DWORD* pNumMethod);
// no extra definition needed, ASCII/UNICODE use the same

typedef BOOL (WINAPI *DMGETLICVERSIONFCT)(DWORD* pSLVersion, DWORD* pNumMethod);

#ifdef UNICODE
#define NAME_DMGETLICVERSION    "DMGetLicVersion"
#else
#define NAME_DMGETLICVERSION    "DMGetLicVersion"
#endif


/*-----------------------------------------------------------------------
* DMGetDMTagLimit
*/
BOOL WINAPI DMGetDMTagLimitA( DWORD dwLimitModeFlags, LPDWORD pdwDMLicTags, LPDWORD pdwDMTagLimit, PCMN_ERRORA pError );
BOOL WINAPI DMGetDMTagLimitW( DWORD dwLimitModeFlags, LPDWORD pdwDMLicTags, LPDWORD pdwDMTagLimit, PCMN_ERRORW pError );

typedef BOOL (WINAPI *DMGETDMTAGLIMITFCTA)( DWORD dwLimitModeFlags, LPDWORD pdwDMLicTags, LPDWORD pdwDMTagLimit, PCMN_ERRORA pError );
typedef BOOL (WINAPI *DMGETDMTAGLIMITFCTW)( DWORD dwLimitModeFlags, LPDWORD pdwDMLicTags, LPDWORD pdwDMTagLimit, PCMN_ERRORW pError );

#ifdef UNICODE
#define NAME_DMGETDMTAGLIMIT    "DMGetDMTagLimitW"
#define DMGetDMTagLimit         DMGetDMTagLimitW
typedef DMGETDMTAGLIMITFCTW     DMGETDMTAGLIMITFCT;
#else
#define NAME_DMGETDMTAGLIMIT    "DMGetDMTagLimitA"
#define DMGetDMTagLimit         DMGetDMTagLimitA
typedef DMGETDMTAGLIMITFCTA     DMGETDMTAGLIMITFCT;
#endif


/*-----------------------------------------------------------------------
* DMGetTLGTagLimit
*/
BOOL WINAPI DMGetTLGTagLimitA( DWORD dwLimitModeFlags, LPDWORD pdwTLGLicTags, LPDWORD pdwTLGTagLimit, PCMN_ERRORA pError );
BOOL WINAPI DMGetTLGTagLimitW( DWORD dwLimitModeFlags, LPDWORD pdwTLGLicTags, LPDWORD pdwTLGTagLimit, PCMN_ERRORW pError );

typedef BOOL (WINAPI *DMGETTLGTAGLIMITFCTA)( DWORD dwLimitModeFlags, LPDWORD pdwTLGLicTags, LPDWORD pdwTLGTagLimit, PCMN_ERRORA pError );
typedef BOOL (WINAPI *DMGETTLGTAGLIMITFCTW)( DWORD dwLimitModeFlags, LPDWORD pdwTLGLicTags, LPDWORD pdwTLGTagLimit, PCMN_ERRORW pError );

#ifdef UNICODE
#define NAME_DMGETTLGTAGLIMIT    "DMGetTLGTagLimitW"
#define DMGetTLGTagLimit         DMGetTLGTagLimitW
typedef DMGETTLGTAGLIMITFCTW     DMGETTLGTAGLIMITFCT;
#else
#define NAME_DMGETTLGTAGLIMIT    "DMGetTLGTagLimitA"
#define DMGetTLGTagLimit         DMGetTLGTagLimitA
typedef DMGETTLGTAGLIMITFCTA     DMGETTLGTAGLIMITFCT;
#endif


#define GETTAGLIMIT_FLAG_CS             0
#define GETTAGLIMIT_FLAG_RT             1
#define GETTAGLIMIT_FLAG_IGNOREDEMO     2
#define GETTAGLIMIT_FLAG_CS_IGNOREDEMO  2
#define GETTAGLIMIT_FLAG_RT_IGNOREDEMO  3



/*-----------------------------------------------------------------------
 * VARIABLE_COMMON_EX
 */
typedef struct tagMCP_VARIABLE_COMMON_EXA
{
    DWORD   dwVarType;      // Variablentyp
    DWORD   dwCreatorID;    // Erzeuger ID
    DWORD   dwVarLength;    // Variablenlänge
    DWORD   dwVarProperty;  // Variablen Eigenschaften (Interne / Externe Variable)
    DWORD   dwFormat;       // Formatanpassung
    DWORD   dwOSOffset;     // Offset im Wertepuffer der Struktur
    DWORD   dwASOffset;     // Offset im AS Puffer
    char    szStructTypeName[MAX_DM_VAR_NAME + 1];    // Name des Strukturtypes v.d. Instanz angelegt wird
} MCP_VARIABLE_COMMON_EXA, FAR *LPMCP_VARIABLE_COMMON_EXA ;
typedef struct tagMCP_VARIABLE_COMMON_EXW
{
    DWORD   dwVarType;      // Variablentyp
    DWORD   dwCreatorID;    // Erzeuger ID
    DWORD   dwVarLength;    // Variablenlänge
    DWORD   dwVarProperty;  // VariablenFlags (Interne / Externe Variable)
    DWORD   dwFormat;       // Formatanpassung
    DWORD   dwOSOffset;     // Offset im Wertepuffer der Struktur
    DWORD   dwASOffset;     // Offset im AS Puffer
    WCHAR   szStructTypeName[MAX_DM_VAR_NAME + 1];    // Name des Strukturtypes v.d. Instanz angelegt wird

} MCP_VARIABLE_COMMON_EXW, FAR *LPMCP_VARIABLE_COMMON_EXW;

#ifdef UNICODE
typedef MCP_VARIABLE_COMMON_EXW    MCP_VARIABLE_COMMON_EX;
typedef LPMCP_VARIABLE_COMMON_EXW  LPMCP_VARIABLE_COMMON_EX;
#else
typedef MCP_VARIABLE_COMMON_EXA    MCP_VARIABLE_COMMON_EX;
typedef LPMCP_VARIABLE_COMMON_EXA  LPMCP_VARIABLE_COMMON_EX;
#endif

/*
 * VARIABLE_LIMITS_EX
 */
typedef struct tagMCP_VARIABLE_LIMITS_EXA
{
    double  dTopLimit;          // Obergrenze
    double  dBottomLimit;       // Untergrenze
    double  dStartValue;        // Startwert
    double  dSubstituteValue;   // Ersatzwert
    DWORD   dwLimitFlags;          // Limitflags (Gültigkeit der Defaultwerte,etc)
    DWORD   dwTextBibStartText;   // Vorbesetzung f. Startwert aus der Textbiliothek
    char    szTextStartText[255]; // Vorbesetzung f. Startwert der Textvariablen
    DWORD   dwTextBibSubstitude;  // Vorbesetzung f. Ersatzwert aus der Textbiliothek
    char    szTextSubstitude[255];// Vorbesetzung f. Ersatzwert der Textvariablen

} MCP_VARIABLE_LIMITS_EXA, FAR *LPMCP_VARIABLE_LIMITS_EXA;
typedef struct tagMCP_VARIABLE_LIMITS_EXW
{
    double  dTopLimit;          // Obergrenze
    double  dBottomLimit;       // Untergrenze
    double  dStartValue;        // Startwert
    double  dSubstituteValue;   // Ersatzwert
    DWORD   dwLimitFlags;          // Limitflags (Gültigkeit der Defaultwerte,etc)
    DWORD   dwTextBibStartText;   // Vorbesetzung f. Startwert aus der Textbiliothek
    WCHAR   szTextStartText[255]; // Vorbesetzung f. Startwert der Textvariablen
    DWORD   dwTextBibSubstitude;  // Vorbesetzung f. Ersatzwert aus der Textbiliothek
    WCHAR   szTextSubstitude[255];// Vorbesetzung f. Ersatzwert der Textvariablen

} MCP_VARIABLE_LIMITS_EXW, FAR *LPMCP_VARIABLE_LIMITS_EXW;

#ifdef UNICODE
typedef MCP_VARIABLE_LIMITS_EXW    MCP_VARIABLE_LIMITS_EX;
typedef LPMCP_VARIABLE_LIMITS_EXW  LPMCP_VARIABLE_LIMITS_EX;
#else
typedef MCP_VARIABLE_LIMITS_EXA    MCP_VARIABLE_LIMITS_EX;
typedef LPMCP_VARIABLE_LIMITS_EXA  LPMCP_VARIABLE_LIMITS_EX;
#endif


/*
 * VARIABLE_PROTOCOL_EX
 */
typedef struct tagMCP_VARIABLE_PROTOCOL_EXA
{
    DWORD   dwProtocolFlags;       // Protocolflags
} MCP_VARIABLE_PROTOCOL_EXA, FAR *LPMCP_VARIABLE_PROTOCOL_EXA ;
typedef struct tagMCP_VARIABLE_PROTOCOL_EXW
{
    DWORD   dwProtocolFlags;       // Protocolflags
} MCP_VARIABLE_PROTOCOL_EXW, FAR *LPMCP_VARIABLE_PROTOCOL_EXW;

#ifdef UNICODE
typedef MCP_VARIABLE_PROTOCOL_EXW    MCP_VARIABLE_PROTOCOL_EX;
typedef LPMCP_VARIABLE_PROTOCOL_EXW  LPMCP_VARIABLE_PROTOCOL_EX;
#else
typedef MCP_VARIABLE_PROTOCOL_EXA    MCP_VARIABLE_PROTOCOL_EX;
typedef LPMCP_VARIABLE_PROTOCOL_EXA  LPMCP_VARIABLE_PROTOCOL_EX;
#endif

/*
 * MCP_NEWVARIABLE_DATA_EX
 */
typedef struct tagMCP_NEWVARIABLE_DATA_EXA
{
    DWORD                       dwFlags;                                 /* MCP_NVAR_FLAGs  */
    char                        szProjectFile[_MAX_PATH +1];             /* ProjektPfad + Name */
    char                        szConnection[MAX_DM_CONNECTION_NAME +3]; /* VerbindungsName */
    char                        szVarName[MAX_DM_VAR_NAME +1];           /* VariablenName   */
    char                        szGroupName[MAX_DM_GROUP_NAME +1];       /* GruppenName     */
    MCP_VARIABLE_COMMON_EXA     Common;                                  /* MCP_VARIABLE_COMMON Struktur */
    MCP_VARIABLE_PROTOCOL_EXA   Protocol;                                /* MCP_VARIABLE_PROTOCOL Struktur */
    MCP_VARIABLE_LIMITS_EXA     Limits;                                  /* MCP_VARIABLE_LIMITS Struktur */
    char                        szSpecific[MAX_DM_VAR_SPECIFIC +1];      /* Kanal / Verbindungsabhängige Parameter */
} MCP_NEWVARIABLE_DATA_EXA, FAR *LPMCP_NEWVARIABLE_DATA_EXA;
typedef struct tagMCP_NEWVARIABLE_DATA_EXW
{
    DWORD                       dwFlags;                                 /* MCP_NVAR_FLAGs  */
    WCHAR                       szProjectFile[_MAX_PATH +1];             /* ProjektPfad + Name */
    WCHAR                       szConnection[MAX_DM_CONNECTION_NAME +3]; /* VerbindungsName */
    WCHAR                       szVarName[MAX_DM_VAR_NAME +1];           /* VariablenName   */
    WCHAR                       szGroupName[MAX_DM_GROUP_NAME +1];       /* GruppenName     */
    MCP_VARIABLE_COMMON_EXW     Common;                                  /* MCP_VARIABLE_COMMON Struktur */
    MCP_VARIABLE_PROTOCOL_EXW   Protocol;                                /* MCP_VARIABLE_PROTOCOL Struktur */
    MCP_VARIABLE_LIMITS_EXW     Limits;                                  /* MCP_VARIABLE_LIMITS Struktur */
    WCHAR                       szSpecific[MAX_DM_VAR_SPECIFIC +1];      /* Kanal / Verbindungsabhängige Parameter */
} MCP_NEWVARIABLE_DATA_EXW, FAR *LPMCP_NEWVARIABLE_DATA_EXW;

#ifdef UNICODE
typedef MCP_NEWVARIABLE_DATA_EXW   MCP_NEWVARIABLE_DATA_EX;
typedef LPMCP_NEWVARIABLE_DATA_EXW LPMCP_NEWVARIABLE_DATA_EX;
#else
typedef MCP_NEWVARIABLE_DATA_EXA   MCP_NEWVARIABLE_DATA_EX;
typedef LPMCP_NEWVARIABLE_DATA_EXA LPMCP_NEWVARIABLE_DATA_EX;
#endif

/*
 * DM_ENUM_TYPES_PROC
 */
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_TYPES_PROCA)(
    LPCSTR                      lpszTypeName,
    DWORD                       dwTypeID,
    DWORD                       dwCreatorID,
    LPVOID                      lpvUser);
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_TYPES_PROCW)(
    LPCWSTR                     lpszTypeName,
    DWORD                       dwTypeID,
    DWORD                       dwCreatorID,
    LPVOID                      lpvUser);

#ifdef UNICODE
typedef DM_ENUM_TYPES_PROCW    DM_ENUM_TYPES_PROC;
#else
typedef DM_ENUM_TYPES_PROCA    DM_ENUM_TYPES_PROC;
#endif

/*
 * DM_ENUM_TYPEMEMBERS_PROC
 */
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_TYPEMEMBERS_PROCA)(
    LPCSTR                      lpszMemberName,
    LPVOID                      lpvUser);
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_TYPEMEMBERS_PROCW)(
    LPCWSTR                     lpszMemberName,
    LPVOID                      lpvUser);


#ifdef UNICODE
typedef DM_ENUM_TYPEMEMBERS_PROCW    DM_ENUM_TYPEMEMBERS_PROC;
#else
typedef DM_ENUM_TYPEMEMBERS_PROCA    DM_ENUM_TYPEMEMBERS_PROC;
#endif

/*
 * DM_ENUM_TYPEMEMBERS_PROC_EX
 */
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_TYPEMEMBERS_PROC_EXA)(
    LPDM_VARKEYA                lpdmVarKey,
    LPMCP_NEWVARIABLE_DATA_EXA  lpdmVarDataEx,
    LPVOID                      lpvUser);
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_TYPEMEMBERS_PROC_EXW)(
    LPDM_VARKEYW                lpdmVarKey,
    LPMCP_NEWVARIABLE_DATA_EXW  lpdmVarDataEx,
    LPVOID                      lpvUser);

#ifdef UNICODE
typedef DM_ENUM_TYPEMEMBERS_PROC_EXW    DM_ENUM_TYPEMEMBERS_PROC_EX;
#else
typedef DM_ENUM_TYPEMEMBERS_PROC_EXA    DM_ENUM_TYPEMEMBERS_PROC_EX;
#endif

/*-----------------------------------------------------------------------
 * GAPICreateType
 */
BOOL WINAPI GAPICreateTypeA(
    LPCSTR                      lpszProjectFile,
    LPDM_TYPE_DESCRIPTORA       lpdmType,
    DWORD                       dwFlags,
    DWORD                       dwCreatorID,
    LPMCP_NEWVARIABLE_DATA_EXA  lpMemberdata,
    LPCMN_ERRORA                lpdmError);


BOOL WINAPI GAPICreateTypeW(
    LPCWSTR                     lpszProjectFile,
    LPDM_TYPE_DESCRIPTORW       lpdmType,
    DWORD                       dwFlags,
    DWORD                       dwCreatorID,
    LPMCP_NEWVARIABLE_DATA_EXW  lpMemberdata,
    LPCMN_ERRORW                lpdmError);


typedef BOOL (WINAPI *GAPICREATETYPEFCTA)(
    LPCSTR                      lpszProjectFile,
    LPDM_TYPE_DESCRIPTORA       lpdmType,
    DWORD                       dwFlags,
    DWORD                       dwCreatorID,
    LPMCP_NEWVARIABLE_DATA_EXA  lpMemberdata,
    LPCMN_ERRORA                lpdmError);
typedef BOOL (WINAPI *GAPICREATETYPEFCTW)(
    LPCWSTR                     lpszProjectFile,
    LPDM_TYPE_DESCRIPTORW       lpdmType,
    DWORD                       dwFlags,
    DWORD                       dwCreatorID,
    LPMCP_NEWVARIABLE_DATA_EXW  lpMemberdata,
    LPCMN_ERRORW                lpdmError);

#ifdef UNICODE
#define NAME_GAPICREATETYPE         "GAPICreateTypeW"
#define GAPICreateType              GAPICreateTypeW
typedef GAPICREATETYPEFCTW          GAPICREATETYPEFCT;
#else
#define NAME_GAPICREATETYPE         "GAPICreateTypeA"
#define GAPICreateType              GAPICreateTypeA
typedef GAPICREATETYPEFCTA          GAPICREATETYPEFCT;
#endif

/*-----------------------------------------------------------------------
 * GAPIDeleteType
 */
BOOL WINAPI GAPIDeleteTypeA(
    LPCSTR                      lpszProjectFile,
    DWORD                       dwFlags,
    DWORD                       dwCreatorID,
    LPCSTR                      lpszTypeName,
    LPCMN_ERRORA                lpdmError);


BOOL WINAPI GAPIDeleteTypeW(
    LPCWSTR                     lpszProjectFile,
    DWORD                       dwFlags,
    DWORD                       dwCreatorID,
    LPCWSTR                     lpszTypeName,
    LPCMN_ERRORW                lpdmError);


typedef BOOL (WINAPI *GAPIDELETETYPEFCTA)(
    LPCSTR                      lpszProjectFile,
    DWORD                       dwFlags,
    DWORD                       dwCreatorID,
    LPCSTR                      lpszTypeName,
    LPCMN_ERRORA                lpdmError);
typedef BOOL (WINAPI *GAPIDELETETYPEFCTW)(
    LPCWSTR                     lpszProjectFile,
    DWORD                       dwFlags,
    DWORD                       dwCreatorID,
    LPCWSTR                     lpszTypeName,
    LPCMN_ERRORW                lpdmError);

#ifdef UNICODE
#define NAME_GAPIDELETETYPE      "GAPIDeleteTypeW"
#define GAPIDeleteType           GAPIDeleteTypeW
typedef GAPIDELETETYPEFCTW       GAPIDELETETYPEFCT;
#else
#define NAME_GAPIDELETETYPE      "GAPIDeleteTypeA"
#define GAPIDeleteType           GAPIDeleteTypeA
typedef GAPIDELETETYPEFCTA       GAPIDELETETYPEFCT;
#endif

/*-----------------------------------------------------------------------
 * GAPIEnumTypes
 */
BOOL WINAPI GAPIEnumTypesA(
    LPCSTR                      lpszProjectFile,
    DM_ENUM_TYPES_PROCA         lpfnCallback,
    LPVOID                      lpvUser,
    LPCMN_ERRORA                lpdmError);


BOOL WINAPI GAPIEnumTypesW(
    LPCWSTR                     lpszProjectFile,
    DM_ENUM_TYPES_PROCW         lpfnCallback,
    LPVOID                      lpvUser,
    LPCMN_ERRORW                lpdmError);


typedef BOOL (WINAPI *GAPIENUMTYPESFCTA)(
    LPCSTR                      lpszProjectFile,
    DM_ENUM_TYPES_PROCA         lpfnCallback,
    LPVOID                      lpvUser,
    LPCMN_ERRORA                lpdmError);
typedef BOOL (WINAPI *GAPIENUMTYPESFCTW)(
    LPCWSTR                     lpszProjectFile,
    DM_ENUM_TYPES_PROCW         lpfnCallback,
    LPVOID                      lpvUser,
    LPCMN_ERRORW                lpdmError);

#ifdef UNICODE
#define NAME_GAPIENUMTYPES      "GAPIEnumTypesW"
#define GAPIEnumTypes           GAPIEnumTypesW
typedef GAPIENUMTYPESFCTW       GAPIENUMTYPESFCT;
#else
#define NAME_GAPIENUMTYPES      "GAPIEnumTypesA"
#define GAPIEnumTypes           GAPIEnumTypesA
typedef GAPIENUMTYPESFCTA       GAPIENUMTYPESFCT;
#endif

/*-----------------------------------------------------------------------
 * GAPIEnumTypeMembers
 */
BOOL WINAPI GAPIEnumTypeMembersA(
    LPCSTR                      lpszProjectFile,
    LPCSTR                      lpszTypeName,
    DM_ENUM_TYPEMEMBERS_PROCA   lpfnCallback,
    LPVOID                      lpvUser,
    LPCMN_ERRORA                lpdmError);


BOOL WINAPI GAPIEnumTypeMembersW(
    LPCWSTR                     lpszProjectFile,
    LPCWSTR                     lpszTypeName,
    DM_ENUM_TYPEMEMBERS_PROCW   lpfnCallback,
    LPVOID                      lpvUser,
    LPCMN_ERRORW                lpdmError);


typedef BOOL (WINAPI *GAPIENUMTYPEMEMBERSFCTA)(
    LPCSTR                      lpszProjectFile,
    LPCSTR                      lpszTypeName,
    DM_ENUM_TYPEMEMBERS_PROCA   lpfnCallback,
    LPVOID                      lpvUser,
    LPCMN_ERRORA                lpdmError);
typedef BOOL (WINAPI *GAPIENUMTYPEMEMBERSFCTW)(
    LPCWSTR                     lpszProjectFile,
    LPCWSTR                     lpszTypeName,
    DM_ENUM_TYPEMEMBERS_PROCW   lpfnCallback,
    LPVOID                      lpvUser,
    LPCMN_ERRORW                lpdmError);

#ifdef UNICODE
#define NAME_GAPIENUMTYPEMEMBERS      "GAPIEnumTypeMembersW"
#define GAPIEnumTypeMembers           GAPIEnumTypeMembersW
typedef GAPIENUMTYPEMEMBERSFCTW       GAPIENUMTYPEMEMBERSFCT;
#else
#define NAME_GAPIENUMTYPEMEMBERS      "GAPIEnumTypeMembersA"
#define GAPIEnumTypeMembers           GAPIEnumTypeMembersA
typedef GAPIENUMTYPEMEMBERSFCTA       GAPIENUMTYPEMEMBERSFCT;
#endif

/*-----------------------------------------------------------------------
 * GAPIEnumTypeMembersEx
 */
BOOL WINAPI GAPIEnumTypeMembersExA(
    LPCSTR                          lpszProjectFile,
    LPCSTR                          lpszTypeName,
    DM_ENUM_TYPEMEMBERS_PROC_EXA    lpfnCallback,
    LPVOID                          lpvUser,
    LPCMN_ERRORA                    lpdmError);


BOOL WINAPI GAPIEnumTypeMembersExW(
    LPCWSTR                         lpszProjectFile,
    LPCWSTR                         lpszTypeName,
    DM_ENUM_TYPEMEMBERS_PROC_EXW    lpfnCallback,
    LPVOID                          lpvUser,
    LPCMN_ERRORW                    lpdmError);


typedef BOOL (WINAPI *GAPIENUMTYPEMEMBERSEXFCTA)(
    LPCSTR                          lpszProjectFile,
    LPCSTR                          lpszTypeName,
    DM_ENUM_TYPEMEMBERS_PROC_EXA    lpfnCallback,
    LPVOID                          lpvUser,
    LPCMN_ERRORA                    lpdmError);
typedef BOOL (WINAPI *GAPIENUMTYPEMEMBERSEXFCTW)(
    LPCWSTR                         lpszProjectFile,
    LPCWSTR                         lpszTypeName,
    DM_ENUM_TYPEMEMBERS_PROC_EXW    lpfnCallback,
    LPVOID                          lpvUser,
    LPCMN_ERRORW                    lpdmError);

#ifdef UNICODE
#define NAME_GAPIENUMTYPEMEMBERSEX      "GAPIEnumTypeMembersExW"
#define GAPIEnumTypeMembersEx           GAPIEnumTypeMembersExW
typedef GAPIENUMTYPEMEMBERSEXFCTW       GAPIENUMTYPEMEMBERSEXFCT;
#else
#define NAME_GAPIENUMTYPEMEMBERSEX      "GAPIEnumTypeMembersExA"
#define GAPIEnumTypeMembersEx           GAPIEnumTypeMembersExA
typedef GAPIENUMTYPEMEMBERSEXFCTA       GAPIENUMTYPEMEMBERSEXFCT;
#endif

/*-----------------------------------------------------------------------
 * GAPICreateTypInstance
 */
BOOL WINAPI GAPICreateTypInstanceA(
    LPCSTR                          lpszProjectFile,
    LPMCP_NEWVARIABLE_DATA_EXA      lpTypeInstance,
    DWORD                           dwNumMembers,
    DWORD                           dwFlags,
    LPMCP_NEWVARIABLE_DATA_EXA      lpMemberData,
    LPCMN_ERRORA                    lpdmError);


BOOL WINAPI GAPICreateTypInstanceW(
    LPCWSTR                         lpszProjectFile,
    LPMCP_NEWVARIABLE_DATA_EXW      lpTypeInstance,
    DWORD                           dwNumMembers,
    DWORD                           dwFlags,
    LPMCP_NEWVARIABLE_DATA_EXW      lpMemberData,
    LPCMN_ERRORW                    lpdmError);


typedef BOOL (WINAPI *GAPICREATETYPINSTANCEFCTA)(
    LPCSTR                          lpszProjectFile,
    LPMCP_NEWVARIABLE_DATA_EXA      lpTypeInstance,
    DWORD                           dwNumMembers,
    DWORD                           dwFlags,
    LPMCP_NEWVARIABLE_DATA_EXA      lpMemberData,
    LPCMN_ERRORA                    lpdmError);
typedef BOOL (WINAPI *GAPICREATETYPINSTANCEFCTW)(
    LPCWSTR                         lpszProjectFile,
    LPMCP_NEWVARIABLE_DATA_EXW      lpTypeInstance,
    DWORD                           dwNumMembers,
    DWORD                           dwFlags,
    LPMCP_NEWVARIABLE_DATA_EXW      lpMemberData,
    LPCMN_ERRORW                    lpdmError);

#ifdef UNICODE
#define NAME_GAPICREATETYPINSTANCE      "GAPICreateTypInstanceW"
#define GAPICreateTypInstance           GAPICreateTypInstanceW
typedef GAPICREATETYPINSTANCEFCTW       GAPICREATETYPINSTANCEFCT;
#else
#define NAME_GAPICREATETYPINSTANCE      "GAPICreateTypInstanceA"
#define GAPICreateTypInstance           GAPICreateTypInstanceA
typedef GAPICREATETYPINSTANCEFCTA       GAPICREATETYPINSTANCEFCT;
#endif



/////////////////////////////////////////////////////////////////////////////////////////////
// DM-API:  4th Generation ( supported in WinCC 4.0 )
/////////////////////////////////////////////////////////////////////////////////////////////

/*
 * MCP_VARIABLE_SCALES
 */
typedef struct tagMCP_VARIABLE_SCALESA
{
    DWORD   dwVarScaleFlags;    // ( DM_VARSCALE_NOSCALE, DM_VARSCALE_LINEAR )
    double  doMinProc;
    double  doMaxProc;
    double  doMinVar;
    double  doMaxVar;
} MCP_VARIABLE_SCALESA, FAR *LPMCP_VARIABLE_SCALESA ;
typedef struct tagMCP_VARIABLE_SCALESW
{
    DWORD   dwVarScaleFlags;    // ( DM_VARSCALE_NOSCALE, DM_VARSCALE_LINEAR )
    double  doMinProc;
    double  doMaxProc;
    double  doMinVar;
    double  doMaxVar;
} MCP_VARIABLE_SCALESW, FAR *LPMCP_VARIABLE_SCALESW;

#ifdef UNICODE
typedef MCP_VARIABLE_SCALESW    MCP_VARIABLE_SCALES;
typedef LPMCP_VARIABLE_SCALESW  LPMCP_VARIABLE_SCALES;
#else
typedef MCP_VARIABLE_SCALESA    MCP_VARIABLE_SCALES;
typedef LPMCP_VARIABLE_SCALESA  LPMCP_VARIABLE_SCALES;
#endif

/*
 * MCP_CHNCONVERSION
 */
typedef struct tagMCP_CHNCONVERSIONA
{
    BOOL    bUsesChannelConversion;
  CHAR  szUnit[MAX_DM_UNIT_NAME +1];    /* Name der Unit */
    char    szChnName[_MAX_PATH+1];
} MCP_CHNCONVERSIONA, FAR *LPMCP_CHNCONVERSIONA ;

typedef struct tagMCP_CHNCONVERSIONW
{
    BOOL    bUsesChannelConversion;
  WCHAR szUnit[MAX_DM_UNIT_NAME +1];    /* Name der Unit */
    WCHAR   szChnName[_MAX_PATH+1];
} MCP_CHNCONVERSIONW, FAR *LPMCP_CHNCONVERSIONW;

#ifdef UNICODE
typedef MCP_CHNCONVERSIONW    MCP_CHNCONVERSION;
typedef LPMCP_CHNCONVERSIONW  LPMCP_CHNCONVERSION;
#else
typedef MCP_CHNCONVERSIONA    MCP_CHNCONVERSION;
typedef LPMCP_CHNCONVERSIONA  LPMCP_CHNCONVERSION;
#endif

/*
 * MCP_NEWVARIABLE_DATA_STRUCT4
 */
typedef struct tagMCP_NEWVARIABLE_DATA_EX4
{
    DWORD                        dwFlags;                                 /* MCP_NVAR_FLAGs  */
    char                         szProjectFile[_MAX_PATH +1];             /* ProjektPfad + Name */
    char                         szConnection[MAX_DM_CONNECTION_NAME +3]; /* VerbindungsName */
    char                         szVarName[MAX_DM_VAR_NAME +1];           /* VariablenName   */
    char                         szGroupName[MAX_DM_GROUP_NAME +1];       /* GruppenName     */
    MCP_VARIABLE_COMMON_EXA      Common;                                  /* MCP_VARIABLE_COMMON Struktur */
    MCP_VARIABLE_PROTOCOL_EXA    Protocol;                                /* MCP_VARIABLE_PROTOCOL Struktur */
    MCP_VARIABLE_LIMITS_EXA      Limits;                                  /* MCP_VARIABLE_LIMITS Struktur */
    char                         szSpecific[MAX_DM_VAR_SPECIFIC +1];      /* Kanal / Verbindungsabhängige Parameter */
    MCP_VARIABLE_SCALESA         Scaling;
} MCP_NEWVARIABLE_DATA_EX4A, FAR *LPMCP_NEWVARIABLE_DATA_EX4A;
typedef struct tagMCP_NEWVARIABLE_DATA_EX4W
{
    DWORD                        dwFlags;                                 /* MCP_NVAR_FLAGs  */
    WCHAR                        szProjectFile[_MAX_PATH +1];             /* ProjektPfad + Name */
    WCHAR                        szConnection[MAX_DM_CONNECTION_NAME +3]; /* VerbindungsName */
    WCHAR                        szVarName[MAX_DM_VAR_NAME +1];           /* VariablenName   */
    WCHAR                        szGroupName[MAX_DM_GROUP_NAME +1];       /* GruppenName     */
    MCP_VARIABLE_COMMON_EXW      Common;                                  /* MCP_VARIABLE_COMMON Struktur */
    MCP_VARIABLE_PROTOCOL_EXW    Protocol;                                /* MCP_VARIABLE_PROTOCOL Struktur */
    MCP_VARIABLE_LIMITS_EXW      Limits;                                  /* MCP_VARIABLE_LIMITS Struktur */
    WCHAR                        szSpecific[MAX_DM_VAR_SPECIFIC +1];      /* Kanal / Verbindungsabhängige Parameter */
    MCP_VARIABLE_SCALESW         Scaling;
} MCP_NEWVARIABLE_DATA_EX4W, FAR *LPMCP_NEWVARIABLE_DATA_EX4W;

#ifdef UNICODE
typedef MCP_NEWVARIABLE_DATA_EX4W   MCP_NEWVARIABLE_DATA_EX4;
typedef LPMCP_NEWVARIABLE_DATA_EX4W LPMCP_NEWVARIABLE_DATA_EX4;
#else
typedef MCP_NEWVARIABLE_DATA_EX4A   MCP_NEWVARIABLE_DATA_EX4;
typedef LPMCP_NEWVARIABLE_DATA_EX4A LPMCP_NEWVARIABLE_DATA_EX4;
#endif

/*
 * MCP_NEWVARIABLE_DATA_4
 */
typedef struct tagMCP_NEWVARIABLE_DATA_4
{
    DWORD                       dwFlags;                                 /* MCP_NVAR_FLAGs  */
    char                        szProjectFile[_MAX_PATH +1];             /* ProjektPfad + Name */
    char                        szConnection[MAX_DM_CONNECTION_NAME +3]; /* VerbindungsName */
    char                        szVarName[MAX_DM_VAR_NAME +1];           /* VariablenName   */
    char                        szGroupName[MAX_DM_GROUP_NAME +1];       /* GruppenName     */
    MCP_VARIABLE_COMMONA        Common;                                  /* MCP_VARIABLE_COMMON Struktur */
    MCP_VARIABLE_PROTOCOLA      Protocol;                                /* MCP_VARIABLE_PROTOCOL Struktur */
    MCP_VARIABLE_LIMITSA        Limits;                                  /* MCP_VARIABLE_LIMITS Struktur */
    char                        szSpecific[MAX_DM_VAR_SPECIFIC +1];      /* Kanal / Verbindungsabhängige Parameter */
    MCP_VARIABLE_SCALESA        Scaling;
} MCP_NEWVARIABLE_DATA_4A, FAR *LPMCP_NEWVARIABLE_DATA_4A;
typedef struct tagMCP_NEWVARIABLE_DATA_4W
{
    DWORD                       dwFlags;                                 /* MCP_NVAR_FLAGs  */
    WCHAR                       szProjectFile[_MAX_PATH +1];             /* ProjektPfad + Name */
    WCHAR                       szConnection[MAX_DM_CONNECTION_NAME +3]; /* VerbindungsName */
    WCHAR                       szVarName[MAX_DM_VAR_NAME +1];           /* VariablenName   */
    WCHAR                       szGroupName[MAX_DM_GROUP_NAME +1];       /* GruppenName     */
    MCP_VARIABLE_COMMONW        Common;                                  /* MCP_VARIABLE_COMMON Struktur */
    MCP_VARIABLE_PROTOCOLW      Protocol;                                /* MCP_VARIABLE_PROTOCOL Struktur */
    MCP_VARIABLE_LIMITSW        Limits;                                  /* MCP_VARIABLE_LIMITS Struktur */
    WCHAR                       szSpecific[MAX_DM_VAR_SPECIFIC +1];      /* Kanal / Verbindungsabhängige Parameter */
    MCP_VARIABLE_SCALESW        Scaling;
} MCP_NEWVARIABLE_DATA_4W, FAR *LPMCP_NEWVARIABLE_DATA_4W;

#ifdef UNICODE
typedef MCP_NEWVARIABLE_DATA_4W   MCP_NEWVARIABLE_DATA_4;
typedef LPMCP_NEWVARIABLE_DATA_4W LPMCP_NEWVARIABLE_DATA_4;
#else
typedef MCP_NEWVARIABLE_DATA_4A   MCP_NEWVARIABLE_DATA_4;
typedef LPMCP_NEWVARIABLE_DATA_4A LPMCP_NEWVARIABLE_DATA_4;
#endif

/*-----------------------------------------------------------------------
 * GAPICreateNewVariable4
 */
BOOL WINAPI GAPICreateNewVariable4A(
    LPMCP_NEWVARIABLE_DATA_4A pData,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI GAPICreateNewVariable4W(
    LPMCP_NEWVARIABLE_DATA_4W pData,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *GAPICREATENEWVARIABLE4FCTA)(
    LPMCP_NEWVARIABLE_DATA_4A pData,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *GAPICREATENEWVARIABLE4FCTW)(
    LPMCP_NEWVARIABLE_DATA_4W pData,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_GAPICREATENEWVARIABLE4      "GAPICreateNewVariable4W"
#define GAPICreateNewVariable4           GAPICreateNewVariable4W
typedef GAPICREATENEWVARIABLE4FCTW       GAPICREATENEWVARIABLE4FCT;
#else
#define NAME_GAPICREATENEWVARIABLE4      "GAPICreateNewVariable4A"
#define GAPICreateNewVariable4           GAPICreateNewVariable4A
typedef GAPICREATENEWVARIABLE4FCTA       GAPICREATENEWVARIABLE4FCT;
#endif

/*-----------------------------------------------------------------------
 * GAPICreateNewVariableEx4
 */
BOOL WINAPI GAPICreateNewVariableEx4A(
    DWORD dwCreatorID,
    LPMCP_NEWVARIABLE_DATA_4A pData,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI GAPICreateNewVariableEx4W(
    DWORD dwCreatorID,
    LPMCP_NEWVARIABLE_DATA_4W pData,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *GAPICREATENEWVARIABLEEX4FCTA)(
    DWORD dwCreatorID,
    LPMCP_NEWVARIABLE_DATA_4A pData,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *GAPICREATENEWVARIABLEEX4FCTW)(
    DWORD dwCreatorID,
    LPMCP_NEWVARIABLE_DATA_4W pData,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_GAPICREATENEWVARIABLEEX4      "GAPICreateNewVariableEx4W"
#define GAPICreateNewVariableEx4           GAPICreateNewVariableEx4W
typedef GAPICREATENEWVARIABLEEX4FCTW       GAPICREATENEWVARIABLEEX4FCT;
#else
#define NAME_GAPICREATENEWVARIABLEEX4      "GAPICreateNewVariableEx4A"
#define GAPICreateNewVariable4Ex           GAPICreateNewVariableEx4A // Altes belassen für Altuser
#define GAPICreateNewVariableEx4           GAPICreateNewVariableEx4A
typedef GAPICREATENEWVARIABLEEX4FCTA       GAPICREATENEWVARIABLEEX4FCT;
#endif

// Variablen enumeration
/*
 * DM_VARIABLE_DATA4
 */
typedef struct tagDM_VARIABLE_DATA4A {
    DM_TYPEREFA             dmTypeRef;                                /* Type of variable-data */
    DM_VARLIMIT             dmVarLimit;                               /* limits of variables */
    VARIANT                 dmStart;                                  /* start value */
    VARIANT                 dmDefault;                                /* default value */
    DWORD                   dwNotify;                                 /* Protokolleinträge generieren für...*/
    DWORD                   dwFlags;                                  /* Ersatzwert verwenden */
    CHAR                    szSpecific[MAX_DM_VAR_SPECIFIC +1];       /* parameter */
    CHAR                    szGroup[MAX_DM_GROUP_NAME +1];            /* Variablengruppen */
    CHAR                    szConnection[MAX_DM_CONNECTION_NAME +1];  /* Verbindung */
    CHAR                    szChannel[_MAX_PATH +1];                  /* Name des Kanals */
    CHAR                    szUnit[MAX_DM_UNIT_NAME +1];              /* Name der Unit */
    MCP_VARIABLE_SCALESA    Scaling;                                  /* Skalierungsparameter */
    DWORD                   dwASDataSize;                              /* Variablenlänge */
    DWORD                   dwOSDataSize;                            /* Variablenlänge */
    DWORD                   dwVarProperty;                            /* Variablen Eigenschaften (Interne / Externe Variable) */
    DWORD                   dwFormat;                                 /* Formatanpassung */
} DM_VARIABLE_DATA4A, FAR *LPDM_VARIABLE_DATA4A;

typedef struct tagDM_VARIABLE_DATA4W {
    DM_TYPEREFW             dmTypeRef;                                /* Type of variable-data */
    DM_VARLIMIT             dmVarLimit;                               /* limits of variables */
    VARIANT                 dmStart;                                  /* start value */
    VARIANT                 dmDefault;                                /* default value */
    DWORD                   dwNotify;                                 /* Protokolleinträge generieren für...*/
    DWORD                   dwFlags;                                  /* Ersatzwert verwenden */
    WCHAR                   szSpecific[MAX_DM_VAR_SPECIFIC +1];       /* parameter */
    WCHAR                   szGroup[MAX_DM_GROUP_NAME +1];            /* Variablengruppen */
    WCHAR                   szConnection[MAX_DM_CONNECTION_NAME +1];  /* Verbindung */
    WCHAR                   szChannel[_MAX_PATH +1];                  /* Name des Kanals */
    WCHAR                   szUnit[MAX_DM_UNIT_NAME +1];              /* Name der Unit */
    MCP_VARIABLE_SCALESW    Scaling;                                  /* Skalierungsparameter */
    DWORD                   dwASDataSize;                              /* Variablenlänge */
    DWORD                   dwOSDataSize;                            /* Variablenlänge */
    DWORD                   dwVarProperty;                            /* Variablen Eigenschaften (Interne / Externe Variable) */
    DWORD                   dwFormat;                                 /* Formatanpassung */
} DM_VARIABLE_DATA4W, FAR *LPDM_VARIABLE_DATA4W;

#ifdef UNICODE
typedef DM_VARIABLE_DATA4W         DM_VARIABLE_DATA4;
typedef LPDM_VARIABLE_DATA4W       LPDM_VARIABLE_DATA4;
#else
typedef DM_VARIABLE_DATA4A         DM_VARIABLE_DATA4;
typedef LPDM_VARIABLE_DATA4A       LPDM_VARIABLE_DATA4;
#endif


/*
 * DM_ENUM_VARIABLE_PROC4
 */
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_VARIABLE_PROC4A)(
    LPDM_VARKEYA         lpdmVarKey,
    LPDM_VARIABLE_DATA4A lpdmVarData,
    LPVOID               lpvUser);
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_VARIABLE_PROC4W)(
    LPDM_VARKEYW         lpdmVarKey,
    LPDM_VARIABLE_DATA4W lpdmVarData,
    LPVOID               lpvUser);

#ifdef UNICODE
typedef DM_ENUM_VARIABLE_PROC4W      DM_ENUM_VARIABLE_PROC4;
#else
typedef DM_ENUM_VARIABLE_PROC4A      DM_ENUM_VARIABLE_PROC4;
#endif

/*-----------------------------------------------------------------------
 * DMEnumVarData4
 */
BOOL WINAPI DMEnumVarData4A(
    LPCSTR lpszProjectFile,
    LPDM_VARKEYA lpdmVarKey,
    DWORD dwItems,
    DM_ENUM_VARIABLE_PROC4A lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI DMEnumVarData4W(
    LPCWSTR lpszProjectFile,
    LPDM_VARKEYW lpdmVarKey,
    DWORD dwItems,
    DM_ENUM_VARIABLE_PROC4W lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *DMENUMVARDATAFCT4A)(
    LPCSTR lpszProjectFile,
    LPDM_VARKEYA lpdmVarKey,
    DWORD dwItems,
    DM_ENUM_VARIABLE_PROC4A lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMENUMVARDATAFCT4W)(
    LPCWSTR lpszProjectFile,
    LPDM_VARKEYW lpdmVarKey,
    DWORD dwItems,
    DM_ENUM_VARIABLE_PROC4W lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMENUMVARDAT4A      "DMEnumVarData4W"
#define DMEnumVarData4           DMEnumVarData4W
typedef DMENUMVARDATAFCT4W       DMENUMVARDATAFCT4;
#else
#define NAME_DMENUMVARDAT4A      "DMEnumVarData4A"
#define DMEnumVarData4           DMEnumVarData4A
typedef DMENUMVARDATAFCT4A       DMENUMVARDATAFCT4;
#endif
#define NAME_DMENUMVARDATA4      NAME_DMENUMVARDAT4A
#define DMENUMVARDATA4FCT        DMENUMVARDATAFCT4


/*-----------------------------------------------------------------------
 * GAPICreateType4
 */
BOOL WINAPI GAPICreateType4A(
    LPCSTR                      lpszProjectFile,
    LPDM_TYPE_DESCRIPTORA       lpdmType,
    DWORD                       dwFlags,
    DWORD                       dwCreatorID,
    LPMCP_NEWVARIABLE_DATA_EX4A lpMemberdata,
    LPMCP_CHNCONVERSIONA        pChnConversion,
    LPCMN_ERRORA                lpdmError);


BOOL WINAPI GAPICreateType4W(
    LPCWSTR                     lpszProjectFile,
    LPDM_TYPE_DESCRIPTORW       lpdmType,
    DWORD                       dwFlags,
    DWORD                       dwCreatorID,
    LPMCP_NEWVARIABLE_DATA_EX4W lpMemberdata,
    LPMCP_CHNCONVERSIONW        pChnConversion,
    LPCMN_ERRORW                lpdmError);


typedef BOOL (WINAPI *GAPICREATETYPE4FCTA)(
    LPCSTR                      lpszProjectFile,
    LPDM_TYPE_DESCRIPTORA       lpdmType,
    DWORD                       dwFlags,
    DWORD                       dwCreatorID,
    LPMCP_NEWVARIABLE_DATA_EX4A lpMemberdata,
    LPMCP_CHNCONVERSIONA        pChnConversion,
    LPCMN_ERRORA                lpdmError);
typedef BOOL (WINAPI *GAPICREATETYPE4FCTW)(
    LPCWSTR                     lpszProjectFile,
    LPDM_TYPE_DESCRIPTORW       lpdmType,
    DWORD                       dwFlags,
    DWORD                       dwCreatorID,
    LPMCP_NEWVARIABLE_DATA_EX4W lpMemberdata,
    LPMCP_CHNCONVERSIONW        pChnConversion,
    LPCMN_ERRORW                lpdmError);

#ifdef UNICODE
#define NAME_GAPICREATETYPE4         "GAPICreateType4W"
#define GAPICreateType4              GAPICreateType4W
typedef GAPICREATETYPE4FCTW          GAPICREATETYPE4FCT;
#else
#define NAME_GAPICREATETYPE4         "GAPICreateType4A"
#define GAPICreateType4              GAPICreateType4A
typedef GAPICREATETYPE4FCTA          GAPICREATETYPE4FCT;
#endif

/*-----------------------------------------------------------------------
 * GAPICreateTypInstance4
 */
BOOL WINAPI GAPICreateTypInstance4A(
    LPCSTR                          lpszProjectFile,
    LPMCP_NEWVARIABLE_DATA_EX4A     lpTypeInstance,
    DWORD                           dwNumMembers,
    DWORD                           dwFlags,
    LPMCP_NEWVARIABLE_DATA_EX4A     lpMemberData,
    LPCMN_ERRORA                    lpdmError);


BOOL WINAPI GAPICreateTypInstance4W(
    LPCWSTR                         lpszProjectFile,
    LPMCP_NEWVARIABLE_DATA_EX4W     lpTypeInstance,
    DWORD                           dwNumMembers,
    DWORD                           dwFlags,
    LPMCP_NEWVARIABLE_DATA_EX4W     lpMemberData,
    LPCMN_ERRORW                    lpdmError);


typedef BOOL (WINAPI *GAPICREATETYPINSTANCE4FCTA)(
    LPCSTR                          lpszProjectFile,
    LPMCP_NEWVARIABLE_DATA_EX4A     lpTypeInstance,
    DWORD                           dwNumMembers,
    DWORD                           dwFlags,
    LPMCP_NEWVARIABLE_DATA_EX4A     lpMemberData,
    LPCMN_ERRORA                    lpdmError);
typedef BOOL (WINAPI *GAPICREATETYPINSTANCE4FCTW)(
    LPCWSTR                         lpszProjectFile,
    LPMCP_NEWVARIABLE_DATA_EX4W     lpTypeInstance,
    DWORD                           dwNumMembers,
    DWORD                           dwFlags,
    LPMCP_NEWVARIABLE_DATA_EX4W     lpMemberData,
    LPCMN_ERRORW                    lpdmError);

#ifdef UNICODE
#define NAME_GAPICREATETYPINSTANCE4      "GAPICreateTypInstance4W"
#define GAPICreateTypInstance4           GAPICreateTypInstance4W
typedef GAPICREATETYPINSTANCE4FCTW       GAPICREATETYPINSTANCE4FCT;
#else
#define NAME_GAPICREATETYPINSTANCE4      "GAPICreateTypInstance4A"
#define GAPICreateTypInstance4           GAPICreateTypInstance4A
typedef GAPICREATETYPINSTANCE4FCTA       GAPICREATETYPINSTANCE4FCT;
#endif

/*
 * DM_ENUM_TYPEMEMBERS_PROC_EX4
 */
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_TYPEMEMBERS_PROC_EX4A)(
    LPDM_VARKEYA                lpdmVarKey,
    LPMCP_NEWVARIABLE_DATA_EX4A lpdmVarDataEx,
    LPVOID                      lpvUser);
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_TYPEMEMBERS_PROC_EX4W)(
    LPDM_VARKEYW                lpdmVarKey,
    LPMCP_NEWVARIABLE_DATA_EX4W lpdmVarDataEx,
    LPVOID                      lpvUser);

#ifdef UNICODE
typedef DM_ENUM_TYPEMEMBERS_PROC_EX4W    DM_ENUM_TYPEMEMBERS_PROC_EX4;
#else
typedef DM_ENUM_TYPEMEMBERS_PROC_EX4A    DM_ENUM_TYPEMEMBERS_PROC_EX4;
#endif

/*-----------------------------------------------------------------------
 * GAPIEnumTypeMembersEx4
 */
BOOL WINAPI GAPIEnumTypeMembersEx4A(
    LPCSTR                          lpszProjectFile,
    LPCSTR                          lpszTypeName,
    DM_ENUM_TYPEMEMBERS_PROC_EX4A   lpfnCallback,
    LPVOID                          lpvUser,
    LPCMN_ERRORA                    lpdmError);


BOOL WINAPI GAPIEnumTypeMembersEx4W(
    LPCWSTR                         lpszProjectFile,
    LPCWSTR                         lpszTypeName,
    DM_ENUM_TYPEMEMBERS_PROC_EX4W   lpfnCallback,
    LPVOID                          lpvUser,
    LPCMN_ERRORW                    lpdmError);


typedef BOOL (WINAPI *GAPIENUMTYPEMEMBERSEX4FCTA)(
    LPCSTR                          lpszProjectFile,
    LPCSTR                          lpszTypeName,
    DM_ENUM_TYPEMEMBERS_PROC_EX4A   lpfnCallback,
    LPVOID                          lpvUser,
    LPCMN_ERRORA                    lpdmError);
typedef BOOL (WINAPI *GAPIENUMTYPEMEMBERSEX4FCTW)(
    LPCWSTR                         lpszProjectFile,
    LPCWSTR                         lpszTypeName,
    DM_ENUM_TYPEMEMBERS_PROC_EX4W   lpfnCallback,
    LPVOID                          lpvUser,
    LPCMN_ERRORW                    lpdmError);

#ifdef UNICODE
#define NAME_GAPIENUMTYPEMEMBERSEX4     "GAPIEnumTypeMembersEx4W"
#define GAPIEnumTypeMembersEx4           GAPIEnumTypeMembersEx4W
typedef GAPIENUMTYPEMEMBERSEX4FCTW       GAPIENUMTYPEMEMBERSEX4FCT;
#else
#define NAME_GAPIENUMTYPEMEMBERSEX4     "GAPIEnumTypeMembersEx4A"
#define GAPIEnumTypeMembersEx4           GAPIEnumTypeMembersEx4A
typedef GAPIENUMTYPEMEMBERSEX4FCTA       GAPIENUMTYPEMEMBERSEX4FCT;
#endif



/*
 * VARIABLE_LIMITS5
 */
typedef struct tagMCP_VARIABLE_LIMITS5A
{
    VARIANT varTopLimit;          // Obergrenze
    VARIANT varBottomLimit;       // Untergrenze
    VARIANT varStartValue;        // Startwert
    VARIANT varSubstituteValue;   // Ersatzwert
    BOOL    bTopLimit;          // Ersatzwert verwenden bei Obergrenze
    BOOL    bBottomLimit;       // Ersatzwert verwenden bei Untergrenze
    BOOL    bStartValue;        // Ersatzwert verwenden als Startwert
    BOOL    bConnectionErr;     // Ersatzwert verwenden bei Verbindungsfehlern
    BOOL    bTopLimitValid;     // Wert Obergrenze gültig
    BOOL    bBottomLimitValid;  // Wert Untergrenze gültig
    BOOL    bStartValueValid;   // Startwert gültig
    BOOL    bSubstValueValid;   // Ersatzwert gültig
} MCP_VARIABLE_LIMITS5A, FAR *LPMCP_VARIABLE_LIMITS5A;
typedef struct tagMCP_VARIABLE_LIMITS5W
{
    VARIANT varTopLimit;          // Obergrenze
    VARIANT varBottomLimit;       // Untergrenze
    VARIANT varStartValue;        // Startwert
    VARIANT varSubstituteValue;   // Ersatzwert
    BOOL    bTopLimit;          // Ersatzwert verwenden bei Obergrenze
    BOOL    bBottomLimit;       // Ersatzwert verwenden bei Untergrenze
    BOOL    bStartValue;        // Ersatzwert verwenden als Startwert
    BOOL    bConnectionErr;     // Ersatzwert verwenden bei Verbindungsfehlern
    BOOL    bTopLimitValid;     // Wert Obergrenze gültig
    BOOL    bBottomLimitValid;  // Wert Untergrenze gültig
    BOOL    bStartValueValid;   // Startwert gültig
    BOOL    bSubstValueValid;   // Ersatzwert gültig
} MCP_VARIABLE_LIMITS5W, FAR *LPMCP_VARIABLE_LIMITS5W;

#ifdef UNICODE
typedef MCP_VARIABLE_LIMITS5W    MCP_VARIABLE_LIMITS5;
typedef LPMCP_VARIABLE_LIMITS5W  LPMCP_VARIABLE_LIMITS5;
#else
typedef MCP_VARIABLE_LIMITS5A    MCP_VARIABLE_LIMITS5;
typedef LPMCP_VARIABLE_LIMITS5A  LPMCP_VARIABLE_LIMITS5;
#endif

/*
 * MCP_NEWVARIABLE_DATA_5
 */
typedef struct tagMCP_NEWVARIABLE_DATA_5
{
    DWORD                       dwFlags;                                 /* MCP_NVAR_FLAGs  */
    char                        szProjectFile[_MAX_PATH +1];             /* ProjektPfad + Name */
    char                        szConnection[MAX_DM_CONNECTION_NAME +3]; /* VerbindungsName */
    char                        szVarName[MAX_DM_VAR_NAME +1];           /* VariablenName   */
    char                        szGroupName[MAX_DM_GROUP_NAME +1];       /* GruppenName     */
    MCP_VARIABLE_COMMONA        Common;                                  /* MCP_VARIABLE_COMMON Struktur */
    MCP_VARIABLE_PROTOCOLA      Protocol;                                /* MCP_VARIABLE_PROTOCOL Struktur */
    MCP_VARIABLE_LIMITS5A       Limits;                                  /* MCP_VARIABLE_LIMITS Struktur */
    char                        szSpecific[MAX_DM_VAR_SPECIFIC +1];      /* Kanal / Verbindungsabhängige Parameter */
    MCP_VARIABLE_SCALESA        Scaling;
} MCP_NEWVARIABLE_DATA_5A, FAR *LPMCP_NEWVARIABLE_DATA_5A;
typedef struct tagMCP_NEWVARIABLE_DATA_5W
{
    DWORD                       dwFlags;                                 /* MCP_NVAR_FLAGs  */
    WCHAR                       szProjectFile[_MAX_PATH +1];             /* ProjektPfad + Name */
    WCHAR                       szConnection[MAX_DM_CONNECTION_NAME +3]; /* VerbindungsName */
    WCHAR                       szVarName[MAX_DM_VAR_NAME +1];           /* VariablenName   */
    WCHAR                       szGroupName[MAX_DM_GROUP_NAME +1];       /* GruppenName     */
    MCP_VARIABLE_COMMONW        Common;                                  /* MCP_VARIABLE_COMMON Struktur */
    MCP_VARIABLE_PROTOCOLW      Protocol;                                /* MCP_VARIABLE_PROTOCOL Struktur */
    MCP_VARIABLE_LIMITS5W       Limits;                                  /* MCP_VARIABLE_LIMITS Struktur */
    WCHAR                       szSpecific[MAX_DM_VAR_SPECIFIC +1];      /* Kanal / Verbindungsabhängige Parameter */
    MCP_VARIABLE_SCALESW        Scaling;
} MCP_NEWVARIABLE_DATA_5W, FAR *LPMCP_NEWVARIABLE_DATA_5W;

#ifdef UNICODE
typedef MCP_NEWVARIABLE_DATA_5W   MCP_NEWVARIABLE_DATA_5;
typedef LPMCP_NEWVARIABLE_DATA_5W LPMCP_NEWVARIABLE_DATA_5;
#else
typedef MCP_NEWVARIABLE_DATA_5A   MCP_NEWVARIABLE_DATA_5;
typedef LPMCP_NEWVARIABLE_DATA_5A LPMCP_NEWVARIABLE_DATA_5;
#endif

/*-----------------------------------------------------------------------
 * GAPICreateNewVariableEx5
 */
BOOL WINAPI GAPICreateNewVariable5A(
    DWORD dwCreatorID,
    LPMCP_NEWVARIABLE_DATA_5A pData,
    LPCMN_ERRORA lpdmError);


BOOL WINAPI GAPICreateNewVariable5W(
    DWORD dwCreatorID,
    LPMCP_NEWVARIABLE_DATA_5W pData,
    LPCMN_ERRORW lpdmError);


typedef BOOL (WINAPI *GAPICREATENEWVARIABLE5FCTA)(
    DWORD dwCreatorID,
    LPMCP_NEWVARIABLE_DATA_5A pData,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *GAPICREATENEWVARIABLE5FCTW)(
    DWORD dwCreatorID,
    LPMCP_NEWVARIABLE_DATA_5W pData,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_GAPICREATENEWVARIABLE5      "GAPICreateNewVariable5W"
#define GAPICreateNewVariable5           GAPICreateNewVariable5W
typedef GAPICREATENEWVARIABLE5FCTW       GAPICREATENEWVARIABLE5FCT;
#else
#define NAME_GAPICREATENEWVARIABLE5      "GAPICreateNewVariable5A"
#define GAPICreateNewVariable5           GAPICreateNewVariable5A
typedef GAPICREATENEWVARIABLE5FCTA       GAPICREATENEWVARIABLE5FCT;
#endif


BOOL WINAPI GAPIStartA(LPCSTR       lpszProjectFile,
                       DWORD        dwCacheSize,
                       LPCMN_ERRORA lpdmError  );


BOOL WINAPI GAPIStartW(LPCWSTR      lpszProjectFile,
                       DWORD        dwCacheSize,
                       LPCMN_ERRORW lpdmError  );


typedef BOOL (WINAPI *GAPISTARTFCTA)(
                        LPCSTR       lpszProjectFile,
                        DWORD        dwCacheSize,
                        LPCMN_ERRORA lpdmError  );
typedef BOOL (WINAPI *GAPISTARTFCTW)(
                        LPCWSTR      lpszProjectFile,
                        DWORD        dwCacheSize,
                        LPCMN_ERRORW lpdmError  );

#ifdef UNICODE
#define NAME_GAPISTART      "GAPIStartW"
#define GAPIStart           GAPIStartW
typedef GAPISTARTFCTW          GAPISTARTFCT;
#else
#define NAME_GAPISTART      "GAPIStartA"
#define GAPIStart           GAPIStartA
typedef GAPISTARTFCTA          GAPISTARTFCT;
#endif

BOOL WINAPI GAPIEndA(LPSTR szError,
                     DWORD dwErrorSize,
                     LPCMN_ERRORA  lpdmError  );


BOOL WINAPI GAPIEndW(LPWSTR szError,
                     DWORD dwErrorSize,
                     LPCMN_ERRORW  lpdmError  );


typedef BOOL (WINAPI *GAPIENDFCTA)(
                        LPSTR szError,
                        DWORD dwErrorSize,
                        LPCMN_ERRORA lpdmError  );
typedef BOOL (WINAPI *GAPIENDFCTW)(
                        LPWSTR szError,
                        DWORD dwErrorSize,
                        LPCMN_ERRORW lpdmError  );

#ifdef UNICODE
#define NAME_GAPIEND      "GAPIEndW"
#define GAPIEnd           GAPIEndW
typedef GAPIENDFCTW          GAPIENDFCT;
#else
#define NAME_GAPIEND      "GAPIEndA"
#define GAPIEnd           GAPIEndA
typedef GAPIENDFCTA          GAPIENDFCT;
#endif


BOOL WINAPI DMGetOwnServerPrefixA(LPSTR lpszPrefix,
                     DWORD dwSize,
                     LPCMN_ERRORA  lpdmError  );

BOOL WINAPI DMGetOwnServerPrefixW(LPWSTR lpszPrefix,
                     DWORD dwSize,
                     LPCMN_ERRORW  lpdmError  );

typedef BOOL (WINAPI *DMGETOWNSERVERPREFIXFCTA)(
                        LPSTR lpszPrefix,
                        DWORD dwSize,
                        LPCMN_ERRORA lpdmError  );
typedef BOOL (WINAPI *DMGETOWNSERVERPREFIXFCTW)(
                        LPWSTR lpszPrefix,
                        DWORD dwSize,
                        LPCMN_ERRORW lpdmError  );

#ifdef UNICODE
#define NAME_DMGETOWNSERVERPREFIX      "DMGetOwnServerPrefixW"
#define DMGetOwnServerPrefix           DMGetOwnServerPrefixW
typedef DMGETOWNSERVERPREFIXFCTW          DMGETOWNSERVERPREFIXFCT;
#else
#define NAME_DMGETOWNSERVERPREFIX      "DMGetOwnServerPrefixA"
#define DMGetOwnServerPrefix           DMGetOwnServerPrefixA
typedef DMGETOWNSERVERPREFIXFCTA          DMGETOWNSERVERPREFIXFCT;
#endif


//-----------------------------------------------------------------------
// Variablen enumeration
/*-----------------------------------------------------------------------
 * DMEnumVariables5
 */

typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_VAR_PROC_5A)(
    VARIANT*            lpvVariantData,
    LPVOID              lpvUser);
typedef BOOL (* __OUTSIDE_CISS__ DM_ENUM_VAR_PROC_5W)(
    VARIANT*            lpvVariantData,
    LPVOID              lpvUser);

#ifdef UNICODE
typedef DM_ENUM_VAR_PROC_5W      DM_ENUM_VAR_PROC_5;
#else
typedef DM_ENUM_VAR_PROC_5A      DM_ENUM_VAR_PROC_5;

#endif


BOOL WINAPI DMEnumVariables5A(
    LPCSTR lpszProjectFile,
    LPDM_VARFILTERA lpdmVarFilter,
    DWORD dwEnumBlockCount,
    DM_ENUM_VAR_PROC_5A lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError
);

BOOL WINAPI DMEnumVariables5W(
    LPCWSTR lpszProjectFile,
    LPDM_VARFILTERW lpdmVarFilter,
    DWORD dwEnumBlockCount,
    DM_ENUM_VAR_PROC_5W lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError
);

BOOL WINAPI DMEnumVariables5W(
    LPCWSTR lpszProjectFile,
    LPDM_VARFILTERW lpdmVarFilter,
    DWORD dwEnumBlockCount,
    DM_ENUM_VAR_PROC_5W lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError
);

typedef BOOL (WINAPI *DMENUMVARIABLES5A)(
    LPCSTR lpszProjectFile,
    LPDM_VARFILTERA lpdmVarFilter,
    DWORD dwEnumBlockCount,
    DM_ENUM_VAR_PROC_5A lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMENUMVARIABLES5W)(
    LPCWSTR lpszProjectFile,
    LPDM_VARFILTERW lpdmVarFilter,
    DWORD dwEnumBlockCount,
    DM_ENUM_VAR_PROC_5W lpfnEnum,
    LPVOID lpvUser,
    LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMENUMVARIABLES5      "DMEnumVariables5W"


#define DMEnumVariables5           DMEnumVariables5W
typedef DMENUMVARIABLES5W          DMENUMVARIABLES5;
#else
#define NAME_DMENUMVARIABLES5      "DMEnumVariables5A"
#define DMEnumVariables5           DMEnumVariables5A
typedef DMENUMVARIABLES5A          DMENUMVARIABLES5;
#endif




BOOL WINAPI DMIsServiceProjectA(DWORD* pdwServiceProject,
                     LPCMN_ERRORA  lpdmError  );

BOOL WINAPI DMIsServiceProjectW(DWORD* pdwServiceProject,
                     LPCMN_ERRORW  lpdmError  );

typedef BOOL (WINAPI *DMISSERVICEPROJECTFCTA)(
                        DWORD* pdwServiceProject,
                        LPCMN_ERRORA lpdmError  );
typedef BOOL (WINAPI *DMISSERVICEPROJECTFCTW)(
                        DWORD* pdwServiceProject,
                        LPCMN_ERRORW lpdmError  );

#ifdef UNICODE
#define NAME_DMISSERVICEPROJECT      "DMIsServiceProjectW"
#define DMIsServiceProject           DMIsServiceProjectW
typedef DMISSERVICEPROJECTFCTW       DMISSERVICEPROJECTFCT;
#else
#define NAME_DMISSERVICEPROJECT      "DMIsServiceProjectA"
#define DMIsServiceProject           DMIsServiceProjectA
typedef DMISSERVICEPROJECTFCTA       DMISSERVICEPROJECTFCT;
#endif



BOOL WINAPI DMOpenProjectDocPlusA(
                     LPCSTR        lpszProjectMCXFile,
                     LPSTR         lpszProjectMCPFile,
                     DWORD         dwBufCharCountMCP,
                     LPCMN_ERRORA  lpdmError  );

BOOL WINAPI DMOpenProjectDocPlusW(
                     LPCWSTR       lpszProjectMCXFile,
                     LPWSTR        lpszProjectMCPFile,
                     DWORD         dwBufCharCountMCP,
                     LPCMN_ERRORW  lpdmError  );

typedef BOOL (WINAPI *DMOPENPROJECTDOCPLUSFCTA)(
                        LPCSTR       lpszProjectMCXFile,
                        LPSTR        lpszProjectMCPFile,
                        DWORD        dwBufCharCountMCP,
                        LPCMN_ERRORA lpdmError  );
typedef BOOL (WINAPI *DMOPENPROJECTDOCPLUSFCTW)(
                        LPCWSTR      lpszProjectMCXFile,
                        LPWSTR       lpszProjectMCPFile,
                        DWORD        dwBufCharCountMCP,
                        LPCMN_ERRORW lpdmError  );

#ifdef UNICODE
#define NAME_DMOPENPROJECTDOCPLUS      "DMOpenProjectDocPlusW"
#define DMOpenProjectDocPlus           DMOpenProjectDocPlusW
typedef DMOPENPROJECTDOCPLUSFCTW       DMOPENPROJECTDOCPLUSFCT;
#else
#define NAME_DMOPENPROJECTDOCPLUS      "DMOpenProjectDocPlusA"
#define DMOpenProjectDocPlus           DMOpenProjectDocPlusA
typedef DMOPENPROJECTDOCPLUSFCTA       DMOPENPROJECTDOCPLUSFCT;
#endif



BOOL WINAPI DMOpenProjectPlusA(
                     HWND          hwndParent,
                     LPSTR         lpszProjectMCXFile,
                     DWORD         dwBufCharCountMCX,
                     LPSTR         lpszProjectMCPFile,
                     DWORD         dwBufCharCountMCP,
                     LPCMN_ERRORA  lpdmError  );

BOOL WINAPI DMOpenProjectPlusW(
                     HWND          hwndParent,
                     LPWSTR        lpszProjectMCXFile,
                     DWORD         dwBufCharCountMCX,
                     LPWSTR        lpszProjectMCPFile,
                     DWORD         dwBufCharCountMCP,
                     LPCMN_ERRORW  lpdmError  );

typedef BOOL (WINAPI *DMOPENPROJECTPLUSFCTA)(
                        HWND          hwndParent,
                        LPSTR         lpszProjectMCXFile,
                        DWORD         dwBufCharCountMCX,
                        LPSTR         lpszProjectMCPFile,
                        DWORD         dwBufCharCountMCP,
                        LPCMN_ERRORA  lpdmError  );
typedef BOOL (WINAPI *DMOPENPROJECTPLUSFCTW)(
                        HWND          hwndParent,
                        LPWSTR        lpszProjectMCXFile,
                        DWORD         dwBufCharCountMCX,
                        LPWSTR        lpszProjectMCPFile,
                        DWORD         dwBufCharCountMCP,
                        LPCMN_ERRORW  lpdmError  );

#ifdef UNICODE
#define NAME_DMOPENPROJECTPLUS      "DMOpenProjectPlusW"
#define DMOpenProjectPlus           DMOpenProjectPlusW
typedef DMOPENPROJECTPLUSFCTW       DMOPENPROJECTPLUSFCT;
#else
#define NAME_DMOPENPROJECTPLUS      "DMOpenProjectPlusA"
#define DMOpenProjectPlus           DMOpenProjectPlusA
typedef DMOPENPROJECTPLUSFCTA       DMOPENPROJECTPLUSFCT;
#endif


//RQ AP01291487
/*-----------------------------------------------------------------------
 * DMDeactivateRTProjectEx
 */
BOOL WINAPI DMDeactivateRTProjectExA(DWORD dwOptions, LPCMN_ERRORA lpdmError);
BOOL WINAPI DMDeactivateRTProjectExW(DWORD dwOptions, LPCMN_ERRORW lpdmError);

typedef BOOL (WINAPI *DMDEACTIVATERTPROJECTEXFCTA)(DWORD dwOptions, LPCMN_ERRORA lpdmError);
typedef BOOL (WINAPI *DMDEACTIVATERTPROJECTEXFCTW)(DWORD dwOptions, LPCMN_ERRORW lpdmError);

#ifdef UNICODE
#define NAME_DMDEACTIVATERTPROJECTEX      "DMDeactivateRTProjectExW"
#define DMDeactivateRTProjectEx           DMDeactivateRTProjectExW
typedef DMDEACTIVATERTPROJECTEXFCTW       DMDEACTIVATERTPROJECTEXFCT;
#else
#define NAME_DMDEACTIVATERTPROJECTEX      "DMDeactivateRTProjectExA"
#define DMDeactivateRTProjectEx           DMDeactivateRTProjectExA
typedef DMDEACTIVATERTPROJECTEXFCTA       DMDEACTIVATERTPROJECTEXFCT;
#endif


#ifdef __cplusplus
}
#endif

#ifdef _MSC_VER
  #pragma pack (pop, DMCLIENT_Packing_Alignment)
#endif

#endif // __DMCLIENT_H__
