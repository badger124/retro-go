# Retro-Go 테마 만들기

이 문서는 테마가 무엇인지, 어떻게 구성되는지, 그리고 어떻게 만드는지를 설명합니다.

예시 테마는 이 프로젝트의 [themes](/themes/) 폴더에서 확인할 수 있습니다.


## 테마 구조

테마는 `sd:/retro-go/themes` 안에 위치한 폴더로, 다음 파일들을 포함합니다:

````
/retro-go/themes
└── example
    ├── background.png
    ├── background_*.png
    ├── banner_*.png
    ├── logo_*.png
    ├── preview.png
    └── theme.json
````

| 파일명 | 형식 | 설명 | 필수 여부 |
|--|--|--|--|
| `theme.json` | JSON | 테마 메타데이터(설명, 작성자, 색상 등)를 포함합니다. | 필수 |
| `preview.png` | PNG 160x120 | 테마 선택기에 표시되는 테마 미리보기입니다. | 선택 |
| `background.png` | PNG 320x240 | 런처의 기본 배경 이미지입니다. | 선택 |
| `background_<탭이름>.png` | PNG 320x240 | 런처의 탭별 배경 이미지입니다. | 선택 |
| `banner_<탭이름>.png` | PNG 272x24 | 런처의 탭별 배너 이미지입니다. | 선택 |
| `logo_<탭이름>.png` | PNG 46x50 | 런처의 탭별 로고 이미지입니다. | 선택 |


### theme.json

`theme.json` 파일에는 테마에서 사용하는 색상과 작성자 메타데이터가 포함됩니다.

모든 필드는 선택 사항이지만, 개별 값을 생략하는 것은 권장하지 않습니다. 대체 값은 retro-go 버전에 따라 달라질 수 있기 때문입니다. 섹션 전체를 생략하는 것은 괜찮습니다. 예를 들어 런처만 테마로 적용하고 다이얼로그는 그대로 두고 싶다면 해당 섹션을 생략할 수 있습니다.

색상은 RGB565 형식이며, 정수 또는 16진수 문자열로 표현할 수 있습니다. 일부 항목에서는 특수 값 `transparent`(투명)도 사용할 수 있습니다.

<details>
  <summary>theme.json 예시 보기</summary>

````json
{
    "description": "기본 Retro-Go 테마",
    "website": "https://github.com/ducalex/retro-go/",
    "author": "ducalex",
    "dialog": {
        "__comment": "전역 다이얼로그 색상",
        "background": "0x0010",
        "foreground": "0xFFFF",
        "border": "0x6B4D",
        "header": "0xFFFF",
        "scrollbar": "0xFFFF",
        "shadow": "none",
        "item_standard": "0xFFFF",
        "item_disabled": "0x8410",
        "item_message": "0xBDF7"
    },
    "launcher_1": {
        "__comment": "런처 색상 변형 1",
        "background": "0x0000",
        "foreground": "0xFFDE",
        "list_standard_bg": "transparent",
        "list_standard_fg": "0x8410",
        "list_selected_bg": "transparent",
        "list_selected_fg": "0xFFFF"
    },
    "launcher_2": {
        "__comment": "런처 색상 변형 2",
        "background": "0x0000",
        "foreground": "0xFFDE",
        "list_standard_bg": "transparent",
        "list_standard_fg": "0x8410",
        "list_selected_bg": "transparent",
        "list_selected_fg": "0x07E0"
    },
    "launcher_3": {
        "__comment": "런처 색상 변형 3",
        "background": "0x0000",
        "foreground": "0xFFDE",
        "list_standard_bg": "transparent",
        "list_standard_fg": "0x8410",
        "list_selected_bg": "0xFFFF",
        "list_selected_fg": "0x0000"
    },
    "launcher_4": {
        "__comment": "런처 색상 변형 4",
        "background": "0x0000",
        "foreground": "0xFFDE",
        "list_standard_bg": "transparent",
        "list_standard_fg": "0xAD55",
        "list_selected_bg": "0xFFFF",
        "list_selected_fg": "0x0000"
    }
}
````
</details>


### 이미지

이미지 파일의 크기는 가능한 한 작게 유지하는 것이 좋습니다. 로딩 속도에 직접적인 영향을 줍니다. PNG 파일을 저장할 때 가능한 낮은 비트 심도를 사용하거나, [pngquant](https://pngquant.org/) 같은 도구를 활용하세요.

마젠타(rgb(255, 0, 255) / 0xF81F)는 일부 상황에서 투명도 색상으로 사용됩니다.
