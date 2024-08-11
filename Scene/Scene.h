#pragma once

class Scene final
{
public:
	Scene(class Context* context);
	~Scene();

	void Update();

	// 만들어질 때
	const std::shared_ptr<class Actor> CreateActor(const bool& is_active = true);

	// 만들어진걸 추가할 때
	void AddActor(const std::shared_ptr<class Actor>& actor);

	const std::vector<std::shared_ptr<class Actor>>& GetActors() const { return actors; }

private:
	class Context* context = nullptr;
	class Renderer* renderer = nullptr;

	bool is_update = true;

	std::shared_ptr<D3D11_Pipeline> pipeline = nullptr;

	std::vector<std::shared_ptr<class Actor>> actors;
};

// IA - VS - RS - PS - OM

/*
	Input Assembler Stage (IA) - 입력 조립기 단계
		- 기본 데이터(Vertex)를 넣는 단계이다.

	Vertex(정점)
		- DX의 기본 데이터
		- 컴퓨터는 점을 이어서 만든 도형에 이미지를 덮어 씌어 화면에 출력한다.
		- 정점은 사용자에 따라 다양하게 만들어지기 때문에 DX에서 자료형을
		제공하지 않고 사용자가 직접 정의하여 사용해야 한다.

	사용자가 IA단계에서 세팅해야 하는 것들
		- Vertex Buffer				: 실제 점들의 데이터를 넣어준다.
		- Input Layout				: 어떤 유형의 점들을 사용할 것인가? -> 각 하나의 점이 가지는 정보가 무엇인지 ex) 위치, 색상, 기울기
		- Primitive Topology		: 점들을 어떠한 방식으로 이을것인가?
		- Index Buffer(생략가능)	: 점들을 어떤 순서로 이을것인가? ->  점별로 인덱스를 부여하여 인덱스를 기준으로 점을 연결한다.
*/

/*
	Vertex Shader Stage(VS) - 정점을 가공하는 단계
		- Shader : GPU를 동작시키는 함수들의 집합, 언어
		- HLSL : High Level Shader Language, DX에서 사용하는 쉐이더 언어
		- IA, RS, OM 단계에서는 코딩이 불가능해 값을 세팅만 해줄 수 있지만
		VS, PS 단계는 쉐이더로 코딩이 가능하다.
		- VS 단계에서 정점들의 공간 변환(transform)을 해준다.
*/

/*
	Rasterizer Stage(RS) - 이미지 픽셀화
		- 만들어진 모델을 화면에 송출하기 위해 3D 이미지를 2D(픽셀)데이터로 바꿔준다.
		-> 모니터는 2D화면 이므로
		- Viewport : 그릴 영역, 화면에 어느 영역에 그릴것인지를 정한다.
		- VS에서 공간변환을 하기 쉽게 정규화했던 공간을 RS에서 viewport만큼 늘려준다.
		
		- RS의 역활
			ㅁ Clipping				: VS에서 변환된 Clip공간을 사용해 보이지 않는 부분을 자른다.
			ㅁ NDC					: Clip공간을 NDC로 바꾼다. -> 동차를 1로 맞춰준다
			ㅁ Back-face Culling		: 보이지 않는 면을 제거한다.
			ㅁ Viewport Transform	: NDC 공간을 Viewport만큼 늘려준다.
			ㅁ Scan Transform		: PS에서 처리해야할 픽셀을 지정한다.

		- 위의 작업을 어떤식으로 처리할 것인지를 Rasterizer State를 만들어 옵션을 정한다.
*/

/*
	Pixel Shader Stage(PS) 픽셀을 가공하는 단계
		- RS 단계를 거쳐 픽셀화된 이미지에 각각의 픽셀마다
		커스텀 한 내용을 처리해준다.
		- RS가 지정한 픽셀의 개수만큼 호출된다.
*/

/*
	Output Merger Stage(OM) - 모든 과정을 거쳐 최종본을 만드는 단계
		- Render Target View : RTV를 제공해주면 만들어진 최종본을 RTV에 그린다.
		후에 스왑체인을 통해 완성한 이미지를 전면 버퍼로 보낸다.
		- Blend State에 따라 색을 섞어주는 작업을 한다.
*/


/*
	공간
		ㅁ VS : World, View, Projection, Clip(DX에서는 Projection과 결합되어있음) 변환을 담당
		ㅁ RS : NDC, Viewport 변환을 담당

	- Local
		ㅁ 지역 좌표계, 물체를 설계할 때 물체를 기준으로 원점을 잡은 설계도상의 좌표, 모델 좌표라고도 한다.
		ㅁ 만약 여러개의 물체를 로컨 좌표 기준으로 출력할 경우 원점의 좌표가 겹치게 된다.
		-> 물체 기준이 아닌 우리가 출력할 세계(World)로 바꾸어야 한다.

	- World
		ㅁ 설계도상의 좌표를 실제 세계로 옮긴 좌표
		ㅁ 월드 변환을 하여 크기 변환, 이동, 회전등의
		변환을 하여 지역에서 월드로 바꾼다.

	- View
		ㅁ 월드 공간에 놓여진 객체를 바라볼 방향, 내 눈(Camera)를 기준으로 하는 공간
		ㅁ 내가 세상을 어디서, 어디를 볼 것인지를 정한다
		ㅁ 뷰 변환을 통해 카메라 기준으로 월드를 재배치한다.

	- Projection
		ㅁ 투영(Projection) : 어떠한 물체를 다른 3차원 공간이나 2차원 평면에 옮기는 작업
		ㅁ 3D 데이터를 2D 데이터로 변경

	- Clip
		ㅁ Clipping을 사용하기 위해 정규화한 좌표
		ㅁ Clipping : 보이는 영역 이외에 부분을 잘라내는 것

	- NDC
		ㅁ 정규화된 좌표계

	- Clip Vs NDC
		ㅁ Clip -> Homogenous space - 동차 공간
			ㅁ 동차가 정규화가 안된 상태

		ㅁ NDC -> Euclidean space
			ㅁ 동차가 1로 정규화된 상태

	- Viewport
		ㅁ 정규화한 공간을 우리가 정한 크기만큼 늘린 공간
*/

