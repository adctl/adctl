/* Copyright (c) 2014 Google Inc.
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

#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#ifdef __OBJC__
#include <objc/NSObjCRuntime.h>
#endif

#include "gpg/achievement.h"
#include "gpg/achievement_manager.h"
#include "gpg/builder.h"
#include "gpg/debug.h"
#include "gpg/default_callbacks.h"
#include "gpg/game_services.h"
#include "gpg/leaderboard.h"
#include "gpg/leaderboard_manager.h"
#include "gpg/platform_configuration.h"
#include "gpg/player_manager.h"
#include "gpg/score_page.h"
#include "gpg/types.h"
#include <functional>

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>

const int32_t BUFFER_SIZE = 256;

// Wrap macro in do/while to ensure ;
#define LOGI(...) do { \
    char c[BUFFER_SIZE]; \
    snprintf(c, BUFFER_SIZE, __VA_ARGS__); \
    CFStringRef str = CFStringCreateWithCString(kCFAllocatorDefault, c, \
                                                kCFStringEncodingMacRoman); \
    CFShow(str); \
    CFRelease(str); \
  } while (false)

#else

#include "android/log.h"
#define DEBUG_TAG "StateManager"
#define LOGI(...) \
    ((void)__android_log_print(ANDROID_LOG_INFO, DEBUG_TAG, __VA_ARGS__))

#endif // __APPLE__


class StateManager {
 public:
  static void InitServices(
      gpg::PlatformConfiguration const &pc,
      gpg::GameServices::Builder::OnAuthActionStartedCallback started_callback,
      gpg::GameServices::Builder::OnAuthActionFinishedCallback
          finished_callback);
  static gpg::GameServices *GetGameServices();
  static void BeginUserInitiatedSignIn();
  static void SignOut();
  static void UnlockAchievement(const char *achievementId);
  static void SubmitHighScore(const char *leaderboardId, uint64_t score);
  static void ShowAchievements();
  static void ShowLeaderboards();
  static void ShowLeaderboard(const char *leaderboardId);
  static bool IsAuthInProgress() {
    return is_auth_in_progress_;
  }
  /**
  * @brief This callbacks could be set for custom behavior on auth results.
  * By default are null.
  * @note
  */
  static std::function<void(gpg::AuthOperation op)> extra_auth_callback_started;
  static std::function<void(gpg::AuthOperation op, gpg::AuthStatus status)> extra_auth_callback_finished;
  static bool IsSignedIn() { return isSignedIn; }
private:
  static bool isSignedIn;
  static bool is_auth_in_progress_;
  static std::unique_ptr<gpg::GameServices> game_services_;
};

#endif  // STATE_MANAGER_H
