# OpenCV 수업시간에 사용한 코드들입니다.

코드 사용을 위해 우선 opencv 라이브러리를 import 해야하는데, 아래 글을 참고하시기 바랍니다.

<br><br><br><br><br><br>


- 최초 세팅
    
    image watch 라는 플러그인이 있음. 이미지가 바뀌는걸 쉽게 볼 수 있음.
    
    https://marketplace.visualstudio.com/items?itemName=VisualCPPTeam.ImageWatchForVisualStudio2022
    
    ![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/035eff15-f09a-479e-a15a-a5c35f797427/e393b8d9-7083-4614-b940-a6c6e411ed1e/Untitled.png)
    
    visual studio 닫고 다운로드 진행
    
    다운 끝나면 다시 visual studio 열어서 
    
    ![설치 잘 됬는지 확인하고 열어줌](https://prod-files-secure.s3.us-west-2.amazonaws.com/035eff15-f09a-479e-a15a-a5c35f797427/6904a794-a8fa-45c3-af8a-cc09350f88b0/Untitled.png)
    
    설치 잘 됬는지 확인하고 열어줌
    
    opencv를 설치해줘야 사용이 가능함. 사용할라면 여기에 opencv 포팅해줘야함. 디버깅 할 때 사용하는게 image watch
    
    이제 opencv를 깔아보자. https://opencv.org/ →library→release
    
    ![src 다운로드 cmake 파일을 사용할 때 사용하는거고](https://prod-files-secure.s3.us-west-2.amazonaws.com/035eff15-f09a-479e-a15a-a5c35f797427/d6f97f08-3dd1-46b2-ae78-2c38d65bbbb7/Untitled.png)
    
    src 다운로드 cmake 파일을 사용할 때 사용하는거고
    
    우리는 window버전을 받아서 exe파일을 사용해야한다..!
    
    exe로 압축풀고 build에 해당하는 정보가 포함되어 있느것을 확인할 수 있다. 실행할때 사용할 bin→dll들과 include→lib들이 들어있다.
    
    우리가 진행중인 sln폴더에 opencv라는 thirdparty폴더를 만들고 안에 아까 받은 opencv 폴더를 넣어준다.
    
    ![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/035eff15-f09a-479e-a15a-a5c35f797427/e4579af7-4f09-4563-9a19-c6c8220845a3/Untitled.png)
    
    ![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/035eff15-f09a-479e-a15a-a5c35f797427/07402b86-a46d-4d32-a5c5-1c3002a482aa/Untitled.png)
    
    ![프로젝트→속성 →일반→ 출력 디렉터리→ $(SolutionDir)$Build\$(Platform)](https://prod-files-secure.s3.us-west-2.amazonaws.com/035eff15-f09a-479e-a15a-a5c35f797427/db461bf5-49ea-490f-a820-2cff0120c141/Untitled.png)
    
    프로젝트→속성 →일반→ 출력 디렉터리→ $(SolutionDir)$Build\$(Platform)
    
    solutiondir= D:\woochangmin\C++\src\_AI\VisionAPP이고 그 아래에
    
    \Build가 만들어지고 그 아래에 \x64로 해주고 
    
    여기까지 했으면 하나 더 해야 되는데 대상이
    
    디버그와 릴리즈를 구분하기 위해서 디버그일때는 뒤에 d를 붙여주고싶다.
    
    ![디버그 출력 디렉터리 설정](https://prod-files-secure.s3.us-west-2.amazonaws.com/035eff15-f09a-479e-a15a-a5c35f797427/644c058c-c305-474e-8023-cf584ff6edaa/Untitled.png)
    
    디버그 출력 디렉터리 설정
    
    opencv는 디버그와 릴리즈가 performance차이가 좀 나서 테스트는 디버그에서 성능확인은 릴리즈에서 해줘야됨.
    
    ![릴리즈 출력 디렉터리 설정](https://prod-files-secure.s3.us-west-2.amazonaws.com/035eff15-f09a-479e-a15a-a5c35f797427/22fb3325-f885-454b-8fd5-c43f6c72f9f7/Untitled.png)
    
    릴리즈 출력 디렉터리 설정
    
    릴리즈로 바꾸고 출력디렉터리 변경
    
    실행한 번 하면 폴더안에 $build파일이 생긴다.
    
    ![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/035eff15-f09a-479e-a15a-a5c35f797427/639438d4-fe69-4410-a6c1-60d2d42e8c7e/Untitled.png)
    
    ![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/035eff15-f09a-479e-a15a-a5c35f797427/a292010c-013d-4428-bb28-3b4a6de4ada3/Untitled.png)
    
    기존 코드들이 이렇게 되있으면 
    
    ![디버그와 릴리즈 바꿔가면서 소스파일 속성에서 빌드에서 제외해줘야됨.](https://prod-files-secure.s3.us-west-2.amazonaws.com/035eff15-f09a-479e-a15a-a5c35f797427/8f0b44b5-86a6-477c-ae02-4038e17ed5df/Untitled.png)
    
    디버그와 릴리즈 바꿔가면서 소스파일 속성에서 빌드에서 제외해줘야됨.
    
    ![프로젝트→속성→C/C++→언어→언어표준을 ISO C++20 표준(/std:c++20)로 변경](https://prod-files-secure.s3.us-west-2.amazonaws.com/035eff15-f09a-479e-a15a-a5c35f797427/7804aa2c-4c75-4d6a-bd13-9e8fac899105/Untitled.png)
    
    프로젝트→속성→C/C++→언어→언어표준을 ISO C++20 표준(/std:c++20)로 변경
    
    ![라이브러리 상대경로:   ..\thirdparty\opencv-4.8.0_wnd\build\include](https://prod-files-secure.s3.us-west-2.amazonaws.com/035eff15-f09a-479e-a15a-a5c35f797427/6d49097a-39a1-4819-b6a6-b445cfada651/Untitled.png)
    
    라이브러리 상대경로:   ..\thirdparty\opencv-4.8.0_wnd\build\include
    
    상대경로로 해야한다. 절대경로로 하면 다른 사람에게 줄때 경로가 꼬여서 error난다. 디버그도 해주고 릴리즈도 해줘야된다잉~
    
    ![링커 라이브러리 추가 ..\thirdparty\opencv-4.8.0_wnd\build\x64\vc16\lib](https://prod-files-secure.s3.us-west-2.amazonaws.com/035eff15-f09a-479e-a15a-a5c35f797427/c7b04ca7-0f94-4042-96ed-92ecd3e4adc5/Untitled.png)
    
    링커 라이브러리 추가 ..\thirdparty\opencv-4.8.0_wnd\build\x64\vc16\lib
    
    디버그도 해주고 릴리즈도 해줘야된다잉~
    
    ![아까 세팅한 설정들은 proj 기준으로 위쪽에 있는 cpp,h파일들은 모두 적용된다](https://prod-files-secure.s3.us-west-2.amazonaws.com/035eff15-f09a-479e-a15a-a5c35f797427/b6d856a0-2d2e-4a20-b109-f5dab671f8a2/Untitled.png)
    
    아까 세팅한 설정들은 proj 기준으로 위쪽에 있는 cpp,h파일들은 모두 적용된다
    
    프로젝트 속성→C/C++→링커→입력→추가종속성 추가(..\thirdparty\opencv-4.8.0_wnd\build\x64\vc16\lib안에 lib파일 2개)를 해야하지만 이런식으로 추가하면 프로젝트 추가할때마다 계속  추가해줘야됨. 그래서 헤더파일 하나 만들어서 사용할 헤더파일들 다 복사해서 넣어줌 넣어야 할 파일은 아래에 있다.
    
    ```cpp
    //Common.h
    #pragma once
    
    #include <string>
    #include <iostream>
    #include <algorithm>
    #include <vector>
    #include <map>
    #include <memory>
    #include <fstream> // ofstream header
    #include <format>
    
    #define OPENCV_480
    #include <opencv2/opencv.hpp>
    #include <opencv2/core/ocl.hpp>
    #ifdef OPENCV_480
    #include <opencv2/highgui/highgui_c.h>
    #include <opencv2/imgproc/imgproc_c.h>
    #include <opencv2/imgproc/types_c.h>
    #endif // OPENCV_480
    
    #ifdef _DEBUG//디버그일때는 뭘쓰고 
    #pragma comment(lib,"opencv_world480d.lib")//pragma comment는 
    #else	//RELEASE//릴리즈일때는 뭘쓰고 
    #pragma comment(lib,"opencv_world480.lib")
    #endif
    using namespace std;
    using namespace cv;
    ```
    
    ```cpp
    //03.ISP_Basic.cpp
    //image signal processing
    
    #include "Common.h"
    
    int main()
    {
    	std::string fileName = "../KCCImageNet/stop_img.png";
    	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_ANYCOLOR);
    	return 1;
    }
    ```
    
    실행시키면 오류 뜨는데
    
    ![D:\woochangmin\C++\src\_AI\VisionAPP\thirdparty\opencv-4.8.0_wnd\build\x64\vc16\bin](https://prod-files-secure.s3.us-west-2.amazonaws.com/035eff15-f09a-479e-a15a-a5c35f797427/0b211655-4288-4f13-ab31-ff6cb1f7c1a1/Untitled.png)
    
    D:\woochangmin\C++\src\_AI\VisionAPP\thirdparty\opencv-4.8.0_wnd\build\x64\vc16\bin
    
    이거 dll파일들 복사
    
    ![D:\woochangmin\C++\src\_AI\VisionAPP\$Build\x64 붙여넣고](https://prod-files-secure.s3.us-west-2.amazonaws.com/035eff15-f09a-479e-a15a-a5c35f797427/35db938e-64fb-4458-8c7f-5bc37a9e203f/Untitled.png)
    
    D:\woochangmin\C++\src\_AI\VisionAPP\$Build\x64 붙여넣고
    
    ![빌드→디버그→보기→다른창→image watch 열고](https://prod-files-secure.s3.us-west-2.amazonaws.com/035eff15-f09a-479e-a15a-a5c35f797427/87fd62e7-5146-41fc-ab36-cb49c2f142fd/Untitled.png)
    
    빌드→디버그→보기→다른창→image watch 열고
    
    라인을 cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_ANYCOLOR); 다음 라인으로 넘겨주면 image watch에 이미지가 뜸. 그 이미지 확대해보면 아래에 색깔정보가 나옴.
    
    ![rgb는 3X UINT8 보면 3채널이고 채널 당 1 바이트씩이라는 뜻임.](https://prod-files-secure.s3.us-west-2.amazonaws.com/035eff15-f09a-479e-a15a-a5c35f797427/d3b729db-131b-4b6d-b041-d5f09196d130/Untitled.png)
    
    rgb는 3X UINT8 보면 3채널이고 채널 당 1 바이트씩이라는 뜻임.
    
    ![RGB값을 표현하는거고 아래가 R 가운데가 G 위가 B ](https://prod-files-secure.s3.us-west-2.amazonaws.com/035eff15-f09a-479e-a15a-a5c35f797427/c71e9a37-08b5-4577-86b2-039e46028aaf/Untitled.png)
    
    RGB값을 표현하는거고 아래가 R 가운데가 G 위가 B 
    
    ![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/035eff15-f09a-479e-a15a-a5c35f797427/b94b2cbb-180c-4110-932f-4390bd539dca/Untitled.png)
    
    ![imread의 내부를 살펴보면 2의 승으로 읽기모드의 섹터를 구분해놓은걸 볼 수 있다. ](https://prod-files-secure.s3.us-west-2.amazonaws.com/035eff15-f09a-479e-a15a-a5c35f797427/d5ee8a5f-d7fa-4676-9d3e-b1ab62571362/Untitled.png)
    
    imread의 내부를 살펴보면 2의 승으로 읽기모드의 섹터를 구분해놓은걸 볼 수 있다. 
    
    ![write할때 어떤 형식으로 저장할지 ex)JPG,PNG …..](https://prod-files-secure.s3.us-west-2.amazonaws.com/035eff15-f09a-479e-a15a-a5c35f797427/e689a39e-e678-42ed-b05a-9242f16ade74/Untitled.png)
    
    write할때 어떤 형식으로 저장할지 ex)JPG,PNG …..
    
    ---
    
    ![imread함수가 있는 라이브러리 확장자가 hpp](https://prod-files-secure.s3.us-west-2.amazonaws.com/035eff15-f09a-479e-a15a-a5c35f797427/e22cbf58-d0e1-44c3-9c6a-316da141d5ff/Untitled.png)
    
    imread함수가 있는 라이브러리 확장자가 hpp
    
    - hpp파일의 사용 이유
        
        결론: 템플릿의 경우 선언과 구현을 동시에 하기 위해 사용하고 C++문법사용을 명시하기 위해 hpp로 명명 
        
         .hpp 또는 .hxx 확장자는 왜 굳이 사용했을까? 일반적으로는 C++에서도 헤더파일의 확장자 이름은 C에서처럼 .h를 사용한다. 다만 C++ 문법중에는 정의와 선언을 분리하기 어려운 상황이 몇 가지 있다. 그 중 한가지가 바로 템플릿(template)을 구현할 때이다. 템플릿의 타입이 컴파일시간에 지정되므로 헤더파일에 정의가 구현되어야 한다. 그래서 이런 경우에는 특별히 C++ 문법을 썼음을 확실히 명시하기 위해 .hpp, h++, .hxx 등을 사용하는 것이다. 템플릿 정의의 경우 .txx, .tpp, .tpl이라고 표시하기도 한다. 인라인 함수 정의에는 .ii, .ixx, .ipp, .inl을 쓰기도 한다.
        
    
    ---
    
    enum이 플래그 상태를 명시하기에는 굉장히 좋은 타입이다.
    
    ```cpp
    CV_EXPORTS_W Mat imread( const String& filename, int flags = IMREAD_COLOR );
    //const를 붙여준거는 파일 링크를 불러올때 변하지 않게 하기 위해
    //디폴트 플래그는 IMREAD_COLOR = 1, always convert image to the 3 channel BGR color image.
    //
    //흑백이미지를 컬러로 가져올때는 컬러버퍼는 그레이버퍼 대비 3배더 많은 공간을 차지하고 잇음.
    //그래서 흑백이미지로 컬러이미지를 표현할때에는 RGB에 같은 값을 넣어주게 되는 거임.
    //stinkbug.png(흑백이미지)를 컬러로 열면확인할 수 있음.
    
    ```
    
    ![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/035eff15-f09a-479e-a15a-a5c35f797427/0f6635c1-14d6-40f7-b08f-75f91638096d/Untitled.png)
    
    ![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/035eff15-f09a-479e-a15a-a5c35f797427/2754d1e4-9d47-4f79-86f4-10cab2a6f13a/Untitled.png)
    
    ![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/035eff15-f09a-479e-a15a-a5c35f797427/0511217f-42b7-471c-912d-f6ff72d25d27/Untitled.png)
    
    cpp파일의 내용을 라이브러리에서 상대방에게 열어주고 싶다면 export를 사용하고 반대로 라이브러리의 cpp내용을 사용하고 싶다면 import를 사용한다.
    
    만약 flag의 color를 cv::ImreadModes::IMREAD_GRAYSCALE로 바꾼다면 아래처럼나온다.
    
    ![gray scale은 1X UINT8 보면1채널이고 채널 당 1 바이트씩이라는 뜻임.](https://prod-files-secure.s3.us-west-2.amazonaws.com/035eff15-f09a-479e-a15a-a5c35f797427/b0376634-5bad-4a4c-a4a1-4b47536404c6/Untitled.png)
    
    gray scale은 1X UINT8 보면1채널이고 채널 당 1 바이트씩이라는 뜻임.
    
    ![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/035eff15-f09a-479e-a15a-a5c35f797427/ec0b5c1c-1ec6-4761-8d08-53cbc6ae7611/Untitled.png)
    
    ---
