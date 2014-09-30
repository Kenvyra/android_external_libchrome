// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "mojo/system/message_pipe_endpoint.h"

#include "base/logging.h"

namespace mojo {
namespace system {

void MessagePipeEndpoint::Close() {
}

void MessagePipeEndpoint::CancelAllWaiters() {
  NOTREACHED();
}

MojoResult MessagePipeEndpoint::ReadMessage(UserPointer<void> /*bytes*/,
                                            UserPointer<uint32_t> /*num_bytes*/,
                                            DispatcherVector* /*dispatchers*/,
                                            uint32_t* /*num_dispatchers*/,
                                            MojoReadMessageFlags /*flags*/) {
  NOTREACHED();
  return MOJO_RESULT_INTERNAL;
}

HandleSignalsState MessagePipeEndpoint::GetHandleSignalsState() const {
  NOTREACHED();
  return HandleSignalsState();
}

MojoResult MessagePipeEndpoint::AddWaiter(Waiter* /*waiter*/,
                                          MojoHandleSignals /*signals*/,
                                          uint32_t /*context*/,
                                          HandleSignalsState* signals_state) {
  NOTREACHED();
  if (signals_state)
    *signals_state = HandleSignalsState();
  return MOJO_RESULT_INTERNAL;
}

void MessagePipeEndpoint::RemoveWaiter(Waiter* /*waiter*/,
                                       HandleSignalsState* signals_state) {
  NOTREACHED();
  if (signals_state)
    *signals_state = HandleSignalsState();
}

void MessagePipeEndpoint::Attach(ChannelEndpoint* /*channel_endpoint*/) {
  NOTREACHED();
}

}  // namespace system
}  // namespace mojo
