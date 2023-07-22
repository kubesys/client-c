/*
 * Copyright 2022 ByteDance Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "../common/x86_common/itoa.h"

namespace sonic_json {
namespace internal {
namespace sse {

using sonic_json::internal::x86_common::Utoa_16;
using sonic_json::internal::x86_common::Utoa_8;
using sonic_json::internal::x86_common::UtoaSSE;

}  // namespace sse
}  // namespace internal
}  // namespace sonic_json
