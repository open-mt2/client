#include <Client/UI/Widget/DebugPacksPanel.h>

#if defined(NEOETER_DEBUG_UI)

#include <Client/UI/StdAfx.h>

#include <imgui.h>

#include <Neoeter/Debug/Debug.h>
#include <Neoeter/IO/EterPackManager.h>

#include <Neoeter/Platform/RenderBackend.h>

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

namespace
{
	struct SPreviewTarget
	{
			IDirect3DTexture9* texture = nullptr;
			IDirect3DSurface9* depth = nullptr;
			int width = 0;
			int height = 0;
	};

	enum class EPreviewKind
	{
		None,
		Model,
		Image,
		Text,
	};

	struct SPanelState
	{
			std::vector<CEterPackManager::SPackInfo> packs;
			std::vector<CEterPackManager::SFileInfo> files;
			std::vector<int> filtered;
			std::string filter;
			int selectedPack = -1;
			int selectedFile = -1;
			bool loaded = false;
			std::string previewFile;
			EPreviewKind previewKind = EPreviewKind::None;
			CGraphicThingInstance* instance = nullptr;
			CGraphicThing* thing = nullptr;
			CGraphicImage* image = nullptr;
			std::vector<std::string> textLines;
			bool textTruncated = false;
			SPreviewTarget target;
			float yaw = 35.0f;
			float pitch = 15.0f;
			float zoom = 1.0f;
	};

	SPanelState gs_state;

	bool EndsWithNoCase(const std::string& value, const char* suffix)
	{
		const std::size_t suffixLength = std::strlen(suffix);
		if (value.size() < suffixLength)
			return false;

		for (std::size_t index = 0; index < suffixLength; ++index)
		{
			const unsigned char left = static_cast<unsigned char>(value[value.size() - suffixLength + index]);
			const unsigned char right = static_cast<unsigned char>(suffix[index]);
			if (std::tolower(left) != std::tolower(right))
				return false;
		}

		return true;
	}

	bool ContainsNoCase(const std::string& value, const std::string& needle)
	{
		if (needle.empty())
			return true;

		if (needle.size() > value.size())
			return false;

		for (std::size_t start = 0; start + needle.size() <= value.size(); ++start)
		{
			bool match = true;
			for (std::size_t index = 0; index < needle.size(); ++index)
			{
				const unsigned char left = static_cast<unsigned char>(value[start + index]);
				const unsigned char right = static_cast<unsigned char>(needle[index]);
				if (std::tolower(left) != std::tolower(right))
				{
					match = false;
					break;
				}
			}

			if (match)
				return true;
		}

		return false;
	}

	bool IsImageFile(const std::string& fileName)
	{
		return EndsWithNoCase(fileName, ".dds") ||
			   EndsWithNoCase(fileName, ".tga") ||
			   EndsWithNoCase(fileName, ".jpg") ||
			   EndsWithNoCase(fileName, ".bmp");
	}

	bool IsTextFile(const std::string& fileName)
	{
		return EndsWithNoCase(fileName, ".py") || EndsWithNoCase(fileName, ".txt");
	}

	std::string ExpandTabs(const std::string& line)
	{
		std::string expanded;
		expanded.reserve(line.size());

		for (char character : line)
		{
			if (character == '\t')
				expanded.append(4, ' ');
			else
				expanded.push_back(character);
		}

		return expanded;
	}

	void ReleaseTarget()
	{
		if (gs_state.target.depth)
		{
			gs_state.target.depth->Release();
			gs_state.target.depth = nullptr;
		}

		if (gs_state.target.texture)
		{
			gs_state.target.texture->Release();
			gs_state.target.texture = nullptr;
		}

		gs_state.target.width = 0;
		gs_state.target.height = 0;
	}

