// Copyright 2023 The AccessKit Authors. All rights reserved.
// Licensed under the Apache License, Version 2.0 (found in
// the LICENSE-APACHE file) or the MIT license (found in
// the LICENSE-MIT file), at your option.

use crate::{PythonActionHandler, Rect, TreeUpdate};
use pyo3::prelude::*;

#[pyclass(module = "accesskit.unix")]
pub struct Adapter(accesskit_unix::Adapter);

#[pymethods]
impl Adapter {
    #[staticmethod]
    pub fn create(
        app_name: &str,
        toolkit_name: &str,
        toolkit_version: &str,
        source: Py<PyAny>,
        handler: Py<PyAny>,
    ) -> Option<Self> {
        accesskit_unix::Adapter::new(
            app_name.into(),
            toolkit_name.into(),
            toolkit_version.into(),
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
        )
        .map(Self)
    }

    pub fn set_root_window_bounds(&mut self, outer: Rect, inner: Rect) {
        self.0.set_root_window_bounds(outer.into(), inner.into());
    }

    pub fn update(&self, update: TreeUpdate) {
        self.0.update(update.into());
    }
}
