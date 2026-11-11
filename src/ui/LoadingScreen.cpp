#include <Client/UI/LoadingScreen.h>

#include <Platform/Platform.h>
#include <Neoeter/Platform/Base/Debug.h>
#include <Neoeter/Render/MSWindow.h>

#include <SDL2/SDL.h>

#include <cstdio>
#include <string>
#include <vector>

static std::string g_version = "dev";
#if defined(OPENMT2_RENDERER_GL)
static Uint32 g_fadeInDuration = 0;
static Uint32 g_fadeInElapsed = 0;
static Uint32 g_fadeInLastTick = 0;
static bool g_fadeInPending = false;
#endif

#if defined(OPENMT2_RENDERER_GL)

#include <Platform/GLCompat.h>

namespace
{

	const unsigned char kFont5x7[95 * 5] = {
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x5F,
		0x00,
		0x00,
		0x00,
		0x07,
		0x00,
		0x07,
		0x00,
		0x14,
		0x7F,
		0x14,
		0x7F,
		0x14,
		0x24,
		0x2A,
		0x7F,
		0x2A,
		0x12,
		0x23,
		0x13,
		0x08,
		0x64,
		0x62,
		0x36,
		0x49,
		0x56,
		0x20,
		0x50,
		0x00,
		0x08,
		0x07,
		0x03,
		0x00,
		0x00,
		0x1C,
		0x22,
		0x41,
		0x00,
		0x00,
		0x41,
		0x22,
		0x1C,
		0x00,
		0x2A,
		0x1C,
		0x7F,
		0x1C,
		0x2A,
		0x08,
		0x08,
		0x3E,
		0x08,
		0x08,
		0x00,
		0x80,
		0x70,
		0x30,
		0x00,
		0x08,
		0x08,
		0x08,
		0x08,
		0x08,
		0x00,
		0x00,
		0x60,
		0x60,
		0x00,
		0x20,
		0x10,
		0x08,
		0x04,
		0x02,
		0x3E,
		0x51,
		0x49,
		0x45,
		0x3E,
		0x00,
		0x42,
		0x7F,
		0x40,
		0x00,
		0x72,
		0x49,
		0x49,
		0x49,
		0x46,
		0x21,
		0x41,
		0x49,
		0x4D,
		0x33,
		0x18,
		0x14,
		0x12,
		0x7F,
		0x10,
		0x27,
		0x45,
		0x45,
		0x45,
		0x39,
		0x3C,
		0x4A,
		0x49,
		0x49,
		0x31,
		0x41,
		0x21,
		0x11,
		0x09,
		0x07,
		0x36,
		0x49,
		0x49,
		0x49,
		0x36,
		0x46,
		0x49,
		0x49,
		0x29,
		0x1E,
		0x00,
		0x00,
		0x14,
		0x00,
		0x00,
		0x00,
		0x40,
		0x34,
		0x00,
		0x00,
		0x00,
		0x08,
		0x14,
		0x22,
		0x41,
		0x14,
		0x14,
		0x14,
		0x14,
		0x14,
		0x00,
		0x41,
		0x22,
		0x14,
		0x08,
		0x02,
		0x01,
		0x59,
		0x09,
		0x06,
		0x3E,
		0x41,
		0x5D,
		0x59,
		0x4E,
		0x7C,
		0x12,
		0x11,
		0x12,
		0x7C,
		0x7F,
		0x49,
		0x49,
		0x49,
		0x36,
		0x3E,
		0x41,
		0x41,
		0x41,
		0x22,
		0x7F,
		0x41,
		0x41,
		0x41,
		0x3E,
		0x7F,
		0x49,
		0x49,
		0x49,
		0x41,
		0x7F,
		0x09,
		0x09,
		0x09,
		0x01,
		0x3E,
		0x41,
		0x41,
		0x51,
		0x73,
		0x7F,
		0x08,
		0x08,
		0x08,
		0x7F,
		0x00,
		0x41,
		0x7F,
		0x41,
		0x00,
		0x20,
		0x40,
		0x41,
		0x3F,
		0x01,
		0x7F,
		0x08,
		0x14,
		0x22,
		0x41,
		0x7F,
		0x40,
		0x40,
		0x40,
		0x40,
		0x7F,
		0x02,
		0x1C,
		0x02,
		0x7F,
		0x7F,
		0x04,
		0x08,
		0x10,
		0x7F,
		0x3E,
		0x41,
		0x41,
		0x41,
		0x3E,
		0x7F,
		0x09,
		0x09,
		0x09,
		0x06,
		0x3E,
		0x41,
		0x51,
		0x21,
		0x5E,
		0x7F,
		0x09,
		0x19,
		0x29,
		0x46,
		0x26,
		0x49,
		0x49,
		0x49,
		0x32,
		0x03,
		0x01,
		0x7F,
		0x01,
		0x03,
		0x3F,
		0x40,
		0x40,
		0x40,
		0x3F,
		0x1F,
		0x20,
		0x40,
		0x20,
		0x1F,
		0x3F,
		0x40,
		0x38,
		0x40,
		0x3F,
		0x63,
		0x14,
		0x08,
		0x14,
		0x63,
		0x03,
		0x04,
		0x78,
		0x04,
		0x03,
		0x61,
		0x59,
		0x49,
		0x4D,
		0x43,
		0x00,
		0x7F,
		0x41,
		0x41,
		0x41,
		0x02,
		0x04,
		0x08,
		0x10,
		0x20,
		0x00,
		0x41,
		0x41,
		0x41,
		0x7F,
		0x04,
		0x02,
		0x01,
		0x02,
		0x04,
		0x40,
		0x40,
		0x40,
		0x40,
		0x40,
		0x00,
		0x03,
		0x07,
		0x08,
		0x00,
		0x20,
		0x54,
		0x54,
		0x78,
		0x40,
		0x7F,
		0x28,
		0x44,
		0x44,
		0x38,
		0x38,
		0x44,
		0x44,
		0x44,
		0x28,
		0x38,
		0x44,
		0x44,
		0x28,
		0x7F,
		0x38,
		0x54,
		0x54,
		0x54,
		0x18,
		0x00,
		0x08,
		0x7E,
		0x09,
		0x02,
		0x18,
		0x24,
		0x24,
		0x1C,
		0x78,
		0x7F,
		0x08,
		0x04,
		0x04,
		0x78,
		0x00,
		0x44,
		0x7D,
		0x40,
		0x00,
		0x20,
		0x40,
		0x40,
		0x3D,
		0x00,
		0x7F,
		0x10,
		0x28,
		0x44,
		0x00,
		0x00,
		0x41,
		0x7F,
		0x40,
		0x00,
		0x7C,
		0x04,
		0x78,
		0x04,
		0x78,
		0x7C,
		0x08,
		0x04,
		0x04,
		0x78,
		0x38,
		0x44,
		0x44,
		0x44,
		0x38,
		0x7C,
		0x18,
		0x24,
		0x24,
		0x18,
		0x18,
		0x24,
		0x24,
		0x18,
		0x7C,
		0x7C,
		0x08,
		0x04,
		0x04,
		0x08,
		0x48,
		0x54,
		0x54,
		0x54,
		0x24,
		0x04,
		0x04,
		0x3F,
		0x44,
		0x24,
		0x3C,
		0x40,
		0x40,
		0x20,
		0x7C,
		0x1C,
		0x20,
		0x40,
		0x20,
		0x1C,
		0x3C,
		0x40,
		0x30,
		0x40,
		0x3C,
		0x44,
		0x28,
		0x10,
		0x28,
		0x44,
		0x4C,
		0x90,
		0x90,
		0x90,
		0x7C,
		0x44,
		0x64,
		0x54,
		0x4C,
		0x44,
		0x00,
		0x08,
		0x36,
		0x41,
		0x00,
		0x00,
		0x00,
		0x77,
		0x00,
		0x00,
		0x00,
		0x41,
		0x36,
		0x08,
		0x00,
		0x02,
		0x01,
		0x02,
		0x04,
		0x02,
	};