	void ReleasePreview()
	{
		if (gs_state.instance)
		{
			CGraphicThingInstance::Delete(gs_state.instance);
			gs_state.instance = nullptr;
		}

		gs_state.thing = nullptr;
		gs_state.image = nullptr;
		gs_state.textLines.clear();
		gs_state.textTruncated = false;
		gs_state.previewKind = EPreviewKind::None;
		gs_state.previewFile.clear();
	}

	void RefreshFilter()
	{
		gs_state.filtered.clear();

		for (std::size_t index = 0; index < gs_state.files.size(); ++index)
		{
			if (ContainsNoCase(gs_state.files[index].name, gs_state.filter))
				gs_state.filtered.push_back(static_cast<int>(index));
		}
	}

	void RefreshPacks()
	{
		gs_state.packs = CEterPackManager::Instance().GetPackInfos();
		gs_state.files.clear();
		gs_state.filtered.clear();
		gs_state.selectedPack = -1;
		gs_state.selectedFile = -1;
		ReleasePreview();
	}

	void SelectPack(int index)
	{
		gs_state.selectedPack = index;
		gs_state.selectedFile = -1;
		gs_state.files.clear();
		gs_state.filtered.clear();
		ReleasePreview();

		if (index < 0 || index >= static_cast<int>(gs_state.packs.size()))
			return;

		gs_state.files = CEterPackManager::Instance().GetPackFiles(gs_state.packs[index].name);
		RefreshFilter();
	}

	bool EnsureTarget(int width, int height)
	{
		if (gs_state.target.texture && gs_state.target.width == width && gs_state.target.height == height)
			return true;

		ReleaseTarget();

		LPDIRECT3DDEVICE9 device = CGraphicBase::GetD3DDevice();
		if (!device)
			return false;

		if (FAILED(device->CreateTexture(
				width,
				height,
				1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT,
				&gs_state.target.texture,
				nullptr)))
			return false;

		if (FAILED(device->CreateDepthStencilSurface(
				width,
				height,
				D3DFMT_D16,
				D3DMULTISAMPLE_NONE,
				0,
				TRUE,
				&gs_state.target.depth,
				nullptr)))
		{
			ReleaseTarget();
			return false;
		}

		gs_state.target.width = width;
		gs_state.target.height = height;
		return true;
	}

	void EnsurePreview(const std::string& fileName)
	{
		if (gs_state.previewFile == fileName)
			return;

		ReleasePreview();
		gs_state.previewFile = fileName;

		if (EndsWithNoCase(fileName, ".gr2"))
		{
			CResource* resource = CResourceManager::Instance().GetResourcePointer(fileName.c_str());
			if (!resource || !resource->IsType(CGraphicThing::Type()))
				return;

			CGraphicThing* thing = static_cast<CGraphicThing*>(resource);
			CGraphicThingInstance* instance = CGraphicThingInstance::New();
			instance->ReserveModelThing(1);
			instance->ReserveModelInstance(1);
			instance->RegisterModelThing(0, thing);
			instance->SetModelInstance(0, 0, 0);
			instance->Show();

			if (thing->GetMotionCount() > 0)
			{
				instance->RegisterMotionThing(0, thing);
				instance->SetMotion(0, 0.0f, 0);
			}

			gs_state.instance = instance;
			gs_state.thing = thing;
			gs_state.previewKind = EPreviewKind::Model;
			return;
		}

		if (IsImageFile(fileName))
		{
			CResource* resource = CResourceManager::Instance().GetResourcePointer(fileName.c_str());
			if (!resource || !resource->IsType(CGraphicImage::Type()))
				return;

			CGraphicImage* image = static_cast<CGraphicImage*>(resource);
			if (!image->GetTexturePointer() || image->GetWidth() <= 0 || image->GetHeight() <= 0)
				return;

			gs_state.image = image;
			gs_state.previewKind = EPreviewKind::Image;
			return;
		}

		if (IsTextFile(fileName))
		{
			CEterPackManager::TPackDataPtr data;
			if (!CEterPackManager::Instance().Get(fileName, data) || !data)
				return;

			const std::size_t limit = 512 * 1024;
			std::size_t size = data->size();
			if (size > limit)
			{
				size = limit;
				gs_state.textTruncated = true;
			}

			const std::string text = size > 0 ? std::string(data->data(), size) : std::string();
			std::size_t start = 0;
			while (start <= text.size())
			{
				const std::size_t end = text.find('\n', start);
				if (end == std::string::npos)
				{
					std::string line = text.substr(start);
					if (!line.empty() && line.back() == '\r')
						line.pop_back();
					gs_state.textLines.push_back(ExpandTabs(line));
					break;
				}

				std::string line = text.substr(start, end - start);
				if (!line.empty() && line.back() == '\r')
					line.pop_back();
				gs_state.textLines.push_back(ExpandTabs(line));
				start = end + 1;
			}

			gs_state.previewKind = EPreviewKind::Text;
		}
	}

