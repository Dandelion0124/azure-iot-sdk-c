// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifdef __cplusplus
#include <cstdlib>
#include <cstdint>
#include <cstddef>
#else
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#endif

static void* my_gballoc_malloc(size_t size)
{
    return malloc(size);
}

static void my_gballoc_free(void* ptr)
{
    free(ptr);
}

#include "testrunnerswitcher.h"
#include "umock_c.h"
#include "umocktypes_charptr.h"
#include "umocktypes_stdint.h"
#include "umock_c_negative_tests.h"
#include "azure_c_shared_utility/macro_utils.h"

#include "RIoT.h"
#include "RiotCrypt.h"
#include "RiotDerEnc.h"
#include "RiotX509Bldr.h"
#include "DiceSha256.h"

#define ENABLE_MOCKS
#include "azure_c_shared_utility/gballoc.h"
#include "azure_c_shared_utility/umock_c_prod.h"
#include "azure_c_shared_utility/crt_abstractions.h"

MOCKABLE_FUNCTION(, void, DiceSHA256, const uint8_t*, buf, size_t, bufSize, uint8_t*, digest);
MOCKABLE_FUNCTION(, void, DiceSHA256_2, const uint8_t*, buf1, size_t, bufSize1, const uint8_t*, buf2, size_t, bufSize2, uint8_t*, digest);
MOCKABLE_FUNCTION(, RIOT_STATUS, RiotCrypt_Hash, uint8_t*, hash_result, size_t, resultSize, const void*, data, size_t, dataSize);
MOCKABLE_FUNCTION(, RIOT_STATUS, RiotCrypt_DeriveEccKey, RIOT_ECC_PUBLIC*, publicPart, RIOT_ECC_PRIVATE*, privatePart, const void*, srcData, size_t, srcDataSize, const uint8_t*, label, size_t, labelSize);
MOCKABLE_FUNCTION(, RIOT_STATUS, RiotCrypt_Hash2, uint8_t*, hash_result, size_t, resultSize, const void*, data1, size_t, data1Size, const void*, data2, size_t, data2Size);
MOCKABLE_FUNCTION(, void, DERInitContext, DERBuilderContext*, Context, uint8_t*, Buffer, uint32_t, Length);
MOCKABLE_FUNCTION(, int, X509GetDEREncodedTBS, DERBuilderContext*, Tbs, RIOT_X509_TBS_DATA*, TbsData, RIOT_ECC_PUBLIC*, AliasKeyPub, RIOT_ECC_PUBLIC*, DevIdKeyPub, uint8_t*, Fwid, uint32_t, FwidLen);
MOCKABLE_FUNCTION(, RIOT_STATUS, RiotCrypt_Sign, RIOT_ECC_SIGNATURE*, sig, const void*, data, size_t, dataSize, const RIOT_ECC_PRIVATE*, key);
MOCKABLE_FUNCTION(, int, X509MakeAliasCert, DERBuilderContext*, AliasCert, RIOT_ECC_SIGNATURE*, TbsSig);
MOCKABLE_FUNCTION(, int, X509MakeRootCert, DERBuilderContext*, RootCert, RIOT_ECC_SIGNATURE*, TbsSig);

MOCKABLE_FUNCTION(, int, X509GetDEREcc, DERBuilderContext*, Context, RIOT_ECC_PUBLIC, Pub, RIOT_ECC_PRIVATE, Priv);
MOCKABLE_FUNCTION(, int, DERtoPEM, DERBuilderContext*, Context, uint32_t, Type, char*, PEM, uint32_t*, Length);

MOCKABLE_FUNCTION(, int, X509GetDEREccPub, DERBuilderContext*, Context, RIOT_ECC_PUBLIC, Pub);
MOCKABLE_FUNCTION(, int, X509GetDeviceCertTBS, DERBuilderContext*, Tbs, RIOT_X509_TBS_DATA *, TbsData, RIOT_ECC_PUBLIC*, DevIdKeyPub);
MOCKABLE_FUNCTION(, int, X509MakeDeviceCert, DERBuilderContext*, DeviceIDCert, RIOT_ECC_SIGNATURE*, TbsSig);
MOCKABLE_FUNCTION(, int, X509GetAliasCertTBS, DERBuilderContext*, Tbs, RIOT_X509_TBS_DATA*, TbsData, RIOT_ECC_PUBLIC*, AliasKeyPub, RIOT_ECC_PUBLIC*, DevIdKeyPub, uint8_t*, Fwid, uint32_t, FwidLen);
MOCKABLE_FUNCTION(, int, X509GetDERCsrTbs, DERBuilderContext*, Context, RIOT_X509_TBS_DATA*, TbsData, RIOT_ECC_PUBLIC*, DeviceIDPub);
MOCKABLE_FUNCTION(, int, X509GetDERCsr, DERBuilderContext*, Context, RIOT_ECC_SIGNATURE*, Signature);

