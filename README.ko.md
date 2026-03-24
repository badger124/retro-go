# Retro-Go 한국어 설명서

## 목차
- [프로젝트 개요](#프로젝트-개요)
- [지원 시스템](#지원-시스템)
- [디렉토리 구조](#디렉토리-구조)
- [주요 파일 설명](#주요-파일-설명)
  - [루트 디렉토리](#루트-디렉토리)
  - [components/retro-go (공용 라이브러리)](#componentsretro-go-공용-라이브러리)
  - [launcher (런처 앱)](#launcher-런처-앱)
  - [retro-core (통합 에뮬레이터)](#retro-core-통합-에뮬레이터)
  - [prboom-go (DOOM 에뮬레이터)](#prboom-go-doom-에뮬레이터)
  - [gwenesis (메가 드라이브 에뮬레이터)](#gwenesis-메가-드라이브-에뮬레이터)
  - [fmsx (MSX 에뮬레이터)](#fmsx-msx-에뮬레이터)
  - [tools (빌드/개발 도구)](#tools-빌드개발-도구)
  - [themes (테마)](#themes-테마)
  - [assets (리소스)](#assets-리소스)
- [빌드 방법](#빌드-방법)
- [수정 방법 가이드](#수정-방법-가이드)
  - [새 기기(타겟) 추가](#새-기기타겟-추가)
  - [번역(다국어) 추가](#번역다국어-추가)
  - [테마 커스터마이징](#테마-커스터마이징)
  - [에뮬레이터 수정](#에뮬레이터-수정)
- [설치 방법](#설치-방법)
- [사용 방법](#사용-방법)
- [자주 묻는 문제](#자주-묻는-문제)
- [라이선스](#라이선스)

---

## 프로젝트 개요

**Retro-Go**는 ESP32 기반 장치에서 레트로 게임을 플레이할 수 있는 오픈소스 펌웨어입니다.  
공식 지원 기기는 **ODROID-GO**와 **MRGC-G32**이며, 다양한 커뮤니티 지원 기기도 존재합니다.

프로젝트는 **런처** 1개와 **여러 에뮬레이터 앱**으로 구성되어 있으며,  
매우 제한된 CPU, 메모리, 플래시 자원에서 동작하도록 최적화되어 있습니다.

### 주요 특징
- 인게임 메뉴
- 즐겨찾기 및 최근 플레이 목록
- GB 컬러 팔레트, RTC 조정 및 저장
- NES 컬러 팔레트, PAL 롬, NSF 지원
- 화면 확대/필터 옵션
- 고속 동작(Fast Forward)
- 커스터마이징 가능한 런처
- 커버 아트 및 세이브 스테이트 미리보기
- 게임당 다중 저장 슬롯
- Wi-Fi 파일 매니저
- 그 외 다수

---

## 지원 시스템

| 제조사 | 시스템 |
|--------|--------|
| Nintendo | NES, SNES (느림), 게임보이, 게임보이 컬러, Game & Watch |
| Sega | SG-1000, 마스터 시스템, 메가 드라이브/제네시스, 게임 기어 |
| Coleco | ColecoVision |
| NEC | PC Engine |
| Atari | Lynx |
| 기타 | DOOM (모드 포함) |
| Konami | MSX |

---

## 디렉토리 구조

```
retro-go/
├── README.md               # 영문 메인 설명서
├── README.ko.md            # 한국어 설명서 (이 파일)
├── BUILDING.md             # 빌드 방법 문서
├── PORTING.md              # 새 기기 포팅 방법 문서
├── THEMING.md              # 테마 제작 문서
├── LOCALIZATION.md         # 번역/현지화 문서
├── CHANGELOG.md            # 버전별 변경 내역
├── COPYING                 # 라이선스 (GPLv2)
├── base.cmake              # CMake 빌드 공통 설정
├── rg_tool.py              # 주요 빌드/플래시/모니터 도구
├── Dockerfile              # Docker 빌드 환경 설정
├── retro-go.code-workspace # VS Code 워크스페이스 설정
│
├── components/
│   └── retro-go/           # 공용 라이브러리 (모든 앱이 공유)
│
├── launcher/               # 런처 애플리케이션
├── retro-core/             # 통합 에뮬레이터 (NES, GBC, SMS, PCE, Lynx, G&W, SNES)
├── prboom-go/              # DOOM 에뮬레이터
├── gwenesis/               # 세가 메가 드라이브/제네시스 에뮬레이터
├── fmsx/                   # MSX 에뮬레이터
│
├── tools/                  # 개발/빌드 보조 도구 스크립트
├── themes/                 # 기본 테마 파일
└── assets/                 # 아이콘, 미리보기 이미지 등
```

---

## 주요 파일 설명

### 루트 디렉토리

| 파일 | 역할 |
|------|------|
| `rg_tool.py` | 빌드, 플래시, 모니터링 등 모든 개발 작업을 통합하는 핵심 Python 스크립트. `idf.py` 대신 이 도구를 사용해야 합니다. |
| `base.cmake` | 모든 앱에서 공통으로 사용하는 CMake 빌드 설정 파일. |
| `Dockerfile` | Docker 컨테이너 기반 빌드 환경을 정의하는 파일. |
| `retro-go.code-workspace` | VS Code용 워크스페이스 파일. 인텔리센스를 위한 경로 설정 포함. |
| `BUILDING.md` | 빌드 환경 구성 및 명령어 설명 문서. |
| `PORTING.md` | 새로운 ESP32 기기를 지원하도록 포팅하는 방법 설명 문서. |
| `THEMING.md` | 커스텀 테마를 만드는 방법 설명 문서. |
| `LOCALIZATION.md` | 새 언어 번역을 추가하는 방법 설명 문서. |
| `CHANGELOG.md` | 버전별 변경 내역. |
| `COPYING` | GPLv2 오픈소스 라이선스 전문. |

---

### components/retro-go (공용 라이브러리)

모든 앱(런처, 에뮬레이터)이 공통으로 사용하는 하드웨어 추상화 및 유틸리티 라이브러리입니다.

#### 핵심 소스 파일

| 파일 | 역할 |
|------|------|
| `rg_system.c / .h` | 시스템 초기화, 앱 부팅, 크래시 처리, 전원 관리 등 핵심 시스템 기능. |
| `rg_audio.c / .h` | 오디오 출력 드라이버. 내장 스피커 및 외부 DAC 지원. |
| `rg_display.c / .h` | 디스플레이(LCD) 드라이버. ILI9341/ST7789 컨트롤러 지원. |
| `rg_input.c / .h` | 버튼 입력 처리. GPIO, ADC, I2C, 시프트 레지스터 방식 지원. |
| `rg_storage.c / .h` | SD 카드 파일 시스템 접근(읽기/쓰기/탐색). |
| `rg_settings.c / .h` | 앱별 설정값 저장 및 불러오기. |
| `rg_network.c / .h` | Wi-Fi 연결, NTP 시간 동기화, HTTP 파일 매니저 서버 기능. |
| `rg_gui.c / .h` | 메뉴, 다이얼로그, 리스트 등 UI 컴포넌트 렌더링. |
| `rg_surface.c / .h` | 픽셀 버퍼(Surface) 관리 및 이미지 처리. |
| `rg_i2c.c / .h` | I2C 버스 통신 드라이버. |
| `rg_utils.c / .h` | 문자열, 메모리, CRC 등 범용 유틸리티 함수. |
| `rg_localization.c / .h` | 다국어(현지화) 지원. `_(...)` 매크로로 문자열 번역. |
| `translations.h` | 실제 번역 문자열 테이블 (영어, 프랑스어, 독일어 등). |
| `config.h` | 기기별 설정을 포함하는 공용 설정 파일 (실제 설정은 타겟 폴더에 있음). |

#### drivers/ 폴더
디스플레이, 오디오, 입력 장치를 위한 저수준 하드웨어 드라이버들이 위치합니다.

#### targets/ 폴더
지원하는 각 하드웨어 기기(타겟)별 설정 파일이 들어있습니다.  
각 타겟 폴더에는 아래 파일들이 포함됩니다:

| 파일 | 역할 |
|------|------|
| `config.h` | 해당 기기의 GPIO 핀 맵, 화면 해상도, 버튼 배치 등 하드웨어 설정. |
| `sdkconfig` | ESP-IDF의 컴파일 설정 파일 (CPU 주파수, PSRAM 설정 등). |
| `env.py` | `rg_tool.py`가 사용할 환경 변수 (ESP32 칩 종류, 보드 이름 등). |

지원 타겟 목록: `odroid-go`, `mrgc-g32`, `mrgc-gbm`, `esplay-micro`, `retro-esp32`, `byteboi-rev1`, `esp32-s3-devkit`, `fri3d-2024`, `nullnano`, `rachel-esp32`, `redroid-go`, `retro-ruler-V1`, `t-deck-plus`, `vmu`, `sdl2(데스크탑 테스트용)` 등.

#### libs/ 폴더
외부 라이브러리(lodepng, cJSON 등)가 포함되어 있습니다.

---

### launcher (런처 앱)

SD 카드에서 롬 파일을 탐색하고, 에뮬레이터를 실행하는 홈 화면 앱입니다.

| 파일 | 역할 |
|------|------|
| `main/main.c` | 런처 앱의 진입점. 초기화 및 메인 루프. |
| `main/applications.c / .h` | 설치된 에뮬레이터 앱 목록 관리. |
| `main/browser.c / .h` | 롬 파일을 탐색하는 파일 브라우저 UI. |
| `main/bookmarks.c / .h` | 즐겨찾기 및 최근 플레이 기록 관리. |
| `main/gui.c / .h` | 런처 전용 UI 렌더링 및 탭 관리. |
| `main/updater.c / .h` | 펌웨어 업데이트 기능. |
| `main/webui.c / .h` | Wi-Fi 파일 매니저 웹 서버. |
| `main/webui.html.h` | 웹 파일 매니저의 HTML 페이지 (C 헤더로 포함됨). |
| `main/images.c` | 런처에서 사용하는 이미지 데이터 (자동 생성 파일). |
| `CMakeLists.txt` | 런처 앱의 CMake 빌드 설정. |

---

### retro-core (통합 에뮬레이터)

NES, 게임보이 컬러(GBC), SMS/게임 기어/ColecoVision, PC Engine, Lynx, Game & Watch, SNES 등  
여러 에뮬레이터를 하나의 앱으로 묶어 플래시 용량을 절약한 통합 에뮬레이터입니다.

| 파일 | 역할 |
|------|------|
| `main/main.c` | 앱 진입점. 실행할 롬의 확장자를 보고 적절한 에뮬레이터 코어를 선택. |
| `main/main_nes.c` | NES 에뮬레이터 (nofrendo 기반) 초기화 및 실행 루프. |
| `main/main_gbc.c` | 게임보이/게임보이 컬러 에뮬레이터 (gnuboy 기반) 초기화 및 실행 루프. |
| `main/main_sms.c` | SMS/게임 기어/ColecoVision 에뮬레이터 (smsplus 기반) 초기화 및 실행 루프. |
| `main/main_pce.c` | PC Engine 에뮬레이터 (pce-go 기반) 초기화 및 실행 루프. |
| `main/main_lynx.cpp` | Atari Lynx 에뮬레이터 (handy 기반) 초기화 및 실행 루프. |
| `main/main_gw.c` | Game & Watch 에뮬레이터 (gw-emulator 기반) 초기화 및 실행 루프. |
| `main/main_snes.c` | SNES 에뮬레이터 (snes9x 기반) 초기화 및 실행 루프. |
| `main/shared.h` | retro-core 내에서 공유되는 공통 선언. |
| `components/nofrendo/` | NES 에뮬레이터 코어. |
| `components/gnuboy/` | 게임보이/게임보이 컬러 에뮬레이터 코어. |
| `components/smsplus/` | SMS/게임 기어/ColecoVision 에뮬레이터 코어. |
| `components/pce-go/` | PC Engine 에뮬레이터 코어. |
| `components/handy/` | Atari Lynx 에뮬레이터 코어. |
| `components/gw-emulator/` | Game & Watch 에뮬레이터 코어. |
| `components/snes9x/` | SNES 에뮬레이터 코어 (Snes9x 2005 포팅). |

---

### prboom-go (DOOM 에뮬레이터)

DOOM 및 다양한 모드(.WAD 파일)를 플레이할 수 있는 앱입니다.  
PrBoom 2.5.0을 ESP32에 포팅한 것입니다.

| 파일/폴더 | 역할 |
|----------|------|
| `main/` | ESP32용 포트 코드 (입력 처리, 오디오, 디스플레이 연동). |
| `components/prboom/` | DOOM 엔진 원본 코드 (PrBoom 기반). |
| `CMakeLists.txt` | 앱 CMake 빌드 설정. |

---

### gwenesis (메가 드라이브 에뮬레이터)

세가 메가 드라이브(제네시스) 에뮬레이터입니다.  
bzhxx의 Gwenesis를 ESP32에 포팅한 것입니다.

| 파일/폴더 | 역할 |
|----------|------|
| `main/` | ESP32용 포트 코드. |
| `components/gwenesis/` | 메가 드라이브 에뮬레이터 코어. |
| `CMakeLists.txt` | 앱 CMake 빌드 설정. |

---

### fmsx (MSX 에뮬레이터)

MSX 컴퓨터 에뮬레이터입니다.  
Marat Fayzullin의 fMSX를 ESP32에 포팅한 것입니다.  
※ 이 컴포넌트는 별도의 비상업용 라이선스가 적용됩니다.

| 파일/폴더 | 역할 |
|----------|------|
| `main/` | ESP32용 포트 코드. |
| `components/fmsx/` | MSX 에뮬레이터 코어. |
| `CMakeLists.txt` | 앱 CMake 빌드 설정. |

---

### tools (빌드/개발 도구)

| 파일 | 역할 |
|------|------|
| `gen_images.py` | `themes/default` 폴더의 이미지들을 읽어 `launcher/main/images.c`를 자동 생성. 런처 이미지 수정 후 반드시 실행 필요. |
| `mkfw.py` | `.fw` 펌웨어 파일 생성 스크립트. |
| `mkrelease.py` | 릴리즈 빌드 자동화 스크립트. |
| `rg_locate_str.py` | 번역이 필요한 문자열(`_(...)` 패턴)을 찾아 누락된 번역을 `missing_translation.txt`에 출력하는 도구. |
| `font_converter.py` | 폰트 변환 도구. |
| `font_editor.py` | 폰트 편집 도구. |
| `build_sdl2.sh` | 데스크탑(SDL2) 버전 빌드 스크립트. |
| `patches/` | ESP-IDF에 적용할 패치 파일들. `sdcard-fix` (ODROID-GO 필수), `panic-hook` (크래시 로그 저장용). |
| `theme-creator.html` | 브라우저에서 실행하는 테마 미리보기/제작 도구. |

---

### themes (테마)

런처의 비주얼 테마 파일들이 위치합니다.

| 폴더 | 역할 |
|------|------|
| `default/` | 기본 테마. 배경 이미지, 배너, 로고 PNG 파일 포함. |
| `classic/` | 클래식 스타일 테마. |

테마는 SD 카드의 `/retro-go/themes/` 폴더에 복사하면 런처에서 선택할 수 있습니다.  
각 테마 폴더에는 `theme.json`, `background.png`, `banner_*.png`, `logo_*.png`, `preview.png` 파일이 포함됩니다.

---

### assets (리소스)

| 파일 | 역할 |
|------|------|
| `icon.raw` | rg_tool.py에서 `.fw` 파일 생성 시 사용하는 아이콘 이미지. |
| `retro-go-preview.jpg` | README에 사용되는 프로젝트 미리보기 스크린샷. |

---

## 빌드 방법

### 사전 준비
1. [ESP-IDF 4.4 ~ 5.3](https://docs.espressif.com/projects/esp-idf/en/release-v4.4/esp32/get-started/index.html) 설치  
2. ESP-IDF 환경 활성화 (`export.sh` 또는 `export.bat` 실행)  
3. 필요한 경우 `tools/patches/` 안의 패치 적용

### 주요 빌드 명령어

```bash
# 전체 펌웨어 이미지(.fw) 빌드 (ODROID-GO 기준)
python rg_tool.py --target=odroid-go build-fw

# 전체 이미지(.img) 빌드 (esptool로 플래시용)
python rg_tool.py --target=odroid-go build-img

# 특정 앱만 빌드
python rg_tool.py --target=odroid-go build launcher retro-core

# 기기에 플래시
python rg_tool.py --target=odroid-go --port=COM3 flash launcher

# 플래시 후 시리얼 모니터 열기
python rg_tool.py --target=odroid-go --port=COM3 run launcher

# 클린 빌드 (전체 초기화 후 재빌드)
python rg_tool.py clean
python rg_tool.py --target=odroid-go release
```

### 환경 변수로 기본값 설정
```bash
export RG_TOOL_TARGET=odroid-go
export RG_TOOL_PORT=/dev/ttyUSB0
python rg_tool.py build-fw
```

---

## 수정 방법 가이드

### 새 기기(타겟) 추가

1. `components/retro-go/targets/` 폴더 안에서 가장 유사한 기기 폴더를 복사합니다.  
   예: `cp -r components/retro-go/targets/odroid-go components/retro-go/targets/my-device`

2. 복사한 폴더의 `config.h`를 수정합니다:
   - `RG_TARGET_NAME` 을 새 폴더 이름으로 변경
   - GPIO 핀 번호, 화면 크기, 버튼 매핑 등을 실제 하드웨어에 맞게 수정

3. `env.py`에서 ESP32 칩 종류를 실제 칩에 맞게 변경합니다.

4. `sdkconfig`를 생성/수정합니다:
   ```bash
   python rg_tool.py --target=my-device build launcher
   cd launcher
   idf.py menuconfig   # PSRAM, 플래시 속도, CPU 주파수 등 설정
   cd ..
   mv launcher/sdkconfig components/retro-go/targets/my-device/sdkconfig
   ```

5. 빌드 및 테스트:
   ```bash
   python rg_tool.py --target=my-device --port=/dev/ttyUSB0 build-img
   python rg_tool.py --target=my-device --port=/dev/ttyUSB0 install launcher
   ```

특정 타겟에만 적용되는 코드를 추가하려면 `#ifdef RG_TARGET_MY_DEVICE` 블록을 사용합니다:
```c
#ifdef RG_TARGET_MY_DEVICE
    // 이 기기에서만 실행되는 코드
#endif
```

---

### 번역(다국어) 추가

1. `components/retro-go/rg_localization.h`의 `rg_language_t` 열거형에 새 언어를 추가합니다:
   ```c
   typedef enum {
       RG_LANG_EN = 0,
       RG_LANG_FR,
       RG_LANG_DE,
       RG_LANG_KO,   // <-- 한국어 추가
       RG_LANG_MAX
   } rg_language_t;
   ```

2. `components/retro-go/translations.h`에서 각 문자열 항목에 한국어 번역을 추가합니다:
   ```c
   {
       [RG_LANG_EN] = "Yes",
       [RG_LANG_FR] = "Oui",
       [RG_LANG_DE] = "Ja",
       [RG_LANG_KO] = "예",   // <-- 한국어 번역 추가
   },
   ```

3. 누락된 번역 문자열을 확인하려면 아래 도구를 사용합니다:
   ```bash
   python tools/rg_locate_str.py
   # 결과가 missing_translation.txt 파일에 저장됩니다
   ```

---

### 테마 커스터마이징

1. `themes/default/` 폴더를 복사하여 새 테마 폴더를 만듭니다.

2. 이미지 파일들을 수정합니다 (Photoshop, GIMP 등 사용):
   - `background.png` (320×240 픽셀) - 기본 배경
   - `background_<탭이름>.png` - 탭별 배경
   - `banner_<탭이름>.png` (272×24 픽셀) - 탭 배너
   - `logo_<탭이름>.png` (46×50 픽셀) - 탭 로고
   - `preview.png` (160×120 픽셀) - 테마 선택 화면 미리보기

3. `theme.json` 파일에서 색상을 수정합니다 (RGB565 형식):
   ```json
   {
       "description": "나만의 테마",
       "author": "내 이름",
       "dialog": {
           "background": "0x0010",
           "foreground": "0xFFFF"
       }
   }
   ```

4. 기본 테마 이미지를 수정한 경우에는 반드시 아래 명령어로 `images.c`를 재생성합니다:
   ```bash
   python tools/gen_images.py
   ```

5. SD 카드의 `/retro-go/themes/내테마폴더/` 에 파일을 복사하면 런처 설정에서 선택 가능합니다.

---

### 에뮬레이터 수정

각 에뮬레이터는 `components/` 폴더 안에 원본 코드가 있고,  
`main/main_<에뮬레이터>.c` 파일에서 ESP32 환경에 맞게 연결합니다.

**예시: NES 에뮬레이터 수정**
- 원본 코어: `retro-core/components/nofrendo/`
- ESP32 포트 코드: `retro-core/main/main_nes.c`

특정 에뮬레이터만 빌드하여 빠르게 테스트할 수 있습니다:
```bash
python rg_tool.py --target=odroid-go --port=COM3 run retro-core
```

---

## 설치 방법

### ODROID-GO
1. [릴리즈 페이지](https://github.com/ducalex/retro-go/releases/)에서 `retro-go_*.fw` 파일 다운로드.
2. SD 카드의 `/odroid/firmware/` 폴더에 복사.
3. B 버튼을 누른 채 전원 켜기 → 파일 목록에서 선택하여 플래시.

### MRGC-G32
1. `retro-go_*_mrgc-g32.fw` 파일 다운로드.
2. SD 카드의 `/espgbc/firmware/` 폴더에 복사.
3. MENU 버튼을 누른 채 전원 켜기 → 파일 목록에서 선택하여 플래시.

### 기타 기기 (esptool 사용)
1. 해당 기기용 `.img` 파일 다운로드.
2. USB 케이블로 PC에 연결.
3. 아래 명령 실행:
   ```bash
   esptool.py write_flash --flash_size detect 0x0 retro-go_*.img
   ```

---

## 사용 방법

### 게임 커버 아트
SD 카드의 `romart/` 폴더에 PNG 이미지(160×168, 8비트)를 넣으면 게임 목록에 커버 아트가 표시됩니다.
- 파일명 기반: `/romart/nes/Super Mario.png` (확장자 제외)
- CRC32 기반: `/romart/nes/A/ABCDE123.png` (게임의 CRC32 값 사용)

### BIOS 파일
일부 에뮬레이터는 BIOS 파일이 필요합니다. SD 카드의 다음 경로에 위치시키세요:
- GB: `/retro-go/bios/gb_bios.bin`
- GBC: `/retro-go/bios/gbc_bios.bin`
- FDS: `/retro-go/bios/fds_bios.bin`
- MSX: `/retro-go/bios/msx/` 폴더에 `MSX.ROM` 등 파일 넣기

### Wi-Fi 설정
SD 카드에 `/retro-go/config/wifi.json` 파일을 생성합니다:
```json
{
    "ssid0": "내-와이파이",
    "password0": "내-비밀번호"
}
```
연결 후 기기의 IP 주소로 웹 브라우저 접속 시 파일 매니저를 사용할 수 있습니다.

### 세이브 및 불러오기
- 인게임 메뉴(보통 MENU 버튼)에서 세이브 스테이트 저장/불러오기 가능.
- 게임당 최대 여러 개의 슬롯 지원.

---

## 자주 묻는 문제

### 검은 화면 / 부팅 루프
Retro-Go는 크래시를 자동으로 감지하여 복구를 시도합니다.  
부팅 루프에 빠진 경우, 전원 켤 때 `↓` 방향 버튼을 누른 채로 켜면 런처로 돌아갈 수 있습니다.

### 크래시 로그 확인
`tools/patches/panic-hook` 패치가 적용된 경우, 크래시 발생 시 `/sd/crash.log`에 로그가 저장됩니다.  
백트레이스를 분석하려면:
```bash
xtensa-esp32-elf-addr2line -ifCe app-name/build/app-name.elf
```

### ZIP 파일 지원
대부분의 에뮬레이터는 ZIP 파일을 지원합니다.  
ZIP 안에는 롬 파일 하나만 포함되어야 하며, 메모리가 부족한 경우 로드가 실패할 수 있습니다.

### 음질 문제 (ODROID-GO)
상단 볼륨 레벨이 너무 크거나 하단 볼륨 레벨이 왜곡될 수 있습니다.  
스피커 선에 33Ω 저항을 직렬 연결하면 개선됩니다.  
또는 외부 DAC 모드를 사용하세요 (메뉴 → `Audio Out: Ext DAC`).

---

## 라이선스

이 프로젝트의 대부분은 [GPLv2 라이선스](COPYING)를 따릅니다.  
단, 아래 컴포넌트는 별도 라이선스가 적용됩니다:
- `fmsx/components/fmsx` : MSX 에뮬레이터 - 비상업용 커스텀 라이선스
- `retro-core/components/handy` : Lynx 에뮬레이터 - zlib 라이선스
