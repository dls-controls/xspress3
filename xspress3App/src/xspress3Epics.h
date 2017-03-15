/**
 * Author: Diamond Light Source, Copyright 2013
 *
 * License: This file is part of 'xspress3'
 * 
 * 'xspress3' is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * 'xspress3' is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with 'xspress3'.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @brief Asyn port driver for the Xspress3 system.
 *
 * @author Matthew Pearson
 * @date Sept 2012
 */

#ifndef XSPRESS3_H
#define XSPRESS3_H

#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <epicsTime.h>
#include <epicsThread.h>
#include <epicsEvent.h>
#include <epicsMutex.h>
#include <epicsString.h>
#include <epicsStdio.h>
#include <cantProceed.h>
#include <epicsTypes.h>

#include <asynOctetSyncIO.h>

#include "ADDriver.h"

#include <iostream>
using std::string;


/* These are the drvInfo strings that are used to identify the parameters.
 * They are used by asyn clients, including standard asyn device support */
//System wide settings
#define xsp3FirstParamString              "XSP3_FIRST"
#define xsp3LastParamString              "XSP3_LAST"
#define xsp3ResetParamString              "XSP3_RESET"
#define xsp3EraseParamString              "XSP3_ERASE"
#define xsp3NumChannelsParamString        "XSP3_NUM_CHANNELS"
#define xsp3MaxNumChannelsParamString     "XSP3_MAX_NUM_CHANNELS"
#define xsp3MaxSpectraParamString     "XSP3_MAX_SPECTRA"
#define xsp3MaxFramesParamString     "XSP3_MAX_FRAMES"
#define xsp3FrameCountParamString     "XSP3_FRAME_COUNT"
#define xsp3TimeFrameSourceParamString        "XSP3_TIMEFRAME_SOURCE"
#define xsp3ItfgTrigModeParamString        "XSP3_ITFG_TRIG_MODE"
#define xsp3FixedTimeParamString        "XSP3_FIXED_TIME"
#define xsp3NumFramesConfigParamString          "XSP3_NUM_FRAMES_CONFIG"
#define xsp3NumCardsParamString           "XSP3_NUM_CARDS"
#define xsp3ConfigPathParamString           "XSP3_CONFIG_PATH"
#define xsp3ConfigSavePathParamString           "XSP3_CONFIG_SAVE_PATH"
#define xsp3ConnectParamString           "XSP3_CONNECT"
#define xsp3ConnectedParamString           "XSP3_CONNECTED"
#define xsp3DisconnectParamString           "XSP3_DISCONNECT"
#define xsp3SaveSettingsParamString           "XSP3_SAVE_SETTINGS"
#define xsp3RestoreSettingsParamString           "XSP3_RESTORE_SETTINGS"
#define xsp3RunFlagsParamString           "XSP3_RUN_FLAGS"
#define xsp3TriggerParamString           "XSP3_TRIGGER"
#define xsp3InvertF0ParamString           "XSP3_INVERT_F0"
#define xsp3InvertVetoParamString           "XSP3_INVERT_VETO"
#define xsp3DebounceParamString           "XSP3_DEBOUNCE"
#define xsp3FramesPerRowString "XSP3_FRAMES_PER_ROW"
//Settings for a channel
#define xsp3ChanSca4ThresholdParamString        "XSP3_CHAN_SCA4_THRESHOLD"
#define xsp3ChanSca5HlmParamString        "XSP3_CHAN_SCA5_HLM"
#define xsp3ChanSca6HlmParamString        "XSP3_CHAN_SCA6_HLM"
#define xsp3ChanSca5LlmParamString        "XSP3_CHAN_SCA5_LLM"
#define xsp3ChanSca6LlmParamString        "XSP3_CHAN_SCA6_LLM"
#define xsp3ChanDtcFlagsParamString            "XSP3_CHAN_DTC_FLAGS"
#define xsp3ChanDtcAegParamString            "XSP3_CHAN_DTC_AEG"
#define xsp3ChanDtcAeoParamString            "XSP3_CHAN_DTC_AEO"
#define xsp3ChanDtcIwgParamString            "XSP3_CHAN_DTC_IWG"
#define xsp3ChanDtcIwoParamString            "XSP3_CHAN_DTC_IWO"
//Params to enable or disable calculations
#define xsp3RoiEnableParamString        "XSP3_CTRL_MCA_ROI"
#define xsp3DtcEnableParamString        "XSP3_CTRL_DTC"
//Params for large maps
#define pointsPerRowParamString "PointsPerRow"
#define readyForNextRowParamString "ReadyForNextRow"

