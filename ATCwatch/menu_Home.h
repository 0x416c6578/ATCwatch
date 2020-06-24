#pragma once
#include "Arduino.h"
#include "classScreen.h"
#include "images.h"
#include "menu.h"
#include "display.h"
#include "menuAppsBase.h"
#include "ble.h"
#include "time.h"
#include "battery.h"
#include "accl.h"
#include "push.h"
#include "heartrate.h"
#include "inputoutput.h"


class HomeScreen : public TheScreen
{
  public:
    HomeScreen() {
    }

    virtual void pre()
    {
      displayRect(0, 0, 240, 240, 0x0000);
      charge_symbol_change = false;
      //displayImage(0, 136, 24, 24, symbolHeartSmall);
      displayImage(0, 136 + 24 + 2, 24, 24, symbolStepsSmall);
      displayImage(0, 136 + 24 + 2 + 24 + 2, 24, 24, symbolMsgSmall);
    }

    virtual void main()
    {
      time_data_struct time_data = get_time();
      accl_data_struct accl_data = get_accl_data();
      if (day_prev != time_data.day){
        int currentDay = getDayOfWeek(time_data.day, time_data.month, time_data.year);
        switch (currentDay){
          case 0:
            sprintf(day_string, "Sun");
            break;
          case 1:
            sprintf(day_string, "Mon");
            break;
          case 2:
            sprintf(day_string, "Tue");
            break;
          case 3:
            sprintf(day_string, "Wed");
            break;
          case 4:
            sprintf(day_string, "Thu");
            break;
          case 5:
            sprintf(day_string, "Fri");
            break;
          case 6:
            sprintf(day_string, "Sat");
            break;
          default:
            sprintf(day_string, "Unk");
            break;
        }
        reset_step_counter(); //Handle resetting step counter here
      }
      char time_string[14];
      sprintf(time_string, "%02i:%02i", time_data.hr, time_data.min);
      char date_string[14];
      sprintf(date_string, "%02i.%02i.%04i", time_data.day, time_data.month, time_data.year);
      
      displayPrintln(20, 42, time_string, 0xFFFF, 0x0000, 4);
      displayPrintln(20, 77, date_string, 0xFFFF, 0x0000, 3);
      displayPrintln(20, 105, day_string, 0xFFFF, 0x0000, 3);

      //displayPrintln(30, 140, (String)get_last_heartrate() + "     ", 0xFFFF, 0x0000, 2); //Display last heartrate disabled for battery tests
      displayPrintln(30, 140 + 24 + 2, (String)accl_data.steps + "     ", 0xFFFF, 0x0000, 2);
      displayPrintln(30, 140 + 24 + 2 + 24 + 2, get_push_msg(17), 0xFFFF, 0x0000, 2);
      
      if (get_vars_ble_connected())
        displayPrintln(1, 1, "BT Up  ", 0xFFFF, 0x0000, 2);
      else
        displayPrintln(1, 1, "BT Down", 0xFFFF, 0x0000, 2);
        
      char bat_string[10];
      if (!get_charge()) {
        /*if (!charge_symbol_change)displayImage(27, 1, 24, 24, symbolBattery2);
        charge_symbol_change = true;
        int batteryPer = get_battery_percent();
        String batteryDisplay = "";
        if (batteryPer < 10)
          batteryDisplay = " " + (String)batteryPer;
        else
          batteryDisplay = (String)batteryPer;
        displayPrintln(33, 9, batteryDisplay, 0x0000, 0xFFFF);*/
        sprintf(bat_string, "%02i%% C", get_battery_percent());
      } else {
        sprintf(bat_string, "%02i%%  ", get_battery_percent());
      }
      displayPrintln(1, 18, bat_string, 0xFFFF, 0x0000, 2);
      day_prev = time_data.day;
    }

    virtual void up()
    {
      inc_vars_menu();
    }

    virtual void down()
    {
      dec_vars_menu();
    }

    virtual void left()
    {
      ble_write("TESTSTRING");   
    }
    virtual void right()
    {
    }
  private:
    bool charge_symbol_change = false;
    int day_prev = -1;
    char day_string[5] = {'U','n','k'};

};