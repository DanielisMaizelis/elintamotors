#include "main.h"
#if !REPORT
  // After making the changes, run the program
  //--------------------------------------------------
  // 1. Current Sensor, Stand-By

    // Min Current in Amperes (A):
    #define CURRENT_STAND_BY_MIN   (float)/*Insert raw value here---->*/-1.0//A

    // Max Current in Amperes (A):
    #define CURRENT_STAND_BY_MAX   (float)/*Insert raw value here---->*/0.4//A

  //--------------------------------------------------

  // 2. Current Sensor, Load before connection tests:

    // Min Current in Amperes (A):
    #define CURRENT_LOAD_MIN       (float)/*Insert raw value here---->*/0//A

    // Max Current in Amperes (A):
    #define CURRENT_LOAD_MAX       (float)/*Insert raw value here---->*/0.4//A

  //--------------------------------------------------

  // 3. Current Sensor, Load after connection tests:

    // Min Current in Amperes (A):
    #define CURRENT_LOAD_AGAIN_MIN (float)/*Insert raw value here---->*/0//A

    // Max Current in Amperes (A):
    #define CURRENT_LOAD_AGAIN_MAX (float)/*Insert raw value here---->*/0.4//A

  //--------------------------------------------------

  // 4. Drivetrain 14 Pin Voltage Min and Max:

    // Min Voltage in (V):
    #define VOLTAGE_DT14_MIN      (float)/*Insert raw value here---->*/2.71//V

    // Max Voltage in (V):
    #define VOLTAGE_DT14_MAX      (float)/*Insert raw value here---->*/2.81//V

  //--------------------------------------------------

  // 5. Drivetrain 23 Pin Voltage Min and Max:

    // Min Voltage in (V):
    #define VOLTAGE_DT23_MIN      (float)/*Insert raw value here---->*/2.85//V

    // Max Voltage in (V):
    #define VOLTAGE_DT23_MAX      (float)/*Insert raw value here---->*/3.05//V

  //--------------------------------------------------
#endif
