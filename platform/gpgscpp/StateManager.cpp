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

#include "StateManager.h"

bool StateManager::isSignedIn = false;
bool StateManager::is_auth_in_progress_ = false;
std::unique_ptr<gpg::GameServices> StateManager::game_services_;
std::function<void(gpg::AuthOperation op)> StateManager::extra_auth_callback_started = nullptr;
std::function<void(gpg::AuthOperation op, gpg::AuthStatus status)> StateManager::extra_auth_callback_finished = nullptr;

gpg::GameServices *StateManager::GetGameServices() {
  return game_services_.get();
}

void StateManager::BeginUserInitiatedSignIn() {
  if (!game_services_->IsAuthorized()) {
    LOGI("StartAuthorizationUI");
    game_services_->StartAuthorizationUI();
  }
}

void StateManager::SignOut() {
  if (game_services_->IsAuthorized()) {
    LOGI("SignOut");
    game_services_->SignOut();
  }
}

void StateManager::UnlockAchievement(char const *achievement_id) {
  if (game_services_->IsAuthorized()) {
    LOGI("Achievement unlocked");
    game_services_->Achievements().Unlock(achievement_id);
  }
}

void StateManager::SubmitHighScore(char const *leaderboard_id, uint64_t score) {
  if (game_services_->IsAuthorized()) {
    LOGI("High score submitted");
    game_services_->Leaderboards().SubmitScore(leaderboard_id, score);
  }
}

void StateManager::ShowAchievements() {
  if (game_services_->IsAuthorized()) {
    LOGI("Show achievements");
    game_services_->Achievements().ShowAllUI([](gpg::UIStatus const &status) {
        if (gpg::IsSuccess(status)) {
            LOGI("Achievements Result: success.");
        } else {
            LOGI("Achievements Result: failure.");
        }
    });
  }
}

void StateManager::ShowLeaderboards() {
  if (game_services_->IsAuthorized()) {
    LOGI("Show leaderboards");
    game_services_->Leaderboards().ShowAllUI([](gpg::UIStatus const &status) {
        if (gpg::IsSuccess(status)) {
            LOGI("Leaderboards Result: success.");
        } else {
            LOGI("Leaderboards Result: failure.");
        }
    });
  }
}

void StateManager::InitServices(
    gpg::PlatformConfiguration const &pc,
    gpg::GameServices::Builder::OnAuthActionStartedCallback started_callback,
    gpg::GameServices::Builder::OnAuthActionFinishedCallback finished_callback) {

  LOGI("Initializing Services");
  if (!game_services_) {
    LOGI("Uninitialized services, so creating");
    game_services_ = gpg::GameServices::Builder()
        .SetOnAuthActionStarted([started_callback](gpg::AuthOperation op) {
          is_auth_in_progress_ = true;
          if (started_callback != nullptr) {
            started_callback(op);
          }
            if(extra_auth_callback_started != nullptr){
                extra_auth_callback_started(op);
            }
        })
        .SetOnAuthActionFinished([finished_callback](gpg::AuthOperation op,
                                                     gpg::AuthStatus status) {
          LOGI("Sign in finished with a result of %d", status);
          is_auth_in_progress_ = false;
          if (finished_callback != nullptr) {
            finished_callback(op, status);
          }
          if(extra_auth_callback_finished != nullptr){
               extra_auth_callback_finished(op,status);
          }
        })
        .SetDefaultOnLog(gpg::LogLevel::VERBOSE)
        .Create(pc);
  }
  LOGI("Created");
}
