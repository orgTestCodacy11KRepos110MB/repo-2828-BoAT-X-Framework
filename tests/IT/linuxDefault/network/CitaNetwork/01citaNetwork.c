/*
 * @Description:
 * @Author: aitos
 * @Date: 2022-09-04 11:28:58
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-09-13 14:37:50
 */
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
#include "tcase_network.h"
#include "boatiotsdk.h"
#include "boatlog.h"

/**
 * test node url
 */

const BCHAR *demoUrl_1 = "http://127.0.0.1:1337";
const BCHAR *demoUrl_2 = "http://127.0.0.2:1337";

const BCHAR *chainID = "0x1";
const BUINT8 gBinChainId[32] = {
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x01};
const BUINT32 version = 2;

/**
****************************************************************************************
* @brief:
*  This function get the configuration to create cita network;
* @param[in/out] *config
*  output of the configuration
* @param[in] version
* @param[in] *chainid
* @param[in] *url
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
BOAT_RESULT getCitaNetworkConfig(BoatCitaNetworkConfig *config, BUINT32 version, BCHAR *chainid, BCHAR *url)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    /* check param*/
    if ((NULL == config) || NULL == url || NULL == chainid)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    if ((strlen(url) + 1) > sizeof(config->node_url_buf))
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    BUINT8 binChainId[32] = {0};
    BUINT32 bin_len;
    memset(config->chain_id_buf, 0x00, sizeof(config->chain_id_buf));
    bin_len = UtilityHexToBin(binChainId, 32, chainid, TRIMBIN_TRIM_NO, BOAT_FALSE);
    if (bin_len > 0)
    {
        memcpy(&config->chain_id_buf[32 - bin_len], binChainId, bin_len);
    }
    config->version = version;
    strcpy(config->node_url_buf, url);
    return result;
}

