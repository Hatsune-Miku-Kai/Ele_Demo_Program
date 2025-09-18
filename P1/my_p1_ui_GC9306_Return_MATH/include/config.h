#ifndef __CONFIG_h
#define __CONFIG_h
#include "Arduino.h"
#include <vector>
using namespace std;

	typedef struct robotGeometry
	{
		float low_shank_length;
		float high_shank_length;
		float base_offset;
		float end_effector_offsetx;
		float end_effector_offsetz;
	}kiner_coords;

	typedef struct robotCoords
	{
		float xmm;
		float ymm;
		float zmm;
		float emm;
	}robot_coords;
	typedef struct robotRads
	{
		float rot;
		float low;
		float high;
		float end;
	}robot_rads;

inline float SQ(float input)
{
	return input * input;
}

#endif