const int INTERFACE_MASK = asynInt32Mask | asynInt32ArrayMask |
    asynFloat64Mask | asynFloat32ArrayMask | asynFloat64ArrayMask |
    asynDrvUserMask | asynOctetMask | asynGenericPointerMask;
const int INTERRUPT_MASK = asynInt32Mask | asynInt32ArrayMask |
    asynFloat64Mask | asynFloat32ArrayMask | asynFloat64ArrayMask |
    asynOctetMask | asynGenericPointerMask;

extern "C" {
  int xspress3Config(const char *portName, int numChannels, int numCards, const char *baseIP, int maxFrames, int maxSpectra, int maxBuffers, size_t maxMemory, int debug, int simTest);
}


class Xspress3 : public ADDriver {
    friend class Xspress3Det;
 public:
  Xspress3(const char *portName, int numChannels, int numCards, const char *baseIP, int maxFrames, int maxSpectra, int maxBuffers, size_t maxMemory, int debug, int simTest);
  Xspress3(const char *portName, int numChannels);
  virtual ~Xspress3();
  void mainLoop();

  /* These are the methods that we override from asynPortDriver */
  virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
  virtual asynStatus writeFloat64(asynUser *pasynUser, epicsFloat64 value);
  virtual asynStatus writeOctet(asynUser *pasynUser, const char *value, 
                                    size_t nChars, size_t *nActual);
  virtual void report(FILE *fp, int details);

   private:

  //Put private functions here
  void checkStatus(int status, const char *function, const char *parentFunction);
  asynStatus disconnect(void);
  asynStatus saveSettings(void);
  asynStatus restoreSettings(void);
  asynStatus checkConnected(void);
  asynStatus checkRoi(int channel, int roi, int llm, int hlm);
  asynStatus erase(void);
  asynStatus eraseSCAMCAROI(void);
  asynStatus checkSaveDir(const char *dirName);
  asynStatus readSCAParams(void);
  asynStatus readDTCParams(void);
  asynStatus setupITFG(void); 
  asynStatus mapTimeFrameSource(int mode, int invert_f0, int invert_veto, int debounce, int *apiMode, int* apiItfgMode);
  asynStatus setTriggerMode(int mode, int num_frames, double exposure_time, int invert_f0, int invert_veto, int debounce );
  asynStatus collectParamsAndSetTriggerMode();
  void createInitialParameters();
  bool setInitialParameters(int maxFrames, int numCards, int maxSpectra);
  void pushEvent(const epicsUInt8& message);
  void adReportError(const char* message);
  bool createMCAArray(size_t dims[2], NDArray *&pMCA, NDDataType_t dataType);
  bool readFrame(double* pSCA, double* pMCAData, int frameNumber, int maxSpectra);
  bool readFrame(u_int32_t* pSCA, u_int32_t* pMCAData, int frameNumber, int maxSpectra);
  asynStatus setWindow(int channel, int sca, int llm, int hlm);
  asynStatus connect(void);
  template <typename SCAtype>
  void writeOutScas(SCAtype pSCA, int numChannels);
  void setStartingParameters();
  const NDDataType_t getDataType();
  void getDims(size_t (&dims)[2]);
  asynStatus checkHistBusy(int checkTimes);
  void setNDArrayAttributes(NDArray *&pMCA, int frameNumber);
  void setAcqStopParameters(bool aborted);
  int getNumFramesToAcquire();
  void doNDCallbacksIfRequired(NDArray *pMCA);
  int getNumFramesRead();
  void grabFrame(int frameNumber);
  int acquireNFrames(int numToAcquire);
  void startAcquisition();
  bool checkQueue(const epicsUInt8 request, bool block);
  void addScalerAttributes(NDArray *&pMCA);

