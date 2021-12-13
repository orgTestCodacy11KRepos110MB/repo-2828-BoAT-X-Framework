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
#include "tcase_common.h"

static BoatHlchainmakerWallet* g_chaninmaker_wallet_ptr;
static BoatHlchainmakerWalletConfig wallet_config = {0};
#define USE_ONETIME_WALLET

static BOAT_RESULT chainmakerWalletPrepare(void)
{
    BOAT_RESULT index;

    //set user private key context
    wallet_config.user_prikey_cfg.prikey_genMode = BOAT_WALLET_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    wallet_config.user_prikey_cfg.prikey_type    = BOAT_WALLET_PRIKEY_TYPE_SECP256R1;
    wallet_config.user_prikey_cfg.prikey_format  = BOAT_WALLET_PRIKEY_FORMAT_PKCS;
    wallet_config.user_prikey_cfg.prikey_content.field_ptr = (BUINT8 *)chainmaker_user_key;
    wallet_config.user_prikey_cfg.prikey_content.field_len = strlen(chainmaker_user_key) + 1; 

    //set user cert context
    wallet_config.user_cert_cfg.length = strlen(chainmaker_user_cert);
    memcpy(wallet_config.user_cert_cfg.content, chainmaker_user_cert, wallet_config.user_cert_cfg.length);
    
    //set url and name
#ifdef TEST_CHAINMAKER__NODE_URL
    strncpy(wallet_config.node_url_cfg, TEST_CHAINMAKER__NODE_URL, strlen(TEST_CHAINMAKER__NODE_URL));
#else 
    strncpy(wallet_config.node_url_cfg, test_chainmaker_node_url, strlen(test_chainmaker_node_url));
#endif
    strncpy(wallet_config.host_name_cfg, test_chainmaker_host_name, strlen(test_chainmaker_host_name));

    //tls ca cert
    wallet_config.tls_ca_cert_cfg.length = strlen(chainmaker_tls_ca_cert);
    memcpy(wallet_config.tls_ca_cert_cfg.content, chainmaker_tls_ca_cert, wallet_config.tls_ca_cert_cfg.length);

    // create wallet
#if defined(USE_ONETIME_WALLET)
    index = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, NULL, &wallet_config, sizeof(BoatHlchainmakerWalletConfig));
#elif defined(USE_CREATE_PERSIST_WALLET)
    index = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, "chainmaker.cfg", &wallet_config, sizeof(BoatHlchainmakerWalletConfig));
#elif defined(USE_LOAD_PERSIST_WALLET)
    index = BoatWalletCreate(BOAT_PROTOCOL_CHAINMAKER, "chainmaker.cfg", NULL, sizeof(BoatHlchainmakerWalletConfig));
#else
    return BOAT_ERROR;
#endif
    if (index == BOAT_ERROR)
    {
        return BOAT_ERROR;
    }
    
    g_chaninmaker_wallet_ptr = BoatGetWalletByIndex(index);
    if (g_chaninmaker_wallet_ptr == NULL)
    {
        return BOAT_ERROR;
    }

    return BOAT_SUCCESS;
}


static BOAT_RESULT param_init_check(BoatHlchainmakerTx* tx_ptr)
{
    BOAT_RESULT result = BOAT_SUCCESS;

    if (tx_ptr == NULL)
    {
        return BOAT_ERROR;
    }

    result = strncmp(tx_ptr->client_para.chain_id, chain_id_str, strlen(chain_id_str));
    if (result != 0) 
    {
        return BOAT_ERROR;
    }

    result = strncmp(tx_ptr->client_para.org_id, org_id_str, strlen(org_id_str));
    if (result != 0) 
    {
        return BOAT_ERROR;
    }

    if (tx_ptr->wallet_ptr != g_chaninmaker_wallet_ptr)
    {
         return BOAT_ERROR;
    }

    return result;
}


START_TEST(test_002Param_0001TxinitSuccess) 
{
    BSINT32 rtnVal;
    BoatHlchainmakerTx    tx_ptr;
    rtnVal = chainmakerWalletPrepare();
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    rtnVal = BoatHlChainmakerTxInit(g_chaninmaker_wallet_ptr, chain_id_str, org_id_str, &tx_ptr);
    ck_assert(rtnVal == BOAT_SUCCESS);
    ck_assert(param_init_check(&tx_ptr) == BOAT_SUCCESS);

}
END_TEST