/*
	동차 좌표란 무엇인가?
	VECTOR3로 된 (1, 0, 0) 해당 값을 볼 때 이 값이
	좌표인지 방향인지 구분이 안됨, x가 1인 좌표인지 x축을 바라보는 방향인지 알 수 없음
	해당 값이 좌표인지 방향인지 명확하게 하기위해 한차원을 확장해버림 (1, 0, 0) -> (1, 0, 0, 1)
	추가된 4번째 값을 w라고 부르며 w = 0 일경우 방향, w != 0 경우 위치를 나타낸다.
	1차원을 표현할때는 2차원, 2차원을 표현할때는 3차원으로 표현함 // 아래 설명 참고


	2차원의 단면인 1차원 데이터 공간
	-------------------------0----- y = 4
					x가 6이라고 가정

	--------0---------------------- y = 2
		x가 3이라고 가정

	1차원 이라고 볼 때 위의 두 점은 서로 다른 차원에 존재하므로 만날 수 없다.
	서로 만나게 하기 위해서는 차원을 맞춰야 하기 때문에 서로 차원을 맞춰주기위해
	동차값인(여기서는 y)를 맞춰 서로 같은 차원에 위치하게 한다.

	이 개념을 적용하면 3차원의 단면은 2차원이고 4차원의 단면은 3차원이다.
	그러면 서로 다른 4차원에서 추가된 값인 w를 맞춰준다면 두 차원은 3차원상에
	같은 위치에 존재한다고 본다.
*/

/*
	4x4 항등행렬
	
	로컬 -> 월드

	      1열
		   x y z w
	1행	 x 1 0 0 0     x 0 0 0
		 y 0 1 0 0  => 0 y 0 0
		 z 0 0 1 0	   0 0 z 0
		 w 0 0 0 1	   0 0 0 w

	항등(단위)행렬 : 대각선의 원소가 모두 1이고 나머지는 모두 0인 정방행렬이다
					, 1과 같은 의미를 가지는 행렬

	정방행렬 : 행과 열이 같은 행렬
	
	백터 : 방향과 크기를 가진 값
	예) 백터 v[1, 0] = x(방향)으로 1(크기)만큼 간다.

	위의 4x4 정방 행렬이 가지는 기하학적 의미

	y   z
	^  /
	| /
	|/  ㅁ
	|------> x
	w(0 0 0) 

	x = (1 0 0 0)
	y = (0 1 0 0)
	z = (0 0 1 0)
	w = (0 0 0 1)

	각 위치의 값을 (x, y, z, w)로 본다면
	x, y, z는 w(동차)값이 0인 방향을 의미하는 백터라는 뜻이고
	w는 w(동차)값이 1인, 위치를 의미하는 좌표라는 뜻이다.
	그렇기 때문에 기하학에서 4x4 항등행렬은 중심 좌표(0, 0, 0)에서
	x, y, z축 방향으로 1씩 가는 공간이라고 볼 수 있다.

	만약 x 백터값을 2로 바꾼다면 백터는 방향과 크기를 가지니 공간이 x방향으로 2배로 늘어났다고 할 수 있다.
	그렇게 된다면 해당 공간 내부에 있는 물체(데이터)도 x방향으로 2배가 늘어나게 된다.
	-> 공간의 크기(x, y, z)을 늘리면 내부의 물체(데이터)도 늘어나게 된다.

	만약 w(0, 0, 0, 1)값의 0, 0, 0을 수정하게 되면 어떻게 될까?
	w(10, 0, 0, 1)로 바꾸게 된다면 중심좌표가 (0, 0, 0) -> (10, 0, 0)으로 바뀌게 될것이다.
	중심 좌표가 변경되었다는 것은 해당 위치로 공간이 이동했다는 뜻이고
	당연히 공간 내부의 물체(데이터)도 이동했다는 뜻이다.
	-> 공간의 위치 w(0, 0, 0)을 바꾸면 공간이 이동한다.


		좌표계
		DirectX - LH : z가 화면으로 들어가는 좌표계, 시계 방향 회전

		y    z
		^   /
		|  /
		| /
		|------> x


		OpenGL - RH : z가 화면으로 나오는 좌표계, 반시계 방향 회전
			y
			^
			|
			|
			|------> x
		   /
		  /
		 /
		z
*/