#pragma once

// Start of every rendered frame (open-mt2/devkit#137).
//
// The UI draws over whatever the back buffer holds. After a GL swap that is an
// older frame, so any region the UI leaves undrawn alternates between stale
// frames. Every GL target (Linux and Windows) clears the colour buffer each
// frame, not only the depth buffer.
//
// Templated on the graphic so the decision can be tested without a device.

template <typename TGraphic>
inline void BeginFrameClear(TGraphic& graphic)
{
	graphic.ClearDepthBuffer();

#if defined(_DEBUG) || defined(OPENMT2_RENDERER_GL)
	graphic.SetClearColor(0.1f, 0.1f, 0.15f);
	graphic.Clear();
#endif
}
