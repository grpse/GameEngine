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

	enum class ShadingType {
		Phong = 0, 
		GouraudAD = 1, 
		GouraudADS = 2 
	};

	enum class RenderEngine {
		OpenGL = 0,
		Close2GL = 1
	};

	enum class ClockOrientation {
		CounterClockwise = 0,
		Clockwise = 1
	};

	

public:
	virtual void start()
	{
		std::cout << "Scene Started!" << std::endl;

		// SETUP CAMERA
		Rect viewport = GameLoop::getInstance().getWindow().getViewport();
		MainCameraFormat.fieldOfView = 45.0f;
		MainCameraFormat.aspectRatio = viewport.width / viewport.height;
		MainCameraFormat.nearPlane = 0.001f;
		MainCameraFormat.farPlane = 10000.0;
		OriginalMainCameraFormat = MainCameraFormat;

		MainCamera.setFormat(MainCameraFormat, Camera::Type::Perspective);
		MainCamera.transform.setLocalPosition({ 0, 2, -10 });
		MainCamera.transform.setLocalRotation(Vector3(3.1415f / 2.0f, 3.1415f, 0));
		OriginalCameraPosition = CameraPosition = MainCamera.transform.getLocalPosition();
		OriginalCameraRotation = CameraRotation = Math::eulerAngles(MainCamera.transform.getLocalRotation());
		
		// SETUP SKY
		SkyMap = Loader::loadCubeMap("skybox/cloudysky", "png");
		directional.direction = Vector3(-1, -1, -1);
		directional.color = Color32(1, 1, 1, 1);
		directional.intensity = 1;

		// SETUP COW
		Cow = Loader::loadMeshAsArrayForDynamic("cow_up.in", 0.001f, true);
		OriginalCowTransform.setLocalPosition({ 0, 0, 0 });
		OriginalCowTransform.setLocalScale({ 1, 1, 1 });
		OriginalCowTransform.setLocalRotation(Vector3( 0, 0, 0 ));
		CowTransform = OriginalCowTransform;

		// SETUP SHADERS
		SkyShaderProgram.addProgram(SkyboxShader);
		ShaderPrograms[(int)RenderEngine::OpenGL][(int)ShadingType::Phong].addProgram(PhongShading);
		ShaderPrograms[(int)RenderEngine::OpenGL][(int)ShadingType::GouraudAD].addProgram(GouraudAD);
		ShaderPrograms[(int)RenderEngine::OpenGL][(int)ShadingType::GouraudADS].addProgram(GouraudADS);
		ShaderPrograms[(int)RenderEngine::Close2GL][(int)ShadingType::Phong].addProgram(PhongSoftwareShading);
		ShaderPrograms[(int)RenderEngine::Close2GL][(int)ShadingType::GouraudAD].addProgram(GouraudSoftwareADShading);
		ShaderPrograms[(int)RenderEngine::Close2GL][(int)ShadingType::GouraudADS].addProgram(GouraudSoftwareADSShading);

		ShaderProgram::build();
	}

	virtual void onGUI()
	{
		ImGui::Begin(WindowName.c_str());
		ImGui::Text("FPS: %d", FPS);

		ImGui::Separator();
		ImGui::Text("Render Settings:");
		CurrentShadingType = (ShadingType)SelectShadingTypeFromComboBox();
		CurrentRenderMode = (Renderer::Mode)SelectRenderModeFromComboBox();
		CurrentRenderEngine = (RenderEngine)SelectRenderEngineFromComboBox();
		CurrentClockOrientation = (ClockOrientation)SelectClockDrawOrientation();
		ImGui::Checkbox("Backface Culling", &BackfaceCulling);
		ImGui::Separator();
		
		ImGui::Text("Camera Settings:");
		ImGui::DragFloat("Aspect Ratio", &AspectRatio, 0.01f, 0.01f, 5.0f);
		ImGui::DragFloat("Field Of View", &FieldOfView, 0.001f, 30.0f, 120.0f);
		ImGui::DragFloat("Near Plane", &NearPlane, 0.0001f, 0.0001f, 1.0f);
		ImGui::DragFloat("Far Plane", &FarPlane, 0.0001f, 1.0f, 10000.0f);
		ImGui::Separator();

		ImGui::Text("Cow Settings:");
		ImGui::DragFloat3("Position", Math::value_ptr(CowPosition), 0.001f);
		ImGui::DragFloat3("Scale", Math::value_ptr(CowScale), 0.001f);
		ImGui::DragFloat3("Rotation", Math::value_ptr(CowRotation), 0.001f);
		ImGui::ColorPicker4("Color", Math::value_ptr(CowColor));
		ImGui::Separator();

		ImGui::Button("Reset");

		ImGui::End();
	}

	virtual void update(float dt)
	{
		SecondsCount += dt;
		FPS_Counter += 1;
		if (SecondsCount >= 1)
		{
			FPS = FPS_Counter;
			SecondsCount = 0;
			FPS_Counter = 0;
		}

		// UPDATE HEADER
		if (CurrentRenderEngine == RenderEngine::OpenGL)
		{
			WindowName = "OpenGL";
			wasOpenGLLastTime = true;
		}
		else
		{
			WindowName = "Close2GL";
			if (wasOpenGLLastTime)
			{
				hasUploadedToOpenGLVerticesAgain = false;
				wasOpenGLLastTime = false;
			}
		}

		// UPDATE RENDER SETTINGS
		CurrentShaderProgram = ShaderPrograms[(int)CurrentRenderEngine][(int)CurrentShadingType];
		if (CurrentClockOrientation == ClockOrientation::CounterClockwise)
		{
			renderer.setFrontCounterClockwise();
		}
		else
		{
			renderer.setFrontClockwise();
		}

		// UPDATE CAMERA
		MainCameraFormat.fieldOfView = FieldOfView;
		MainCameraFormat.aspectRatio = AspectRatio;
		MainCameraFormat.nearPlane = NearPlane;
		MainCameraFormat.farPlane = FarPlane;
		MainCamera.setFormat(MainCameraFormat, Camera::Type::Perspective);
		updateCameraPosition(dt);
		updateCameraDistanceFromCow(dt);

		// UPDATE COW SETTINGS
		CowTransform.setLocalPosition(CowPosition);
		CowTransform.setLocalScale(CowScale);
		CowTransform.setLocalRotation(CowRotation);

	}

	void updateCameraPosition(float dt)
	{
		double x, y;
		Input::getMouseDelta(x, y);

		if (Input::isMouseButtonPress(MouseLeftButton) || useCamera)
		{
			useCamera = Input::isMouseButtonRelease(MouseLeftButton) ^ true;

			float rotationDiff = (float)(5.0 * Time::getDeltaTime());

			if (x > 0 || x < 0) {
				CameraXangle += Math::radians(x);

				//xAngle += Math::radians(x);
				CameraPosition.x = Math::sin(CameraXangle) * distanceFromCow;
				CameraPosition.z = -Math::cos(CameraXangle) * distanceFromCow;
			}

			if (y > 0 || y < 0) {
				//const float halfPi = Math::pi<float>() / 2.0f;
				//float nextRotationOnX = rotation.x - y * rotationDiff;
				//rotation.x = Math::clamp(nextRotationOnX, -halfPi, halfPi);
				double tempYAngle = CameraYangle + Math::radians(y);
				CameraYangle = Math::abs(tempYAngle) >= Math::pi<double>() / 2 ? CameraYangle : tempYAngle;
				CameraPosition.y = Math::sin(CameraYangle) * distanceFromCow;
			}

			// UPDATE ORIENTATION
			MainCamera.transform.setLocalPosition(CameraPosition);
			MainCamera.transform.lookAt(CowTransform.getLocalPosition());
		}
	}

	void updateCameraDistanceFromCow(float dt)
	{
		double scrollX, scrollY;
		Input::getScrollOffsetDelta(scrollX, scrollY);
		if (scrollY > 0 || scrollY < 0) {
			distanceFromCow += scrollY * 10 * Time::getDeltaTime();
			distanceFromCow = Math::max(0.01, distanceFromCow);
		}
	}

	void updateCamera3thPersonWalk(float dt)
	{
		float moveDiff = 40.0f * dt;

		Vector3 front = MainCamera.transform.getFront();
		Vector3 right = MainCamera.transform.getRight();
		if (Input::isPressedKeyRepeated(W)) {
			CameraPosition += Math::normalize(front) * moveDiff;
		}

		if (Input::isPressedKeyRepeated(S)) {
			CameraPosition -= Math::normalize(front) * moveDiff;
		}

		if (Input::isPressedKeyRepeated(D)) {
			CameraPosition += Math::normalize(right) * moveDiff;
		}

		if (Input::isPressedKeyRepeated(A)) {
			CameraPosition -= Math::normalize(right) * moveDiff;
		}

		MainCamera.transform.setLocalPosition(CameraPosition);
	}

	void updateCamera3thPersonLookup(float dt)
	{
		double x, y;
		Input::getMouseDelta(x, y);

		float rotationDiff = 5.0f * dt;

		if (x > 0 || x < 0) {
			CameraRotation.y += x * rotationDiff;
		}

		if (y > 0 || y < 0) {
			const float halfPi = Math::pi<float>() / 2.0f;
			float nextRotationOnX = CameraRotation.x - y * rotationDiff;
			CameraRotation.x = Math::clamp(nextRotationOnX, -halfPi, halfPi);
		}

		MainCamera.transform.setLocalRotation(CameraRotation);
	}

	virtual void render()
	{		
		// THIS UPDATES CAMERA TRANSFORM
		MainCamera.transform.getWorldMatrix();
		Rect viewport = GameLoop::getInstance().getWindow().getViewport();

		renderer.setClearColor({ 1, 1, 1, 1 });
		renderer.clearColorAndDepth();
		renderer.setViewport(viewport);

		// RENDER SKY
		renderer.setRenderMode(Renderer::Mode::Triangles);
		renderer.setDepthMask();
		renderer.disableDepthTest();

		skyboxRenderer.render(MainCamera, SkyMap, renderer);


		// RENDER COW
		renderer.enableDepthTest();
		renderer.setDepthMask();

		if (CullbackFaces) 
		{
			renderer.enableCullFace();
			renderer.cullBackFace();
		}
		else
		{
			renderer.disableCullFace();
		}

		CurrentShaderProgram.setUniform("light.position", directional.position);
		CurrentShaderProgram.setUniform("light.direction", directional.direction);
		CurrentShaderProgram.setUniform("light.color", directional.color);
		CurrentShaderProgram.setUniform("light.intensity", directional.intensity);
		CurrentShaderProgram.setUniform("DiffuseColor", CowColor);

		renderer.setRenderMode(CurrentRenderMode);
		if (CurrentRenderEngine == RenderEngine::OpenGL)
		{
			meshRenderer.setShader(CurrentShaderProgram);
			meshRenderer.render(MainCamera, Cow, CowTransform, renderer);
		}
		else if (CurrentRenderEngine == RenderEngine::Close2GL)
		{
			if (!hasUploadedToOpenGLVerticesAgain)
			{
				Cow.getVertexArray().updateBuffer(0, Cow.getVertices().data(), Cow.getVertices().size() * sizeof(Vertex));
				hasUploadedToOpenGLVerticesAgain = true;
			}
			meshSoftwareRenderer.setShader(CurrentShaderProgram);
			meshSoftwareRenderer.render(MainCamera, Cow, CowTransform, renderer);
		}
	}

