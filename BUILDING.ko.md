# 목차
- [Retro-Go 빌드하기](#prerequisites)
- [Retro-Go 포팅하기](#retro-go-포팅하기)
- [IDE 지원](#ide-지원)


# Retro-Go 빌드하기

## 사전 요구사항
[esp-idf](https://docs.espressif.com/projects/esp-idf/en/release-v4.4/esp32/get-started/index.html#get-started-get-prerequisites)가 정상적으로 설치되어 있어야 합니다. 지원 버전은 4.4에서 5.3입니다.

_참고: retro-go 1.44 기준으로 4.4.8을 사용합니다. 1.35~1.43은 4.3, 1.20~1.34는 4.1을 사용했습니다._

### ESP-IDF 패치
완전한 기능을 위해 esp-idf 패치가 필요할 수 있습니다. 패치는 `tools/patches`에 위치하며, 전역 esp-idf 설치에 적용해도 다른 프로젝트나 기기에 영향을 주지 않습니다.
- `sdcard-fix`: ODROID-GO(및 클론 기기)에 **필수**인 패치입니다.
- `panic-hook`: 사용자가 버그를 보고할 수 있도록 돕는 패치입니다. 자세한 내용은 아래 `충돌 로그 캡처` 항목을 참고하세요. 선택 사항이지만 권장합니다.


## 소스 코드 받기

git을 사용하는 방법을 권장하지만, 외부 의존성이 없으므로 프로젝트 페이지에서 ZIP 파일을 다운로드해 압축을 풀어도 됩니다.

retro-go에는 일반적으로 두 개의 활성 브랜치가 있습니다:
- `master`는 가장 최근 릴리즈 코드로, 테스트가 완료된 안정적인 상태입니다.
- `dev`는 다음 릴리즈에 포함될 개발 중인 코드로, 테스트되지 않은 경우가 많습니다.

`git clone -b <브랜치명> https://github.com/ducalex/retro-go/`


## rg_tool.py
ESP-IDF의 `idf.py`는 여러 앱이 포함된 프로젝트를 단독으로 관리하는 데 한계가 있습니다(멀티앱 이미지 빌드, 올바른 파티션에 플래시, 단일 빌드 명령 등). 대신 `rg_tool.py`를 사용해야 하며, 이 도구가 `idf.py` 및 기타 도구에 올바른 인수를 전달합니다.

간결함을 위해 이 문서의 모든 명령어에서 `--target`, `--port` 같은 추가 플래그는 생략합니다. 두 플래그는 각각 환경변수 `RG_TOOL_TARGET`, `RG_TOOL_PORT`로도 지정할 수 있습니다.

사용 가능한 모든 플래그와 명령어는 `python rg_tool.py --help`로 확인하세요.


## 전체 빌드 및 펌웨어 이미지 생성
- SD카드에서 odroid-go-firmware 또는 odroid-go-multi-firmware로 설치하는 `.fw` 파일 생성:\
    `python rg_tool.py build-fw` 또는 `python rg_tool.py release` (클린 빌드)
- esptool.py(시리얼)로 플래시하는 `.img` 파일 생성:\
    `python rg_tool.py build-img` 또는 `python rg_tool.py release` (클린 빌드)

빌드 크기를 줄이려면 원하는 앱만 지정할 수 있습니다. 예를 들어 런처 + DOOM만 빌드하려면:
1. `python rg_tool.py build-fw launcher prboom-go`

`retro-core`에는 NES, PCE, G&W, Lynx, SMS/GG/COL 에뮬레이터가 포함되어 있어 개별 선택이 불가능합니다. 번들로 묶는 이유는 용량 때문으로, 개별 빌드 시 약 3MB인 것이 번들로는 700KB에 불과합니다.


## 이미지 최초 플래시
빌드가 완료된 이미지 파일(`.img` 또는 `.fw`)을 기기에 플래시해야 합니다.

`rg_tool.py`로 `.img` 파일을 플래시하려면:
```
python rg_tool.py --target (대상) --port (USB포트) install (앱)
```

`esptool.py`로 `.img` 파일을 플래시하려면:
```
esptool.py write_flash --flash_size detect 0x0 retro-go_*.img
```

`.fw` 파일을 플래시하려면:

기기에 따라 방법이 다릅니다. [README.md](README.md#installation)를 참고하세요.


## 개별 앱 빌드·플래시·모니터링 (빠른 개발)
전체 Retro-Go 이미지를 한 번 이상 플래시한 후에는 개별 앱만 빠르게 플래시하고 모니터링할 수 있습니다.

1. 플래시: `python rg_tool.py --port=COM3 flash prboom-go`
2. 모니터: `python rg_tool.py --port=COM3 monitor prboom-go`
3. 플래시 후 모니터: `python rg_tool.py --port=COM3 run prboom-go`


## Windows
Windows에서 `./rg_tool.py ...`를 실행하면 잘못된 Python 인터프리터가 호출되거나 아무것도 실행되지 않을 수 있습니다. 이 경우 `python rg_tool.py ...`를 사용하세요.


## 런처 이미지 변경
런처에서 사용하는 모든 이미지(헤더, 로고)는 `themes/default`에 있습니다. 편집 후에는 `tools/gen_images.py` 스크립트를 실행해 `launcher/main/images.c`를 재생성해야 합니다. 이미지 형식에 대한 자세한 내용은 [THEMING.md](THEMING.md)를 참고하세요.


## 번역 업데이트
새 메뉴 항목이나 화면 메시지를 추가할 때는 번역도 함께 추가해야 합니다. Retro-Go는 gettext와 유사한 방식을 사용하며, 개발자는 문자열을 `_(...)`로 감싸기만 하면 됩니다. 실제 번역 추가 방법은 [LOCALIZATION.md](LOCALIZATION.md)를 참고하세요.


## 충돌 로그 캡처
패닉이 발생하면 Retro-Go는 디버그 정보를 `/sd/crash.log`에 저장할 수 있습니다. 드라이버나 시리얼 콘솔 소프트웨어 없이도 사용자가 백트레이스를 손쉽게 수집할 수 있도록 합니다. esp-idf의 패닉 putchar에 약한 후크를 설치해 각 문자를 RTC RAM에 저장하며, 시스템 재부팅 후 해당 데이터를 SD카드로 이동합니다. 이 기능을 활성화하는 작은 esp-idf 패치가 `tools/patches`에 있습니다.

백트레이스를 분석하려면 애플리케이션의 elf 파일이 필요합니다. 분실한 경우 **동일한 esp-idf 및 retro-go 버전**으로 앱을 다시 빌드해 재생성할 수 있습니다. 그런 다음 `xtensa-esp32-elf-addr2line -ifCe app-name/build/app-name.elf`를 실행하세요.



# Retro-Go 포팅하기

## ESP32
새 ESP32 기기에 포팅하는 방법은 [PORTING.md](PORTING.md)를 참고하세요.

## 다른 아키텍처
저는 이 저장소에서 ESP32 이외의 포트를 유지하고 싶지는 않지만, 여러분의 포팅을 쉽게 만들 수 있는 작은 변경이 있다면 알려주세요! Retro-Go의 최소 요구사항은 대략 다음과 같습니다:
- 프로세서: 200Mhz 32비트 리틀엔디언
- 메모리: 2MB
- 컴파일러: C99 (handy-go의 경우 C++03 포함)

모든 애플리케이션은 제약된 환경에 맞게 대폭 수정 또는 재설계되었지만, Retro-Go 및 ESP32 특화 코드는 포트 파일(`main.c`)에만 두도록 세심하게 유지됩니다. 이 덕분에 자신의 코드베이스에서 재사용하기가 매우 쉽습니다!



# IDE 지원

## VS Code
Retro-Go에는 C/C++ 확장을 위한 설정이 포함된 VS Code 워크스페이스 파일이 있습니다.

인텔리센스가 올바르게 동작하려면 *전역* 설정 파일에 경로를 정의해야 합니다:

````json
    "retro-go.sdk-path": "C:/espressif/frameworks/esp-idf-v5.0.4",
    "retro-go.tools-path": "C:/espressif/tools/xtensa-esp32-elf/esp-2021r2-patch3-8.4.0/xtensa-esp32-elf",
````

현재 clangd는 멀티 폴더 워크스페이스에서 잘 동작하지 않아 지원되지 않습니다.