/**
****************************************************************************************
* @brief:
*  This function test case : create onetime cita network success
* @return
****************************************************************************************
*/
START_TEST(test_001citaNetwork_0001CreateOneTimeNetworkSuccess)
{
    BSINT32 rtnVal;
    BoatCitaNetworkConfig network_config;
    BoatCitaNetworkData networkdata;
    /* get configuration */
    rtnVal = getCitaNetworkConfig(&network_config, version, chainID, demoUrl_1);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create onetime network , store type must be RAM */
    rtnVal = BoatCitaNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
    /* check index of onetime network , the index must equal 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* get network data by index , index = 0 */
    rtnVal = BoATCita_GetNetworkByIndex(&networkdata, 0);
    /* check the result of getting network data */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check index in the struct ,must be 0 */
    ck_assert_int_eq(networkdata.networkIndex, 0);
    /* check chainId in the struct , must be same to the configuration */
    ck_assert_int_eq(memcmp(networkdata.chain_id_buf, network_config.chain_id_buf, sizeof(network_config.chain_id_buf)), 0);
    /* check version */
    ck_assert(networkdata.version == version);
    /* check url in the struct , must be same to the configuration */
    ck_assert_int_eq(strcmp(networkdata.node_url_buf, demoUrl_1), 0);
    /* delete the onetime networl */
    BoATCitaNetworkDelete(0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create persist cita network success
*  index of persist netsorks is from 1 to BOAT_MAX_NETWORK_NUM
* @return
****************************************************************************************
*/
START_TEST(test_001citaNetwork_0002CreatePersistNetworkSuccess)
{
    BSINT32 rtnVal;
    BoatCitaNetworkConfig network_config;
    BoatCitaNetworkData networkdata;
    /* get configuration */
    rtnVal = getCitaNetworkConfig(&network_config, version, chainID, demoUrl_2);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoatCitaNetworkCreate(&network_config, BOAT_STORE_TYPE_FLASH);
    /* check index of persist network ,this is the first persist network ,
        so the index must be 1 */
    ck_assert_int_eq(rtnVal, 1);
    /* get network data by index , index = 1 */
    rtnVal = BoATCita_GetNetworkByIndex(&networkdata, 1);
    /* check the result of getting network data */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check index in the struct ,must be 1 */
    ck_assert_int_eq(networkdata.networkIndex, 1);
    /* check chainId in the struct , must be same to the configuration */
    ck_assert_int_eq(memcmp(networkdata.chain_id_buf, network_config.chain_id_buf, sizeof(network_config.chain_id_buf)), 0);
    /* check version */
    ck_assert(networkdata.version == version);
    /* check url in the struct , must be same to the configuration */
    ck_assert_int_eq(strcmp(networkdata.node_url_buf, demoUrl_2), 0);
    /* delete the persist network */
    BoATCitaNetworkDelete(1);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create persist cita network fail that configuration is NULL
* @return
****************************************************************************************
*/
START_TEST(test_001citaNetwork_0003CreatePersistNetworkFail_ParamNULL)
{
    BSINT32 rtnVal;
    BoatCitaNetworkData networkdata;
    /* create persist network , store type must be FLASH */
    rtnVal = BoatCitaNetworkCreate(NULL, BOAT_STORE_TYPE_FLASH);
    /* check result*/
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : delete onetime cita network success
* @return
****************************************************************************************
*/
START_TEST(test_001citaNetwork_0004DeleteOneTimeNetworkSuccess)
{
    BSINT32 rtnVal;
    BoatCitaNetworkConfig network_config;
    BoatCitaNetworkData networkdata;
    /* get configuration */
    rtnVal = getCitaNetworkConfig(&network_config, version, chainID, demoUrl_1);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create onetime network , store type must be RAM */
    rtnVal = BoatCitaNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
    /* check index of onetime network , the index must equal 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* delete the onetime network */
    rtnVal = BoATCitaNetworkDelete(0);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* get the onetime network , index of onetime network must be 0  */
    rtnVal = BoATCita_GetNetworkByIndex(&networkdata, 0);
    /* check the result of getting network data ,
        the onetime network has been deleted ,so return error */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : delete persist cita network success
* @return
****************************************************************************************
*/
START_TEST(test_001citaNetwork_0005DeletePersistNetworkSuccess)
{
    BSINT32 rtnVal;
    BoatCitaNetworkConfig network_config;
    BoatCitaNetworkData networkdata;
    /* get configuration */
    rtnVal = getCitaNetworkConfig(&network_config, version, chainID, demoUrl_1);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoatCitaNetworkCreate(&network_config, BOAT_STORE_TYPE_FLASH);
    /* check index of persist network , the index from 1 to BOAT_MAX_NETWORK_NUM
        this is the first persist network ,so the index must be 1 */
    ck_assert_int_eq(rtnVal, 1);
    /* delete the persist network */
    rtnVal = BoATCitaNetworkDelete(1);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* get the persist network , index of persist network must be 1  */
    rtnVal = BoATCita_GetNetworkByIndex(&networkdata, 1);
    /* check the result of getting network data ,
        the persist network has been deleted ,so return error */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : delete cita network;
*  There is no network, return error;
* @return
****************************************************************************************
*/
START_TEST(test_001citaNetwork_0006DeleteNetwork_NoNetwork)
{
    BSINT32 rtnVal;
    for (size_t i = 0; i <= BOAT_MAX_NETWORK_NUM; i++)
    {
        /* delete the network  */
        rtnVal = BoATCitaNetworkDelete(i);
        /* have no network ,so return error */
        ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : delete cita network;
*  index of network from 0 to BOAT_MAX_NETWORK_NUM;
*  If the index exceeds to BOAT_MAX_NETWORK_NUM, boat return error;
* @return
****************************************************************************************
*/
START_TEST(test_001citaNetwork_0007DeleteNetwork_IndexExceed)
{
    BSINT32 rtnVal;

    /* delete the network  */
    rtnVal = BoATCitaNetworkDelete(BOAT_MAX_NETWORK_NUM + 1);
    /* index exceeds to BOAT_MAX_NETWORK_NUM ,so return error */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INDEX_EXCEED);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get network list with NULL param;
* @return
****************************************************************************************
*/
START_TEST(test_001citaNetwork_0008GetNetworkList_ParamNULL)
{
    BSINT32 rtnVal;

    /* get the network list */
    rtnVal = BoATCita_GetNetworkList(NULL);
    /* check the result */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get network list with no network;
* @return
****************************************************************************************
*/
START_TEST(test_001citaNetwork_0009GetNetworkList_NONetwork)
{
    BSINT32 rtnVal;
    BoatCitaNetworkContext networkList;
    /* get the network list */
    rtnVal = BoATCita_GetNetworkList(&networkList);
    /* check the result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check num of network ;
        have no network and the num must be 0*/
    ck_assert_int_eq(networkList.networkNum, 0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get network list with only one onetime network;
* @return
****************************************************************************************
*/
START_TEST(test_001citaNetwork_0010GetNetworkList_OnlyOnetimeNetwork)
{
    BSINT32 rtnVal;
    BoatCitaNetworkContext networkList;
    BoatCitaNetworkConfig network_config;
    /* creat onetime network */
    /* get configuration */
    rtnVal = getCitaNetworkConfig(&network_config, version, chainID, demoUrl_1);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create onetime network , store type must be RAM */
    rtnVal = BoatCitaNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
    /* check index of onetime network , the index must equal 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* get the network list */
    rtnVal = BoATCita_GetNetworkList(&networkList);
    /* check the result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check num of network ; only one network , num of network must be 1*/
    ck_assert_int_eq(networkList.networkNum, 1);
    /* check index of onetime network in the list , index of onetime must 0 */
    ck_assert_int_eq(networkList.networks[0].networkIndex, 0);
    /* check chainId in the list ,must be same to cteated */
    ck_assert_int_eq(memcmp(networkList.networks[0].chain_id_buf, network_config.chain_id_buf, sizeof(network_config.chain_id_buf)), 0);
    /* check version , must be same to created */
    ck_assert(networkList.networks[0].version == version);
    /* check url ,must be same to created */
    ck_assert_int_eq(strcmp(networkList.networks[0].node_url_buf, demoUrl_1), 0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get network list with only a onetime network and a persist network;
* @return
****************************************************************************************
*/
START_TEST(test_001citaNetwork_0011GetNetworkList_OnetimeAndPersistNetwork)
{
    BSINT32 rtnVal;
    BoatCitaNetworkContext networkList;
    BoatCitaNetworkConfig network_config;
    /* creat onetime network */
    /* get configuration */
    rtnVal = getCitaNetworkConfig(&network_config, version, chainID, demoUrl_1);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create onetime network , store type must be RAM */
    rtnVal = BoatCitaNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
    /* check index of onetime network , the index must equal 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* creat a persist network*/
    /* get configuration */
    rtnVal = getCitaNetworkConfig(&network_config, version, chainID, demoUrl_2);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoatCitaNetworkCreate(&network_config, BOAT_STORE_TYPE_FLASH);
    /* check index of persist network ;
        index of persist networks is form 1 to BOAT_MAX_NETWORK_NUM,
        this is the first persist network, the index must equal 1 */
    ck_assert_int_eq(rtnVal, 1);
    /* get the network list */
    rtnVal = BoATCita_GetNetworkList(&networkList);
    /* check the result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check num of network ; a onetime network and a persist network , num of network must be 2*/
    ck_assert_int_eq(networkList.networkNum, 2);
    /* read persist networks and then read the onetime network ;
        there are a onetime network and a persist network ,so list[0] is persist network , list[1] is onetime network*/
    /* check index of persist network in the list , index of persist netwoek must 1 */
    ck_assert_int_eq(networkList.networks[0].networkIndex, 1);
    /* check chainId in the list ,must be same to cteated */
    ck_assert_int_eq(memcmp(networkList.networks[0].chain_id_buf, gBinChainId, sizeof(gBinChainId)), 0);
    /* check version , must be same to created */
    ck_assert(networkList.networks[0].version == version);
    /* check url ,must be same to created */
    ck_assert_int_eq(strcmp(networkList.networks[0].node_url_buf, demoUrl_2), 0);
    /* check index of onetime network in the list , index of onetime netwoek must 0 */
    ck_assert_int_eq(networkList.networks[1].networkIndex, 0);
    /* check chainId in the list ,must be same to cteated */
    ck_assert_int_eq(memcmp(networkList.networks[1].chain_id_buf, gBinChainId, sizeof(gBinChainId)), 0);
    /* check version , must be same to created */
    ck_assert(networkList.networks[1].version == version);
    /* check url ,must be same to created */
    ck_assert_int_eq(strcmp(networkList.networks[1].node_url_buf, demoUrl_1), 0);
    /* delete all the networks*/
    BoATCitaNetworkDelete(0);
    BoATCitaNetworkDelete(1);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get network data by index with NULL param;
* @return
****************************************************************************************
*/
START_TEST(test_001citaNetwork_0012GetNetworkByIndex_ParamNULL)
{
    BSINT32 rtnVal;
    /* get network data by index , the first param set NULL */
    rtnVal = BoATCita_GetNetworkByIndex(NULL, 0);
    /* check result*/
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get network data by index with exceeded index;
* @return
****************************************************************************************
*/
START_TEST(test_001citaNetwork_0013GetNetworkByIndex_IndexExceed)
{
    BSINT32 rtnVal;
    BoatCitaNetworkData networkdata;
    /* get network data by index , but index bigger than BOAT_MAX_NETWORK_NUM */
    rtnVal = BoATCita_GetNetworkByIndex(&networkdata, BOAT_MAX_NETWORK_NUM + 1);
    /* check result*/
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INDEX_EXCEED);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get onetime network data by index;
* @return
****************************************************************************************
*/
START_TEST(test_001citaNetwork_0014GetNetworkByIndex_Onetime)
{
    BSINT32 rtnVal;
    BoatCitaNetworkData networkdata;
    BoatCitaNetworkConfig network_config;
    /* creat onetime network */
    /* get configuration */
    rtnVal = getCitaNetworkConfig(&network_config, version, chainID, demoUrl_1);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create onetime network , store type must be RAM */
    rtnVal = BoatCitaNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
    /* check index of onetime network , the index must equal 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* creat a persist network*/
    /* get configuration */
    rtnVal = getCitaNetworkConfig(&network_config, version, chainID, demoUrl_2);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoatCitaNetworkCreate(&network_config, BOAT_STORE_TYPE_FLASH);
    /* check index of persist network ;
        index of persist networks is form 1 to BOAT_MAX_NETWORK_NUM,
        this is the first persist network, the index must equal 1 */
    ck_assert_int_eq(rtnVal, 1);
    /* get network data by index , index of onetime network must be 0 */
    rtnVal = BoATCita_GetNetworkByIndex(&networkdata, 0);
    /* check result*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check index of the onetime network in the struct , index of onetime netwoek must 0 */
    ck_assert_int_eq(networkdata.networkIndex, 0);
    /* check chainId in the list ,must be same to cteated */
    ck_assert_int_eq(memcmp(networkdata.chain_id_buf, gBinChainId, sizeof(gBinChainId)), 0);
    /* check version , must be same to created */
    ck_assert(networkdata.version == version);
    /* check url ,must be same to created */
    ck_assert_int_eq(strcmp(networkdata.node_url_buf, demoUrl_1), 0);
    /* delete all the networks*/
    BoATCitaNetworkDelete(0);
    BoATCitaNetworkDelete(1);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get persist network data by index;
* @return
****************************************************************************************
*/
START_TEST(test_001citaNetwork_0015GetNetworkByIndex_Persist)
{
    BSINT32 rtnVal;
    BoatCitaNetworkData networkdata;
    BoatCitaNetworkConfig network_config;
    /* creat onetime network */
    /* get configuration */
    rtnVal = getCitaNetworkConfig(&network_config, version, chainID, demoUrl_1);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create onetime network , store type must be RAM */
    rtnVal = BoatCitaNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
    /* check index of onetime network , the index must equal 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* creat a persist network*/
    /* get configuration */
    rtnVal = getCitaNetworkConfig(&network_config, version, chainID, demoUrl_2);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoatCitaNetworkCreate(&network_config, BOAT_STORE_TYPE_FLASH);
    /* check index of persist network ;
        index of persist networks is form 1 to BOAT_MAX_NETWORK_NUM,
        this is the first persist network, the index must equal 1 */
    ck_assert_int_eq(rtnVal, 1);
    /* get network data by index , index of persist network must be 1 */
    rtnVal = BoATCita_GetNetworkByIndex(&networkdata, 1);
    /* check result*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check index of the persist network in the struct , index of persist netwoek must 1 */
    ck_assert_int_eq(networkdata.networkIndex, 1);
    /* check chainId in the list ,must be same to cteated */
    ck_assert_int_eq(memcmp(networkdata.chain_id_buf, gBinChainId, sizeof(gBinChainId)), 0);
    /* check version , must be same to created */
    ck_assert(networkdata.version == version);
    /* check url ,must be same to created */
    ck_assert_int_eq(strcmp(networkdata.node_url_buf, demoUrl_2), 0);
    /* delete all the networks*/
    BoATCitaNetworkDelete(0);
    BoATCitaNetworkDelete(1);
}
END_TEST

Suite *make_citaNetworkIntfTest_suite(void)
{
    /* Create Suite */
    Suite *s_networkcreate = suite_create("networkCreat");

    /* Create test cases */
    TCase *tc_networkCreat_api = tcase_create("networkCreat_api");

    /* Add a test case to the Suite */
    suite_add_tcase(s_networkcreate, tc_networkCreat_api);
    /* Test cases are added to the test set */
    tcase_add_test(tc_networkCreat_api, test_001citaNetwork_0001CreateOneTimeNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_001citaNetwork_0002CreatePersistNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_001citaNetwork_0003CreatePersistNetworkFail_ParamNULL);
    tcase_add_test(tc_networkCreat_api, test_001citaNetwork_0004DeleteOneTimeNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_001citaNetwork_0005DeletePersistNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_001citaNetwork_0006DeleteNetwork_NoNetwork);
    tcase_add_test(tc_networkCreat_api, test_001citaNetwork_0007DeleteNetwork_IndexExceed);
    tcase_add_test(tc_networkCreat_api, test_001citaNetwork_0008GetNetworkList_ParamNULL);
    tcase_add_test(tc_networkCreat_api, test_001citaNetwork_0009GetNetworkList_NONetwork);
    tcase_add_test(tc_networkCreat_api, test_001citaNetwork_0010GetNetworkList_OnlyOnetimeNetwork);
    tcase_add_test(tc_networkCreat_api, test_001citaNetwork_0011GetNetworkList_OnetimeAndPersistNetwork);
    tcase_add_test(tc_networkCreat_api, test_001citaNetwork_0012GetNetworkByIndex_ParamNULL);
    tcase_add_test(tc_networkCreat_api, test_001citaNetwork_0013GetNetworkByIndex_IndexExceed);
    tcase_add_test(tc_networkCreat_api, test_001citaNetwork_0014GetNetworkByIndex_Onetime);
    tcase_add_test(tc_networkCreat_api, test_001citaNetwork_0015GetNetworkByIndex_Persist);

    return s_networkcreate;
}