private:
	std::string WindowName = "Controls";
	Renderer renderer;
	Mesh Cow;
	Transform CowTransform;
	Transform OriginalCowTransform;
	Transform OriginalCameraTransform;
	Vector3 CameraPosition, CameraRotation, OriginalCameraPosition, OriginalCameraRotation;
	double CameraXangle = 0, CameraYangle = 0, distanceFromCow = 1.0f;
	
	Light directional;
	MeshRenderer meshRenderer;
	MeshSoftwareRenderer meshSoftwareRenderer;
	SkyboxRenderer skyboxRenderer;
	CubeMap SkyMap;
	ShaderProgram SkyShaderProgram;
	ShaderProgram CurrentShaderProgram;
	ShaderProgram ShaderPrograms[2][3];
	Camera MainCamera;
	Camera::Format MainCameraFormat;
	Camera::Format OriginalMainCameraFormat;

	uint FPS = 0;
	uint FPS_Counter = 0;
	double SecondsCount = 0;
	bool hasUploadedToOpenGLVerticesAgain = false;
	bool wasOpenGLLastTime = false;
	bool useCamera = false;
	bool CullbackFaces;

	float FarPlane = 10000.0f, NearPlane = 0.0001f, FieldOfView = Math::radians(45.0f), AspectRatio = 4.0f/3.0f;
	Vector3 CowPosition = Vector3(0), CowRotation = Vector3(0), CowScale = Vector3(1);
	Color32 CowColor = { 1, 1, 1, 1 };
	bool BackfaceCulling = false;
	ShadingType CurrentShadingType = ShadingType::Phong;
	Renderer::Mode CurrentRenderMode = Renderer::Mode::Triangles;
	RenderEngine CurrentRenderEngine = RenderEngine::OpenGL;
	ClockOrientation CurrentClockOrientation = ClockOrientation::CounterClockwise;

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
		if (i == 0) return 4;
		else if (i == 1) return 1;
		else if (i == 2) return 0;
		return i;
	}

	int SelectRenderEngineFromComboBox()
	{
		uint i = 0;
		TypeEntry renderEngine[2] = { { "OpenGL", false },{ "Close2GL", false } };
		static TypeEntry typeSelected = renderEngine[0];
		DrawComboSelectType("Engine", typeSelected, renderEngine, 2, i);
		return i;
	}

	int SelectClockDrawOrientation()
	{
		uint i = 0;
		TypeEntry clockOrientation[2] = { { "CounterClockwise", false },{ "Clockwise", false } };
		static TypeEntry typeSelected = { "CounterClockwise", false };
		DrawComboSelectType("Clock Orientation", typeSelected, clockOrientation, 2, i);
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
	loop.getWindow().setViewport({ 0, 0, 800, 800 });
	loop.getWindow().start();
	loop.setScene(new TestScene);
	loop.start();
	return 0;
}