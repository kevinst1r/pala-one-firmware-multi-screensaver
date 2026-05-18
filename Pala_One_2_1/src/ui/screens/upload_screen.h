#ifndef PALA_UI_SCREENS_UPLOAD_SCREEN_H
#define PALA_UI_SCREENS_UPLOAD_SCREEN_H

#include "src/state.h"   // File, FS
#include "src/ui/screen.h"

// ============================================================================
//  Per-session state for a book / sleep-image upload. The struct is public
//  so the upload-stream handlers in `web/web.cpp` can read/write it through
//  `UploadScreen::state()`; only one upload session exists at a time, so
//  storing it as a static member of UploadScreen is appropriate.
// ============================================================================
struct UploadState {
  File bookTmpFile;
  File sleepTmpFile;
  File appTmpFile;

  String bookTmpPath;
  String bookPendingUtf8Tail;
  String bookFinalName;
  bool bookOk = false;
  String bookError;
  // Cross-chunk state for streaming compactText() during upload, so a
  // whitespace or newline run that spans a chunk boundary collapses
  // correctly. Reset in UPLOAD_FILE_START. See pure/text_util.h.
  bool bookCompactLastWasSpace = false;
  int  bookCompactNewlineCount = 0;

  String sleepTmpPath;
  bool sleepOk = false;
  String sleepError;

  // App .bin upload — same shape as book/sleep, third sibling field set.
  // Not pretty (per-type fields rather than a discriminated union) but the
  // rewrite hasn't generalized UploadState yet; see docs/APPS_LAYER.md §8.
  String appTmpPath;
  String appFinalName;
  bool appOk = false;
  String appError;

  uint32_t startedMs = 0;
};

class UploadScreen : public Screen {
public:
  void onEnter() override;
  void onButton(const ButtonEvent& e) override;
  void draw() override;
  void onIdleTick() override;

  // The SoftAP can't keep running while the device deep-sleeps.
  bool allowSleep() const override { return false; }

  // The upload session is a singleton (only one transfer in flight). The web
  // upload-stream handlers in `web/web.cpp` reach the session state through
  // this accessor, avoiding a global `g_upload`.
  static UploadState& state();

private:
  static UploadState s_state;

  void startSession();
  void stopSessionToLibrary();
};

extern UploadScreen g_uploadScreen;

#endif  // PALA_UI_SCREENS_UPLOAD_SCREEN_H
