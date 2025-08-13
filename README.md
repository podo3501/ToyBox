# DirectX12 기반 UI & 렌더링 엔진 프로젝트

본 프로젝트는 DirectX12 기반의 UI 및 렌더링 엔진 개발을 목표로 하였으며, 다음과 같은 주요 기능과 개선 작업을 구현하였습니다.

## 주요 기능 및 개선 사항

- **DirectX12 리소스 관리 및 유닛 테스트**
  - 메모리 누수 방지를 위한 체계적 리소스 관리
  - Google Test 및 GMock 기반 유닛 테스트 도입

- **마우스 입력 처리**
  - 입력 처리 클래스 통합
  - 마우스 트래킹 기능 추가

- **UI 컴포넌트 모듈화**
  - 버튼 포함 UI 컴포넌트의 데이터와 렌더링 분리
  - 이미지 그리드(1, 3, 9 분할) 및 이미지 확대 기능 구현
  - UI 컴포넌트 이름 생성 및 관리 로직 개선으로 이름 중복 문제 해소 및 성능 향상

- **직렬화 시스템**
  - JSON 기반 TMP 프로그램을 통한 UI 데이터 저장/불러오기 기능
  - Write/Read를 통합한 Serialize 함수로 최적화

- **툴 및 편집 기능**
  - ImGui 연동으로 툴 기반 UI 개발 지원
  - UI 편집, Undo/Redo, 컴포넌트 연결 시각화
  - TextureResourceBinder를 통한 텍스처 소스 좌표 관리(Atlas Texture) 및 실시간 편집

- **게임 루프 및 성능 최적화**
  - 템플릿 메소드 패턴으로 게임 루프와 툴 루프 분리
  - Tracy Profiler 연동 및 성능 최적화 작업 수행

---

## 사용 기술 및 라이브러리

- **DirectXTK12 (DirectX Tool Kit for DirectX 12)**  
  DirectX12 기반 UI 및 렌더링, 텍스처·스프라이트·폰트 관리

- **Google Test (GTest) / Google Mock (GMock)**  
  자동화 유닛 테스트 환경 구축 및 TDD 적용

- **C++20 표준 라이브러리**  
  스마트 포인터, 람다, 컨테이너 등 최신 C++ 기능 활용

- **Nlohmann JSON**  
  TMP 기반 UI 데이터 직렬화/역직렬화 및 파일 입출력

- **ImGui (Immediate Mode GUI)**  
  개발용 툴 및 디버깅 UI 구현

- **Tracy Profiler**  
  실시간 성능 분석 및 최적화

---

## 사용된 주된 디자인 패턴

- **템플릿 메소드 패턴 (Template Method Pattern)**  
  게임 루프와 툴 루프를 분리, 공통 로직은 상위 클래스, 세부 동작은 하위 클래스에서 구현

- **옵저버 패턴 (Observer Pattern)**  
  WndProcListener 등 이벤트 전달 시 컴포넌트 간 느슨한 결합과 확장성 확보

- **컴포지트 패턴 (Composite Pattern)**  
  UI 컴포넌트를 트리 구조로 구성, 단일/복합 객체 동일 처리

- **어댑터 패턴 (Adapter Pattern)**  
  컴포지트 패턴으로 구성된 UI 컴포넌트와 다른 클래스 간 인터페이스 연결

- **커맨드 패턴 (Command Pattern)**  
  Undo/Redo 기능 구현, 사용자의 작업을 명령 객체로 캡슐화

- **전략 패턴 (Strategy Pattern)**  
  마우스 입력 처리 및 렌더링 알고리즘 교체 시 유연성 제공

- **CRTP (Curiously Recurring Template Pattern)**  
  노드 관리 클래스 작성, 컴파일 타임 다형성과 효율적 코드 재사용 지원

- **트레잇 패턴 (Traits Pattern)**  
  JSON 직렬화 시 TMP와 결합, 타입별 특성을 분리하고 일관된 직렬화 구현 지원

> 참고: 프로젝트 내에서는 이 외에도 다양한 디자인 패턴이 활용되었으나, 여기서는 주된 패턴들만 정리하였습니다.
