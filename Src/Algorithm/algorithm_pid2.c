#include "algorithm_pid2.h"
#include "algorithm_ahrs.h"
#include "usart1_dbg.h"
#define Radian_to_Angle 1.1459156f
#define PID_P 0
#define PID_I 0
#define PID_D 0
#define PID_G_P 0.3
#define PID_G_I 0
#define PID_G_D 0
//0.24
arm_pid_instance_f32 gPidInstanceRoll =
{
  .A0 = 0.0,
  .A1 = 0.0,
  .A2 = 0.0,
  .state = {0.0},
  .Kp = PID_P,
  .Ki = PID_I,
  .Kd = PID_D,
};

arm_pid_instance_f32 gPidInstancePitch =
{
  .A0 = 0.0,
  .A1 = 0.0,
  .A2 = 0.0,
  .state = {0.0},
  .Kp = PID_P,
  .Ki = PID_I,
  .Kd = PID_D,
};

arm_pid_instance_f32 gPidInstanceYaw =
{
  .A0 = 0.0,
  .A1 = 0.0,
  .A2 = 0.0,
  .state = {0.0},
  .Kp = PID_P,
  .Ki = PID_I,
  .Kd = PID_D,
};

arm_pid_instance_f32 gPidInstanceGyrRoll =
{
  .A0 = 0.0,
  .A1 = 0.0,
  .A2 = 0.0,
  .state = {0.0},
  .Kp = PID_G_P,
  .Ki = PID_G_I,
  .Kd = PID_G_D,
};

arm_pid_instance_f32 gPidInstanceGyrPitch =
{
  .A0 = 0.0,
  .A1 = 0.0,
  .A2 = 0.0,
  .state = {0.0},
  .Kp = PID_G_P,
  .Ki = PID_G_I,
  .Kd = PID_G_D,
};

arm_pid_instance_f32 gPidInstanceGyrYaw =
{
  .A0 = 0.0,
  .A1 = 0.0,
  .A2 = 0.0,
  .state = {0.0},
  .Kp = PID_G_P,
  .Ki = PID_G_I,
  .Kd = PID_G_D,
};

void PID2_Init()
{
  arm_pid_init_f32 (&gPidInstanceRoll, 1);
  arm_pid_init_f32 (&gPidInstancePitch, 1);
  arm_pid_init_f32 (&gPidInstanceYaw, 1);
  arm_pid_init_f32 (&gPidInstanceGyrRoll, 1);
  arm_pid_init_f32 (&gPidInstanceGyrPitch, 1);
  arm_pid_init_f32 (&gPidInstanceGyrYaw, 1);
}

void PID2_Reset()
{
  arm_pid_reset_f32 (&gPidInstanceRoll);
  arm_pid_reset_f32 (&gPidInstancePitch);
  arm_pid_reset_f32 (&gPidInstanceYaw);
  arm_pid_reset_f32 (&gPidInstanceGyrRoll);
  arm_pid_reset_f32 (&gPidInstanceGyrPitch);
  arm_pid_reset_f32 (&gPidInstanceGyrYaw);
}

float32_t PID2_Process(float32_t inRoll, float32_t inPitch, float32_t inYaw, uint16_t inThrust)
{
  float32_t x1 = 0,x2 = 0,x3 = 0,x4 = 0;
  float32_t outRoll = 0.0, outPitch = 0.0, outYaw = 0.0;
  float32_t outGyrRoll = 0.0, outGyrPitch = 0.0, outGyrYaw = 0.0;
  outRoll = arm_pid_f32(&gPidInstanceRoll, inRoll);
  outPitch = arm_pid_f32(&gPidInstancePitch, inPitch);
  outYaw = arm_pid_f32(&gPidInstanceYaw, inYaw);
#if 1
  outGyrRoll = arm_pid_f32(&gPidInstanceGyrRoll, outRoll - Radian_to_Angle * Gyr[1]);
  outGyrPitch = arm_pid_f32(&gPidInstanceGyrPitch, outPitch - Radian_to_Angle * Gyr[0]);
  outGyrYaw = arm_pid_f32(&gPidInstanceGyrYaw, outYaw - Radian_to_Angle * Gyr[2]);

  x1 = inThrust - outGyrRoll - outGyrPitch + outGyrYaw;
  x2 = inThrust - outGyrRoll + outGyrPitch - outGyrYaw;
  x3 = inThrust + outGyrRoll + outGyrPitch + outGyrYaw;
  x4 = inThrust + outGyrRoll - outGyrPitch - outGyrYaw;
#else
  x1 = inThrust - outRoll - outPitch + outYaw;
  x2 = inThrust - outRoll + outPitch - outYaw;
  x3 = inThrust + outRoll + outPitch + outYaw;
  x4 = inThrust + outRoll - outPitch - outYaw;
#endif
  Motor_Out((int16_t)x1, (int16_t)x2, (int16_t)x3, (int16_t)x4);
  return 0;
}