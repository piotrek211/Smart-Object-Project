
/**
 * @file <argos3/plugins/robots/object/control_interface/ci_smartobject_leds_actuator.cpp>
 */

#include "ci_smartobject_leds_actuator.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * This function expects the stack to have either two or four arguments.
    * The first argument must always be the index of the LED to set.
    * Then, in case two arguments are passed, the second argument can be the string
    * definition of a color. In case of four arguments, the RGB values are expected.
    */
   int LuaLEDSetSingleColor(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 2 && lua_gettop(pt_lua_state) != 4) {
         return luaL_error(pt_lua_state, "robot.leds.set_single_color() expects 2 or 4 arguments");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TNUMBER);
      size_t unIdx = lua_tonumber(pt_lua_state, 1);
      /* Get reference to actuator */
      CCI_SmartObjectLEDsActuator* pcAct = CLuaUtility::GetDeviceInstance<CCI_SmartObjectLEDsActuator>(pt_lua_state, "leds");
      if(unIdx < 1 || unIdx > pcAct->GetNumLEDs()) {
         return luaL_error(pt_lua_state, "passed index %d out of bounds [1,%d]", unIdx, pcAct->GetNumLEDs());
      }
      /* Create color buffer */
      CColor cColor;
      if(lua_gettop(pt_lua_state) == 2) {
         luaL_checktype(pt_lua_state, 2, LUA_TSTRING);
         try {
            cColor.Set(lua_tostring(pt_lua_state, 2));
         }
         catch(CARGoSException& ex) {
            return luaL_error(pt_lua_state, ex.what());
         }
      }
      else {
         luaL_checktype(pt_lua_state, 2, LUA_TNUMBER);
         luaL_checktype(pt_lua_state, 3, LUA_TNUMBER);
         luaL_checktype(pt_lua_state, 4, LUA_TNUMBER);
         cColor.Set(lua_tonumber(pt_lua_state, 2),
                    lua_tonumber(pt_lua_state, 3),
                    lua_tonumber(pt_lua_state, 4));
      }
      /* Perform action */
      pcAct->SetSingleColor(unIdx - 1, cColor);
      return 0;
   }

   /*
    * This function expects the stack to have either one or three arguments.
    * In case one argument is passed, it must be the string definition of a color.
    * In case of three arguments, the RGB values are expected.
    */
   int LuaLEDSetAllColors(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 1 && lua_gettop(pt_lua_state) != 3) {
         return luaL_error(pt_lua_state, "robot.leds.set_all_colors() expects 1 or 3 arguments");
      }
      /* Create color buffer */
      CColor cColor;
      if(lua_gettop(pt_lua_state) == 1) {
         luaL_checktype(pt_lua_state, 1, LUA_TSTRING);
         try {
            cColor.Set(lua_tostring(pt_lua_state, 1));
         }
         catch(CARGoSException& ex) {
            return luaL_error(pt_lua_state, ex.what());
         }
      }
      else {
         luaL_checktype(pt_lua_state, 1, LUA_TNUMBER);
         luaL_checktype(pt_lua_state, 2, LUA_TNUMBER);
         luaL_checktype(pt_lua_state, 3, LUA_TNUMBER);
         cColor.Set(lua_tonumber(pt_lua_state, 1),
                    lua_tonumber(pt_lua_state, 2),
                    lua_tonumber(pt_lua_state, 3));
      }
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_SmartObjectLEDsActuator>(pt_lua_state, "leds")->
         SetAllColors(cColor);
      return 0;
   }
#endif

   /****************************************/
   /****************************************/

   size_t CCI_SmartObjectLEDsActuator::GetNumLEDs() const {
     return m_tSettings.size();
   }

   /****************************************/
   /****************************************/

   void CCI_SmartObjectLEDsActuator::SetSingleColor(UInt32 un_led_number,
                                         const CColor& c_color) {

      UInt32 un_leds_per_led = m_tSettings.size()/8;  
      size_t starting_index = un_led_number * un_leds_per_led;                             
      for (size_t i = starting_index; i < (starting_index + un_leds_per_led); i++) {
      	m_tSettings[i] = c_color;
      }
   }
      
   /****************************************/
   /****************************************/

   void CCI_SmartObjectLEDsActuator::SetAllColors(const CColor& c_color) {
      for(size_t i = 0; i < m_tSettings.size(); ++i) {
         m_tSettings[i] = c_color;
      }
   }

   /****************************************/
   /****************************************/

   void CCI_SmartObjectLEDsActuator::SetAllColors(const TSettings& c_colors) {
      m_tSettings = c_colors;
   }

   /****************************************/
   /****************************************/

   void CCI_SmartObjectLEDsActuator::SetSingleIntensity(UInt32 un_led_number,
                                             UInt8 un_intensity) {
      m_tSettings[un_led_number].SetAlpha(un_intensity);
   }

   /****************************************/
   /****************************************/

   void CCI_SmartObjectLEDsActuator::SetAllIntensities(UInt8 un_intensity) {
      for(size_t i = 0; i < m_tSettings.size(); ++i) {
         m_tSettings[i].SetAlpha(un_intensity);
      }
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_SmartObjectLEDsActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "leds");
      CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
      CLuaUtility::AddToTable(pt_lua_state, "set_single_color", &LuaLEDSetSingleColor);
      CLuaUtility::AddToTable(pt_lua_state, "set_all_colors", &LuaLEDSetAllColors);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

}