#undef ENABLE_MOCKS

#include "dps_hsm_riot.h"

static const char* TEST_STRING_VALUE = "Test_String_Value";
static const char* TEST_CN_VALUE = "riot-common-device";

static int umocktypes_copy_RIOT_ECC_PRIVATE(RIOT_ECC_PRIVATE* dest, const RIOT_ECC_PRIVATE* src)
{
    int result;
    if (src == NULL)
    {
        dest = NULL;
        result = 0;
    }
    else
    {
        memcpy(dest->data, src->data, sizeof(dest->data));
        result = 0;
    }
    return result;
}

static void umocktypes_free_RIOT_ECC_PRIVATE(RIOT_ECC_PRIVATE* value)
{
    (void)value;
}

static char* umocktypes_stringify_RIOT_ECC_PRIVATE(const RIOT_ECC_PRIVATE* value)
{
    char* result;
    if (value == NULL)
    {
        result = (char*)my_gballoc_malloc(5);
        if (result != NULL)
        {
            (void)memcpy(result, "NULL", 5);
        }
    }
    else
    {
        int length = snprintf(NULL, 0, "{ %p }", value->data);
        if (length < 0)
        {
            result = NULL;
        }
        else
        {
            result = (char*)my_gballoc_malloc(length + 1);
            (void)snprintf(result, length + 1, "{ %p }", value->data);
        }
    }
    return result;
}

static int umocktypes_are_equal_RIOT_ECC_PRIVATE(RIOT_ECC_PRIVATE* left, RIOT_ECC_PRIVATE* right)
{
    int result;
    if (left == right)
    {
        result = 1;
    }
    else if ((left == NULL) || (right == NULL))
    {
        result = 0;
    }
    else
    {
        result = memcmp(left->data, right->data, sizeof(left->data));
    }
    return result;
}

static int umocktypes_copy_RIOT_ECC_PUBLIC(RIOT_ECC_PUBLIC* dest, const RIOT_ECC_PUBLIC* src)
{
    int result;
    if (src == NULL)
    {
        dest = NULL;
        result = 0;
    }
    else
    {
        dest->infinity = src->infinity;
        memcpy(dest->x.data, src->x.data, sizeof(dest->x.data) );
        memcpy(dest->y.data, src->x.data, sizeof(dest->y.data));
        result = 0;
    }
    return result;
}

static void umocktypes_free_RIOT_ECC_PUBLIC(RIOT_ECC_PUBLIC* value)
{
    (void)value;
}

static char* umocktypes_stringify_RIOT_ECC_PUBLIC(const RIOT_ECC_PUBLIC* value)
{
    char* result;
    if (value == NULL)
    {
        result = (char*)my_gballoc_malloc(5);
        if (result != NULL)
        {
            (void)memcpy(result, "NULL", 5);
        }
    }
    else
    {
        int length = snprintf(NULL, 0, "{ %p, %p, %d }",
            value->x.data, value->y.data, (int)value->infinity);
        if (length < 0)
        {
            result = NULL;
        }
        else
        {
            result = (char*)my_gballoc_malloc(length + 1);
            (void)snprintf(result, length + 1, "{ %p, %p, %d }",
                value->x.data, value->y.data, (int)value->infinity);
        }
    }
    return result;
}

static int umocktypes_are_equal_RIOT_ECC_PUBLIC(RIOT_ECC_PUBLIC* left, RIOT_ECC_PUBLIC* right)
{
    int result;
    if (left == right)
    {
        result = 1;
    }
    else if ((left == NULL) || (right == NULL))
    {
        result = 0;
    }
    else
    {
        result = memcmp(left->x.data, right->x.data, sizeof(left->x.data));
    }
    return result;
}

static int my_mallocAndStrcpy_s(char** destination, const char* source)
{
    (void)source;
    size_t src_len = strlen(source);
    *destination = (char*)my_gballoc_malloc(src_len + 1);
    strcpy(*destination, source);
    return 0;
}

static void my_DiceSHA256(const uint8_t *buf, size_t bufSize, uint8_t *digest)
{
    (void)buf;
    unsigned char counter = 0x01;
    for (size_t index = 0; index < bufSize; index++)
    {
        digest[0] = counter++;
    }
}

static void my_DiceSHA256_2(const uint8_t *buf1, size_t bufSize1, const uint8_t *buf2, size_t bufSize2, uint8_t *digest)
{
    (void)buf1;
    (void)bufSize1;
    (void)buf2;
    unsigned char counter = 0x01;
    for (size_t index = 0; index < bufSize2; index++)
    {
        digest[0] = counter++;
    }
}

