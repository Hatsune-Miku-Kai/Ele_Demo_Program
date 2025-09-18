#pragma once
#ifndef ROBOTGEOMETRY_HPP_
#define ROBOTGEOMETRY_HPP_
#include <math.h>
#include "config.hpp"
#define PI 3.1415926535



class RobotGeometry
{
public:
	RobotGeometry();

	void calculateCoords();
	void calculateGrad();

public:
	float low_shank_length;
	float high_shank_length;
	float base_offset;
	float end_effector_offsetx;
	float end_effector_offsetz;

	float xmm;
	float ymm;
	float zmm;
	float emm;
	float rot;
	float low;
	float high;
	float end;
};

#endif
