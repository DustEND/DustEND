#include "deviceinfo.h"

#define _WIN32_DCOM
#include <iostream>
#include <comdef.h>
#include <comutil.h>
#include <Wbemidl.h>
#include <algorithm>

#include <QDebug>

#pragma comment(lib, "wbemuuid.lib")

std::string osName()
{
    HRESULT hres;
    std::string procName = "";

    CoUninitialize();
    hres =  CoInitializeEx(0, COINIT_MULTITHREADED);
    hres =  CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres)) {
        std::cout << "Failed to initialize COM library. Error code = 0x"
                  << std::hex << hres << std::endl;
        return "";                  // Program has failed.
    }

    hres =  CoInitializeSecurity(NULL,
                                 -1,                          // COM authentication
                                 NULL,                        // Authentication services
                                 NULL,                        // Reserved
                                 RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
                                 RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
                                 NULL,                        // Authentication info
                                 EOAC_NONE,                   // Additional capabilities
                                 NULL                         // Reserved
                                 );

    if (FAILED(hres)) {
        std::cout << "Failed to initialize security. Error code = 0x"
                  << std::hex << hres << std::endl;
        CoUninitialize();
        return "";                    // Program has failed.
    }

    IWbemLocator *pLoc = NULL;

    hres = CoCreateInstance(CLSID_WbemLocator,
                            0,
                            CLSCTX_INPROC_SERVER,
                            IID_IWbemLocator, (LPVOID *) &pLoc);

    if (FAILED(hres)) {
        std::cout << "Failed to create IWbemLocator object."
                  << " Err code = 0x"
                  << std::hex << hres << std::endl;
        CoUninitialize();
        return "";                 // Program has failed.
    }

    IWbemServices *pSvc = NULL;

    hres = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
                               NULL,                    // User name. NULL = current user
                               NULL,                    // User password. NULL = current
                               0,                       // Locale. NULL indicates current
                               NULL,                    // Security flags.
                               0,                       // Authority (e.g. Kerberos)
                               0,                       // Context object
                               &pSvc                    // pointer to IWbemServices proxy
                               );

    if (FAILED(hres)) {
        std::cout << "Could not connect. Error code = 0x"
                  << std::hex << hres << std::endl;
        pLoc->Release();
        CoUninitialize();
        return "";                // Program has failed.
    }

    std::cout << "Connected to ROOT\\CIMV2 WMI namespace" << std::endl;

    hres = CoSetProxyBlanket(pSvc,                        // Indicates the proxy to set
                             RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
                             RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
                             NULL,                        // Server principal name
                             RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
                             RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
                             NULL,                        // client identity
                             EOAC_NONE                    // proxy capabilities
                             );

    if (FAILED(hres)) {
        std::cout << "Could not set proxy blanket. Error code = 0x"
                  << std::hex << hres << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return "";               // Program has failed.
    }

    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(bstr_t("WQL"),
                           bstr_t("SELECT * FROM Win32_OperatingSystem"),
                           WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                           NULL,
                           &pEnumerator);

    if (FAILED(hres)) {
        std::cout << "Query for operating system name failed."
                  << " Error code = 0x"
                  << std::hex << hres << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return "";               // Program has failed.
    }

    IWbemClassObject *pclsObj;
    ULONG uReturn = 0;

    while (pEnumerator) {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
                                       &pclsObj, &uReturn);
        if(0 == uReturn)
            break;

        VARIANT vtProp;
        hr = pclsObj->Get(L"Caption", 0, &vtProp, 0, 0);
        procName = _com_util::ConvertBSTRToString(vtProp.bstrVal);

        VariantClear(&vtProp);
        pclsObj->Release();
    }
    pSvc->Release();
    pLoc->Release();
    //pEnumerator->Release();
    //pclsObj->Release();
    CoUninitialize();
    CoUninitialize();
    CoUninitialize();

    return procName;
}

