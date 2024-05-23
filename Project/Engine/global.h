﻿#pragma once


#include <stdlib.h>
#include <Windows.h>
#include <typeinfo>

#include <string>

#include <vector>
#include <list>
#include <map>
#include <unordered_map>

using std::vector;
using std::list;
using std::unordered_map;
using std::map;
using std::make_pair;

using std::wstring;
using std::string;

#include <memory>
using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
using std::make_unique;
using std::weak_ptr;

#include <wrl.h>
using namespace Microsoft::WRL;

#include <filesystem>
using namespace std::filesystem;

#include <fstream>
using std::ofstream;
using std::ifstream;
using std::endl;


// Directx 11 헤더 및 라이브러리 참조
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

using namespace DirectX;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

// DirectxTex
#include <DirectXTex/DirectXTex.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex_debug")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex")
#endif


// SimpleMath
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;




// 엔진 헤더 참조
#include "singleton.h"
#include "define.h"
#include "struct.h"
#include "func.h"


// Magic Enum 헤더 참조
#include <Magic_Enum/magic_enum_all.hpp>

// String Table(Gen)
#include "GenStrings.h"

// String Table(User)
#include "UserStrings.h"

