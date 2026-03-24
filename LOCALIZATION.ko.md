# Retro-Go 현지화(로컬라이제이션)

이 문서는 Retro-Go에서 사용하는 현지화 프로토콜을 설명합니다.


# C 파일

`translations.h`에는 원본 메시지(영어)와 대응하는 번역이 담겨 있습니다. 예:
````c
{
    [RG_LANG_EN] = "Yes",
    [RG_LANG_FR] = "Oui",
    [RG_LANG_ES] = "Si",
    [RG_LANG_KO] = "예",
},
````

## 새 언어를 추가하려면

`rg_localization.h`의 열거형을 업데이트합니다:
````c
typedef enum
{
    RG_LANG_EN,
    RG_LANG_FR,
    RG_LANG_ES, // <-- 스페인어 추가 예시
    RG_LANG_KO, // <-- 한국어 추가 예시

    RG_LANG_MAX
} rg_language_t;
````

그런 다음 `translations.h`의 각 문자열 항목에 새 언어의 번역을 추가하면 됩니다.

### 한국어 추가 방법 (요약)

1. `rg_localization.h`에 `RG_LANG_KO` 값 추가 (`RG_LANG_MAX` 바로 앞)
2. `translations.h`의 모든 항목에 `[RG_LANG_KO] = "..."` 번역 추가
3. 빌드 후 설정 메뉴의 **Language** 항목에서 **Korean** 선택 가능


# Python 도구

`rg_locate_str.py`는 Retro-Go 프로젝트의 각 파일에서 `_("` 패턴 앞에 있는 모든 문자열을 찾아내는 간단한 Python 도구입니다. 이 도구는 해당 문자열들을 `translations.h`의 항목과 비교하여 누락된 번역을 `missing_translation.txt` 파일에 저장합니다.
