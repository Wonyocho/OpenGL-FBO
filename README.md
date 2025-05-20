# OpenGL 그래픽스 과제3

## 개요
- **기본 과제:**  
  - 기하학적 오브젝트(집, 나무, 울타리 등)와 텍스처를 활용하여 예술적 씬을 구성
  - 랜덤 Terrain(지형) 생성 및 텍스처링
  - Scene Render-to-Texture(FBO) 구현
  - DepthBuffer(깊이버퍼) 결과 출력 기능(D키 토글)
- **환경:** C++11, OpenGL, GLEW, FreeGLUT, PPM 텍스처 파일 사용

## 주요 구현 기능
1. 랜덤 Terrain 지형 생성 및 텍스처링
	프로그램 실행 시, terrain(지형) 평면을 랜덤하게 약간 울퉁불퉁하게 생성

	텍스처(terrain.ppm) 매핑

	PPMImageLoader 모듈을 별도 분리하여 이미지 파일 로드

2. 기하학적 오브젝트/건물 생성
  Piloti: 일정 간격의 원기둥(기둥), 4x1 배열

	FirstFloor/SecondFloor: 건물 1,2층(직육면체), 텍스처 적용

	Fence: 건물 주변을 감싸는 ㅁ자형 울타리, 텍스처 타일링

	Window: SecondFloor에 3면(앞/뒤/오른쪽) 윈도우, 얇은 직육면체, 텍스처

3. 마우스 및 카메라 조작
	마우스 드래그 → 카메라 시점 회전 (MouseHandler 모듈 분리)

	좌표축 표시(terrain 모서리)

4. Render-to-Texture (FBO)
	SceneRenderTarget/SceneRenderer

	실제 OpenGL 장면 전체를 FBO(FrameBuffer Object)에 한 번 그린 뒤
	이를 2D quad로 화면에 출력

	해상도 고정(512x512, 필요시 변경 가능)

5. DepthBuffer(깊이버퍼) 시각화
	FBO에 깊이 텍스처도 같이 생성/렌더링

	D 키 토글:

	일반 컬러 씬 ↔ 깊이버퍼(흑백) 화면 전환

	Depth는 원본 OpenGL 값 그대로(후처리 없음, 하얗게 보일 수 있음)

