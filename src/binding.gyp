{
  "targets": [
    {
      "target_name": "odk",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "sources": [
        "src/odk.cpp",
        "src/napi.cpp"
      ],
      "include_dirs": [      
        "<!@(node -p \"require('node-addon-api').include\")",
		    "C:\Program Files (x86)\Siemens\ODK\include",
        ".\src"
        
        
      

      ],
      "libraries": [
        "apcli_S.lib",
        "ArchClnt.lib",
        "CCMsRtCliPlus.lib",
        "CCUACAPI.lib",
        "CSigapi.lib",
        "CSRT_D.lib",
        "DB.lib",
        "dmclient.lib",
        "dywit_s.lib",
        "gscgr_s.lib",
        "lbm_api.lib",
        "MSCSCli.lib",
        "MsRtCli.lib",
        "Ohio_d.lib",
        "Pass_s.lib",
        "PdeCsCli.lib",
        "PdeRtCli.lib",
        "pdlcs_s.lib",
        "PDLRT_S.lib",
        "ptm_api.lib",
        "rdlcsas.lib",
        "RPJApi.lib",
        "s5common.lib",
        "sci32s.lib",
        "Ssmcsapi.lib",
        "ssmrt.lib",
        "StdOb_d.lib",
        "SyDiagD.lib",
        "Tagtable.lib",
        "TEXT_CS.lib",
        "Text_RT.lib",
        "ul_lib32.lib",
        "UseGen.lib", 
        "apgen_S.lib",
        "comsupp.lib"
      ],
      "link_settings": {
            'library_dirs': [
                'C:\Program Files (x86)\Siemens\ODK\lib',
                'C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\Tools\\MSVC\\14.30.30705\\lib\\spectre\\x86',                
            ]
      },
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS',"WIN32", "NDEBUG","_CONSOLE","_MBCS" ],
    }
  ]
}