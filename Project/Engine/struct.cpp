#include "pch.h"

ofstream& operator<<(ofstream& fout, const tLightInfo& info)
{
	fout << "[LightType]" << endl;
	auto str = magic_enum::enum_name((LIGHT_TYPE)info.LightType);
	fout << ToString(str) << endl;

	fout << "[LightColor]" << endl;
	fout << info.vColor << endl;

	fout << "[Specular]" << endl;
	fout << info.vSpecular << endl;

	fout << "[Ambient]" << endl;
	fout << info.vAmbient << endl;

	fout << "[WorldPos]" << endl;
	fout << info.vWorldPos << endl;

	fout << "[WorldDir]" << endl;
	fout << info.vWorldDir << endl;

	fout << "[Radius]" << endl;
	fout << info.fRadius << endl;

	fout << "[Angle]" << endl;
	fout << info.fAngle;

    return fout;
}

ifstream& operator>>(ifstream& fin, tLightInfo& info)
{
	string tag, str;
	getline(fin, tag); // [LightType]
	getline(fin, str);

	auto type = magic_enum::enum_cast<LIGHT_TYPE>(str);

	if (!type.has_value())
	{
		MessageBox(nullptr, L"���� Enum Ÿ���Դϴ�.", L"Light 3D Load", 0);
		assert(0);
	}
	info.LightType = (int)type.value();

	getline(fin, tag); // [LightColor]
	fin >> info.vColor;
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [Specular]
	fin >> info.vSpecular;
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [Ambient]
	fin >> info.vAmbient;
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [WorldPos]
	fin >> info.vWorldPos;
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [WorldDir]
	fin >> info.vWorldDir;
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [Radius]
	fin >> info.fRadius;
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [Angle]
	fin >> info.fAngle;
	getline(fin, str); // ���� ó��

	return fin;
}

ofstream& operator<<(ofstream& fout, const tParticleModule& module)
{
	// Sapwn ���
	fout << "[" << ToString(magic_enum::enum_name(PARTICLE_MODULE::SPAWN)) << "]" << endl;
	fout << module.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] << endl;

	fout << "[SpawnColor]" << endl;
	fout << module.vSpawnColor << endl;

	fout << "[SpawnMinScale]" << endl;
	fout << module.vSpawnMinScale << endl;
	fout << "[SpawnMaxScale]" << endl;
	fout << module.vSpawnMaxScale << endl;

	fout << "[MinLife]" << endl;
	fout << module.MinLife << endl;
	fout << "[MaxLife]" << endl;
	fout << module.MaxLife << endl;

	fout << "[MinMass]" << endl;
	fout << module.MinMass << endl;
	fout << "[MaxMass]" << endl;
	fout << module.MaxMass << endl;

	fout << "[SpawnRate]" << endl;
	fout << module.SpawnRate << endl;

	fout << "[SpaceType] (0 : LocalSpace, 1 : WorldSpace)" << endl;
	fout << module.SpaceType << endl;

	fout << "[SpawnShape] (0 : Sphere, 1 : Box)" << endl;
	fout << module.SpawnShape << endl;

	fout << "[Radius]" << endl;
	fout << module.Radius << endl;

	fout << "[SpawnBoxScale]" << endl;
	fout << module.vSpawnBoxScale << endl;

	// Add Velocity
	fout << "[" << ToString(magic_enum::enum_name(PARTICLE_MODULE::ADD_VELOCITY)) << "]" << endl;
	fout << module.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] << endl;

	fout << "[AddVelocityType] (0 : From Center, 1: To Center, 2: Fix Direction)" << endl;
	fout << module.AddVelocityType << endl;

	fout << "[MinSpeed]" << endl;
	fout << module.MinSpeed << endl;
	fout << "[MaxSpeed]" << endl;
	fout << module.MaxSpeed << endl;
	
	fout << "[FixedAngle]" << endl;
	fout << module.FixedAngle << endl;

	fout << "[FixedDirection]" << endl;
	fout << module.FixedDirection << endl;

	// Scale
	fout << "[" << ToString(magic_enum::enum_name(PARTICLE_MODULE::SCALE)) << "]" << endl;
	fout << module.arrModuleCheck[(UINT)PARTICLE_MODULE::SCALE] << endl;

	fout << "[ScaleRatio]" << endl;
	fout << module.vScaleRatio << endl;

	// Noise Force
	fout << "[" << ToString(magic_enum::enum_name(PARTICLE_MODULE::NOISE_FORCE)) << "]" << endl;
	fout << module.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] << endl;

	fout << "[NoiseForceScale]" << endl;
	fout << module.NoiseForceScale << endl;

	fout << "[NoiseForceTerm]" << endl;
	fout << module.NoiseForceTerm << endl;

	// Drag
	fout << "[" << ToString(magic_enum::enum_name(PARTICLE_MODULE::DRAG)) << "]" << endl;
	fout << module.arrModuleCheck[(UINT)PARTICLE_MODULE::DRAG] << endl;

	fout << "[DragTime]" << endl;
	fout << module.DragTime << endl;

	// Render
	fout << "[" << ToString(magic_enum::enum_name(PARTICLE_MODULE::RENDER)) << "]" << endl;
	fout << module.arrModuleCheck[(UINT)PARTICLE_MODULE::RENDER] << endl;

	fout << "[VelocityAlignment]" << endl;
	fout << module.VelocityAlignment << endl;
	fout << "[AlphaBasedLife] (0 : off, 1 : NomrlizedAge, 2: Age)" << endl;
	fout << module.AlphaBasedLife << endl;
	fout << "[AlphaMaxAge]" << endl;
	fout << module.AlphaMaxAge << endl;

	return fout;
}

