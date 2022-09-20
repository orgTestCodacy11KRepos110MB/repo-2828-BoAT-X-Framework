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
#include "tcase_platon.h"
#define EXCEED_STR_MAX_LEN 4097

#define TEST_EIP155_COMPATIBILITY   BOAT_TRUE
#define TEST_PLATON_CHAIN_ID        210309
#define TEST_GAS_LIMIT              "0x6691B7"
#define TEST_GAS_PRICE              "0x4A817C800"
#define TEST_IS_SYNC_TX             BOAT_TRUE
#define TEST_RECIPIENT_ADDRESS      "lat159js9hw63x2y8m65mhgprm3eu9f4c7xmv3lym4"

extern BoatPlatONWalletConfig get_platon_wallet_settings();

START_TEST(test_006GetBalance_0001GetSuccess) 
{
    BOAT_RESULT result;
    BoatPlatONTx tx_ctx;
    BCHAR *cur_balance_wei = NULL;
    BoatFieldVariable parse_result = {NULL, 0};

    BoatIotSdkInit();

    result = platonWalletPrepare();
    ck_assert(result == BOAT_SUCCESS);

    result = BoatPlatONTxInit(g_platon_wallet_ptr, &tx_ctx, BOAT_TRUE, NULL,
                           "0x333333",
                           (BCHAR *)TEST_RECIPIENT_ADDRESS);
    ck_assert_int_eq(result, BOAT_SUCCESS);


    cur_balance_wei = BoatPlatONWalletGetBalance(g_platon_wallet_ptr, TEST_RECIPIENT_ADDRESS);
	result          = BoatPlatONParseRpcResponseStringResult(cur_balance_wei, &parse_result);

    ck_assert_int_eq(result, BOAT_SUCCESS);
    ck_assert_ptr_ne(parse_result.field_ptr, NULL);

    BoatFree(parse_result.field_ptr);

    BoatIotSdkDeInit();
}
END_TEST

Suite *make_transactions_suite(void)
{
    /* Create Suite */
    Suite *s_transaction = suite_create("transaction");


    /* Create test cases */
    TCase *tc_transaction_api = tcase_create("transaction_api");


    /* Add a test case to the Suite */
    suite_add_tcase(s_transaction, tc_transaction_api);      
 
    /* Test cases are added to the test set */
    tcase_add_test(tc_transaction_api, test_006GetBalance_0001GetSuccess); 
    
    return s_transaction;
}