std::string processorName()
{
    HRESULT hres;
    std::string procName = "";

    CoUninitialize();
    hres =  CoInitializeEx(0, COINIT_MULTITHREADED);
    hres =  CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres)) {
        std::cout << "Failed to initialize COM library. Error code = 0x"
                  << std::hex << hres << std::endl;
        return "";                  // Program has failed.
    }

    hres =  CoInitializeSecurity(NULL,
                                 -1,                          // COM authentication
                                 NULL,                        // Authentication services
                                 NULL,                        // Reserved
                                 RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
                                 RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
                                 NULL,                        // Authentication info
                                 EOAC_NONE,                   // Additional capabilities
                                 NULL                         // Reserved
                                 );

    if (FAILED(hres)) {
        std::cout << "Failed to initialize security. Error code = 0x"
                  << std::hex << hres << std::endl;
        CoUninitialize();
        return "";                    // Program has failed.
    }

    IWbemLocator *pLoc = NULL;

    hres = CoCreateInstance(CLSID_WbemLocator,
                            0,
                            CLSCTX_INPROC_SERVER,
                            IID_IWbemLocator, (LPVOID *) &pLoc);

    if (FAILED(hres)) {
        std::cout << "Failed to create IWbemLocator object."
                  << " Err code = 0x"
                  << std::hex << hres << std::endl;
        CoUninitialize();
        return "";                 // Program has failed.
    }

    IWbemServices *pSvc = NULL;

    hres = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
                               NULL,                    // User name. NULL = current user
                               NULL,                    // User password. NULL = current
                               0,                       // Locale. NULL indicates current
                               NULL,                    // Security flags.
                               0,                       // Authority (e.g. Kerberos)
                               0,                       // Context object
                               &pSvc                    // pointer to IWbemServices proxy
                               );

    if (FAILED(hres)) {
        std::cout << "Could not connect. Error code = 0x"
                  << std::hex << hres << std::endl;
        pLoc->Release();
        CoUninitialize();
        return "";                // Program has failed.
    }

    std::cout << "Connected to ROOT\\CIMV2 WMI namespace" << std::endl;

    hres = CoSetProxyBlanket(pSvc,                        // Indicates the proxy to set
                             RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
                             RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
                             NULL,                        // Server principal name
                             RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
                             RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
                             NULL,                        // client identity
                             EOAC_NONE                    // proxy capabilities
                             );

    if (FAILED(hres)) {
        std::cout << "Could not set proxy blanket. Error code = 0x"
                  << std::hex << hres << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return "";               // Program has failed.
    }

    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(bstr_t("WQL"),
                           bstr_t("SELECT * FROM Win32_Processor"),
                           WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                           NULL,
                           &pEnumerator);

    if (FAILED(hres)) {
        std::cout << "Query for operating system name failed."
                  << " Error code = 0x"
                  << std::hex << hres << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return "";               // Program has failed.
    }

    IWbemClassObject *pclsObj;
    ULONG uReturn = 0;

    while (pEnumerator) {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
                                       &pclsObj, &uReturn);

        if(0 == uReturn)
            break;

        VARIANT vtProp;
        hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
        procName = _com_util::ConvertBSTRToString(vtProp.bstrVal);

        VariantClear(&vtProp);
        pclsObj->Release();
    }

    pSvc->Release();
    pLoc->Release();
    //pEnumerator->Release();
    //pclsObj->Release();
    CoUninitialize();
    CoUninitialize();
    CoUninitialize();

    return procName;
}

std::string videoName()
{
    HRESULT hres;
    std::string procName = "";

    CoUninitialize();
    hres =  CoInitializeEx(0, COINIT_MULTITHREADED);
    hres =  CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres)) {
        std::cout << "Failed to initialize COM library. Error code = 0x"
                  << std::hex << hres << std::endl;
        return "";                  // Program has failed.
    }

    hres =  CoInitializeSecurity(NULL,
                                 -1,                          // COM authentication
                                 NULL,                        // Authentication services
                                 NULL,                        // Reserved
                                 RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
                                 RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
                                 NULL,                        // Authentication info
                                 EOAC_NONE,                   // Additional capabilities
                                 NULL                         // Reserved
                                 );

    if (FAILED(hres)) {
        std::cout << "Failed to initialize security. Error code = 0x"
                  << std::hex << hres << std::endl;
        CoUninitialize();
        return "";                    // Program has failed.
    }

    IWbemLocator *pLoc = NULL;

    hres = CoCreateInstance(CLSID_WbemLocator,
                            0,
                            CLSCTX_INPROC_SERVER,
                            IID_IWbemLocator, (LPVOID *) &pLoc);

    if (FAILED(hres)) {
        std::cout << "Failed to create IWbemLocator object."
                  << " Err code = 0x"
                  << std::hex << hres << std::endl;
        CoUninitialize();
        return "";                 // Program has failed.
    }

    IWbemServices *pSvc = NULL;

    hres = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
                               NULL,                    // User name. NULL = current user
                               NULL,                    // User password. NULL = current
                               0,                       // Locale. NULL indicates current
                               NULL,                    // Security flags.
                               0,                       // Authority (e.g. Kerberos)
                               0,                       // Context object
                               &pSvc                    // pointer to IWbemServices proxy
                               );

    if (FAILED(hres)) {
        std::cout << "Could not connect. Error code = 0x"
                  << std::hex << hres << std::endl;
        pLoc->Release();
        CoUninitialize();
        return "";                // Program has failed.
    }

    std::cout << "Connected to ROOT\\CIMV2 WMI namespace" << std::endl;

    hres = CoSetProxyBlanket(pSvc,                        // Indicates the proxy to set
                             RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
                             RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
                             NULL,                        // Server principal name
                             RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
                             RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
                             NULL,                        // client identity
                             EOAC_NONE                    // proxy capabilities
                             );

    if (FAILED(hres)) {
        std::cout << "Could not set proxy blanket. Error code = 0x"
                  << std::hex << hres << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return "";               // Program has failed.
    }

    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(bstr_t("WQL"),
                           bstr_t("SELECT * FROM Win32_VideoController"),
                           WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                           NULL,
                           &pEnumerator);

    if (FAILED(hres)) {
        std::cout << "Query for operating system name failed."
                  << " Error code = 0x"
                  << std::hex << hres << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return "";               // Program has failed.
    }

    IWbemClassObject *pclsObj;
    ULONG uReturn = 0;

    while (pEnumerator) {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
                                       &pclsObj, &uReturn);

        if(0 == uReturn)
            break;

        VARIANT vtProp;
        hr = pclsObj->Get(L"Caption", 0, &vtProp, 0, 0);
        procName = _com_util::ConvertBSTRToString(vtProp.bstrVal);

        VariantClear(&vtProp);
        pclsObj->Release();
    }

    pSvc->Release();
    pLoc->Release();
    //pEnumerator->Release();
    //pclsObj->Release();
    CoUninitialize();
    CoUninitialize();
    CoUninitialize();

    return procName;
}

