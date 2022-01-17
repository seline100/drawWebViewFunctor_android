// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ANDROID_WEBVIEW_BROWSER_GFX_AW_DRAW_FN_IMPL_H_
#define ANDROID_WEBVIEW_BROWSER_GFX_AW_DRAW_FN_IMPL_H_

#include <jni.h>
#include "draw_fn.h"
#include "DrawGLFunctor.h"

//using namespace android;
namespace android_webview {

class AwDrawFnImpl {
 public:

  AwDrawFnImpl(android::JNIFunctorContext* context);

  AwDrawFnImpl(const AwDrawFnImpl&) = delete;
  AwDrawFnImpl& operator=(const AwDrawFnImpl&) = delete;

  ~AwDrawFnImpl();

  void ReleaseHandle(JNIEnv* env);
  jint GetFunctorHandle(JNIEnv* env);

  int functor_handle() { return functor_handle_; }
  void OnSync(AwDrawFn_OnSyncParams* params);
  void OnContextDestroyed();
  void DrawGL(AwDrawFn_DrawGLParams* params);
  void InitVk(AwDrawFn_InitVkParams* params);
  void DrawVk(AwDrawFn_DrawVkParams* params);
  void PostDrawVk(AwDrawFn_PostDrawVkParams* params);
  void RemoveOverlays(AwDrawFn_RemoveOverlaysParams* params);

 private:
  int functor_handle_;
  android::JNIFunctorContext* mContext;
};

}  // namespace android_webview

#endif  // ANDROID_WEBVIEW_BROWSER_GFX_AW_DRAW_FN_IMPL_H_