	int TextWidth(const std::string& text, float scale)
	{
		int length = static_cast<int>(text.size());
		if (length <= 0)
			return 0;
		return static_cast<int>(length * 6 * scale - scale);
	}

	void DrawText(const std::string& text, float x, float y, float scale, float r,
				  float g, float b, float alpha)
	{
		glColor4f(r, g, b, alpha);
		glBegin(GL_QUADS);

		float cursor = x;
		for (size_t i = 0; i < text.size(); ++i)
		{
			unsigned char c = static_cast<unsigned char>(text[i]);
			if (c == '\t')
				c = ' ';
			if (c < 32 || c > 126)
				c = '?';

			const unsigned char* glyph = &kFont5x7[(c - 32) * 5];
			for (int column = 0; column < 5; ++column)
			{
				for (int row = 0; row < 7; ++row)
				{
					if (glyph[column] & (1 << row))
					{
						float px = cursor + column * scale;
						float py = y + row * scale;
						glVertex2f(px, py);
						glVertex2f(px + scale, py);
						glVertex2f(px + scale, py + scale);
						glVertex2f(px, py + scale);
					}
				}
			}
			cursor += 6 * scale;
		}

		glEnd();
	}

	std::string FitLine(const std::string& line, int maxCharacters)
	{
		if (maxCharacters <= 0 || static_cast<int>(line.size()) <= maxCharacters)
			return line;
		return line.substr(line.size() - maxCharacters);
	}

