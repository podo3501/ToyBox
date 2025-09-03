# DirectX12 기반 UI & 렌더링 엔진 프로젝트(TDD 기반 개발)

DirectX12 기반 UI & 렌더링 엔진 프로젝트로, TDD 기반 개발을 통해 안정성과 유지보수성을 강화했습니다.  
UI 컴포넌트 모듈화, JSON 직렬화 시스템, ImGui를 이용한 UI툴 지원, Tracy Profiler 기반 성능 최적화 테스트 등 다양한 기능을 제공합니다.

## 주요 기능

- **Test-Driven Development로 개발**
  - Google Test와 gMock을 사용

- **UI 컴포넌트 모듈화**
  - 버튼 포함 UI 컴포넌트의 데이터와 렌더링 분리
  - 이미지 그리드(1, 3, 9 분할) 및 이미지 확대 기능 구현
  - UI 컴포넌트 이름 생성 및 관리 로직 개선으로 이름 중복 문제 해소 및 성능 향상
 
- **UI 노드 구조 및 생명주기 관리**
  - UI를 노드 기반 구조로 설계하여 다른 UI에 계층적으로 부착 가능
  - 씬 전환 시에도 특정 UI(예: 메신저 UI)를 메모리에서 해제하지 않고 유지, 새로운 씬에 즉시 연결해 재사용 가능
  - 독립적인 생명주기 관리로 대규모 UI 시스템에서 효율성과 확장성 제공
 
- **UI 컴포넌트 이름 관리**
  - 새로운 UI 생성 시 자동으로 고유 이름을 부여하여 중복 문제를 방지
  - 대규모 UI 트리 구조에서도 효율적인 탐색 및 관리 가능
  - 네임스페이스 단위로 이름을 관리하여, 다른 네임스페이스 간에는 동일 이름 사용 가능

- **직렬화 시스템**
  - JSON 기반 TMP 프로그램을 통한 UI 데이터 저장/불러오기 기능
  - Write/Read를 통합한 Serialize 함수로 최적화

- **툴 및 편집 기능**
  - ImGui를 이용한 툴UI 개발
  - UI 편집, Undo/Redo 기능 구현, 컴포넌트 연결 시각화
  - 툴 내에서 여러 UI 창을 동시에 띄울 수 있어, 작업 중 필요한 창을 즉시 열고 조작 가능
  - 복잡한 UI 트리 구조도 실시간으로 확인하고 관리할 수 있어 효율적인 개발 환경 제공
 
- **Texture 관리 및 편집**
  - TextureResourceBinder를 통한 Atlas Texture 좌표 관리 및 실시간 편집
  - 자동 이미지 영역 감지 기능으로 텍스처 분할과 매핑 과정을 간소화

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
  스마트 포인터, 람다 뿐만 아니라 함수 템플릿 시그니쳐 같은 C++20 기능까지 사용

- **Nlohmann JSON**  
  Nlohmann JSON을 활용한 TMP 기반 UI 데이터 직렬화를 사용한 파일 입출력

- **ImGui (Immediate Mode GUI)**  
  ImgGui 라이브러리 사용 UI툴 개발

- **Tracy Profiler**  
  실시간 성능 분석을 통한 프로그램 최적화 확인

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