START_TEST(test_002Param_0002TxinitxFailureNullpara) 
{
    BSINT32 rtnVal;
    BoatHlchainmakerTx    tx_ptr;
    rtnVal = chainmakerWalletPrepare();
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    rtnVal = BoatHlChainmakerTxInit(NULL, chain_id_str, org_id_str, &tx_ptr);
    ck_assert(rtnVal == BOAT_ERROR_INVALID_ARGUMENT);

    rtnVal = BoatHlChainmakerTxInit(g_chaninmaker_wallet_ptr, NULL, org_id_str, &tx_ptr);
    ck_assert(rtnVal == BOAT_ERROR_INVALID_ARGUMENT);

    rtnVal = BoatHlChainmakerTxInit(g_chaninmaker_wallet_ptr, chain_id_str, NULL, &tx_ptr);
    ck_assert(rtnVal == BOAT_ERROR_INVALID_ARGUMENT);

    rtnVal = BoatHlChainmakerTxInit(g_chaninmaker_wallet_ptr, chain_id_str, org_id_str, NULL);
    ck_assert(rtnVal == BOAT_ERROR_INVALID_ARGUMENT);
}
END_TEST


START_TEST(test_002Param_0003AddTxParamSuccess) 
{
    BSINT32 rtnVal;
    BoatHlchainmakerTx tx_ptr;

    rtnVal = BoatHlchainmakerAddTxParam(&tx_ptr, 6, "key1", "vlaue1", "key2", "vlaue2", 
                                                    "key3", "vlaue3");
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
}
END_TEST


START_TEST(test_002Param_0004AddTxParamFailureShortParam) 
{
    BSINT32 rtnVal;
    BoatHlchainmakerTx tx_ptr;

    rtnVal = BoatHlchainmakerAddTxParam(&tx_ptr, 6, "key1", "vlaue1", "key2", "vlaue2", "key3");
    ck_assert_int_eq(rtnVal, BOAT_ERROR_INVALID_ARGUMENT);
}
END_TEST

START_TEST(test_002Param_0005AddTxParamFailureLongParam) 
{
    BSINT32 rtnVal;
    BoatHlchainmakerTx tx_ptr;

    rtnVal = BoatHlchainmakerAddTxParam(&tx_ptr, 12, "key1", "vlaue1", "key2", "vlaue2", "key3", "vlaue3", 
                                                     "key4", "vlaue4", "key5", "vlaue5", "key6", "vlaue6");
    ck_assert_int_eq(rtnVal, BOAT_ERROR_INVALID_ARGUMENT);
}
END_TEST

START_TEST(test_002Param_0006AddTxParamFailureOddParam) 
{
    BSINT32 rtnVal;
    BoatHlchainmakerTx tx_ptr;

    rtnVal = BoatHlchainmakerAddTxParam(&tx_ptr, 9, "key1", "vlaue1", "key2", "vlaue2", "key3", "vlaue3", 
                                                     "key4", "vlaue4", "key5");
    ck_assert_int_eq(rtnVal, BOAT_ERROR_INVALID_ARGUMENT);
}
END_TEST

START_TEST(test_002Param_0007AddTxParamSucessNumberNULLParam) 
{
    BSINT32 rtnVal;
    BoatHlchainmakerTx    tx_ptr;

    rtnVal = BoatHlchainmakerAddTxParam(&tx_ptr, 0, NULL);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
}
END_TEST

START_TEST(test_002Param_0008AddTxParamFailureTxNULLParam) 
{
    BSINT32 rtnVal;
    BoatHlchainmakerTx    tx_ptr;

    rtnVal = BoatHlchainmakerAddTxParam(NULL, 6, "key1", "vlaue1", "key2", "vlaue2", "key3", "value3");
    ck_assert_int_eq(rtnVal, BOAT_ERROR_INVALID_ARGUMENT);
}
END_TEST


Suite *make_parameters_suite(void) 
{
    /* Create Suite */
    Suite *s_param = suite_create("param");

    /* Create test cases */
    TCase *tc_param_api = tcase_create("param_api");

    /* Add a test case to the Suite */
    suite_add_tcase(s_param, tc_param_api);       
    /* Test cases are added to the test set */
    tcase_add_test(tc_param_api, test_002Param_0001TxinitSuccess);  
    tcase_add_test(tc_param_api, test_002Param_0002TxinitxFailureNullpara);  
    tcase_add_test(tc_param_api, test_002Param_0003AddTxParamSuccess);  
    tcase_add_test(tc_param_api, test_002Param_0004AddTxParamFailureShortParam);  
    tcase_add_test(tc_param_api, test_002Param_0005AddTxParamFailureLongParam); 
    tcase_add_test(tc_param_api, test_002Param_0006AddTxParamFailureOddParam);   
    tcase_add_test(tc_param_api, test_002Param_0007AddTxParamSucessNumberNULLParam);  
    tcase_add_test(tc_param_api, test_002Param_0008AddTxParamFailureTxNULLParam);  

    return s_param;
}

