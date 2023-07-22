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

#include "simd_dispatch.h"

#include INCLUDE_ARCH_FILE(quote.h)

namespace sonic_json {
namespace internal {

SONIC_USING_ARCH_FUNC(parseStringInplace);
SONIC_USING_ARCH_FUNC(Quote);

}  // namespace internal
}  // namespace sonic_json
