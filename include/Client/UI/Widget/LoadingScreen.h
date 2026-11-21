#pragma once

void LoadingScreen_SetVersion(const char* version);
void LoadingScreen_FadeIn(float seconds);
void LoadingScreen_FadeOut(float seconds);
void LoadingScreen_BeginFadeIn(float seconds);
void LoadingScreen_RenderFadeIn();
void LoadingScreen_Show();
void LoadingScreen_ShowError(const char* message);
