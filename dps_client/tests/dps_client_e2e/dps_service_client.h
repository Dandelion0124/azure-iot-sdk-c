// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef DPS_SERVICE_CLIENT_H
#define DPS_SERVICE_CLIENT_H

#include "azure_c_shared_utility/macro_utils.h"
#include "azure_c_shared_utility/umock_c_prod.h"

#include "azure_c_shared_utility/buffer_.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

    typedef struct DPS_SERVICE_CLIENT_TAG* DPS_SERVICE_CLIENT_HANDLE;

    MOCKABLE_FUNCTION(, DPS_SERVICE_CLIENT_HANDLE, dps_service_create, const char*, hostname, const char*, key_name, const char*, key);
    MOCKABLE_FUNCTION(, void, dps_serivce_destroy, DPS_SERVICE_CLIENT_HANDLE, handle);

    MOCKABLE_FUNCTION(, int, dps_service_enroll_tpm_device, DPS_SERVICE_CLIENT_HANDLE, handle, const char*, registration_id, const char*, device_id, BUFFER_HANDLE, endorsement_key);
    MOCKABLE_FUNCTION(, int, dps_service_enroll_x509_device, DPS_SERVICE_CLIENT_HANDLE, handle, const char*, registration_id, const char*, signer_cert);
    MOCKABLE_FUNCTION(, int, dps_service_remove_device, DPS_SERVICE_CLIENT_HANDLE, handle, const char*, registration_id);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DPS_SERVICE_CLIENT_H */
