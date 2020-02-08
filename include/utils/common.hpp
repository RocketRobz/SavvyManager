#pragma once

#include <3ds.h>

#ifdef __cplusplus
extern "C" {
#endif
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifdef __cplusplus
}

#include "gfx.hpp"
#include "gui.hpp"
#include "screenCommon.hpp"

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <curl/curl.h>

#endif

extern char * arg0;

#define WORKING_DIR       "/3ds/"

#define HBL_FILE_NAME     APP_TITLE  ".3dsx"
#define HBL_FILE_PATH     WORKING_DIR  "/"  HBL_FILE_NAME