	std::vector<std::string> ReadLogTail(size_t maxLines)
	{
		std::vector<std::string> lines;

		FILE* file = fopen("log.txt", "rt");
		if (!file)
			return lines;

		std::string data;
		char buffer[4096];
		size_t count = 0;
		while ((count = fread(buffer, 1, sizeof(buffer), file)) > 0)
		{
			data.append(buffer, count);
			if (data.size() > 131072)
				data.erase(0, data.size() - 65536);
		}
		fclose(file);

		size_t start = 0;
		while (start < data.size())
		{
			size_t end = data.find('\n', start);
			if (end == std::string::npos)
				end = data.size();

			std::string line = data.substr(start, end - start);
			if (!line.empty() && line.back() == '\r')
				line.pop_back();

			if (!line.empty())
			{
				if (line.size() > 20 && line[4] == ' ' && line[7] == ':' &&
					line[10] == ':' && line[16] == ' ' && line[17] == ':' &&
					line[18] == ':')
					line = line.substr(20);
				lines.push_back(line);
			}

			start = end + 1;
		}

		if (lines.size() > maxLines)
			lines.erase(lines.begin(), lines.end() - maxLines);

		return lines;
	}

	void DrawFrame(float alpha)
	{
		SDL_Window* window = CMSWindow::GetSDLWindow();
		SDL_GLContext context = CMSWindow::GetGLContext();
		if (!window || !context)
			return;

		SDL_GL_MakeCurrent(window, context);

		int width = 0;
		int height = 0;
		SDL_GetWindowSize(window, &width, &height);
		if (width <= 0 || height <= 0)
			return;

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
		GLint savedViewport[4];
		glGetIntegerv(GL_VIEWPORT, savedViewport);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0.0, static_cast<double>(width), static_cast<double>(height), 0.0,
				-1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float textScale = static_cast<float>(width) / 640.0f;
		if (textScale < 1.0f)
			textScale = 1.0f;
		if (textScale > 2.0f)
			textScale = 2.0f;

		const float margin = 16.0f;

		float versionY = height - margin - 7.0f * textScale;
		int versionWidth = TextWidth(g_version, textScale);
		DrawText(g_version, width - margin - versionWidth, versionY, textScale, 0.85f,
				 0.87f, 0.90f, alpha);

		const std::string title = "OpenMT2";
		float titleScale = textScale * 1.25f;
		int titleWidth = TextWidth(title, titleScale);
		float titleY = versionY - 7.0f * titleScale - 3.0f * textScale;
		DrawText(title, width - margin - titleWidth, titleY, titleScale, 1.0f, 1.0f,
				 1.0f, alpha);

		int rightColumnWidth = versionWidth;
		if (titleWidth > rightColumnWidth)
			rightColumnWidth = titleWidth;

		int maxLogWidth = width - 2 * margin - rightColumnWidth - 8 * textScale;
		if (maxLogWidth < 0)
			maxLogWidth = 0;
		int maxCharacters = static_cast<int>(maxLogWidth / (6.0f * textScale));
		if (maxCharacters < 0)
			maxCharacters = 0;

		std::vector<std::string> lines = ReadLogTail(16);
		float lineHeight = 9.0f * textScale;
		float logTopLimit = margin + 7.0f * textScale;
		int maxLines = static_cast<int>((versionY - logTopLimit) / lineHeight) + 1;
		if (maxLines < 0)
			maxLines = 0;
		if (maxLines > 12)
			maxLines = 12;

		int count = static_cast<int>(lines.size());
		if (count > maxLines)
			count = maxLines;

		for (int i = 0; i < count; ++i)
		{
			std::string line =
				FitLine(lines[lines.size() - count + i], maxCharacters);
			float y = versionY - static_cast<float>(count - 1 - i) * lineHeight;
			DrawText(line, margin, y, textScale, 0.55f, 0.58f, 0.62f, alpha);
		}

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glViewport(savedViewport[0], savedViewport[1], savedViewport[2], savedViewport[3]);
		glPopClientAttrib();
		glPopAttrib();

		SDL_GL_SwapWindow(window);
	}

