// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_COMMON_COMMON_CUSTOM_TYPES_STRUCT_TRAITS_H_
#define MOJO_COMMON_COMMON_CUSTOM_TYPES_STRUCT_TRAITS_H_

#include "base/files/file.h"
#include "base/i18n/rtl.h"
#include "base/strings/utf_string_conversions.h"
#include "base/unguessable_token.h"
#include "base/version.h"
#include "mojo/common/file.mojom-shared.h"
#include "mojo/common/mojo_common_export.h"
#include "mojo/common/string16.mojom-shared.h"
#include "mojo/common/text_direction.mojom-shared.h"
#include "mojo/common/time.mojom-shared.h"
#include "mojo/common/unguessable_token.mojom-shared.h"
#include "mojo/common/version.mojom-shared.h"

namespace mojo {

template <>
struct StructTraits<common::mojom::String16DataView, base::string16> {
  static ConstCArray<uint16_t> data(const base::string16& str) {
    return ConstCArray<uint16_t>(str.size(),
                                 reinterpret_cast<const uint16_t*>(str.data()));
  }

  static bool Read(common::mojom::String16DataView data, base::string16* out);
};

template <>
struct StructTraits<common::mojom::VersionDataView, base::Version> {
  static bool IsNull(const base::Version& version) {
    return !version.IsValid();
  }
  static void SetToNull(base::Version* out) {
    *out = base::Version(std::string());
  }
  static const std::vector<uint32_t>& components(const base::Version& version);
  static bool Read(common::mojom::VersionDataView data, base::Version* out);
};

// If base::UnguessableToken is no longer 128 bits, the logic below and the
// mojom::UnguessableToken type should be updated.
static_assert(sizeof(base::UnguessableToken) == 2 * sizeof(uint64_t),
              "base::UnguessableToken should be of size 2 * sizeof(uint64_t).");

template <>
struct StructTraits<common::mojom::UnguessableTokenDataView,
                    base::UnguessableToken> {
  static uint64_t high(const base::UnguessableToken& token) {
    return token.GetHighForSerialization();
  }

  static uint64_t low(const base::UnguessableToken& token) {
    return token.GetLowForSerialization();
  }

  static bool Read(common::mojom::UnguessableTokenDataView data,
                   base::UnguessableToken* out);
};

template <>
struct StructTraits<common::mojom::FileDataView, base::File> {
  static bool IsNull(const base::File& file) { return !file.IsValid(); }

  static void SetToNull(base::File* file) { *file = base::File(); }

  static mojo::ScopedHandle fd(base::File& file);
  static bool Read(common::mojom::FileDataView data, base::File* file);
};

template <>
struct EnumTraits<common::mojom::TextDirection, base::i18n::TextDirection> {
  static common::mojom::TextDirection ToMojom(
      base::i18n::TextDirection text_direction);
  static bool FromMojom(common::mojom::TextDirection input,
                        base::i18n::TextDirection* out);
};

}  // namespace mojo

#endif  // MOJO_COMMON_COMMON_CUSTOM_TYPES_STRUCT_TRAITS_H_
