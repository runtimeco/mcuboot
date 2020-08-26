// Copyright (c) 2017-2019 Linaro LTD
// Copyright (c) 2019 JUUL Labs
// Copyright (c) 2019 Arm Limited
//
// SPDX-License-Identifier: Apache-2.0

// Query the bootloader's capabilities.

use enum_iterator::IntoEnumIterator;
use std::{
    fmt,
};

#[repr(u32)]
#[derive(Copy, Clone, Debug, Eq, IntoEnumIterator, PartialEq)]
#[allow(unused)]
pub enum Caps {
    RSA2048              = (1 << 0),
    EcdsaP224            = (1 << 1),
    EcdsaP256            = (1 << 2),
    SwapUsingScratch     = (1 << 3),
    OverwriteUpgrade     = (1 << 4),
    EncRsa               = (1 << 5),
    EncKw                = (1 << 6),
    ValidatePrimarySlot  = (1 << 7),
    RSA3072              = (1 << 8),
    Ed25519              = (1 << 9),
    EncEc256             = (1 << 10),
    SwapUsingMove        = (1 << 11),
    DowngradePrevention  = (1 << 12),
    EncX25519            = (1 << 13),
    DirectXIP            = (1 << 14),
}

impl Caps {
    pub fn present(self) -> bool {
        let caps = unsafe { bootutil_get_caps() };
        (caps as u32) & (self as u32) != 0
    }

    /// Query for the number of images that have been configured into this
    /// MCUboot build.
    pub fn get_num_images() -> usize {
        (unsafe { bootutil_get_num_images() }) as usize
    }

    /// Return true if this configuration performs an image swap.
    pub fn has_swap() -> bool {
        Caps::SwapUsingScratch.present() ||
            Caps::SwapUsingMove.present()
    }

    /// For debugging, print all of the Caps we are compiled for.
    pub fn show() {
        println!("Capabilities: {}", unsafe { bootutil_get_caps() });
    }
}

impl fmt::Display for Caps {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let mut comma = false;
        write!(f, "{{")?;
        for cap in Caps::into_enum_iter() {
            if (cap as u32) & (*self as u32) != 0 {
                if comma {
                    write!(f, ",")?;
                }
                comma = true;
                write!(f, "{:?}", cap)?;
            }
        }
        write!(f, "}}")
    }
}

extern "C" {
    fn bootutil_get_caps() -> Caps;
    fn bootutil_get_num_images() -> u32;
}
