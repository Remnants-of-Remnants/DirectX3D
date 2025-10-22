## vcpkg 설치(사용중이라면 필요 없음)
- git clone https://github.com/microsoft/vcpkg.git
- cd vcpkg
- ./bootstrap-vcpkg.bat
- ./vcpkg integrate install


## vcpkg 사용한 라이브러리 추가

### DirectXTex
- vcpkg install directxtex[openexr]:x64-windows
- vcpkg install physx:x64-windows
- 프로젝트 속성(Engine) → C/C++ → 추가 포함 디렉터리 → $(VcpkgRoot)\installed\x64-windows\include 추가

- ---

## FBX Library 파일 추가

### 경로
- External\Library\FBXLoader\x64\debug\libfbxsdk.dll
- External\Library\FBXLoader\x64\release\libfbxsdk.lib

### 파일 링크
- https://ko.dll-files.com/libfbxsdk.dll.html

---

### 애셋 다운로드
- https://drive.google.com/drive/folders/1MU7BtdgXdCYT3r3RM8ZlJkWkVzCJaH5S?usp=sharing
- 압축 해제 후 OutputFile/contents 폴더에 복사
