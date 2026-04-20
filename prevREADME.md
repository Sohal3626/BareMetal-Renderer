# BareMetal-Renderer
---
# 샤따 내림 
이 프로젝트는 프로그램 구조 재설계와 복습을 목적으로 새로운 프로젝트를 시작함에 따라 종료되었습니다.  
더 이상의 업데이트는 진행하지 않을 예정이며, 후속 프로젝트에 이어서 진행할 예정입니다.  
[BareMetal-Renderer2](https://github.com/Sohal3626/BareMetal-Renderer2)
---
## 프로젝트 개요
요즘 세상이 살기 ㅈㄴ 좋아졌다. 온갖 정보가 웹에 다 널려있어 공부하고 싶은 자료 찾기가 매우 쉬워졌다.
과거에는 혼자 공부해야하 한다는 어려움이나 언어적 한계로 어려움을 겪은 일이 많았다. 하지만 지금은 대 AI의 시대가 아닌가?
그냥 제미나이한테 공부하고 싶다 하면 로드맵을 알아서 찾아와서 나한테 강의까지 해준다.
이 프로젝트의 목적은 렌더링의 기본적인 파이프라인을 공부하기 위해 AI의 도움을 받아 Tiny Renderer 스타일의 튜토리얼을 진행하는 것이다.

## 예제
```cpp
int size_w = [YOUR CANVAS WIDTH];
int size_h = [YOUR CANVAS HEIGHT];
Canvas cv(size_w, size_h);

Model md([YOUR OBJ PATH]);
Material mat([YOUR MTL PATH]); // 주의 : .mtl 파일 내의 map_Kd 절의 내용이 파일명이 아닌 경로일 경우 파일명으로 바꿔주어야함
DataBuilder db;
std::vector<RenderUnit> units = db.build(md, mat, [YOUR TEXTURE DIRECTORY]); // 주의: 파일의 경로가 아닌 디렉토리의 경로여야함
Matrix44 tp{};

Renderer::draw_model(cv, units, tp);

cv.save_ppm([YOUR PPM NAME]);
```

## 파일 구조
```
BareMetal-Render/
│
├── include/
│   ├── Canvas.h
│   ├── Geometry.h
│   ├── Material.h
│   ├── Model.h
│   ├── RenderData.h
│   ├── Renderer.h
│   ├── Texture.h
│   └── stb_image.h (Third-Party)
├── src/
│   ├── Canvas.cpp
│   ├── Material.cpp
│   ├── Model.cpp
│   ├── RenderData.cpp
│   ├── Renderer.cpp
│   └── std_image_impl.cpp (Third-Party)
├── main.cpp
├── .gitignore
└── README.md
```

## 현재 진행

### 완성한 것들
- 캔버스 저장
- 브레젠험을 통한 선 긋기
- 삼각형 그리기
- 모델 파싱
- 모델 래스터라이징
- 플랫 셰이딩
- 단일 텍스쳐 매핑
- mtl 파싱을 통한 여러 텍스쳐 매핑
  
### 앞으로 할것들
 - 보간을 이용한 좀 더 개지리리는 셰이딩
 - mtl 파싱을 통한 질감 구현
 - 그 밖에 내가 모르는 ㅈ같은 것들

---
# API 세부사항

## Renderer.h
이 렌더러의 주요 기능이 다 여기에 모여있다. 선 긋기, 래스터라이징, 모델 렌더링 API를 제공한다.

### Renderer
렌더링 API 함수들을 저장하는 클래스이다. 그냥 스태틱 전역 변수로 선언하면 되는데 왜 내가 이걸 클래스로 만들었는지는 잘 모르겠다. 아마도 객체지향 비슷한 무언가를 하고 싶었던것으로 추정된다.
나중에 다른 렌더링 설정을 가진 인스턴스를 여러개 써야 할 일이 생길 수도 있으니 좋은거 아닐까 싶다.

- `draw_line(Canvas &cv, int x0, int y0, int x1, int y1, Color c1))`  
      내가 이 프로젝트 파고 처음으로 만든 함수다. 거짓말이다. 지금 생각해보니 Canvas::set_pixel이었던거 같다. 아무튼 함수 이름처럼 브레젠험 알고리즘을 통해 선을 그어준다. 지금 보니까 점도 벡터가 아니라 그냥 정수로 받고 있다.
      시그니쳐가 조금 많이 더럽지만 상관없다. 왜냐하면 지금 ㅈ도 쓸 일 없다.


- `fill_triangle(Canvas& canvas, Vec<3> p0, Vec<3> p1, Vec<3> p2, const Texture& texture, double intensity, Vec<2>* uvs);`
      캔버스와 세 점, 텍스쳐, 밝기, 텍스쳐상의 좌표를 입력받아 삼각형을 칠하는 함수이다. 삼각형 폴리곤을 래스터라이징 하는데에 사용된다.
      처음엔 삼각형을 반으로 나누어 스캔라인 방식으로 색칠했지만 지금은 삼각형이 들어 있을 만한 범위를 구한 뒤
      barycentric 좌표를 계산하여 삼각형 안에 있는 픽셀만 색칠하는 식으로 바뀌었다.
      현대 그래픽스에서는 barycentric을 통해 픽셀을 칠한다는데 이건 CPU렌더러라 스캔라인 방식으로 색칠하는게 더 빠른거같다.
      하지만 아쉽게도 스캔라인 방식은 내가 ^문화대혁명^ 해 버리는 바람에 지금은 볼 수 없다.


- `draw_model(Canvas &canvas, const std::vector<RenderUnit>& units, const Matrix44 &transform, const Vec<3>& light_dir)`  
      캔버스와 모델 데이터인 `units`, 회전/투영 행렬, 광원의 방향을 입력받아 모델을 렌더링한다.
      units를 통해 모델의 점, 면, 텍스쳐 등의 정보를 전달받아 회전행렬을 곱하고 각 폴리곤마다 광량을 계산한다.
      이후, 처리한 데이터들을 이용해 `fill_triangle()`반복 호출하여 모델을 캔버스에 래스터라이징한다.


- `transfrom(double p_scale, double angle_x = 0, double angle_y = 0, double angle_z = 0, double camera_z = 3)`
      모델의 회전, 카메라와 피사체의 거리 등을 행렬 계산을 이용해 처리하고 있다.
      행렬 계산 딸깍 한 번으로 점들을 회전/투영시킨다는 점에서 선형대수의 위대함과 수학적 아름다움을 느낄 수 있지만 나는 빡대가리라 그런건 못 느껴서 회전행렬을 그냥 외워버렸다.
      참고로 `angleX, angleY, angleZ`는 degree가 아닌 radian으로 입력해야한다.  
      ~~TMI를 좀 풀자면 학교 수치해석 시험에서 학생들이 전자계산기 사용법을 몰라 Radian을 Degree에 두고 계산하여 다같이 점수가 나락간 일이 있었는데
      교수님이 넓은 아량으로 똑같이 틀린 애들이 너무 많으니 전부 부분점수를 주시는 방향으로 자비를 베풀어 주셨다. 그냥 라디안 하니까 이 썰이 생각났다.~~


- `draw_horizontal_line(Canvas& cv, int x1, int x2, int y, Color c1, Color c2)`  
      문화대혁명 속에서도 중국의 문화가 말살되지 않았던 것 처럼 이 함수도 스캔라인의 흔적으로 남아있다. `fill_triangle()`에서 내부 호출로만 사용되었기 때문에 private 함수로 남아있지만
      지금은 그마저도 없으니 뱀의 다리뼈 같은 흔적기관이 되었다. 어딘가에 쓸 일이 있을수도 있을까 싶어 남겨두었다.


- `get_intensity(const Vec<3> *pts, Vec<3> light_dir)`  
      점3개와 광원의 방향을 입력하면 그 점 3개로 이루어진 면의 밝기를 계산해준다. 삼각형의 두 변을 외적하여 법선벡터를 생성한 후 법선벡터와 광원 벡터를 내적하는 고전적인 방식이다.
  

## Geometry.h
자주 쓰이는 수학적 자료구조와 계산들을 헤더 하나에 정리해 두었다. 

### Vec<N>
이 구조체는 크기가 N인 double 타입 벡터를 더 편하게 생성하기 위해 만들어졌다.  
멤버로는 `double`타입 `std::vector`인 `data`를 가진다.
- `Vec()`, `Vec(std::vector<double> v)`, `Vec(std::initializer_list<double> v)`  
    이 구조체는 기본 생성자, `std::vector`타입의 복사 생성자, array를 통한 복사 생성자를 지원한다.
  
- 이 구조체는 아래와 같은 연산자 오버로딩을 지원한다.  
    |연산자|대상|내용|
    |-|---|---|
    |+|`const Vec<N>`|벡터 가산|
    |-|`const Vec<N>`|벡터 감산|
    |*|`const double`|스칼라 승산|
    |/|`const double`|스칼라 제산|
    |[]|`const int`|인덱스를 통한 데이터 접근|
  
    벡터 가산, 감산은 좌측 피연산자와 오른쪽 피연산자의N이 같을때만 가능하니 깜빡 잊었다가 좆되지 않도록 조심하자.

- 이 구조체의 N = 3인 버전은 `Color`와 `Point`라는 별칭으로 사용되고 있다. 참고로 위 Renderer.h 설명을 읽어봤다면 알겠지만 Point는 만들어놓고 다 Vec<3>로 선언해서 쓰고있는 상태다. 다시 말해 있으나 마나 하다.

### Matrix44  
이 구조체는 `double` 타입 4×4 행렬을 저장한다. 멤버로는 `double`타입 `std::vector m`이 있다.
- `Matrix44()`  
    이 구조체는 기본 생성자만 지원한다. 기본 생성자는 이 행렬을 identity matrix형태로 초기화한다.
  

- `idx(const int i, const int j)`  
    이 구조체의 데이터 `m`은 1차원 벡터이다. `idx()`는 이 데이터에 직관적인 2차원 인덱스를 통해 접근할 수 있도록 한다.
  

- `operator*(Vec<4>&)`  
  행렬과 길이가 4인 벡터의 승산 연산을 시행한다.


- `Matrix44 operator*(const Matrix44& b)`
  행렬과 행렬의 승산 연산을 시행한다.


- `perspective(Vec<4>& v)`  
  렌더링 상의 원근 투영을 위한 함수이다. 파라미터 `v`로는 $-1 / 카메라의 거리$ 가 주어진다.
  3D좌표 (x, y, z)는 계산 과정에서 (x, y, z, w)형태의 4차 동차좌표로 표현되는데 이 좌표를 화면에 프로젝션한 후 (x, y, z)를 w로 나눠 원근 효과를 만들어낸다.사실 아직 완벽하게 이해를 못해서 공식을 유도하지는 못하겠다. 2학년때 선형대수 열심히 들어었어야 했다. (┬┬﹏┬┬) (┬┬﹏┬┬) (┬┬﹏┬┬) (┬┬﹏┬┬) (┬┬﹏┬┬)  
  
### static 필드 함수
- `crossV3(Vec<3>& a, Vec<3>& b)`  
  길이가 3인 벡터의 외적을 계산한다.
   

- `dotV3(Vec<3>& a, Vec<3>& b)`  
  길이가 3인 벡터의 내적을 계산한다.


- `barycentric(Vec<3>* pts, Vec<2> p)`  
  삼각형의 세 꼭짓점과 화면 상의 점 하나를 입력하면 이를 삼각형 내의 barycentric 좌표로 변환하여 리턴해준다. 

## Canvas.h
그림을 그릴 캔버스를 관리하는 클래스이다. 멤버 변수로는 캔버스의 크기를 저장하는 `width`, `height`, 캔버스의 셀들을 저장하는 `std::vector`인 `pixels`, 셀들의 z좌표를 저장하는 `double`형 `std::vector`인 `zbuffer`가 있다.

- `Canvas(int w, int h)`   
      캔버스의 생성자이다. 기본생성자랑 복사생성자는 만들기 귀찮아서 이거 하나가 다다. `w`와 `h`를 저장하고 `pixels`의 크기를 w*h로, 기본값은 검정색인 `{0, 0, 0}`로 초기화하며,
        `zbuffer`또한 w\*h크기, 기본값은 double의 최소값으로 초기화한다.


- `set_pixel(int x, int y, const Color& color, double z)`   
        캔버스에서 2D배열 기준으로 x행, y열에 위치한 점을 `color`색상으로 세팅한다. `pixels`는 1차원 배열이기 떄문에 x와 y는 1차원 배열 기준의 인덱스로 변환되어 동작한다.
        Z-Buffer가 적용되어 새로 그릴 셀의 Z좌표인 `z`가 `zbuffer`의 해당 인덱스값보다 클 경우에만 픽셀을 칠해준다. 더 작을 경우에는 기록말살 당한다. 마치 스탈린이랑 찍은 사진에서 삭제당한 예조프처럼 말이다.


- `save_ppm(const std::string& filename)`  
        지금까지 캔버스에 고생해서 그린 그림을 ppm 파일에 옮겨적어준다. 나는 ppm이라는 포맷의 존재조차 모르다가 이거 하면서 처음 알게됐다.
        참고로 ppm에서 픽셀의 색상 rgb값은 0부터 255사이의 실수로 나타나는데 이 프로그램에서는 색깔을 0부터 1사이의 실수로 표현하고 있다. $[0, 1]$ 포맷을 $[0, 255]$ 포맷으로 바꿔주는 역할을 이 `save_ppm()`이 맡는다.


- `getWidth()`  
        캔버스의 너비를 리턴하는 getter다.


- `getHeight()`  
        캔버스의 높이를 리턴하는 getter다.

## Model.h
`.obj` 타입의 모델링 데이터를 가져와서 파싱하고, 저장하는 클래스이다.  
멤버로는 정점들을 저장하는 `std::vector<Vec3>> verts_`와 텍스쳐 상에서 색깔을 추출해올 픽셀의 좌표인 `std::vector<Vec<2>>tex_coords`, 
각 머터리얼과 폴리곤 면들을 엮어서 저장하는 맵 `std::unordered_map<std::string, std::vector<std::vector<FaceVertex>>> material_map_` 가 있다.  
멤버 변수 `int num_of_faces`는 과거에 머터리얼 없이 면만 렌더러의 전달하던 시절의 흔적이며, 지금은 잘 사용되지 않는다.
나는 스트림 파싱을 ㅈㄴ 못하기 때문에 이게 제일 어려웠다. ㅅㅂ

### FaceVertex
모델링의 면 데이터를 파싱하여 저장하기 위한 컨테이너이다. int값인 `v`, `vt`, `vn` 세 변수를 멤버로 가지고 있다.

### Model
- `Model(const char* filename)`   
        이 클래스의 유일한 생성자이자 모든 것 이다. `filename`경로를  타고 따라가 모델링을 읽어 멤버 변수에 저장한다.   
        나는 obj파일의 내용을 처음 읽어봐서 굉장히 신기했다. obj파일 안에는 정점들을 `v 0.1557, -0.1557, 0.1557`과 같은 식으로 줄마다 써놓는다.
        그리고 좀 내려보면 `f 1557/1557/1557 1557/1557/1557 1557/1557/1557`처럼 생긴 줄이 있는데 이는 정점들을 이어서 만든 삼각형 면, 즉 폴리곤을 의미한다.
        여기서 앞쪽 스트림 한 단위만 떼면 `v / vt / vn`과 같은 형태가 된다. 이것만 보면 v는 사용할 정점이 있는 인덱스, vt는 삼각형에 텍스쳐를 입히기 위한 인덱스, vn는 셰이딩을 위한 법선 벡터를 찾기 위한 인덱스이다.
        f로 시작하는 한 줄을 잘라 `FaceVertex`형 데이터로 만들어 점 하나를 표현한다. 이 `FaceVertex`를 다시 `std::vector`로 만들어 각 폴리곤으로 엮는다.
        이후 이 폴리곤을 의미하는 `std::vector`들을 모아 또 다른 `std::vector`에 저장한다. 그리고 이 벡터를 `material_map_`에 집어넣는다.  
        이 때, 키값이 필요한데 이는 이 면들이 사용하는 머터리얼의 이름이다. obj파일 안에는 중간마다 `usemtl`로 시작하는 줄이 있는데 이는 이 아래의 면들이 어떠한 같은 머터리얼을 사용한다는 의미이다.
        그렇게 나는 렌더러의 주요 기능들을 만들고 Tiny Renderer에서 테스트용으로 사용하는 `african_head.obj`를 돌려보고 잘 나와서 기뻣다. 그리고 인터넷에서 개쩌는 모델링들을 찾아서 렌더링을 시도했다.
        근데 세상에 ㅅㅂ 중간에 구멍이 ㅈㄴ 뚫려서 징그럽게 나온다. 왜인가 봤더니 ㅈ같게 어떤 모델링들은 `f 1557/1557/1557 1557/1557/1557 1557/1557/1557 1557/1557/1557`와 같이 삼각형이 아니라 사각형이 래스터라이징 단위로 되어있다.
        그래서 기본 단위가 다각형으로 주어지는 경우, 삼각형 여러개로 쪼개서 저장하도록 하였다.  


- `nvert()`  
        멤버 변수 `verts_`의 크기를 리턴하는 getter다.
   

- `nfaces()`  
        멤버 변수 `num_of_faces`의 크기를 리턴하는 getter다.


- `ntex()`  
        멤버 변수 `tex_coords_`의 크기를 리턴하는 getter다.


- `vert(int i)`  
        멤버 변수 `vert_`의 i번째 인덱스를 리턴하는 getter다.


- `tex(int i)`  
        멤버 변수 `tex_coords`의 i번째 인덱스를 리턴하는 getter다.


- `normal(int i)`  
       파싱된 vn 데이터의 i번쨰 인덱스를 리턴하는 함수여야 하지만 현재 vn 처리 기능이 구현되지 않아 Vec<3>{0, 0, 0}을 리턴하는 더미 함수로 동작하고 있다.


- `get_material_map()`
       `material_map`을 리턴하는 getter다.


## Texture.h
이 헤더에서는 `stb_img.h`를 사용해 `.png`이미지를 모델링의 텍스쳐로 사용할 수 있도록 처리한다.

### Texture
텍스쳐 이미지를 저장하는 클래스이다. 멤버 변수로는 `width`, `height`, `channels`, `data`가 있다.
- `Texture(const char* filename)`  
    이 클래스의 선언자이다. png파일의 경로를 입력받아 변수들을 초기화한다. 이미지 처리는 바이너리 데이터를 디코딩 해서 1차원 배열로 
    평탄화하여 저장되는 식이라는데 더 자세히는 모르겠다.  
    하지만 마법의 헤더`stb_img.h`가 다 처리해주니까 아무튼 그런갑다 하고 넘어가는게 맞는거같다.  


- `get_color(double u, double v)`  
    멤버 변수 `data`에 들어있는 이미지 상에서 `u`, `v`좌표에 있는 픽셀을 가져온다. 이미지 데이터에서는 픽셀 하나당
    인덱스 3개를 사용하면서 R G B R G B 순서로 데이터가 나열되어 있는데 이 색상값을 뽑아오기 위한 좌표 변환이 들어가 있다.


## Material.h
`.mtl`파일을 파싱하여 저장하고, 렌더링 과정에서 참조할 수 있도록 제공하는 역할을 수행한다.

### MaterialData
`.mtl`파일에서 읽어와 파싱된 데이터를 담는 컨테이너 구조체이다. mtl파일 내의 키워드들이 그대로 멤버 변수로 들어가 있다.


### Material
머터리얼을 불러와 읽고 파싱하여 `MaterialData`에 저장하는 클래스이다. 멤버 변수로 머터리얼을 종류별로 저장하는 맵 `std::map<std::string, MaterialData> mmap`를 가지고 있다.
- `Material(const char* filename)`  
    `filename`에 타고 들어가 `.mtl`파일을 읽어온다. mtl 파일에서 데이터를 추출하여 `MaterialData`에 저장하고 이 `MaterialData`를 머터리얼의 이름인 `newmtl`이후의 내용에 따라 분류하여 `mmap`에 삽입한다.


- `getMaterial(const std::string& mtlname)`  
    `mtlname`을 키로 사용하는 데이터가 `mmap`안에 있다면 그에 대응하는 `MaterialData`를 반환한다. 존재하지 않는다면 렌더링 과정에서 데이터 누락으로 인한 예외를 피하기 위해 기본값으로 초기화된 `MaterialData`를 반환한다.


## RenderData.h (<- ㅂㅅ)
`Material.h`, `Model.h`에서 처리한 데이터들을 가져와 `Renderer`에서 쉽게 처리할 수 있도록 데이터를 묶어주는 역할이다. 모델의 위치를 계산하고 가운데에 배치하는 작업도 이 단계에서 이루어진다.
(이게 진짜 개발하면서 제일 머리 터졌던거 같다. 그래픽스 공부하면 수학때문에 대가리깨질거라 생각하지 누가 데이터 담아서 자르고 모아서 뱉고 이런거에서 ㅈ같음을 느낄거라 상상할까? 근데 이게 현실이다. 결국 쓰레기더미가 완성됐다.)

### RenderUnit 
최종적으로 조립된 데이터들이 들어오는 구조체이다. 머터리얼의 이름, 텍스쳐, 폴리곤 데이터 등이 저장돼있다.

### DataBuilder
데이터를 조립하는 `build()` 함수를 가지고 있다. 멤버 변수로는 `std::vector<RenderUnit> res`가 있다.

- `build(Model &model, Material &mtd_lib, const std::string &texture_dir)`  
    모델 데이터와 머터리얼 데이터, 텍스쳐 폴더의 경로를 가져와 RenderUnit을 조립하여 멤버 변수 `std::vector<RenderUnit> res`에 저장하여 반환한다.
    주의할 점은 `texture_dir`가 텍스쳐의 경로가 아니라 텍스쳐 폴더의 경로라는 것이다. 전달받은 `Material`로부터 전달받은 텍스쳐의 이름을 이용하여 폴더 내의 텍스쳐들의 경로를 조립하여 저장한다.
    모델의 크기를 구해 점을 정규화해 저장하는 역할도 수행한다.
    (이 함수는 할 말이 많다. 아니 시발 내가 뭔 짓을 해둔지 모르겠다. res를 바로 반환하면서 왜 멤버 변수로 저장했는지도 의문이고, 모델 스케일링하고 중간에 배치하는것도 왜 따로 함수 분리를 안했는지 의문이며,
    머터리얼마다 `RenderUnit`을 생성하려 했는데 시발 하나의 `RenderUnit`안에 모든 데이터를 배열로 달아서 쳐넣는 미친짓을 해놨다. 아마 이거 만들때 새벽에 코딩하는라 빨리 자고싶어서 지능이 작살난 상태로 코딩했나보다.
    아무튼 리팩토링 1순위 코드이다.)


## TexCache
텍스쳐를 `DateBuilder`에 저장해두면 메모리 낭비가 심할까봐 따로 포인터로 캐싱해서 맵에 저장하기 위한 클래스이다. 멤버 변수로는 `std::unordered_map<std::string, std::shared_ptr<Texture>> cache`가 있다.

- `get_texture(const std::string& path)`  
    경로를 따라가 텍스쳐 파일을 `cache`에 저장한다.

---
# 기타 사항
## Third-party library
- stb_img.h  
  url : https://github.com/nothings/stb/blob/master/stb_image.h

## Reference
- Tiny renderer  
  url : https://github.com/ssloy/tinyrenderer 
    
