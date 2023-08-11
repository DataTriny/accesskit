// Copyright 2023 The AccessKit Authors. All rights reserved.
// Licensed under the Apache License, Version 2.0 (found in
// the LICENSE-APACHE file) or the MIT license (found in
// the LICENSE-MIT file), at your option.

use crate::{PythonActionHandler, TreeUpdate};
use accesskit_windows::{HWND, LPARAM, WPARAM};
use pyo3::prelude::*;

#[derive(Clone)]
#[pyclass(module = "accesskit.windows")]
pub struct UiaInitMarker(accesskit_windows::UiaInitMarker);

#[pymethods]
impl UiaInitMarker {
    #[new]
    pub fn __new__() -> Self {
        Self(accesskit_windows::UiaInitMarker::new())
    }
}

impl From<UiaInitMarker> for accesskit_windows::UiaInitMarker {
    fn from(marker: UiaInitMarker) -> Self {
        marker.0
    }
}

#[pyclass(module = "accesskit.windows")]
pub struct QueuedEvents(Option<accesskit_windows::QueuedEvents>);

#[pymethods]
impl QueuedEvents {
    pub fn raise_events(&mut self) {
        let events = self.0.take().unwrap();
        events.raise();
    }
}

impl From<accesskit_windows::QueuedEvents> for QueuedEvents {
    fn from(events: accesskit_windows::QueuedEvents) -> Self {
        Self(Some(events))
    }
}

#[pyclass(module = "accesskit.windows")]
pub struct Adapter(accesskit_windows::Adapter);

#[pymethods]
impl Adapter {
    #[new]
    pub fn new(
        hwnd: isize,
        initial_state: TreeUpdate,
        handler: Py<PyAny>,
        uia_init_marker: UiaInitMarker,
    ) -> Self {
        Self(accesskit_windows::Adapter::new(
            HWND(hwnd),
            initial_state.into(),
            Box::new(PythonActionHandler(handler)),
            uia_init_marker.into(),
        ))
    }

    /// You must call `accesskit.windows.QueuedEvents.raise` on the returned value. It can be `None` in case of error.
    pub fn update(&self, update: TreeUpdate) -> QueuedEvents {
        self.0.update(update.into()).into()
    }

    pub fn handle_wm_getobject(&self, wparam: usize, lparam: isize) -> Option<isize> {
        self.0
            .handle_wm_getobject(WPARAM(wparam), LPARAM(lparam))
            .map(|lresult| lresult.into().0)
    }
}

/// This class must only be used from the main thread.
#[pyclass(module = "accesskit.windows", unsendable)]
pub struct SubclassingAdapter(accesskit_windows::SubclassingAdapter);

#[pymethods]
impl SubclassingAdapter {
    #[new]
    pub fn new(hwnd: isize, source: Py<PyAny>, handler: Py<PyAny>) -> Self {
        Self(accesskit_windows::SubclassingAdapter::new(
            HWND(hwnd),
            move || {
                Python::with_gil(|py| {
                    source
                        .call0(py)
                        .unwrap()
                        .extract::<TreeUpdate>(py)
                        .unwrap()
                        .into()
                })
            },
            Box::new(PythonActionHandler(handler)),
        ))
    }

    /// You must call `accesskit.windows.QueuedEvents.raise` on the returned value. It can be `None` in case of error.
    pub fn update(&self, update: TreeUpdate) -> QueuedEvents {
        self.0.update(update.into()).into()
    }

    /// You must call `accesskit.windows.QueuedEvents.raise` on the returned value. It can be `None` in case of error or if the window is not active.
    pub fn update_if_active(
        &self,
        py: Python<'_>,
        update_factory: Py<PyAny>,
    ) -> Option<QueuedEvents> {
        self.0
            .update_if_active(|| {
                let update = update_factory.call0(py).unwrap();
                update.extract::<TreeUpdate>(py).unwrap().into()
            })
            .map(Into::into)
    }
}