	uintptr_t RenderPreview(int width, int height)
	{
		if (!gs_state.instance || width <= 0 || height <= 0 || !EnsureTarget(width, height))
			return 0;

		LPDIRECT3DDEVICE9 device = CGraphicBase::GetD3DDevice();
		CPythonGraphic* graphic = CPythonGraphic::InstancePtr();
		if (!device || !graphic)
			return 0;

		const uintptr_t nativeTexture =
			Platform_GetRenderBackend().GetNativeTexture(gs_state.target.texture->GetTextureHandle());
		if (!nativeTexture)
			return 0;

		IDirect3DSurface9* previousTarget = nullptr;
		IDirect3DSurface9* previousDepth = nullptr;
		D3DVIEWPORT9 previousViewport;
		device->GetRenderTarget(0, &previousTarget);
		device->GetDepthStencilSurface(&previousDepth);
		device->GetViewport(&previousViewport);

		IDirect3DSurface9* colorSurface = nullptr;
		gs_state.target.texture->GetSurfaceLevel(0, &colorSurface);

		if (!colorSurface)
		{
			if (previousDepth)
				previousDepth->Release();
			if (previousTarget)
				previousTarget->Release();
			return 0;
		}

		device->SetRenderTarget(0, colorSurface);
		device->SetDepthStencilSurface(gs_state.target.depth);

		D3DVIEWPORT9 viewport;
		viewport.X = 0;
		viewport.Y = 0;
		viewport.Width = static_cast<uint32_t>(width);
		viewport.Height = static_cast<uint32_t>(height);
		viewport.MinZ = 0.0f;
		viewport.MaxZ = 1.0f;
		device->SetViewport(&viewport);
		device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFF20242C, 1.0f, 0);

		PlatformDword previousZEnable = TRUE;
		PlatformDword previousZWriteEnable = TRUE;
		PlatformDword previousZFunc = D3DCMP_LESSEQUAL;
		PlatformDword previousCullMode = D3DCULL_CW;
		device->GetRenderState(D3DRS_ZENABLE, &previousZEnable);
		device->GetRenderState(D3DRS_ZWRITEENABLE, &previousZWriteEnable);
		device->GetRenderState(D3DRS_ZFUNC, &previousZFunc);
		device->GetRenderState(D3DRS_CULLMODE, &previousCullMode);

		graphic->PushState();
		graphic->SetGameRenderState();
		graphic->SetOmniLight();

		device->SetRenderState(D3DRS_ZENABLE, TRUE);
		device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

		D3DXVECTOR3 minimum;
		D3DXVECTOR3 maximum;
		gs_state.instance->GetBoundBox(&minimum, &maximum);

		D3DXVECTOR3 center = (minimum + maximum) * 0.5f;
		D3DXVECTOR3 size = maximum - minimum;
		float radius = D3DXVec3Length(&size) * 0.5f;

		if (radius <= 0.0f || !(radius < 100000.0f))
		{
			center = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			radius = 100.0f;
		}

