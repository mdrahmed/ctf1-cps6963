#include "POUS.h"
void LOGGER_init__(LOGGER *data__, BOOL retain) {
  __INIT_VAR(data__->EN,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->ENO,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->TRIG,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->MSG,__STRING_LITERAL(0,""),retain)
  __INIT_VAR(data__->LEVEL,LOGLEVEL__INFO,retain)
  __INIT_VAR(data__->TRIG0,__BOOL_LITERAL(FALSE),retain)
}

// Code part
void LOGGER_body__(LOGGER *data__) {
  // Control execution
  if (!__GET_VAR(data__->EN)) {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(FALSE));
    return;
  }
  else {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(TRUE));
  }
  // Initialise TEMP variables

  if ((__GET_VAR(data__->TRIG,) && !(__GET_VAR(data__->TRIG0,)))) {
    #define GetFbVar(var,...) __GET_VAR(data__->var,__VA_ARGS__)
    #define SetFbVar(var,val,...) __SET_VAR(data__->,var,__VA_ARGS__,val)

   LogMessage(GetFbVar(LEVEL),(char*)GetFbVar(MSG, .body),GetFbVar(MSG, .len));
  
    #undef GetFbVar
    #undef SetFbVar
;
  };
  __SET_VAR(data__->,TRIG0,,__GET_VAR(data__->TRIG,));

  goto __end;

__end:
  return;
} // LOGGER_body__() 





void PLC2_init__(PLC2 *data__, BOOL retain) {
  __INIT_VAR(data__->COLORSENSOR_RED,0,retain)
  __INIT_VAR(data__->COLORSENSOR_GREEN,0,retain)
  __INIT_VAR(data__->COLORSENSOR_BLUE,0,retain)
  __INIT_VAR(data__->RANGESENSOR,0,retain)
  __INIT_VAR(data__->PUMP,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->DOSER_RED,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->DOSER_BLUE,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->SETTLETIME,__time_to_timespec(1, 700, 0, 0, 0, 0),retain)
  __INIT_VAR(data__->DOSETIME,__time_to_timespec(1, 2000, 0, 0, 0, 0),retain)
  __INIT_VAR(data__->TREATMENTCOMPLETE,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->STAGE,0,retain)
  __INIT_VAR(data__->DESIREDDISTANCEFILL,7.0,retain)
  __INIT_VAR(data__->MEASUREDDISTANCE,0,retain)
  __INIT_VAR(data__->MINRED,20,retain)
  TON_init__(&data__->DOSERREDTIMER,retain);
  TON_init__(&data__->DOSERBLUETIMER,retain);
  TON_init__(&data__->SETTLETIMER,retain);
  __INIT_VAR(data__->DOSETIMERDONE,0,retain)
  __INIT_VAR(data__->SETTLEEN,__BOOL_LITERAL(FALSE),retain)
}

// Code part
void PLC2_body__(PLC2 *data__) {
  // Initialise TEMP variables

  __SET_VAR(data__->,MEASUREDDISTANCE,,(INT_TO_REAL(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (INT)__GET_VAR(data__->RANGESENSOR,)) / 100.0));
  __SET_VAR(data__->,DESIREDDISTANCEFILL,,7.0);
  __SET_VAR(data__->,MINRED,,20);
  __SET_VAR(data__->,DOSETIME,,__time_to_timespec(1, 3000, 0, 0, 0, 0));
  __SET_VAR(data__->,SETTLETIME,,__time_to_timespec(1, 5000, 0, 0, 0, 0));
  __SET_VAR(data__->DOSERREDTIMER.,IN,,__GET_VAR(data__->DOSER_RED,));
  __SET_VAR(data__->DOSERREDTIMER.,PT,,__GET_VAR(data__->DOSETIME,));
  TON_body__(&data__->DOSERREDTIMER);
  __SET_VAR(data__->DOSERBLUETIMER.,IN,,__GET_VAR(data__->DOSER_BLUE,));
  __SET_VAR(data__->DOSERBLUETIMER.,PT,,__GET_VAR(data__->DOSETIME,));
  TON_body__(&data__->DOSERBLUETIMER);
  __SET_VAR(data__->,DOSETIMERDONE,,__GET_VAR(data__->DOSERREDTIMER.Q,));
  __SET_VAR(data__->SETTLETIMER.,IN,,__GET_VAR(data__->SETTLEEN,));
  __SET_VAR(data__->SETTLETIMER.,PT,,__GET_VAR(data__->SETTLETIME,));
  TON_body__(&data__->SETTLETIMER);
  {
    INT __case_expression = __GET_VAR(data__->STAGE,);
    if ((__case_expression == 0)) {
      if ((__GET_VAR(data__->MEASUREDDISTANCE,) > __GET_VAR(data__->DESIREDDISTANCEFILL,))) {
        __SET_VAR(data__->,PUMP,,__BOOL_LITERAL(TRUE));
      } else {
        __SET_VAR(data__->,PUMP,,__BOOL_LITERAL(FALSE));
        __SET_VAR(data__->,STAGE,,1);
      };
    }
    else if ((__case_expression == 1)) {
      if ((__GET_VAR(data__->COLORSENSOR_RED,) < __GET_VAR(data__->MINRED,))) {
        __SET_VAR(data__->,STAGE,,2);
        __SET_VAR(data__->,DOSER_RED,,__BOOL_LITERAL(TRUE));
      } else if ((__GET_VAR(data__->COLORSENSOR_RED,) > __GET_VAR(data__->COLORSENSOR_BLUE,))) {
        __SET_VAR(data__->,STAGE,,3);
        __SET_VAR(data__->,DOSER_BLUE,,__BOOL_LITERAL(TRUE));
      } else {
        __SET_VAR(data__->,STAGE,,5);
      };
    }
    else if ((__case_expression == 2)) {
      if (__GET_VAR(data__->DOSERREDTIMER.Q,)) {
        __SET_VAR(data__->,DOSER_RED,,__BOOL_LITERAL(FALSE));
        __SET_VAR(data__->,STAGE,,4);
        __SET_VAR(data__->,SETTLEEN,,__BOOL_LITERAL(TRUE));
      };
    }
    else if ((__case_expression == 3)) {
      if (__GET_VAR(data__->DOSERBLUETIMER.Q,)) {
        __SET_VAR(data__->,DOSER_BLUE,,__BOOL_LITERAL(FALSE));
        __SET_VAR(data__->,STAGE,,4);
        __SET_VAR(data__->,SETTLEEN,,__BOOL_LITERAL(TRUE));
      };
    }
    else if ((__case_expression == 4)) {
      if (__GET_VAR(data__->SETTLETIMER.Q,)) {
        __SET_VAR(data__->,STAGE,,1);
        __SET_VAR(data__->,SETTLEEN,,__BOOL_LITERAL(FALSE));
      };
    }
    else if ((__case_expression == 5)) {
      __SET_VAR(data__->,TREATMENTCOMPLETE,,__BOOL_LITERAL(TRUE));
    }
  };

  goto __end;

__end:
  return;
} // PLC2_body__() 





