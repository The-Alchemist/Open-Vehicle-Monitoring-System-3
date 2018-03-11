/*
;    Project:       Open Vehicle Monitor System
;    Date:          14th March 2017
;
;    Changes:
;    1.0  Initial release
;
;    (C) 2011       Michael Stegen / Stegen Electronics
;    (C) 2011-2017  Mark Webb-Johnson
;    (C) 2011        Sonny Chen @ EPRO/DX
;
; Permission is hereby granted, free of charge, to any person obtaining a copy
; of this software and associated documentation files (the "Software"), to deal
; in the Software without restriction, including without limitation the rights
; to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
; copies of the Software, and to permit persons to whom the Software is
; furnished to do so, subject to the following conditions:
;
; The above copyright notice and this permission notice shall be included in
; all copies or substantial portions of the Software.
;
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
; THE SOFTWARE.
*/

#ifndef __OVMS_BOOT_H__
#define __OVMS_BOOT_H__

#include "rom/rtc.h"
#include "esp_system.h"

typedef enum
  {
    BR_PowerOn = 0,                 // standard power on / hard reset
    BR_SoftReset,                   // user requested reset ("module reset")
    BR_EarlyCrash,                  // crash during boot/init phase
    BR_Crash,                       // crash after reaching stable state
  } bootreason_t;

typedef struct
  {
  unsigned int boot_count;          // Number of times system has rebooted (not power on)
  RESET_REASON bootreason_cpu0;     // Reason for last boot on CPU#0
  RESET_REASON bootreason_cpu1;     // Reason for last boot on CPU#1
  bool soft_reset;                  // true = user requested reset ("module reset")
  bool stable_reached;              // true = system has reached stable state (see housekeeping)
  unsigned int crash_count_early;   // Number of times system has crashed before reaching stable state
  unsigned int crash_count_total;   // Total number of times system has crashed since power on
  } boot_data_t;

extern boot_data_t boot_data;

class Boot
  {
  public:
    Boot();
    virtual ~Boot();

  public:
    bootreason_t GetBootReason() { return m_bootreason; }
    const char* GetBootReasonName();
    unsigned int GetCrashCount() { return boot_data.crash_count_total; }
    unsigned int GetEarlyCrashCount() { return m_crash_count_early; }
    void SetSoftReset() { boot_data.soft_reset = true; }
    void Restart() { SetSoftReset(); esp_restart(); }
    bool GetStable() { return boot_data.stable_reached; }
    void SetStable();

  protected:
    bootreason_t m_bootreason;
    unsigned int m_crash_count_early;
  };

extern Boot MyBoot;

void OnBoot();

#endif //#ifndef __OVMS_BOOT_H__