		const float distance = std::max(radius * 4.0f / gs_state.zoom, 1.0f);
		const float yaw = D3DXToRadian(gs_state.yaw);
		const float pitch = D3DXToRadian(gs_state.pitch);
		const D3DXVECTOR3 direction(
			std::sin(yaw) * std::cos(pitch),
			-std::cos(yaw) * std::cos(pitch),
			std::sin(pitch));
		const D3DXVECTOR3 eye = center + direction * distance;
		const D3DXVECTOR3 up(0.0f, 0.0f, 1.0f);

		D3DXMATRIX view;
		D3DXMatrixLookAtRH(&view, &eye, &center, &up);
		STATEMANAGER.SetTransform(D3DTS_VIEW, &view);
		graphic->SetPerspective(
			30.0f,
			static_cast<float>(width) / static_cast<float>(height),
			std::max(radius * 0.05f, 0.5f),
			radius * 20.0f + 1000.0f);

		gs_state.instance->Transform();
		gs_state.instance->UpdateTime();
		gs_state.instance->Deform();
		gs_state.instance->Render();
		gs_state.instance->BlendRender();

		device->SetRenderState(D3DRS_ZENABLE, previousZEnable);
		device->SetRenderState(D3DRS_ZWRITEENABLE, previousZWriteEnable);
		device->SetRenderState(D3DRS_ZFUNC, previousZFunc);
		device->SetRenderState(D3DRS_CULLMODE, previousCullMode);

		graphic->PopState();
		graphic->SetInterfaceRenderState();

		device->SetRenderTarget(0, previousTarget);
		device->SetDepthStencilSurface(previousDepth);
		device->SetViewport(&previousViewport);

		if (colorSurface)
			colorSurface->Release();
		if (previousDepth)
			previousDepth->Release();
		if (previousTarget)
			previousTarget->Release();

