#include "../ai09.h"

void ai09::tech_motion_ann ( void )
{
	const float v_max = 3000.0f;
	static float vmag;
	static float vdir;
	static Timer ann_timer;
	if (timer.time()<1.0) {
		vmag = v_max/10.0f;
		vdir = 0.0f;
		ann_timer.start();
		return;
	}
	
	OwnRobot[attack].target.Angle = 90.0f;
	
	if (ann_timer.time()<2.0) {
		ERRTSetObstacles(attack, 0, 1, 1, 1, 0, 0);
		ERRTNavigate2Point(attack, Vec2(1500, 0), 1, 100, &VELOCITY_PROFILE_AROOM);
	}
	else {
		TVec2 robo_d = OwnRobot[attack].State.Position - Vec2(1500, 0);
		if ((fabs(robo_d.X)>1300)||(fabs(robo_d.Y)>1800)) {
			vdir += 40.0f;
			if (vdir>=360) {
				vdir = 0.0f;
				vmag += v_max/10.0f;
			}
			ann_timer.start();
		}
		else {
			float vmag_conv = vmag / 42.5;
			extern TVec2 vdes_ann;
			vdes_ann = Vec2(vmag_conv*cosDeg(vdir), vmag_conv*sinDeg(vdir));
			Navigate2Point(attack, Vec2(0, 0), 0, 100, &VELOCITY_PROFILE_MAMOOLI);
		}

	}

	
	/*for (float vmag = v_max/10.0f ; vmag < v_max ; vmag += v_max/10.0f ) {
		for (float vdir = 0; vdir < 360 ; vdir += 10) {
			float vmag_conv = vmag / 42.5;
			extern TVec2 vdes_ann;
			vdes_ann = Vec2(vmag_conv*cosDeg(vdir), vmag_conv*sinDeg(vdir));
			
			Timer ann_timer;
			ann_timer.start();
			if (ann_timer.time()<2.0) {
				ERRTSetObstacles(attack, 0, 1, 1, 1, 0, 0);
				ERRTNavigate2Point(attack, Vec2(1500, 0), 1, 100, &VELOCITY_PROFILE_MAMOOLI);
			}
			else {
				TVec2 robo_d = OwnRobot[attack].State.Position - Vec2(1500, 0);
				if ((fabs(robo_d.X)>1300)||(fabs(robo_d.Y)>1800)) {
					continue;
				}
				Navigate2Point(attack, Vec2(0, 0), 0, 100, &VELOCITY_PROFILE_MAMOOLI);
			}

		}
	}*/
}