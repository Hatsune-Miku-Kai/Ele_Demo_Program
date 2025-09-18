#include "robotGeometry.hpp"

RobotGeometry::RobotGeometry()
{
    low_shank_length = 140;
    high_shank_length = 140;
    end_effector_offsetx = 61;
    end_effector_offsetz = 49;
    base_offset = 22.5;

}


/*****************************************************************************************
*
*           ���˶�ѧ:
*                                 input:angles (J1, J2, J3)
*                                 output:coords(x, y, z)
*
*           ����1*3�ĽǶȣ����1*3������
*
******************************************************************************************/
void RobotGeometry::calculateCoords()
{
    float base, J1, J2, J3, J4, J5;
    float x, y, z, e, theta1, theta2, theta3, theta4;

    theta1 = rot;
    theta2 = low;
    theta3 = high;
    theta4 = end;

    base = 0;
    J1 = 0;
    J2 = low_shank_length;
    J3 = high_shank_length;
    J4 = end_effector_offsetx;
    J5 = end_effector_offsetz;

    xmm = J4 * cos(theta1) + base_offset * cos(theta1) + J2 * cos(theta1) * cos(theta2 - PI / 2) + J3 * cos(theta1) * cos(theta3 - PI / 2);

    ymm = J4 * sin(theta1) + base_offset * sin(theta1) + J2 * cos(theta2 - PI / 2) * sin(theta1) + J3 * cos(theta3 - PI / 2) * sin(theta1);

    zmm = J1 - J5 + base - J2 * sin(theta2 - PI / 2) - J3 * sin(theta3 - PI / 2);

    emm = -theta1 + theta4;

}

/*****************************************************************************************
*
*           ���˶�ѧ:
*                                 input: (ymm, xmm, zmm)�ѿ�������
*                                 output: (rot, low, high)1~3�ؽڽǶ�
*
*
*
******************************************************************************************/
void RobotGeometry::calculateGrad() {
    if (xmm > 0) {
        rot = asin(ymm / hypot(ymm, xmm));
    }
    else {
        if (ymm > 0) {
            rot = PI - asin(ymm / hypot(ymm, xmm));
        }
        else {
            rot = -(PI + asin(ymm / hypot(ymm, xmm)));
        }
    }
    float xmm_offset = xmm - base_offset * cos(rot); //��ȥBase����offset
    float ymm_offset = ymm - base_offset * sin(rot); //��ȥBase����offset
    float zmm_offset = zmm + end_effector_offsetz; //��ȥBase����offset

    float rrot_ee = hypot(ymm_offset, xmm_offset);
    float rrot = rrot_ee - end_effector_offsetx; //radius from Top View
    float rside = hypot(rrot, zmm_offset);  //radius from Side View. Use rrot instead of xmm..for everything
    float rside_2 = sq(rside);
    float low_2 = sq(low_shank_length);
    float high_2 = sq(high_shank_length);
    high = PI - acos((low_2 + high_2 - rside_2) / (2 * low_shank_length * high_shank_length));

    //Angle of Lower Stepper Motor  (asin()=Angle To Gripper)
    if (zmm_offset > 0) {
        low = acos(zmm_offset / rside) - acos((low_2 - high_2 + rside_2) / (2 * low_shank_length * rside));
    }
    else {
        low = PI - asin(rrot / rside) - acos((low_2 - high_2 + rside_2) / (2 * low_shank_length * rside));
    }
    high = high + low;

    end = emm + rot;

}
