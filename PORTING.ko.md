# 목차
- [사전 요구사항](#사전-요구사항)
- [대상(Target)](#대상target)
- [포팅](#포팅)
- [패치](#패치)


# 소개

이 문서는 새 ESP32 기기에 대한 지원을 추가하는 과정(retro-go에서 *대상(target)*이라고 부르는 것)을 설명합니다.


# 사전 요구사항

## 하드웨어 사전 요구사항
Retro-Go는 PSRAM(SPIRAM 또는 외부 RAM이라고도 함)이 있는 모든 ESP32 칩(s2, s3, p4 변형 포함) 기기에서 동작합니다. 또한 시리얼 케이블이 필요합니다(대부분의 ESP32 기기는 USB를 통해 시리얼 인터페이스를 제공합니다).


## 소프트웨어 사전 요구사항
esp-idf가 정상적으로 설치되어 있어야 합니다. Retro-Go는 여러 버전을 지원하며, 자세한 내용은 [BUILDING.md](BUILDING.md)를 참고하세요. esp-idf/esp32 개발이 처음이라면, retro-go처럼 복잡한 프로젝트를 시도하기 전에 몇 가지 샘플 프로그램을 기기에 플래시해 환경을 먼저 확인해 보세요.


# 대상(Target)
retro-go에서 대상(target)은 하드웨어 기기를 설명하는 파일 모음(사용할 드라이버, GPIO, 버튼 매핑 등)입니다. 대상은 `components/retro-go/targets/` 안의 폴더이며, 보통 다음 파일들을 포함합니다:

| 파일명        | 설명 |
|---------------|------|
| config.h      | retro-go 설정 파일로, 하드웨어 정보를 retro-go에 알려줍니다. |
| env.py        | rg_tool.py가 불러오는 파일로, ESP32 칩 종류·보드레이트·바이너리 형식 등 환경변수를 설정합니다. |
| sdkconfig     | 보드/기기에 맞는 esp-idf 설정 파일입니다. |


# 포팅
Retro-Go는 재사용 가능한 하드웨어 컴포넌트를 사용하므로 자신의 하드웨어에 맞게 설정하기 쉽습니다.

시작하려면 자신의 기기와 가장 유사한 대상을 찾아 시작점으로 사용하세요. 해당 대상 폴더를 복사해 새 이름을 붙인 뒤, `components/retro-go/config.h`에 새 대상을 추가합니다.


## 대상 파일

### config.h

이 파일에 대부분의 설정이 들어갑니다. 현재 모든 옵션에 대한 완전한 문서는 없지만, 대부분의 옵션이 담긴 odroid-go의 config.h를 참고하세요.

`RG_TARGET_NAME`을 대상 폴더 이름과 일치하도록 수정하는 것을 잊지 마세요.

#### 디스플레이

Retro-Go의 출력 드라이버는 ILI9341/ST7789 단일 드라이버입니다. 다행히 시중의 대부분 화면이 이 컨트롤러를 사용합니다!

올바른 핀아웃(`RG_GPIO_LCD_*`)과 초기화 시퀀스(`RG_SCREEN_INIT`)를 정의해야 합니다. 필요한 명령 시퀀스는 해당 디스플레이/기기용 다른 예제 코드를 참고하세요.

ILI9341/ST7789 드라이버를 사용하지 않는 경우 직접 드라이버를 작성해야 합니다.

`components/retro-go/drivers/ili9341.h`를 복사해 시작점으로 사용하세요. `rg_display.c` 상단에 드라이버를 추가하는 것도 잊지 마세요:

```c
#if RG_SCREEN_DRIVER == 0 /* ILI9341/ST7789 */
#include "drivers/display/ili9341.h"
#elif RG_SCREEN_DRIVER == 2             // <--
#include "drivers/display/my-driver.h"  // <--
#elif RG_SCREEN_DRIVER == 99
#include "drivers/display/sdl2.h"
#else
#include "drivers/display/dummy.h"
#endif
```

#### 입력

Retro-Go에는 다섯 가지 입력 드라이버가 있습니다:
- GPIO: ESP32 핀에 직접 연결된 버튼
- ADC: 보통 전압 분배기로 여러 버튼을 하나의 ESP32 핀에 연결할 때 사용
- I2C: GPIO 확장기라고도 하며, AW9523·PCF9539·MCP23017·PCF8575 등 다양한 칩을 지원
- 시프트 레지스터: SNES 컨트롤러나 74HC165 같은 방식
- 가상: 버튼이 부족한 경우 콤보를 없는 키에 매핑. 예: Start+Select = 메뉴

`config.h`에서 `RG_GAMEPAD_*_MAP`을 정의해 여러 드라이버를 조합할 수 있습니다. 설정 방법은 `rg_input.h` 또는 다른 대상을 참고하세요.


### sdkconfig

이 파일은 ESP32 자체를 위한 빌드에 사용됩니다.

Retro-Go는 대부분 sdkconfig에 종속되지 않으며 내용은 사용하는 칩/보드에 따라 결정됩니다. 단, 다음 사항에 주의하세요:
- 메인 태스크 스택 크기는 retro-go를 위해 최소 8KB 이상이어야 합니다 ([CONFIG_ESP_MAIN_TASK_STACK_SIZE](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/kconfig-reference.html#config-esp-main-task-stack-size))
- CPU 주파수는 최대치로, 전력 관리는 비활성화해야 합니다 ([CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/kconfig-reference.html#config-esp-default-cpu-freq-mhz), [CONFIG_PM_ENABLE](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/kconfig-reference.html#config-pm-enable))
- SPIRAM은 기기에 맞게 활성화 및 구성해야 합니다 ([CONFIG_SPIRAM_\*](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/kconfig-reference.html#esp-psram))
- 플래시 및 SPIRAM 속도는 기기가 지원하는 최대치로 설정하세요 ([CONFIG_ESPTOOLPY_FLASHFREQ](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/kconfig-reference.html#config-esptoolpy-flashfreq), [CONFIG_SPIRAM_SPEED](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/kconfig-reference.html#config-spiram-speed))
- FATFS에서 긴 파일명(LFN) 지원을 활성화해야 합니다 ([CONFIG_FATFS_LFN_HEAP](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/kconfig-reference.html#config-fatfs-long-filenames))
- FATFS에서 UTF-8 지원을 활성화해야 합니다 ([CONFIG_FATFS_API_ENCODING_UTF_8](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/kconfig-reference.html#config-fatfs-api-encoding))

ESP-IDF는 `idf.py menuconfig`로 편집할 수 있으며, retro-go에서는 다음 단계를 따르세요:

1. 대상에 맞게 런처를 빌드합니다 (올바른 ESP32 보드 선택 및 기본 sdkconfig 생성)
    - `./rg_tool.py clean`
    - `./rg_tool.py --target=my-target build launcher`
2. 런처 디렉터리로 이동: `cd launcher`
3. `idf.py menuconfig`를 실행하고 필요한 변경 후 저장 및 종료
4. 선택 사항: 새 설정으로 앱 테스트 (이 시점에서 `rg_tool.py clean`을 실행하면 새 설정이 삭제되므로 **하지 마세요**)
    - `cd ..`
    - `./rg_tool.py --target=my-target run launcher`
5. 만족스러우면, `sdkconfig` 파일을 런처에서 대상 폴더로 복사하여 모든 앱에서 사용되도록 합니다
    - `cd ..`
    - `mv -f launcher/sdkconfig components/retro-go/targets/my-target/sdkconfig`

대상의 `sdkconfig` 파일을 수정할 때마다 `./rg_tool.py clean`을 실행해야 적용됩니다.


### env.py

`env.py`의 대상 보드를 자신의 보드에 맞게 변경하세요.


이 모든 단계를 완료하면 `rg_tool`로 앱을 빌드할 수 있습니다. 플래시에 대한 자세한 내용은 [BUILDING.md](BUILDING.md#이미지-최초-플래시)를 참고하세요. **`.fw`가 아닌 `.img` 단계를 따르세요.**


# 패치

모든 것을 `config.h`만으로 해결할 수는 없습니다. 때로는 하드웨어에 특화된 코드를 추가하기 위해 retro-go를 직접 수정해야 합니다. 이러한 수정은 업스트림에 반영하기 더 어렵지만, 수행하기는 쉽습니다.

빌드 시스템은 사용 중인 대상에 대한 상수를 자동으로 정의합니다. 예를 들어 대상 이름이 `my-target`이면 `RG_TARGET_MY_TARGET`이 정의됩니다(이름은 대문자로 변환되고 `-`는 `_`로 대체됩니다).

retro-go 어디에서든 `#ifdef` 블록으로 감싸 특정 대상에만 적용되는 코드를 추가할 수 있습니다:

````c
#ifdef RG_TARGET_MY_TARGET
// 여기에 코드 추가
#endif
````