		return nativeTexture;
	}

	void DrawResourceInfo(const CEterPackManager::SFileInfo& file)
	{
		ImGui::TextUnformatted(file.name.c_str());
		ImGui::Text("Size: %.1f KiB", static_cast<double>(file.size) / 1024.0);

		switch (gs_state.previewKind)
		{
			case EPreviewKind::Model:
			{
				ImGui::Separator();
				ImGui::Text("Models: %d", gs_state.thing->GetModelCount());
				ImGui::Text("Motions: %d", gs_state.thing->GetMotionCount());

				for (int index = 0; index < gs_state.thing->GetModelCount(); ++index)
				{
					CGrannyModel* model = gs_state.thing->GetModelPointer(index);
					if (!model)
						continue;

					ImGui::Text(
						"Model %d: %d vertices, %d indices, %d meshes, %u materials",
						index,
						model->GetVertexCount(),
						model->GetIdxCount(),
						model->GetMeshCount(),
						model->GetMaterialPalette().GetMaterialCount());
				}

				for (int index = 0; index < gs_state.thing->GetMotionCount(); ++index)
				{
					CGrannyMotion* motion = gs_state.thing->GetMotionPointer(index);
					if (!motion)
						continue;

					const char* name = motion->GetName();
					ImGui::BulletText("%s (%.2fs)", name ? name : "", static_cast<double>(motion->GetDuration()));
				}
			}
			break;

			case EPreviewKind::Image:
				ImGui::Text(
					"Dimensions: %d x %d",
					gs_state.image->GetWidth(),
					gs_state.image->GetHeight());
				break;

			case EPreviewKind::Text:
				ImGui::Text("Lines: %d", static_cast<int>(gs_state.textLines.size()));
				if (gs_state.textTruncated)
					ImGui::TextDisabled("Preview truncated to 512 KiB");
				break;

			case EPreviewKind::None:
				break;
		}
	}

	void DrawImagePreview()
	{
		if (!gs_state.image)
			return;

		CGraphicTexture* texture = gs_state.image->GetTexturePointer();
		LPDIRECT3DTEXTURE9 d3dTexture = texture ? texture->GetD3DTexture() : nullptr;
		if (!d3dTexture)
		{
			ImGui::TextUnformatted("Unable to load this image.");
			return;
		}

		const uintptr_t nativeTexture =
			Platform_GetRenderBackend().GetNativeTexture(d3dTexture->GetTextureHandle());
		if (!nativeTexture)
		{
			ImGui::TextUnformatted("Unable to display this image.");
			return;
		}

		const float imageWidth = static_cast<float>(gs_state.image->GetWidth());
		const float imageHeight = static_cast<float>(gs_state.image->GetHeight());
		const ImVec2 available = ImGui::GetContentRegionAvail();

		float scale = std::min(available.x / imageWidth, available.y / imageHeight);
		scale = std::clamp(scale, 0.01f, 4.0f);

		const ImVec2 size(imageWidth * scale, imageHeight * scale);
		const float offsetX = std::max((available.x - size.x) * 0.5f, 0.0f);
		const float offsetY = std::max((available.y - size.y) * 0.5f, 0.0f);

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offsetY);
		ImGui::Image(static_cast<ImTextureID>(nativeTexture), size);
	}

	void DrawTextPreview()
	{
		ImGui::BeginChild("TextPreview", ImVec2(0.0f, 0.0f), true, ImGuiWindowFlags_HorizontalScrollbar);

		ImGuiListClipper clipper;
		clipper.Begin(static_cast<int>(gs_state.textLines.size()));
		while (clipper.Step())
		{
			for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; ++row)
			{
				ImGui::TextDisabled("%5d", row + 1);
				ImGui::SameLine(0.0f, 8.0f);
				ImGui::TextUnformatted(gs_state.textLines[row].c_str());
			}
		}

		ImGui::EndChild();
	}

	void DrawModelPreview()
	{
		const ImVec2 available = ImGui::GetContentRegionAvail();
		float side = std::min(available.x, available.y);
		side = std::max(side, 64.0f);
		side = std::min(side, 640.0f);

		const ImVec2 imagePosition = ImGui::GetCursorScreenPos();
		const ImVec2 imageSize(side, side);

		ImGui::InvisibleButton("##modelpreview", imageSize);

		if (ImGui::IsItemActive())
		{
			const ImVec2 delta = ImGui::GetIO().MouseDelta;
			gs_state.yaw += delta.x * 0.4f;
			gs_state.pitch = std::clamp(gs_state.pitch + delta.y * 0.4f, -80.0f, 80.0f);
		}

		if (ImGui::IsItemHovered() && ImGui::GetIO().MouseWheel != 0.0f)
			gs_state.zoom = std::clamp(gs_state.zoom * (1.0f + ImGui::GetIO().MouseWheel * 0.15f), 0.25f, 6.0f);

		const uintptr_t texture = RenderPreview(static_cast<int>(side), static_cast<int>(side));
		if (!texture)
		{
			ImGui::GetWindowDrawList()->AddRectFilled(
				imagePosition,
				ImVec2(imagePosition.x + side, imagePosition.y + side),
				IM_COL32(32, 36, 44, 255));
			return;
		}

		ImGui::GetWindowDrawList()->AddImage(
			static_cast<ImTextureID>(texture),
			imagePosition,
			ImVec2(imagePosition.x + side, imagePosition.y + side));

		ImGui::TextDisabled("Drag to rotate, wheel to zoom");
	}

	void DrawPacksPanel(bool* visible)
	{
		ImGui::SetNextWindowSize(ImVec2(1080.0f, 640.0f), ImGuiCond_FirstUseEver);

		if (!ImGui::Begin("Packs informations", visible))
		{
			ImGui::End();
			return;
		}

		if (!gs_state.loaded)
		{
			RefreshPacks();
			gs_state.loaded = true;
		}

		if (ImGui::Button("Refresh"))
			RefreshPacks();

		ImGui::SameLine();
		ImGui::Text("%d pack(s) loaded", static_cast<int>(gs_state.packs.size()));
		ImGui::Separator();

		const ImGuiTableFlags tableFlags =
			ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_SizingStretchProp;

		if (ImGui::BeginTable("PackLayout", 3, tableFlags))
		{
			ImGui::TableSetupColumn("Packs", ImGuiTableColumnFlags_WidthFixed, 260.0f);
			ImGui::TableSetupColumn("Resources", ImGuiTableColumnFlags_WidthStretch, 0.45f);
			ImGui::TableSetupColumn("Preview", ImGuiTableColumnFlags_WidthStretch, 0.55f);
			ImGui::TableHeadersRow();

			const float contentHeight = ImGui::GetContentRegionAvail().y;

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::BeginChild("PackList", ImVec2(0.0f, contentHeight), true);
			{
				for (int index = 0; index < static_cast<int>(gs_state.packs.size()); ++index)
				{
					const CEterPackManager::SPackInfo& pack = gs_state.packs[index];

					ImGui::PushID(index);
					if (ImGui::Selectable(pack.name.c_str(), index == gs_state.selectedPack))
						SelectPack(index);
					ImGui::PopID();

					ImGui::TextDisabled(
						"  %s, %d files",
						pack.container.c_str(),
						static_cast<int>(pack.fileCount));
				}
			}
			ImGui::EndChild();

			ImGui::TableNextColumn();
			ImGui::BeginChild("ResourceList", ImVec2(0.0f, contentHeight), true);
			{
				char filter[256] = {};
				snprintf(filter, sizeof(filter), "%s", gs_state.filter.c_str());

				ImGui::SetNextItemWidth(-1.0f);
				if (ImGui::InputTextWithHint("##resourcefilter", "Filter resources", filter, sizeof(filter)))
				{
					gs_state.filter = filter;
					RefreshFilter();
				}

				ImGuiListClipper clipper;
				clipper.Begin(static_cast<int>(gs_state.filtered.size()));
				while (clipper.Step())
				{
					for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; ++row)
					{
						const int fileIndex = gs_state.filtered[row];
						const CEterPackManager::SFileInfo& file = gs_state.files[fileIndex];

						if (ImGui::Selectable(file.name.c_str(), fileIndex == gs_state.selectedFile))
							gs_state.selectedFile = fileIndex;
					}
				}
			}
			ImGui::EndChild();

			ImGui::TableNextColumn();
			ImGui::BeginChild("ResourcePreview", ImVec2(0.0f, contentHeight), true);
			{
				if (gs_state.selectedFile < 0 || gs_state.selectedFile >= static_cast<int>(gs_state.files.size()))
				{
					ImGui::TextUnformatted("Select a resource to inspect it.");
				}
				else
				{
					const CEterPackManager::SFileInfo& file = gs_state.files[gs_state.selectedFile];
					EnsurePreview(file.name);
					DrawResourceInfo(file);
					ImGui::Separator();

					switch (gs_state.previewKind)
					{
						case EPreviewKind::Model:
							DrawModelPreview();
							break;

						case EPreviewKind::Image:
							DrawImagePreview();
							break;

						case EPreviewKind::Text:
							DrawTextPreview();
							break;

						case EPreviewKind::None:
							if (EndsWithNoCase(file.name, ".gr2"))
								ImGui::TextUnformatted("Unable to load this model.");
							else if (IsImageFile(file.name))
								ImGui::TextUnformatted("Unable to load this image.");
							else if (IsTextFile(file.name))
								ImGui::TextUnformatted("Unable to load this file.");
							else
								ImGui::TextUnformatted("No preview available for this file type.");
							break;
					}
				}
			}
			ImGui::EndChild();

			ImGui::EndTable();
		}

		ImGui::End();
	}
}

void DebugPacksPanel_Initialize()
{
	Neoeter::Debug::AddPanel("Debug", "Packs informations", DrawPacksPanel);
}

#endif
