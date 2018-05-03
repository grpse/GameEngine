#include <Engine.h>
#include <iostream>

class TestScene : public Scene
{
private:
	struct TypeEntry
	{
		const char* name;
		bool selected;
	};

public:
	virtual void start()
	{
		std::cout << "Scene Started!" << std::endl;
		Rect viewport = GameLoop::getInstance().getWindow().getViewport();
		MainCameraFormat.fieldOfView = 45.0f;
		MainCameraFormat.aspectRatio = viewport.width / viewport.height;
		MainCameraFormat.nearPlane = 0.001f;
		MainCameraFormat.farPlane = 10000.0;

		MainCamera.setFormat(MainCameraFormat, Camera::Type::Perspective);
	}

	virtual void onGUI()
	{
		ImGui::Begin(WindowName.c_str());

		ImGui::Text("FPS: %d", FPS);
		ImGui::Separator();
		ImGui::Text("Render Settings:");
		int shadingTypeIndex = SelectShadingTypeFromComboBox();
		int renderModeIndex = SelectRenderModeFromComboBox();
		ImGui::Separator();

		ImGui::End();
	}

	virtual void update(float dt)
	{
		//std::cout << "Scene Update " << dt << " ms" << std::endl;
		SecondsCount += dt;
		FPS_Counter += 1;
		if (SecondsCount >= 1)
		{
			FPS = FPS_Counter;
			SecondsCount = 0;
			FPS_Counter = 0;
		}
	}

	virtual void render()
	{

	}

private:
	std::string WindowName = "Controls";
	Mesh Cow;
	CubeMap SkyMap;
	Camera MainCamera;
	Camera::Format MainCameraFormat;
	uint FPS = 0;
	uint FPS_Counter = 0;
	double SecondsCount = 0;

	int SelectShadingTypeFromComboBox()
	{
		uint i = 0;
		TypeEntry shadingType[3] = { { "Phong", false },{ "GouraudAD", false },{ "GouraudADS", false } };
		static TypeEntry typeSelected = shadingType[0];
		DrawComboSelectType("Shading", typeSelected, shadingType, 3, i);
		return i;
	}

	int SelectRenderModeFromComboBox()
	{
		uint i = 0;
		TypeEntry renderMode[3] = { { "Triangles", false },{ "Lines", false },{ "Points", false } };
		static TypeEntry typeSelected = renderMode[0];
		DrawComboSelectType("Mode", typeSelected, renderMode, 3, i);
		return i;
	}

	void DrawComboSelectType(const char* label, TypeEntry& typeSelected, TypeEntry selectType[], size_t entriesSize, size_t& selectedIndex)
	{
		if (ImGui::BeginCombo(label, typeSelected.name))
		{
			for (int i = 0; i < entriesSize; i++)
			{
				bool isSelected = strcmp(typeSelected.name, selectType[i].name) == 0;
				if (ImGui::Selectable(selectType[i].name, isSelected))
					typeSelected = selectType[i];
				if (isSelected)
				{
					selectedIndex = i;
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}
};

int main() {
	GameLoop loop;
	loop.getWindow().setViewport({ 0, 0, 600, 600 });
	loop.getWindow().start();
	loop.setScene(new TestScene);
	loop.start();
	return 0;
}