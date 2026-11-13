#pragma once

// Geode's Android Cocos binary already contains the WebSocket implementation.
// The public Cocos header only needs these opaque libwebsockets declarations.
struct libwebsocket_context;
struct libwebsocket;
struct libwebsocket_protocols;
enum libwebsocket_callback_reasons : int;
