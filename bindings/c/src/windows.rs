// Copyright 2023 The AccessKit Authors. All rights reserved.
// Licensed under the Apache License, Version 2.0 (found in
// the LICENSE-APACHE file) or the MIT license (found in
// the LICENSE-MIT file), at your option.

use crate::{action_handler, opt_struct, tree_update, try_ref_from_ptr, BoxCastPtr, CastPtr};
use accesskit_windows::*;
use std::{os::raw::c_void, ptr};

pub struct windows_uia_init_marker {
    _private: [u8; 0],
}

impl CastPtr for windows_uia_init_marker {
    type RustType = UiaInitMarker;
}

impl BoxCastPtr for windows_uia_init_marker {}

impl windows_uia_init_marker {
    #[no_mangle]
    pub extern "C" fn accesskit_windows_uia_init_marker_new() -> *mut windows_uia_init_marker {
        let marker = UiaInitMarker::new();
        BoxCastPtr::to_mut_ptr(marker)
    }

    /// You don't need to call this if you use `accesskit_windows_adapter_new`.
    #[no_mangle]
    pub extern "C" fn accesskit_windows_uia_init_marker_free(marker: *mut windows_uia_init_marker) {
        windows_uia_init_marker::to_box(marker);
    }
}

pub struct windows_queued_events {
    _private: [u8; 0],
}

impl CastPtr for windows_queued_events {
    type RustType = QueuedEvents;
}

impl BoxCastPtr for windows_queued_events {}

impl windows_queued_events {
    /// Memory is also freed when calling this function.
    #[no_mangle]
    pub extern "C" fn accesskit_windows_queued_events_raise(events: *mut windows_queued_events) {
        if let Some(events) = windows_queued_events::to_box(events) {
            events.raise();
        }
    }
}

opt_struct! { opt_lresult, LRESULT }

pub struct windows_adapter {
    _private: [u8; 0],
}

impl CastPtr for windows_adapter {
    type RustType = Adapter;
}

impl BoxCastPtr for windows_adapter {}

impl windows_adapter {
    #[no_mangle]
    pub extern "C" fn accesskit_windows_adapter_new(
        hwnd: HWND,
        initial_state: tree_update,
        handler: *mut action_handler,
        uia_init_marker: *mut windows_uia_init_marker,
    ) -> *mut windows_adapter {
        let handler = match action_handler::to_box(handler) {
            Some(handler) => handler,
            None => return ptr::null_mut(),
        };
        let uia_init_marker = match windows_uia_init_marker::to_box(uia_init_marker) {
            Some(marker) => *marker,
            None => return ptr::null_mut(),
        };
        let adapter = Adapter::new(hwnd, initial_state.into(), handler, uia_init_marker);
        BoxCastPtr::to_mut_ptr(adapter)
    }

    #[no_mangle]
    pub extern "C" fn accesskit_windows_adapter_free(adapter: *mut windows_adapter) {
        windows_adapter::to_box(adapter);
    }

    #[no_mangle]
    pub extern "C" fn accesskit_windows_adapter_update(
        adapter: *const windows_adapter,
        update: tree_update,
    ) -> *mut windows_queued_events {
        let adapter = try_ref_from_ptr!(adapter);
        let events = adapter.update(update.into());
        BoxCastPtr::to_mut_ptr(events)
    }

    #[no_mangle]
    pub extern "C" fn accesskit_windows_adapter_handle_wm_getobject(
        adapter: *mut windows_adapter,
        wparam: WPARAM,
        lparam: LPARAM,
    ) -> opt_lresult {
        let adapter = try_ref_from_ptr!(adapter);
        let lresult = adapter.handle_wm_getobject(wparam, lparam);
        opt_lresult::from(lresult)
    }
}

pub type tree_update_factory = Option<extern "C" fn(*mut c_void) -> tree_update>;

pub struct windows_subclassing_adapter {
    _private: [u8; 0],
}

impl CastPtr for windows_subclassing_adapter {
    type RustType = SubclassingAdapter;
}

impl BoxCastPtr for windows_subclassing_adapter {}

impl windows_subclassing_adapter {
    #[no_mangle]
    pub extern "C" fn accesskit_windows_subclassing_adapter_new(
        hwnd: HWND,
        source: tree_update_factory,
        source_userdata: *mut c_void,
        handler: *mut action_handler,
    ) -> *mut windows_subclassing_adapter {
        let source = match source {
            Some(source) => source,
            None => return ptr::null_mut(),
        };
        let handler = match action_handler::to_box(handler) {
            Some(handler) => handler,
            None => return ptr::null_mut(),
        };
        let adapter =
            SubclassingAdapter::new(hwnd, move || source(source_userdata).into(), handler);
        BoxCastPtr::to_mut_ptr(adapter)
    }

    #[no_mangle]
    pub extern "C" fn accesskit_windows_subclassing_adapter_free(
        adapter: *mut windows_subclassing_adapter,
    ) {
        windows_subclassing_adapter::to_box(adapter);
    }

    #[no_mangle]
    pub extern "C" fn accesskit_windows_subclassing_adapter_update(
        adapter: *const windows_subclassing_adapter,
        update: tree_update,
    ) -> *mut windows_queued_events {
        let adapter = try_ref_from_ptr!(adapter);
        let events = adapter.update(update.into());
        BoxCastPtr::to_mut_ptr(events)
    }

    /// This function will return a null pointer if there are no events to raise.
    #[no_mangle]
    pub extern "C" fn accesskit_windows_subclassing_adapter_update_if_active(
        adapter: *const windows_subclassing_adapter,
        update_factory: tree_update_factory,
        update_factory_userdata: *mut c_void,
    ) -> *mut windows_queued_events {
        let update_factory = match update_factory {
            Some(update_factory) => update_factory,
            None => return ptr::null_mut(),
        };
        let adapter = try_ref_from_ptr!(adapter);
        let events = adapter.update_if_active(|| update_factory(update_factory_userdata).into());
        match events {
            Some(events) => BoxCastPtr::to_mut_ptr(events),
            None => ptr::null_mut(),
        }
    }
}
