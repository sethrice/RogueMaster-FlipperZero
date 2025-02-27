#pragma once

#include <lib/nfc/nfc_device.h>

#define NESTED_FOLDER EXT_PATH("nfc/.nested")

typedef struct MifareNestedWorker MifareNestedWorker;

typedef enum {
    MifareNestedWorkerStateReady,

    MifareNestedWorkerStateCheck,
    MifareNestedWorkerStateCollecting,
    MifareNestedWorkerStateCollectingStatic,
    MifareNestedWorkerStateValidating,

    MifareNestedWorkerStateStop,
} MifareNestedWorkerState;

typedef enum {
    MifareNestedWorkerEventReserved = 1000,

    MifareNestedWorkerEventNoTagDetected,
    MifareNestedWorkerEventNoncesCollected,
    MifareNestedWorkerEventCollecting,

    MifareNestedWorkerEventNewNonce,
    MifareNestedWorkerEventKeyChecked,
    MifareNestedWorkerEventKeysFound,
    MifareNestedWorkerEventNeedKey,
    MifareNestedWorkerEventAttackFailed,
    MifareNestedWorkerEventCalibrating,
    MifareNestedWorkerEventUnpredictablePRNG,
    MifareNestedWorkerEventNeedPrediction,
    MifareNestedWorkerEventProcessingKeys,
    MifareNestedWorkerEventNeedKeyRecovery,
    MifareNestedWorkerEventNeedCollection
} MifareNestedWorkerEvent;

typedef bool (*MifareNestedWorkerCallback)(MifareNestedWorkerEvent event, void* context);

MifareNestedWorker* mifare_nested_worker_alloc();

void mifare_nested_worker_change_state(
    MifareNestedWorker* mifare_nested_worker,
    MifareNestedWorkerState state);

void mifare_nested_worker_free(MifareNestedWorker* mifare_nested_worker);

void mifare_nested_worker_stop(MifareNestedWorker* mifare_nested_worker);

void mifare_nested_worker_start(
    MifareNestedWorker* mifare_nested_worker,
    MifareNestedWorkerState state,
    NfcDeviceData* dev_data,
    MifareNestedWorkerCallback callback,
    void* context);

typedef struct {
    uint32_t key_type;
    uint32_t block;
    uint32_t target_nt[2];
    uint32_t target_ks[2];
    uint8_t parity[2][4];
    bool collected;
    bool skipped;
} Nonces;

typedef struct {
    uint32_t cuid;
    uint32_t sector_count;
    // 40 (or 16/5) sectors, 2 keys (A/B), 3 tries
    Nonces* nonces[40][2][3];
    uint32_t tries;
} NonceList_t;

typedef struct {
    uint32_t total_keys;
    uint32_t checked_keys;
    uint32_t found_keys;
    uint32_t added_keys;
    uint32_t sector_keys;
    bool tag_lost;
} KeyInfo_t;
