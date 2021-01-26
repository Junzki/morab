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

#ifdef WIN32
#include <Windows.h>
#include <Shlwapi.h>
#include "wgetopt.hpp"
#endif

#if defined(unix) || defined(__APPLE__) || defined(__linux__)
#define __GENERIC_UNIX__
#include <unistd.h>
#endif

// cURLpp
#include "curlpp/cURLpp.hpp"
#include "curlpp/Easy.hpp"
#include "curlpp/Options.hpp"

// LibXml
#include "libxml/HTMLparser.h"
#include "libxml/tree.h"

#endif // !MORAB_STDAFX_H

#pragma warning(pop)
