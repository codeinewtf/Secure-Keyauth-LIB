#pragma once
#pragma comment (lib, "Auth_Integrity_Check.lib")
#pragma comment (lib, "Secure_Memory.lib")
#include "Spoofing.h"
#include "xorstr.hpp"
#include "skStr.h"
#include <Windows.h>
#include <iostream>
#include <nlohmann/json.hpp>

#pragma comment (lib, "advapi32.lib")
#pragma comment(lib, "libcurl.lib")
#pragma comment(lib, "Normaliz.lib")
#pragma comment(lib, "wldap32.lib" )
#pragma comment(lib, "crypt32.lib" )
#pragma comment(lib, "Ws2_32.lib")

#define CURL_STATICLIB 
#include <curl/curl.h>

#include "utils.hpp"