#pragma once
#pragma warning(push)
#pragma warning(disable : 4820)
#pragma warning(disable : 4619)
#pragma warning(disable : 4548)
#pragma warning(disable : 4668)
#pragma warning(disable : 4365)
#pragma warning(disable : 4710)
#pragma warning(disable : 4371)
#pragma warning(disable : 4826)
#pragma warning(disable : 4061)
#pragma warning(disable : 4640)

#ifndef MORAB_STDAFX_H
#define MORAB_STDAFX_H


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

#include <nlohmann/json.hpp>

#ifdef _WIN32
#include <Windows.h>
#include <Shlwapi.h>
#endif

#if defined(unix) || defined(__APPLE__) || defined(__linux__)
#define __GENERIC_UNIX__
#include <unistd.h>
#include <sys/stat.h>
#endif

#if defined(__APPLE__)
#include <sys/syslimits.h>
constexpr const size_t max_path = PATH_MAX;
#elif defined(__linux__)
#include <linux/limits.h>
constexpr const size_t max_path = PATH_MAX;
#elif defined(_WIN32)
constexpr const size_t max_path = MAX_PATH;  // From Windows.h
#endif

// cURLpp
#include "curlpp/cURLpp.hpp"
#include "curlpp/Easy.hpp"
#include "curlpp/Options.hpp"

// LibXml
#include "libxml/HTMLparser.h"
#include "libxml/tree.h"

// Thread Pool
#define THREAD_POOL_NAMESPACE_NAME takeout
#include "thread_pool/thread_pool.hpp"

#endif // !MORAB_STDAFX_H

#pragma warning(pop)
