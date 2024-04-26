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
