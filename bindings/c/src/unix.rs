// Copyright 2023 The AccessKit Authors. All rights reserved.
// Licensed under the Apache License, Version 2.0 (found in
// the LICENSE-APACHE file) or the MIT license (found in
// the LICENSE-MIT file), at your option.

use crate::{action_handler, tree_update, try_ref_from_ptr, BoxCastPtr, CastPtr};
use accesskit::Rect;
use accesskit_unix::Adapter;
use std::{
    ffi::CStr,
    os::raw::{c_char, c_void},
    ptr,
};

#[repr(C)]
pub struct unix_adapter {
    _private: [u8; 0],
}

impl CastPtr for unix_adapter {
    type RustType = Adapter;
}

impl BoxCastPtr for unix_adapter {}

impl unix_adapter {
    #[no_mangle]
    pub extern "C" fn accesskit_unix_adapter_new(
        app_name: *const c_char,
        toolkit_name: *const c_char,
        toolkit_version: *const c_char,
        initial_state: Option<extern "C" fn(*mut c_void) -> tree_update>,
        initial_state_userdata: *mut c_void,
        handler: *mut action_handler,
    ) -> *mut unix_adapter {
        let app_name = unsafe { CStr::from_ptr(app_name).to_string_lossy().into() };
        let toolkit_name = unsafe { CStr::from_ptr(toolkit_name).to_string_lossy().into() };
        let toolkit_version = unsafe { CStr::from_ptr(toolkit_version).to_string_lossy().into() };
        let initial_state = match initial_state {
            Some(initial_state) => initial_state,
            None => return ptr::null_mut(),
        };
        let handler = match action_handler::to_box(handler) {
            Some(handler) => handler,
            None => return ptr::null_mut(),
        };
        let adapter = Adapter::new(
            app_name,
            toolkit_name,
            toolkit_version,
            move || initial_state(initial_state_userdata).into(),
            handler,
        );
        adapter.map_or_else(ptr::null_mut, BoxCastPtr::to_mut_ptr)
    }

    #[no_mangle]
    pub extern "C" fn accesskit_unix_adapter_free(adapter: *mut unix_adapter) {
        unix_adapter::to_box(adapter);
    }

    #[no_mangle]
    pub extern "C" fn accesskit_unix_adapter_set_root_window_bounds(
        adapter: *const unix_adapter,
        outer: Rect,
        inner: Rect,
    ) {
        let adapter = try_ref_from_ptr!(adapter);
        adapter.set_root_window_bounds(outer, inner);
    }

    #[no_mangle]
    pub extern "C" fn accesskit_unix_adapter_update(
        adapter: *const unix_adapter,
        update: tree_update,
    ) {
        let adapter = try_ref_from_ptr!(adapter);
        adapter.update(update.into());
    }
}
