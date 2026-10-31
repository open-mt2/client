#pragma once

// Start of every rendered frame (open-mt2/devkit#137).
//
// The UI draws over whatever the back buffer holds. A frame that leaves part of
// the screen undrawn shows that buffer's old contents, so the colour buffer has
// to be cleared every frame, not only the depth buffer.
//
// Templated on the graphic so the decision can be tested without a device.

template <typename TGraphic>
inline void BeginFrameClear(TGraphic& graphic)
{
	graphic.ClearDepthBuffer();

#if defined(_DEBUG) || defined(OPENMT2_PLATFORM_LINUX)
	graphic.SetClearColor(0.1f, 0.1f, 0.15f);
	graphic.Clear();
#endif
}
