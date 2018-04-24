// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_CPP_SYSTEM_HANDLE_SIGNALS_STATE_H_
#define MOJO_PUBLIC_CPP_SYSTEM_HANDLE_SIGNALS_STATE_H_

#include "mojo/public/c/system/types.h"
#include "mojo/public/cpp/system/system_export.h"

namespace mojo {

// A convenience wrapper around the MojoHandleSignalsState struct.
struct MOJO_CPP_SYSTEM_EXPORT HandleSignalsState final
    : public MojoHandleSignalsState {
  HandleSignalsState() {
    satisfied_signals = MOJO_HANDLE_SIGNAL_NONE;
    satisfiable_signals = MOJO_HANDLE_SIGNAL_NONE;
  }

  HandleSignalsState(MojoHandleSignals satisfied,
                     MojoHandleSignals satisfiable) {
    satisfied_signals = satisfied;
    satisfiable_signals = satisfiable;
  }

  bool operator==(const HandleSignalsState& other) const {
    return satisfied_signals == other.satisfied_signals &&
           satisfiable_signals == other.satisfiable_signals;
  }

  // TODO(rockot): Remove this in favor of operator==.
  bool equals(const HandleSignalsState& other) const {
    return satisfied_signals == other.satisfied_signals &&
           satisfiable_signals == other.satisfiable_signals;
  }

  bool satisfies(MojoHandleSignals signals) const {
    return !!(satisfied_signals & signals);
  }

  bool can_satisfy(MojoHandleSignals signals) const {
    return !!(satisfiable_signals & signals);
  }

  // The handle is currently readable. May apply to a message pipe handle or
  // data pipe consumer handle.
  bool readable() const { return satisfies(MOJO_HANDLE_SIGNAL_READABLE); }

  // The handle is currently writable. May apply to a message pipe handle or
  // data pipe producer handle.
  bool writable() const { return satisfies(MOJO_HANDLE_SIGNAL_WRITABLE); }

  // The handle's peer is closed. May apply to any message pipe or data pipe
  // handle.
  bool peer_closed() const { return satisfies(MOJO_HANDLE_SIGNAL_PEER_CLOSED); }

  // The handle will never be |readable()| again.
  bool never_readable() const {
    return !can_satisfy(MOJO_HANDLE_SIGNAL_READABLE);
  }

  // The handle will never be |writable()| again.
  bool never_writable() const {
    return !can_satisfy(MOJO_HANDLE_SIGNAL_WRITABLE);
  }

  // The handle can never indicate |peer_closed()|. Never true for message pipe
  // or data pipe handles (they can always signal peer closure), but always true
  // for other types of handles (they have no peer.)
  bool never_peer_closed() const {
    return !can_satisfy(MOJO_HANDLE_SIGNAL_PEER_CLOSED);
  }

  // (Copy and assignment allowed.)
};

static_assert(sizeof(HandleSignalsState) == sizeof(MojoHandleSignalsState),
              "HandleSignalsState should add no overhead");

}  // namespace mojo

#endif  // MOJO_PUBLIC_CPP_SYSTEM_HANDLE_SIGNALS_STATE_H_