double ramSize()
{
    HRESULT hres;
    double memorySize = 0.0;

    CoUninitialize();
    hres =  CoInitializeEx(0, COINIT_MULTITHREADED);
    hres =  CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres)) {
        std::cout << "Failed to initialize COM library. Error code = 0x"
                  << std::hex << hres << std::endl;
        return 0.0;                  // Program has failed.
    }

    hres =  CoInitializeSecurity(NULL,
                                 -1,                          // COM authentication
                                 NULL,                        // Authentication services
                                 NULL,                        // Reserved
                                 RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
                                 RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
                                 NULL,                        // Authentication info
                                 EOAC_NONE,                   // Additional capabilities
                                 NULL                         // Reserved
                                 );

    if (FAILED(hres)) {
        std::cout << "Failed to initialize security. Error code = 0x"
                  << std::hex << hres << std::endl;
        CoUninitialize();
        return 0.0;                    // Program has failed.
    }

    IWbemLocator *pLoc = NULL;

    hres = CoCreateInstance(CLSID_WbemLocator,
                            0,
                            CLSCTX_INPROC_SERVER,
                            IID_IWbemLocator, (LPVOID *) &pLoc);

    if (FAILED(hres)) {
        std::cout << "Failed to create IWbemLocator object."
                  << " Err code = 0x"
                  << std::hex << hres << std::endl;
        CoUninitialize();
        return 0.0;                 // Program has failed.
    }

    IWbemServices *pSvc = NULL;

    hres = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
                               NULL,                    // User name. NULL = current user
                               NULL,                    // User password. NULL = current
                               0,                       // Locale. NULL indicates current
                               NULL,                    // Security flags.
                               0,                       // Authority (e.g. Kerberos)
                               0,                       // Context object
                               &pSvc                    // pointer to IWbemServices proxy
                               );

    if (FAILED(hres)) {
        std::cout << "Could not connect. Error code = 0x"
                  << std::hex << hres << std::endl;
        pLoc->Release();
        CoUninitialize();
        return 0.0;                // Program has failed.
    }

    std::cout << "Connected to ROOT\\CIMV2 WMI namespace" << std::endl;

    hres = CoSetProxyBlanket(pSvc,                        // Indicates the proxy to set
                             RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
                             RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
                             NULL,                        // Server principal name
                             RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
                             RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
                             NULL,                        // client identity
                             EOAC_NONE                    // proxy capabilities
                             );

    if (FAILED(hres)) {
        std::cout << "Could not set proxy blanket. Error code = 0x"
                  << std::hex << hres << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 0.0;               // Program has failed.
    }

    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(bstr_t("WQL"),
                           bstr_t("SELECT * FROM Win32_PhysicalMemory"),
                           WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                           NULL,
                           &pEnumerator);

    if (FAILED(hres)) {
        std::cout << "Query for operating system name failed."
                  << " Error code = 0x"
                  << std::hex << hres << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 0.0;               // Program has failed.
    }

    IWbemClassObject *pclsObj;
    ULONG uReturn = 0;

    while (pEnumerator) {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
                                       &pclsObj, &uReturn);

        if(0 == uReturn)
            break;

        VARIANT vtProp;
        hr = pclsObj->Get(L"Capacity", 0, &vtProp, 0, 0);
        BSTR bstr;
        bstr = vtProp.bstrVal;
        std::string sbuf(SysStringLen(bstr),0);
        std::transform(bstr,bstr+sbuf.size(),sbuf.begin(),wctob);
        double curMemory;
        curMemory = atof(sbuf.c_str());
        curMemory /= 1073741824;
        memorySize += curMemory;

        VariantClear(&vtProp);
        pclsObj->Release();
    }

    pSvc->Release();
    pLoc->Release();
    //pEnumerator->Release();
    //pclsObj->Release();
    CoUninitialize();
    CoUninitialize();
    CoUninitialize();

    return memorySize;
}
