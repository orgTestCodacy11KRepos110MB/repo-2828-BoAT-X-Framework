/******************************************************************************
 * Copyright (C) 2018-2021 aitos.io
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#ifndef __TCASE_PLATONE_H__
#define __TCASE_PLATONE_H__

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "check.h"
#include "boatlog.h"
#include "boatiotsdk.h"
#include "boatiotsdk.h"

extern BOAT_RESULT platone_createKeypair(BCHAR * keypairName, BBOOL is_onetime, BUINT8* keypairIndex_ptr);
extern BOAT_RESULT platone_createNetwork(BBOOL is_onetime, BUINT8* networkIndex);
extern BoatPlatoneWallet *platone_get_wallet_ptr();

#endif