  //Put private static data members here
  static const epicsInt32 ctrlDisable_;
  static const epicsInt32 ctrlEnable_;
  static const epicsInt32 runFlag_MCA_SPECTRA_;
  static const epicsInt32 runFlag_PLAYB_MCA_SPECTRA_;
  static const epicsInt32 maxNumRoi_;
  static const epicsInt32 maxStringSize_;
  static const epicsInt32 maxCheckHistPolls_;
  static const epicsInt32 ADAcquireFalse_;
  static const epicsInt32 ADAcquireTrue_;
  static const epicsUInt8 startEvent;
  static const epicsUInt8 stopEvent;
  
  //Put private dynamic here
  int xsp3_handle_;
  int maxSpectra;
  int dtcEnabled;

  //Constructor parameters.
  const epicsUInt32 debug_; //debug parameter for API
  const epicsInt32 numChannels_; //The number of channels
  const epicsUInt32 simTest_; //Run in sim mode
  const std::string baseIP_; //Constructor param - IP address of host system

  u_int32_t *pSCAui;
  double *pSCAd;
  int queueSize;
  epicsMessageQueue *eventQueue;

  //Values used for pasynUser->reason, and indexes into the parameter library.
  int xsp3FirstParam;
  #define XSP3_FIRST_DRIVER_COMMAND xsp3FirstParam
  int xsp3ResetParam;
  int xsp3EraseParam;
  int xsp3NumChannelsParam;
  int xsp3MaxNumChannelsParam;
  int xsp3MaxSpectraParam;
  int xsp3MaxFramesParam;
  int xsp3FrameCountParam;
  int xsp3TimeFrameSourceParam;
  int xsp3FixedTimeParam;
  int xsp3NumFramesConfigParam;
  int xsp3NumCardsParam;
  int xsp3ConfigPathParam;
  int xsp3ConfigSavePathParam;
  int xsp3ConnectParam;
  int xsp3ConnectedParam;
  int xsp3DisconnectParam;
  int xsp3SaveSettingsParam;
  int xsp3RestoreSettingsParam;
  int xsp3RunFlagsParam;             
  int xsp3TriggerParam;             
  int xsp3InvertF0Param;             
  int xsp3InvertVetoParam;             
  int xsp3DebounceParam;
  int xsp3ChanSca4ThresholdParam;          
  int xsp3ChanSca5HlmParam;          
  int xsp3ChanSca6HlmParam;          
  int xsp3ChanSca5LlmParam;          
  int xsp3ChanSca6LlmParam;  
  int xsp3ChanDtcFlagsParam;  
  int xsp3ChanDtcAegParam;  
  int xsp3ChanDtcAeoParam;  
  int xsp3ChanDtcIwgParam;  
  int xsp3ChanDtcIwoParam;  
  int xsp3RoiEnableParam;
  int xsp3DtcEnableParam;
  int pointsPerRowParam;
  int readyForNextRowParam;
  int xsp3ItfgTrigModeParam;
  int xsp3LastParam;
  #define XSP3_LAST_DRIVER_COMMAND xsp3LastParam

};

#define NUM_DRIVER_PARAMS (&XSP3_LAST_DRIVER_COMMAND - &XSP3_FIRST_DRIVER_COMMAND + 1)

int xspress3Config(const char *portName, int numChannels, int numCards,
                   const char *baseIP, int maxFrames, int maxSpectra,
                   int maxBuffers, size_t maxMemory, int debug, int simTest);

#endif //ISC110BL_H