	void DrawFadeOverlay(float alpha)
	{
		SDL_Window* window = CMSWindow::GetSDLWindow();
		SDL_GLContext context = CMSWindow::GetGLContext();
		if (!window || !context)
			return;

		SDL_GL_MakeCurrent(window, context);

		int width = 0;
		int height = 0;
		SDL_GL_GetDrawableSize(window, &width, &height);
		if (width <= 0 || height <= 0)
			return;

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
		GLint savedViewport[4];
		glGetIntegerv(GL_VIEWPORT, savedViewport);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0.0, static_cast<double>(width), static_cast<double>(height), 0.0,
				-1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glDisable(GL_SCISSOR_TEST);
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_FOG);
		glDisable(GL_STENCIL_TEST);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		GLint savedActiveTexture = GL_TEXTURE0;
		glGetIntegerv(GL_ACTIVE_TEXTURE, &savedActiveTexture);
		GLint maxTextureUnits = 1;
		glGetIntegerv(GL_MAX_TEXTURE_UNITS, &maxTextureUnits);
		if (maxTextureUnits > 8)
			maxTextureUnits = 8;
		for (GLint unit = 0; unit < maxTextureUnits; ++unit)
		{
			glActiveTexture(GL_TEXTURE0 + unit);
			glDisable(GL_TEXTURE_2D);
		}

		glColor4f(0.0f, 0.0f, 0.0f, alpha);
		glBegin(GL_QUADS);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(static_cast<float>(width), 0.0f);
		glVertex2f(static_cast<float>(width), static_cast<float>(height));
		glVertex2f(0.0f, static_cast<float>(height));
		glEnd();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glActiveTexture(savedActiveTexture);
		glViewport(savedViewport[0], savedViewport[1], savedViewport[2], savedViewport[3]);
		glPopClientAttrib();
		glPopAttrib();
	}

}

#endif

void LoadingScreen_SetVersion(const char* version)
{
	g_version = version ? version : "";
}

void LoadingScreen_FadeIn(float seconds)
{
#if defined(OPENMT2_RENDERER_GL)
	if (seconds <= 0.0f)
	{
		LoadingScreen_Show();
		return;
	}

	Uint32 startedAt = SDL_GetTicks();
	for (;;)
	{
		float t = static_cast<float>(SDL_GetTicks() - startedAt) /
				  (seconds * 1000.0f);
		if (t > 1.0f)
			t = 1.0f;

		DrawFrame(t * t * (3.0f - 2.0f * t));
		SDL_PumpEvents();

		if (t >= 1.0f)
			break;
		SDL_Delay(16);
	}
#else
	(void)seconds;
#endif
}

void LoadingScreen_FadeOut(float seconds)
{
#if defined(OPENMT2_RENDERER_GL)
	if (seconds <= 0.0f)
	{
		DrawFrame(0.0f);
	}
	else
	{
		Uint32 startedAt = SDL_GetTicks();
		for (;;)
		{
			float t = static_cast<float>(SDL_GetTicks() - startedAt) /
					  (seconds * 1000.0f);
			if (t >= 1.0f)
			{
				DrawFrame(0.0f);
				break;
			}

			float eased = t * t * (3.0f - 2.0f * t);
			DrawFrame(1.0f - eased);
			SDL_PumpEvents();
			SDL_Delay(16);
		}
	}
#else
	(void)seconds;
#endif
}

void LoadingScreen_BeginFadeIn(float seconds)
{
#if defined(OPENMT2_RENDERER_GL)
	g_fadeInDuration = static_cast<Uint32>(seconds * 1000.0f);
	g_fadeInElapsed = 0;
	g_fadeInLastTick = 0;
	g_fadeInPending = seconds > 0.0f;
	Tracef("[Fade] BeginFadeIn %.2fs\n", seconds);
#else
	(void)seconds;
#endif
}

void LoadingScreen_RenderFadeIn()
{
#if defined(OPENMT2_RENDERER_GL)
	if (!g_fadeInPending)
		return;

	Uint32 now = SDL_GetTicks();
	if (g_fadeInLastTick == 0)
	{
		g_fadeInLastTick = now;
		Tracef("[Fade] Fade-in started\n");
		DrawFadeOverlay(1.0f);
		return;
	}

	Uint32 delta = now - g_fadeInLastTick;
	g_fadeInLastTick = now;
	if (delta > 100)
		delta = 100;

	g_fadeInElapsed += delta;
	if (g_fadeInElapsed >= g_fadeInDuration)
	{
		g_fadeInPending = false;
		Tracef("[Fade] Fade-in finished\n");
		return;
	}

	float t = static_cast<float>(g_fadeInElapsed) /
			  static_cast<float>(g_fadeInDuration);
	float eased = t * t * (3.0f - 2.0f * t);
	DrawFadeOverlay(1.0f - eased);
#endif
}

void LoadingScreen_Show()
{
#if defined(OPENMT2_RENDERER_GL)
	DrawFrame(1.0f);
#endif
}

void LoadingScreen_ShowError(const char* message)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "OpenMT2", message,
							 CMSWindow::GetSDLWindow());
}