static int my_DERtoPEM(DERBuilderContext* Context, uint32_t Type, char* PEM, uint32_t* Length)
{
    (void)Context;
    (void)Type;

    strcpy(PEM, TEST_STRING_VALUE);
    *Length = strlen(PEM);
    return 0;
}

DEFINE_ENUM_STRINGS(UMOCK_C_ERROR_CODE, UMOCK_C_ERROR_CODE_VALUES)

static void on_umock_c_error(UMOCK_C_ERROR_CODE error_code)
{
    char temp_str[256];
    (void)snprintf(temp_str, sizeof(temp_str), "umock_c reported error :%s", ENUM_TO_STRING(UMOCK_C_ERROR_CODE, error_code));
    ASSERT_FAIL(temp_str);
}

static TEST_MUTEX_HANDLE g_testByTest;
static TEST_MUTEX_HANDLE g_dllByDll;

BEGIN_TEST_SUITE(dps_hsm_riot_ut)

    TEST_SUITE_INITIALIZE(suite_init)
    {
        int result;

        TEST_INITIALIZE_MEMORY_DEBUG(g_dllByDll);
        g_testByTest = TEST_MUTEX_CREATE();
        ASSERT_IS_NOT_NULL(g_testByTest);

        (void)umock_c_init(on_umock_c_error);

        result = umocktypes_charptr_register_types();
        ASSERT_ARE_EQUAL(int, 0, result);
        result = umocktypes_stdint_register_types();
        ASSERT_ARE_EQUAL(int, 0, result);

        REGISTER_UMOCK_ALIAS_TYPE(XDA_HANDLE, void*);
        REGISTER_UMOCK_ALIAS_TYPE(DPS_SECURE_DEVICE_HANDLE, void*);
        REGISTER_UMOCK_ALIAS_TYPE(SECURE_DEVICE_TYPE, int);
        REGISTER_UMOCK_ALIAS_TYPE(STRING_HANDLE, void*);
        REGISTER_TYPE(RIOT_ECC_PUBLIC, RIOT_ECC_PUBLIC);
        REGISTER_TYPE(RIOT_ECC_PRIVATE, RIOT_ECC_PRIVATE);

        REGISTER_GLOBAL_MOCK_RETURN(RiotCrypt_Hash, RIOT_SUCCESS);
        REGISTER_GLOBAL_MOCK_FAIL_RETURN(RiotCrypt_Hash, RIOT_FAILURE);
        REGISTER_GLOBAL_MOCK_RETURN(RiotCrypt_DeriveEccKey, RIOT_SUCCESS);
        REGISTER_GLOBAL_MOCK_FAIL_RETURN(RiotCrypt_DeriveEccKey, RIOT_FAILURE);
        REGISTER_GLOBAL_MOCK_RETURN(RiotCrypt_Hash2, RIOT_SUCCESS);
        REGISTER_GLOBAL_MOCK_FAIL_RETURN(RiotCrypt_Hash2, RIOT_FAILURE);
        REGISTER_GLOBAL_MOCK_RETURN(X509GetDEREncodedTBS, 0);
        REGISTER_GLOBAL_MOCK_FAIL_RETURN(X509GetDEREncodedTBS, 1);
        REGISTER_GLOBAL_MOCK_RETURN(RiotCrypt_Sign, RIOT_SUCCESS);
        REGISTER_GLOBAL_MOCK_FAIL_RETURN(RiotCrypt_Sign, RIOT_FAILURE);
        REGISTER_GLOBAL_MOCK_RETURN(X509MakeAliasCert, 0);
        REGISTER_GLOBAL_MOCK_FAIL_RETURN(X509MakeAliasCert, 1);
        REGISTER_GLOBAL_MOCK_RETURN(X509GetDEREcc, 0);
        REGISTER_GLOBAL_MOCK_FAIL_RETURN(X509GetDEREcc, 1);
        REGISTER_GLOBAL_MOCK_RETURN(X509GetDEREccPub, 0);
        REGISTER_GLOBAL_MOCK_FAIL_RETURN(X509GetDEREccPub, 1);
        REGISTER_GLOBAL_MOCK_HOOK(DERtoPEM, my_DERtoPEM);
        REGISTER_GLOBAL_MOCK_FAIL_RETURN(DERtoPEM, 1);
        REGISTER_GLOBAL_MOCK_RETURN(X509GetAliasCertTBS, 0);
        REGISTER_GLOBAL_MOCK_FAIL_RETURN(X509GetAliasCertTBS, 1);
        REGISTER_GLOBAL_MOCK_RETURN(X509MakeDeviceCert, 0);
        REGISTER_GLOBAL_MOCK_FAIL_RETURN(X509MakeDeviceCert, 1);
        REGISTER_GLOBAL_MOCK_RETURN(X509MakeRootCert, 0);
        REGISTER_GLOBAL_MOCK_FAIL_RETURN(X509MakeRootCert, 1);
        REGISTER_GLOBAL_MOCK_RETURN(X509GetDERCsr, 0);
        REGISTER_GLOBAL_MOCK_FAIL_RETURN(X509GetDERCsr, 1);
        REGISTER_GLOBAL_MOCK_RETURN(X509GetDeviceCertTBS, 0);
        REGISTER_GLOBAL_MOCK_FAIL_RETURN(X509GetDeviceCertTBS, 1);

        REGISTER_GLOBAL_MOCK_HOOK(DiceSHA256, my_DiceSHA256);
        REGISTER_GLOBAL_MOCK_HOOK(DiceSHA256_2, my_DiceSHA256_2);

        REGISTER_GLOBAL_MOCK_HOOK(gballoc_malloc, my_gballoc_malloc);
        REGISTER_GLOBAL_MOCK_FAIL_RETURN(gballoc_malloc, NULL);
        REGISTER_GLOBAL_MOCK_HOOK(gballoc_free, my_gballoc_free);
        REGISTER_GLOBAL_MOCK_HOOK(mallocAndStrcpy_s, my_mallocAndStrcpy_s);
        REGISTER_GLOBAL_MOCK_FAIL_RETURN(mallocAndStrcpy_s, __LINE__);
    }

    TEST_SUITE_CLEANUP(suite_cleanup)
    {
        umock_c_deinit();

        TEST_MUTEX_DESTROY(g_testByTest);
        TEST_DEINITIALIZE_MEMORY_DEBUG(g_dllByDll);
    }

    TEST_FUNCTION_INITIALIZE(method_init)
    {
        if (TEST_MUTEX_ACQUIRE(g_testByTest))
        {
            ASSERT_FAIL("Could not acquire test serialization mutex.");
        }
        umock_c_reset_all_calls();
    }

    TEST_FUNCTION_CLEANUP(method_cleanup)
    {
        TEST_MUTEX_RELEASE(g_testByTest);
    }

    static int should_skip_index(size_t current_index, const size_t skip_array[], size_t length)
    {
        int result = 0;
        for (size_t index = 0; index < length; index++)
        {
            if (current_index == skip_array[index])
            {
                result = __LINE__;
                break;
            }
        }
        return result;
    }

    static void dps_hsm_riot_create_mock(bool device_signed)
    {
        RIOT_ECC_PUBLIC pub = { 0 };
        RIOT_ECC_PRIVATE pri = { 0 };

        STRICT_EXPECTED_CALL(gballoc_malloc(IGNORED_NUM_ARG));
        STRICT_EXPECTED_CALL(RiotCrypt_Hash(IGNORED_PTR_ARG, IGNORED_NUM_ARG, IGNORED_PTR_ARG, IGNORED_NUM_ARG));
        STRICT_EXPECTED_CALL(RiotCrypt_DeriveEccKey(IGNORED_PTR_ARG, IGNORED_PTR_ARG, IGNORED_PTR_ARG, IGNORED_NUM_ARG, IGNORED_PTR_ARG, IGNORED_NUM_ARG));
        STRICT_EXPECTED_CALL(RiotCrypt_Hash2(IGNORED_PTR_ARG, IGNORED_NUM_ARG, IGNORED_PTR_ARG, IGNORED_NUM_ARG, IGNORED_PTR_ARG, IGNORED_NUM_ARG));
        STRICT_EXPECTED_CALL(RiotCrypt_DeriveEccKey(IGNORED_PTR_ARG, IGNORED_PTR_ARG, IGNORED_PTR_ARG, IGNORED_NUM_ARG, IGNORED_PTR_ARG, IGNORED_NUM_ARG));
        STRICT_EXPECTED_CALL(DERInitContext(IGNORED_PTR_ARG, IGNORED_PTR_ARG, IGNORED_NUM_ARG));
        STRICT_EXPECTED_CALL(X509GetDEREccPub(IGNORED_PTR_ARG, pub))
            .IgnoreArgument_Pub();
        STRICT_EXPECTED_CALL(DERtoPEM(IGNORED_PTR_ARG, PUBLICKEY_TYPE, IGNORED_PTR_ARG, IGNORED_NUM_ARG)); // 7

        STRICT_EXPECTED_CALL(DERInitContext(IGNORED_PTR_ARG, IGNORED_PTR_ARG, IGNORED_NUM_ARG)); // 8
        STRICT_EXPECTED_CALL(X509GetDEREcc(IGNORED_PTR_ARG, pub, pri))
            .IgnoreArgument_Pub()
            .IgnoreArgument_Priv();
        STRICT_EXPECTED_CALL(DERtoPEM(IGNORED_PTR_ARG, ECC_PRIVATEKEY_TYPE, IGNORED_PTR_ARG, IGNORED_NUM_ARG));

        STRICT_EXPECTED_CALL(DERInitContext(IGNORED_PTR_ARG, IGNORED_PTR_ARG, IGNORED_NUM_ARG));
        STRICT_EXPECTED_CALL(X509GetAliasCertTBS(IGNORED_PTR_ARG, IGNORED_PTR_ARG, IGNORED_PTR_ARG, IGNORED_PTR_ARG, IGNORED_PTR_ARG, IGNORED_NUM_ARG));
        STRICT_EXPECTED_CALL(RiotCrypt_Sign(IGNORED_PTR_ARG, IGNORED_PTR_ARG, IGNORED_NUM_ARG, IGNORED_PTR_ARG));
        STRICT_EXPECTED_CALL(X509MakeAliasCert(IGNORED_PTR_ARG, IGNORED_PTR_ARG));

        STRICT_EXPECTED_CALL(DERtoPEM(IGNORED_PTR_ARG, CERT_TYPE, IGNORED_PTR_ARG, IGNORED_NUM_ARG)); //17

        if (device_signed)
        {
            STRICT_EXPECTED_CALL(DERInitContext(IGNORED_PTR_ARG, IGNORED_PTR_ARG, IGNORED_NUM_ARG));
            STRICT_EXPECTED_CALL(X509GetDeviceCertTBS(IGNORED_PTR_ARG, IGNORED_PTR_ARG, IGNORED_NUM_ARG));
            STRICT_EXPECTED_CALL(RiotCrypt_Sign(IGNORED_PTR_ARG, IGNORED_PTR_ARG, IGNORED_NUM_ARG, IGNORED_PTR_ARG));
            STRICT_EXPECTED_CALL(X509MakeDeviceCert(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
        }
        else
        {
            STRICT_EXPECTED_CALL(DERInitContext(IGNORED_PTR_ARG, IGNORED_PTR_ARG, IGNORED_NUM_ARG));
            STRICT_EXPECTED_CALL(X509GetDERCsrTbs(IGNORED_PTR_ARG, IGNORED_PTR_ARG, IGNORED_NUM_ARG));
            STRICT_EXPECTED_CALL(RiotCrypt_Sign(IGNORED_PTR_ARG, IGNORED_PTR_ARG, IGNORED_NUM_ARG, IGNORED_PTR_ARG));
            STRICT_EXPECTED_CALL(X509GetDERCsr(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
        }
        STRICT_EXPECTED_CALL(DERtoPEM(IGNORED_PTR_ARG, CERT_TYPE, IGNORED_PTR_ARG, IGNORED_NUM_ARG));

        STRICT_EXPECTED_CALL(DERInitContext(IGNORED_PTR_ARG, IGNORED_PTR_ARG, IGNORED_NUM_ARG));
        STRICT_EXPECTED_CALL(X509GetDeviceCertTBS(IGNORED_PTR_ARG, IGNORED_PTR_ARG, IGNORED_NUM_ARG));
        STRICT_EXPECTED_CALL(RiotCrypt_Sign(IGNORED_PTR_ARG, IGNORED_PTR_ARG, IGNORED_NUM_ARG, IGNORED_PTR_ARG));
        STRICT_EXPECTED_CALL(X509MakeRootCert(IGNORED_PTR_ARG, IGNORED_PTR_ARG)); // 24
        STRICT_EXPECTED_CALL(DERtoPEM(IGNORED_PTR_ARG, CERT_TYPE, IGNORED_PTR_ARG, IGNORED_NUM_ARG));

        STRICT_EXPECTED_CALL(DERInitContext(IGNORED_PTR_ARG, IGNORED_PTR_ARG, IGNORED_NUM_ARG));
        STRICT_EXPECTED_CALL(X509GetDEREcc(IGNORED_PTR_ARG, pub, pri))
            .IgnoreArgument_Pub()
            .IgnoreArgument_Priv();
        STRICT_EXPECTED_CALL(DERtoPEM(IGNORED_PTR_ARG, ECC_PRIVATEKEY_TYPE, IGNORED_PTR_ARG, IGNORED_NUM_ARG));

        STRICT_EXPECTED_CALL(mallocAndStrcpy_s(IGNORED_PTR_ARG, IGNORED_PTR_ARG));
    }

    /* Tests_SRS_SECURE_DEVICE_RIOT_07_001: [ On success dps_hsm_riot_create shall allocate a new instance of the device auth interface. ] */
    /* Tests_SRS_SECURE_DEVICE_RIOT_07_002: [ dps_hsm_riot_create shall call into the RIot code to sign the RIoT certificate. ] */
    /* Tests_SRS_SECURE_DEVICE_RIOT_07_003: [ dps_hsm_riot_create shall cache the device id public value from the RIoT module. ] */
    /* Tests_SRS_SECURE_DEVICE_RIOT_07_004: [ dps_hsm_riot_create shall cache the alias key pair value from the RIoT module. ] */
    /* Tests_SRS_SECURE_DEVICE_RIOT_07_005: [ dps_hsm_riot_create shall create the Signer regions of the alias key certificate. ]*/
    TEST_FUNCTION(dps_hsm_riot_create_succeed)
    {
        initialize_riot_system();
        umock_c_reset_all_calls();

        //arrange
        dps_hsm_riot_create_mock(true);

        //act
        DPS_SECURE_DEVICE_HANDLE sec_handle = dps_hsm_riot_create();

        //assert
        ASSERT_IS_NOT_NULL(sec_handle);
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        //cleanup
        dps_hsm_riot_destroy(sec_handle);
    }

    /* Tests_SRS_SECURE_DEVICE_RIOT_07_006: [ If any failure is encountered dps_hsm_riot_create shall return NULL ] */
    TEST_FUNCTION(dps_hsm_riot_create_fail)
    {
        initialize_riot_system();
        umock_c_reset_all_calls();

        int negativeTestsInitResult = umock_c_negative_tests_init();
        ASSERT_ARE_EQUAL(int, 0, negativeTestsInitResult);

        //arrange
        dps_hsm_riot_create_mock(true);

        umock_c_negative_tests_snapshot();

        size_t calls_cannot_fail[] = { 5, 8, 11, 16, 21, 26 };

        //act
        size_t count = umock_c_negative_tests_call_count();
        for (size_t index = 0; index < count; index++)
        {
            if (should_skip_index(index, calls_cannot_fail, sizeof(calls_cannot_fail) / sizeof(calls_cannot_fail[0])) != 0)
            {
                continue;
            }

            umock_c_negative_tests_reset();
            umock_c_negative_tests_fail_call(index);

            char tmp_msg[64];
            sprintf(tmp_msg, "dps_hsm_riot_create failure in test %zu/%zu", index, count);

            //act
            DPS_SECURE_DEVICE_HANDLE sec_handle = dps_hsm_riot_create();

            //assert
            ASSERT_IS_NULL_WITH_MSG(sec_handle, tmp_msg);
        }

        //cleanup
        umock_c_negative_tests_deinit();
    }

    /* Tests_SRS_SECURE_DEVICE_RIOT_07_008: [ dps_hsm_riot_destroy shall free the DPS_SECURE_DEVICE_HANDLE instance. ] */
    /* Tests_SRS_SECURE_DEVICE_RIOT_07_009: [ dps_hsm_riot_destroy shall free all resources allocated in this module. ] */
    TEST_FUNCTION(dps_hsm_riot_destroy_succeed)
    {
        //arrange
        initialize_riot_system();
        DPS_SECURE_DEVICE_HANDLE sec_handle = dps_hsm_riot_create();
        umock_c_reset_all_calls();

        STRICT_EXPECTED_CALL(gballoc_free(IGNORED_PTR_ARG));
        STRICT_EXPECTED_CALL(gballoc_free(sec_handle));

        //act
        dps_hsm_riot_destroy(sec_handle);

        //assert
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        //cleanup
    }

    /* Tests_SRS_SECURE_DEVICE_RIOT_07_007: [ if handle is NULL, dps_hsm_riot_destroy shall do nothing. ] */
    TEST_FUNCTION(dps_hsm_riot_destroy_handle_NULL_succeed)
    {
        //arrange
        umock_c_reset_all_calls();

        //act
        dps_hsm_riot_destroy(NULL);

        //assert
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        //cleanup
    }

    /* Tests_SRS_SECURE_DEVICE_RIOT_07_010: [ if handle is NULL, dps_hsm_riot_get_certificate shall return NULL. ] */
    TEST_FUNCTION(dps_hsm_riot_get_certificate_handle_NULL_succeed)
    {
        //arrange
        umock_c_reset_all_calls();

        //act
        char* value = dps_hsm_riot_get_certificate(NULL);

        //assert
        ASSERT_IS_NULL(value);
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        //cleanup
    }

    /* Tests_SRS_SECURE_DEVICE_RIOT_07_013: [ If any failure is encountered dps_hsm_riot_get_certificate shall return NULL ] */
    TEST_FUNCTION(dps_hsm_riot_get_certificate_malloc_fail)
    {
        //arrange
        initialize_riot_system();
        DPS_SECURE_DEVICE_HANDLE sec_handle = dps_hsm_riot_create();
        umock_c_reset_all_calls();

        STRICT_EXPECTED_CALL(gballoc_malloc(IGNORED_NUM_ARG)).SetReturn(NULL);

        //act
        char* value = dps_hsm_riot_get_certificate(sec_handle);

        //assert
        ASSERT_IS_NULL(value);
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        //cleanup
        dps_hsm_riot_destroy(sec_handle);
    }

    /* Tests_SRS_SECURE_DEVICE_RIOT_07_011: [ dps_hsm_riot_get_certificate shall allocate a char* to return the riot certificate. ] */
    /* Tests_SRS_SECURE_DEVICE_RIOT_07_012: [ On success dps_hsm_riot_get_certificate shall return the riot certificate. ] */
    TEST_FUNCTION(dps_hsm_riot_get_certificate_succeed)
    {
        //arrange
        initialize_riot_system();
        DPS_SECURE_DEVICE_HANDLE sec_handle = dps_hsm_riot_create();
        umock_c_reset_all_calls();

        STRICT_EXPECTED_CALL(gballoc_malloc(IGNORED_NUM_ARG));

        //act
        char* value = dps_hsm_riot_get_certificate(sec_handle);

        //assert
        ASSERT_IS_NOT_NULL(value);
        ASSERT_ARE_EQUAL(char_ptr, TEST_STRING_VALUE, value);
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        //cleanup
        my_gballoc_free(value);
        dps_hsm_riot_destroy(sec_handle);
    }

    /* Tests_SRS_SECURE_DEVICE_RIOT_07_014: [ if handle is NULL, dps_hsm_riot_get_alias_key shall return NULL. ] */
    TEST_FUNCTION(dps_hsm_riot_get_alias_key_handle_NULL_succeed)
    {
        //arrange
        umock_c_reset_all_calls();

        //act
        char* value = dps_hsm_riot_get_alias_key(NULL);

        //assert
        ASSERT_IS_NULL(value);
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        //cleanup
    }

    /* Tests_SRS_SECURE_DEVICE_RIOT_07_017: [ If any failure is encountered dps_hsm_riot_get_alias_key shall return NULL ] */
    TEST_FUNCTION(dps_hsm_riot_get_alias_key_malloc_fail)
    {
        //arrange
        initialize_riot_system();
        DPS_SECURE_DEVICE_HANDLE sec_handle = dps_hsm_riot_create();
        umock_c_reset_all_calls();

        STRICT_EXPECTED_CALL(gballoc_malloc(IGNORED_NUM_ARG)).SetReturn(NULL);

        //act
        char* value = dps_hsm_riot_get_alias_key(sec_handle);

        //assert
        ASSERT_IS_NULL(value);
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        //cleanup
        dps_hsm_riot_destroy(sec_handle);
    }

    /* Tests_SRS_SECURE_DEVICE_RIOT_07_015: [ dps_hsm_riot_get_alias_key shall allocate a char* to return the alias certificate. ] */
    /* Tests_SRS_SECURE_DEVICE_RIOT_07_016: [ On success dps_hsm_riot_get_alias_key shall return the alias certificate. ] */
    TEST_FUNCTION(dps_hsm_riot_get_alias_key_succeed)
    {
        //arrange
        initialize_riot_system();
        DPS_SECURE_DEVICE_HANDLE sec_handle = dps_hsm_riot_create();
        umock_c_reset_all_calls();

        STRICT_EXPECTED_CALL(gballoc_malloc(IGNORED_NUM_ARG));

        //act
        char* value = dps_hsm_riot_get_alias_key(sec_handle);

        //assert
        ASSERT_IS_NOT_NULL(value);
        ASSERT_ARE_EQUAL(char_ptr, TEST_STRING_VALUE, value);
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        //cleanup
        my_gballoc_free(value);
        dps_hsm_riot_destroy(sec_handle);
    }

    /* Tests_SRS_SECURE_DEVICE_RIOT_07_022: [ if handle is NULL, dps_hsm_riot_get_signer_cert shall return NULL. ] */
    TEST_FUNCTION(dps_hsm_riot_get_signer_cert_handle_NULL_succeed)
    {
        //arrange
        umock_c_reset_all_calls();

        //act
        char* value = dps_hsm_riot_get_signer_cert(NULL);

        //assert
        ASSERT_IS_NULL(value);
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        //cleanup
    }

    /* Tests_SRS_SECURE_DEVICE_RIOT_07_025: [ If any failure is encountered dps_hsm_riot_get_signer_cert shall return NULL ] */
    TEST_FUNCTION(dps_hsm_riot_get_signer_cert_malloc_fail)
    {
        //arrange
        initialize_riot_system();
        DPS_SECURE_DEVICE_HANDLE sec_handle = dps_hsm_riot_create();
        umock_c_reset_all_calls();

        STRICT_EXPECTED_CALL(gballoc_malloc(IGNORED_NUM_ARG)).SetReturn(NULL);

        //act
        char* value = dps_hsm_riot_get_signer_cert(sec_handle);

        //assert
        ASSERT_IS_NULL(value);
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        //cleanup
        dps_hsm_riot_destroy(sec_handle);
    }

    /* Tests_SRS_SECURE_DEVICE_RIOT_07_024: [ On success dps_hsm_riot_get_signer_cert shall return the signer certificate. ] */
    /* Tests_SRS_SECURE_DEVICE_RIOT_07_023: [ dps_hsm_riot_get_signer_cert shall allocate a char* to return the signer certificate. ] */
    TEST_FUNCTION(dps_hsm_riot_get_signer_cert_succeed)
    {
        //arrange
        initialize_riot_system();
        DPS_SECURE_DEVICE_HANDLE sec_handle = dps_hsm_riot_create();
        umock_c_reset_all_calls();

        STRICT_EXPECTED_CALL(gballoc_malloc(IGNORED_NUM_ARG));

        //act
        char* value = dps_hsm_riot_get_signer_cert(sec_handle);

        //assert
        ASSERT_IS_NOT_NULL(value);
        ASSERT_ARE_EQUAL(char_ptr, TEST_STRING_VALUE, value);
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        //cleanup
        my_gballoc_free(value);
        dps_hsm_riot_destroy(sec_handle);
    }

    /* Tests_SRS_SECURE_DEVICE_RIOT_07_026: [ if handle is NULL, dps_hsm_riot_get_common_name shall return NULL. ] */
    TEST_FUNCTION(dps_hsm_riot_get_common_name_handle_NULL_succeed)
    {
        //arrange
        umock_c_reset_all_calls();

        //act
        char* value = dps_hsm_riot_get_common_name(NULL);

        //assert
        ASSERT_IS_NULL(value);
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        //cleanup
    }

    /* Tests_SRS_SECURE_DEVICE_RIOT_07_028: [ If any failure is encountered dps_hsm_riot_get_signer_cert shall return NULL ] */
    TEST_FUNCTION(dps_hsm_riot_get_common_name_mallocAndStrcpy_s_fail)
    {
        //arrange
        initialize_riot_system();
        DPS_SECURE_DEVICE_HANDLE sec_handle = dps_hsm_riot_create();
        umock_c_reset_all_calls();

        STRICT_EXPECTED_CALL(mallocAndStrcpy_s(IGNORED_PTR_ARG, IGNORED_PTR_ARG)).SetReturn(1);

        //act
        char* value = dps_hsm_riot_get_common_name(sec_handle);

        //assert
        ASSERT_IS_NULL(value);
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        //cleanup
        dps_hsm_riot_destroy(sec_handle);
    }

    /* Tests_SRS_SECURE_DEVICE_RIOT_07_027: [ dps_hsm_riot_get_common_name shall allocate a char* to return the certificate common name. ] */
    TEST_FUNCTION(dps_hsm_riot_get_common_name_succeed)
    {
        //arrange
        initialize_riot_system();
        DPS_SECURE_DEVICE_HANDLE sec_handle = dps_hsm_riot_create();
        umock_c_reset_all_calls();

        STRICT_EXPECTED_CALL(mallocAndStrcpy_s(IGNORED_PTR_ARG, IGNORED_PTR_ARG));

        //act
        char* value = dps_hsm_riot_get_common_name(sec_handle);

        //assert
        ASSERT_IS_NOT_NULL(value);
        ASSERT_ARE_EQUAL(char_ptr, TEST_CN_VALUE, value);
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        //cleanup
        my_gballoc_free(value);
        dps_hsm_riot_destroy(sec_handle);
    }

    /* Tests_SRS_SECURE_DEVICE_RIOT_07_029: [ dps_hsm_riot_interface shall return the SEC_RIOT_INTERFACE structure. ] */
    TEST_FUNCTION(dps_hsm_riot_interface_succeed)
    {
        //arrange
        initialize_riot_system();
        DPS_SECURE_DEVICE_HANDLE sec_handle = dps_hsm_riot_create();
        umock_c_reset_all_calls();

        //act
        const SEC_X509_INTERFACE* riot_iface = dps_hsm_x509_interface();

        //assert
        ASSERT_IS_NOT_NULL(riot_iface);
        ASSERT_IS_NOT_NULL(riot_iface->secure_device_create);
        ASSERT_IS_NOT_NULL(riot_iface->secure_device_destroy);
        ASSERT_IS_NOT_NULL(riot_iface->secure_device_get_cert);
        ASSERT_IS_NOT_NULL(riot_iface->secure_device_get_ak);
        ASSERT_IS_NOT_NULL(riot_iface->secure_device_get_signer_cert);
        ASSERT_IS_NOT_NULL(riot_iface->secure_device_get_root_cert);
        ASSERT_IS_NOT_NULL(riot_iface->secure_device_get_root_key);
        ASSERT_IS_NOT_NULL(riot_iface->secure_device_get_common_name);
        ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

        //cleanup
        dps_hsm_riot_destroy(sec_handle);
    }

    END_TEST_SUITE(dps_hsm_riot_ut)
