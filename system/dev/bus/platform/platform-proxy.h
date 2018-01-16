// Copyright 2017 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <stdint.h>
#include <ddk/device.h>
#include <ddk/protocol/gpio.h>
#include <ddk/protocol/i2c.h>
#include <ddk/protocol/usb-mode-switch.h>

// maximum transfer size we can proxy.
#define PDEV_I2C_MAX_TRANSFER_SIZE 4096

// RPC ops
enum {
    // ZX_PROTOCOL_PLATFORM_DEV
    PDEV_GET_MMIO = 1,
    PDEV_GET_INTERRUPT,
    PDEV_ALLOC_CONTIG_VMO,
    PDEV_CREATE_INTERRUPT_HANDLE,

    // ZX_PROTOCOL_USB_MODE_SWITCH
    PDEV_UMS_GET_INITIAL_MODE,
    PDEV_UMS_SET_MODE,

    // ZX_PROTOCOL_GPIO
    PDEV_GPIO_CONFIG,
    PDEV_GPIO_READ,
    PDEV_GPIO_WRITE,
    PDEV_GPIO_BIND_INTERRUPT,
    PDEV_GPIO_UNBIND_INTERRUPT,

    // ZX_PROTOCOL_I2C
    PDEV_I2C_GET_CHANNEL,
    PDEV_I2C_TRANSACT,
    PDEV_I2C_SET_BITRATE,
    PDEV_I2C_CHANNEL_RELEASE,
};

// context for i2c_transact
typedef struct {
    size_t write_length;
    size_t read_length;
    i2c_complete_cb complete_cb;
    void* cookie;
} pdev_i2c_txn_ctx_t;

typedef struct {
    size_t size;
    uint32_t align_log2;
} pdev_config_vmo_t;

typedef struct {
    pdev_i2c_txn_ctx_t txn_ctx;
    // private context for the server, returned from i2c_get_channel and passed by client
    // for all channel operations
    void* server_ctx;
    uint32_t bitrate;
} pdev_i2c_req_t;

typedef struct {
    pdev_i2c_txn_ctx_t txn_ctx;
    // private context for the server, returned from i2c_get_channel and passed by client
    // for all channel operations
    void* server_ctx;
    size_t max_transfer_size;
} pdev_i2c_resp_t;

typedef struct {
    zx_txid_t txid;
    uint32_t op;
    uint32_t index;
    union {
        pdev_config_vmo_t contig_vmo;
        usb_mode_t usb_mode;
        gpio_config_flags_t gpio_flags;
        uint8_t gpio_value;
        pdev_i2c_req_t i2c;
        uint32_t interrupt_slot;
    };
} pdev_req_t;

typedef struct {
    zx_txid_t txid;
    zx_status_t status;
    union {
        usb_mode_t usb_mode;
        uint8_t gpio_value;
        pdev_i2c_resp_t i2c;
    };
} pdev_resp_t;
