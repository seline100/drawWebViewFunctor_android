// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "aw_draw_fn_impl.h"

#include <utility>

#include "AwDrawFnImpl_jni.h"
#include "hwui/DrawGlInfo.h"

//using namespace android;
namespace android_webview {

namespace {

AwDrawFnFunctionTable* g_draw_fn_function_table = nullptr;

void OnSyncWrapper(int functor, void* data, AwDrawFn_OnSyncParams* params) {
  static_cast<AwDrawFnImpl*>(data)->OnSync(params);
}

void OnContextDestroyedWrapper(int functor, void* data) {
  static_cast<AwDrawFnImpl*>(data)->OnContextDestroyed();
}

void OnDestroyedWrapper(int functor, void* data) {
  delete static_cast<AwDrawFnImpl*>(data);
}

void DrawGLWrapper(int functor, void* data, AwDrawFn_DrawGLParams* params) {
  static_cast<AwDrawFnImpl*>(data)->DrawGL(params);
}

void InitVkWrapper(int functor, void* data, AwDrawFn_InitVkParams* params) {
  static_cast<AwDrawFnImpl*>(data)->InitVk(params);
}

void DrawVkWrapper(int functor, void* data, AwDrawFn_DrawVkParams* params) {
  static_cast<AwDrawFnImpl*>(data)->DrawVk(params);
}

void PostDrawVkWrapper(int functor,
                       void* data,
                       AwDrawFn_PostDrawVkParams* params) {
  static_cast<AwDrawFnImpl*>(data)->PostDrawVk(params);
}

void RemoveOverlaysWrapper(int functor,
                           void* data,
                           AwDrawFn_RemoveOverlaysParams* params) {
  static_cast<AwDrawFnImpl*>(data)->RemoveOverlays(params);
}

}  // namespace

void JNI_AwDrawFnImpl_SetDrawFnFunctionTable(JNIEnv* env,
                                                    jlong function_table) {
  g_draw_fn_function_table =
      reinterpret_cast<AwDrawFnFunctionTable*>(function_table);
}

AwDrawFnImpl::AwDrawFnImpl(android::JNIFunctorContext* context) {
  mContext = context;
  static AwDrawFnFunctorCallbacks g_functor_callbacks{
      &OnSyncWrapper,      &OnContextDestroyedWrapper,
      &OnDestroyedWrapper, &DrawGLWrapper,
      &InitVkWrapper,      &DrawVkWrapper,
      &PostDrawVkWrapper,  &RemoveOverlaysWrapper};

  if (g_draw_fn_function_table->version >= 3) {
    functor_handle_ = g_draw_fn_function_table->create_functor_v3(
        this, kAwDrawFnVersion, &g_functor_callbacks);
  } else {
    functor_handle_ =
        g_draw_fn_function_table->create_functor(this, &g_functor_callbacks);
  }
}

AwDrawFnImpl::~AwDrawFnImpl() {
    if (mContext != NULL) {
        delete mContext;
        mContext = NULL;
    }
}

void AwDrawFnImpl::ReleaseHandle(JNIEnv* env) {
  g_draw_fn_function_table->release_functor(functor_handle_);
}

jint AwDrawFnImpl::GetFunctorHandle(JNIEnv* env) {
  return functor_handle_;
}

void AwDrawFnImpl::OnSync(AwDrawFn_OnSyncParams* params) {
    mContext->invoke(android::DrawGlInfo::kModeSync);
}

void AwDrawFnImpl::OnContextDestroyed() {}

void AwDrawFnImpl::DrawGL(AwDrawFn_DrawGLParams* params) {

    auto* gl_info = new android::DrawGlInfo();
    gl_info->clipLeft = params->clip_left;
    gl_info->clipTop = params->clip_top;
    gl_info->clipRight = params->clip_right;
    gl_info->clipBottom = params->clip_bottom;
    gl_info->width = params->width;
    gl_info->height = params->height;
    gl_info->isLayer = false;
    for (int i = 0; i <= 16; i++) {
        gl_info->transform[i] = params->transform[i];
    }

    mContext->draw(android::DrawGlInfo::kModeDraw, gl_info);
}

void AwDrawFnImpl::InitVk(AwDrawFn_InitVkParams* params) {}

void AwDrawFnImpl::DrawVk(AwDrawFn_DrawVkParams* params) {}

void AwDrawFnImpl::PostDrawVk(AwDrawFn_PostDrawVkParams* params) {}

void AwDrawFnImpl::RemoveOverlays(AwDrawFn_RemoveOverlaysParams* params) {}

}  // namespace android_webview
