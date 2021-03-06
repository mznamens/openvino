//*****************************************************************************
// Copyright 2017-2020 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//*****************************************************************************

#pragma once

#include <pybind11/pybind11.h>
#include "pyngraph/ops/constant.hpp"
#include "pyngraph/ops/get_output_element.hpp"
#include "pyngraph/ops/parameter.hpp"
#include "pyngraph/ops/result.hpp"

namespace py = pybind11;

void regmodule_pyngraph_op(py::module m);