ifstream& operator>>(ifstream& fin, tParticleModule& module)
{
	string tag, str;

	// ���� ���
	getline(fin, tag); // [Module]
	fin >> module.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN];
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [SpawnColor]
	fin >> module.vSpawnColor;
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [SpawnMinScale]
	fin >> module.vSpawnMinScale;
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [SpawnMaxScale]
	fin >> module.vSpawnMaxScale;
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [MinLife]
	fin >> module.MinLife;
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [MinLife]
	fin >> module.MinLife;
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [MaxLife]
	fin >> module.MaxLife;
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [MinMass]
	fin >> module.MinMass;
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [MaxMass]
	fin >> module.MaxMass;
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [SpawnRate]
	fin >> module.SpawnRate;
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [SpaceType] (0 : LocalSpace, 1 : WorldSpace)
	fin >> module.SpaceType;
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [SpawnShape] (0 : Sphere, 1 : Box)
	fin >> module.SpawnShape;
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [Radius]
	fin >> module.Radius;
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [SpawnBoxScale]
	fin >> module.vSpawnBoxScale;
	getline(fin, str); // ���� ó��

	// Add Velocity
	getline(fin, tag); // [Module]
	fin >> module.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY];
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [AddVelocityType] (0 : From Center, 1: To Center, 2: Fix Direction)
	fin >> module.AddVelocityType;
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [MinSpeed]
	fin >> module.MinSpeed;
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [MaxSpeed]
	fin >> module.MaxSpeed;
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [FixedAngle]
	fin >> module.FixedAngle;
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [FixedDirection]
	fin >> module.FixedDirection;
	getline(fin, str); // ���� ó��

	// Scale Module
	getline(fin, tag); // [Module]
	fin >> module.arrModuleCheck[(UINT)PARTICLE_MODULE::SCALE];
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [ScaleRatio]
	fin >> module.vScaleRatio;
	getline(fin, str); // ���� ó��

	// Noise Force
	getline(fin, tag); // [Module]
	fin >> module.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE];
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [NoiseForceScale]
	fin >> module.NoiseForceScale;
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [NoiseForceTerm]
	fin >> module.NoiseForceTerm;
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [ScaleRatio]
	fin >> module.vScaleRatio;
	getline(fin, str); // ���� ó��

	// Drag
	getline(fin, tag); // [Module]
	fin >> module.arrModuleCheck[(UINT)PARTICLE_MODULE::DRAG];
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [DragTime]
	fin >> module.DragTime;
	getline(fin, str); // ���� ó��

	// Render
	getline(fin, tag); // [Module]
	fin >> module.arrModuleCheck[(UINT)PARTICLE_MODULE::RENDER];
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [VelocityAlignment]
	fin >> module.VelocityAlignment;
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [AlphaBasedLife] (0 : off, 1 : NomrlizedAge, 2: Age)
	fin >> module.AlphaBasedLife;
	getline(fin, str); // ���� ó��

	getline(fin, tag); // [AlphaMaxAge]
	fin >> module.AlphaMaxAge;
	getline(fin, str); // ���� ó��

	return fin;
}
