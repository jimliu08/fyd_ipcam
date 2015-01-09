/*
 * $Header: /rd_2/project/Mozart/Components/VIC/Device_Driver/VIC_3.0.0.0/global_codec.h 209   13/12/24 5:35p Joe.tu $
 *
 * Driver for EDMC.
 *
 * Copyright 2008 VN Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Description:
 *
 * $History: global_codec.h $
 * 
 * *****************  Version 209  *****************
 * User: Joe.tu       Date: 13/12/24   Time: 5:35p
 * Updated in $/rd_2/project/Mozart/Components/VIC/Device_Driver/VIC_3.0.0.0
 * 
 * *****************  Version 208  *****************
 * User: Jaja         Date: 13/12/24   Time: 5:19p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/H264Enc/H264Enc_Lib
 * 
 * *****************  Version 207  *****************
 * User: Jaja         Date: 13/12/24   Time: 5:16p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/H264Enc/H264Enc_Lib
 * 
 * *****************  Version 206  *****************
 * User: Jaja         Date: 13/12/24   Time: 5:08p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/H264Enc/H264Enc_Lib
 * 
 * *****************  Version 205  *****************
 * User: Joe.tu       Date: 13/08/19   Time: 11:23a
 * Updated in $/rd_2/project/Mozart/Components/VIC/Device_Driver/VIC_3.0.0.0
 * 
 * *****************  Version 204  *****************
 * User: Jaja         Date: 13/08/16   Time: 10:22a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/H264Enc/H264Enc_Lib
 * 
 * *****************  Version 203  *****************
 * User: Jaja         Date: 13/08/07   Time: 7:18p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/SVCEnc/SVCEnc_Lib
 * 
 * *****************  Version 202  *****************
 * User: Evelyn       Date: 13/06/06   Time: 4:25p
 * Updated in $/rd_2/Peripherals/NVP1114A/Device_Driver/NVP1114A
 * 1. BUGFIX: removed some redundant settings which control those
 * registers dedicated for Audio. - FIXED.
 * 
 * *****************  Version 200  *****************
 * User: Jaja         Date: 13/01/18   Time: 6:17p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/H264Enc/H264Enc_Lib
 *
 * *****************  Version 199  *****************
 * User: Jaja         Date: 13/01/07   Time: 4:41p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/H264Enc/H264Enc_Lib
 *
 * *****************  Version 198  *****************
 * User: Linus.huang  Date: 12/12/19   Time: 7:24p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/ImgBackEndProcess_3.0.0.0/ImgBackEndProcess_Lib
 * MODIFICATION: Add VIDEO_CLEAN_INTERNAL_BUFFER option to
 * EVideoOptionFlags - DONE.
 *
 * *****************  Version 197  *****************
 * User: Angel        Date: 12/12/05   Time: 1:53p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/H264Enc/H264Enc_Lib
 *
 * *****************  Version 196  *****************
 * User: Angel        Date: 12/12/04   Time: 9:30a
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/H264Enc/H264Enc_Lib
 *
 * *****************  Version 195  *****************
 * User: Joe.tu       Date: 12/11/02   Time: 3:10p
 * Updated in $/rd_2/project/Mozart/Components/VIC/Device_Driver/VIC_3.0.0.0
 *
 * *****************  Version 194  *****************
 * User: Yiming.liu   Date: 12/10/04   Time: 3:55p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * MODIFICATION: Add VIDEO_SIGNAL_OPTION_SET_CACHE_COHERENCE option to
 * EVideoSignalOptionFlags - DONE.
 *
 * *****************  Version 193  *****************
 * User: Jaja         Date: 12/10/03   Time: 5:56p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/H264Enc/H264Enc_Lib
 *
 * *****************  Version 192  *****************
 * User: Joe.tu       Date: 12/09/24   Time: 1:37p
 * Updated in $/rd_2/project/Mozart/Components/VIC/Device_Driver/VIC_3.0.0.0
 *
 * *****************  Version 191  *****************
 * User: Alan         Date: 12/08/21   Time: 9:52a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/DeInterlacer/DeInterlacer_Lib
 * MODIFICATION: Add window info structure - DONE.
 *
 * *****************  Version 190  *****************
 * User: Joe.tu       Date: 12/08/20   Time: 6:13p
 * Updated in $/rd_2/project/Mozart/Components/VIC/Device_Driver/VIC_3.0.0.0
 *
 * *****************  Version 189  *****************
 * User: Jaja         Date: 12/08/08   Time: 9:43a
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/H264Enc/H264Enc_Lib
 *
 * *****************  Version 188  *****************
 * User: Jaja         Date: 12/08/07   Time: 1:58p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/H264Enc/H264Enc_Lib
 *
 * *****************  Version 187  *****************
 * User: Joe.tu       Date: 12/06/28   Time: 1:48p
 * Updated in $/rd_2/project/Mozart/Components/VIC/Device_Driver/VIC_3.0.0.0
 *
 * *****************  Version 186  *****************
 * User: Joe.tu       Date: 12/04/02   Time: 10:38a
 * Updated in $/rd_2/project/Mozart/Components/VIC/Device_Driver/VIC
 *
 * *****************  Version 185  *****************
 * User: Aphon.wu     Date: 12/03/26   Time: 10:19a
 * Updated in $/rd_2/project/SoC/Simulation_Models/H264Dec_C_Simulation/H264Dec_Lib
 * MODIFICATION: Rename VIDEO_SET_DEC_FRA_NUM as VIDEO_SET_DECODE_FRAME_NUM of EVideoOptionFlags emulator
 * *****************  Version 184  *****************
 * User: Aphon.wu     Date: 12/03/26   Time: 10:11a
 * Updated in $/rd_2/project/SoC/Simulation_Models/H264Dec_C_Simulation/H264Dec_Lib
 *
 * *****************  Version 182  *****************
 * User: Aphon.wu     Date: 12/03/02   Time: 3:17p
 * Updated in $/rd_2/project/SoC/Simulation_Models/H264Dec_C_Simulation/H264Dec_Lib
 *
 * *****************  Version 181  *****************
 * User: Jon.lin      Date: 11/12/09   Time: 17:44
 * Updated in $/rd_2/project/SoC/Components/EDMC/Device_Driver/EDMC
 * add GPL header
 *
 * *****************  Version 180  *****************
 * User: Joe.tu       Date: 11/11/03   Time: 9:06a
 * Updated in $/rd_2/project/Mozart/Components/VIC/Device_Driver/VIC
 *
 * *****************  Version 179  *****************
 * User: Ian.chen     Date: 11/09/29   Time: 5:14p
 * Updated in $/rd_2/project/SoC/Components/CDE/Simulation_Model/VMA_CDE_Lib
 *
 * *****************  Version 177  *****************
 * User: Ronald       Date: 11/09/28   Time: 11:44a
 * Updated in $/rd_2/project/SoC/Simulation_Models/H264Dec_C_Simulation/H264Dec_Lib
 *
 * *****************  Version 176  *****************
 * User: Joe.tu       Date: 11/09/21   Time: 7:44p
 * Updated in $/rd_2/project/Mozart/Components/VIC/Device_Driver/VIC
 *
 * *****************  Version 175  *****************
 * User: Evelyn       Date: 11/09/14   Time: 5:04p
 * Updated in $/rd_2/project/Mozart/Components/VOC/Device_Driver/VOC
 *
 * *****************  Version 174  *****************
 * User: Joe.tu       Date: 11/08/29   Time: 6:46p
 * Updated in $/rd_2/project/Mozart/Components/VIC/Device_Driver/VIC
 *
 * *****************  Version 173  *****************
 * User: Jaja         Date: 11/08/24   Time: 3:01p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/H264Enc/H264Enc_Lib
 *
 * *****************  Version 172  *****************
 * User: Joe.tu       Date: 11/07/15   Time: 6:12p
 * Updated in $/rd_2/Peripherals/NAGANO/Device_Driver/NAGANO
 *
 * *****************  Version 171  *****************
 * User: Jaja         Date: 11/06/24   Time: 6:35p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/SVCEnc/SVCEnc_Lib
 *
 * *****************  Version 170  *****************
 * User: Bernard      Date: 11/06/24   Time: 9:59a
 * Updated in $/rd_2/project/SoC/Simulation_Models/H264Dec_C_Simulation/H264Dec_Lib
 *
 * *****************  Version 169  *****************
 * User: Jaja         Date: 11/06/20   Time: 4:24p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/SVCEnc/SVCEnc_Lib
 *
 * *****************  Version 168  *****************
 * User: Jaja         Date: 11/06/20   Time: 11:39a
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/SVCEnc/SVCEnc_Lib
 *
 * *****************  Version 167  *****************
 * User: Joe.tu       Date: 11/01/26   Time: 9:54a
 * Updated in $/rd_2/project/Mozart/Components/VIC/Device_Driver/VIC
 *
 * *****************  Version 166  *****************
 * User: Joe.tu       Date: 11/01/21   Time: 11:00a
 * Updated in $/rd_2/project/Mozart/Components/VIC/Device_Driver/VIC
 *
 * *****************  Version 165  *****************
 * User: Joe.tu       Date: 11/01/13   Time: 4:47p
 * Updated in $/rd_2/Peripherals/AD5602/Device_Driver/AD5602
 *
 * *****************  Version 164  *****************
 * User: Albert.shen  Date: 11/01/12   Time: 4:45p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/VideoDisp/VideoDisp_Lib
 * 1. FEATURE: Support VIDEO_SIGNAL_FREQUENCY_30HZ  in EVideoSignalFrequency
 *
 * *****************  Version 163  *****************
 * User: Evelyn       Date: 11/01/12   Time: 3:20p
 * Updated in $/rd_2/project/SoC/Components/VIC/Device_Driver/VIC
 *
 * *****************  Version 162  *****************
 * User: Evelyn       Date: 11/01/12   Time: 3:11p
 * Updated in $/rd_2/project/SoC/Components/VIC/Device_Driver/VIC
 * Release at 2011.01.12
 * 1.FEATURE: Support AutoExposure mode setting. - DONE.
 *
 * *****************  Version 161  *****************
 * User: Bernard      Date: 11/01/05   Time: 6:04p
 * Updated in $/rd_2/misc/GBENGINE/C/GBEngine/GBEngine_Lib
 *
 * *****************  Version 160  *****************
 * User: Evelyn       Date: 10/12/15   Time: 12:02p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/VideoCap/VideoCap_Lib
 *
 * *****************  Version 159  *****************
 * User: Bernard      Date: 10/12/02   Time: 2:01p
 * Updated in $/rd_2/video/H264/C/Decoder/H264Dec/GBEngine_Lib
 * FEATURE: Add VIDEO_CHANGE_BITSTREAM_IO_VECTOR_BUFFERS - Done.
 *
 * *****************  Version 158  *****************
 * User: Bernard      Date: 10/12/02   Time: 1:33p
 * Updated in $/rd_2/misc/GBENGINE/C/GBEngine/GBEngine_Lib
 * FEATURE: Add EIOVectorMode - Done.
 *
 * *****************  Version 157  *****************
 * User: Bernard      Date: 10/11/05   Time: 3:13p
 * Updated in $/rd_2/video/H264/C/Decoder/H264Dec/H264Dec_Lib
 * FEATURE: Support IO Vector mode in EBufTypeFlags. - Done.
 *
 * *****************  Version 156  *****************
 * User: Evelyn       Date: 10/08/13   Time: 12:21p
 * Updated in $/rd_2/project/SoC/Components/VIC/Device_Driver/VIC
 * Release at 2010.08.13
 * 1. FEATURE: Support auto-detect and auto-set NTSC/PAL - DONE
 * 2. MODIFICATION: Remove "devNum" from TAutoExposureState and add a
 * member "dwNum" to TVPLVICDevInfo - DONE
 * 3. MODIFICATION: Init tasklet in Initial_Module instead of Open - DONE
 * 4. MODIFICATION: Check the enable bit of VIC before
 * VIC_AutoExposureOneframeTasklet - DONE
 *
 * *****************  Version 155  *****************
 * User: Albert.shen  Date: 10/08/03   Time: 6:32p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/VideoDisp/VideoDisp_Lib
 *
 * *****************  Version 154  *****************
 * User: Evelyn       Date: 10/08/02   Time: 8:33p
 * Updated in $/rd_2/Peripherals/TW2866/Device Driver/TW2866
 * [Version 2.0.0.0] Release at 2010.08.02
 * 1. MODIFICATION: Add a 60 ms delay in reset function - DONE
 * 2. FEATURE: support TW2866 audio/video drivers synchronization - DONE.
 * 3. FEATURE: support auto-detect standard in open function - DONE.
 * 4. MODIFICATION: Add semaphore in open/release/ioctl functions - DONE.
 *
 * *****************  Version 153  *****************
 * User: Evelyn       Date: 10/08/02   Time: 4:32p
 * Updated in $/rd_2/Peripherals/MN34030PL/Device Driver/MN34030PL
 * [Version 1.0.0.5] Release at 2010.08.02
 * 1. MODIFICATION: Remove video_sensor_width and video_sensor_height
 * setting by VIC to synchronize with vpl_vic.ko v_7.0.0.2 - DONE.
 * 2. MODIFICATION: Add semaphore for ioctl function. - DONE
 *
 * *****************  Version 152  *****************
 * User: Albert.shen  Date: 10/08/02   Time: 10:17a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/VideoDisp/VideoDisp_Lib
 *
 * *****************  Version 151  *****************
 * User: Evelyn       Date: 10/07/23   Time: 11:10p
 * Updated in $/rd_2/project/SoC/Components/VIC/Device_Driver/VIC
 * Release at 2010.07.23
 * 1. FEATURE: Add DC-Iris control - DONE
 *
 * *****************  Version 150  *****************
 * User: Yc.lu        Date: 10/07/07   Time: 10:08a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/AutoExposure/Device_Driver/AutoExposure
 *
 * *****************  Version 149  *****************
 * User: Yc.lu        Date: 10/07/07   Time: 10:04a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/AutoExposure/Device_Driver/AutoExposure
 *
 * *****************  Version 148  *****************
 * User: Yc.lu        Date: 10/07/07   Time: 9:52a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/AutoExposure/Device_Driver/AutoExposure
 * Add bAECoopMode @ auto_exposure_initial_param & dwCurrDCIrisSpeed @
 * TAutoExposureState.
 *
 * *****************  Version 147  *****************
 * User: Yc.lu        Date: 10/07/06   Time: 7:16p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/AutoExposure/Device_Driver/AutoExposure
 * Add sdwCurrDCIrisMove & bDCIrisValid @ TAutoExposureState
 *
 * *****************  Version 146  *****************
 * User: Evelyn       Date: 10/06/21   Time: 6:00p
 * Updated in $/rd_2/Peripherals/OV2710/Device Driver/OV2710
 * 1. MODIFICATION: turn off AE in sensor's DSP function. - DONE
 *
 * *****************  Version 145  *****************
 * User: Evelyn       Date: 10/06/03   Time: 9:00p
 * Updated in $/rd_2/project/SoC/Components/VIC/Device_Driver/VIC
 * Release at 2010.06.03
 * 1. FEATURE: Get the remaining lines of sensor to wordaround the AE
 * delay issue - DONE
 *
 * *****************  Version 144  *****************
 * User: Evelyn       Date: 10/06/01   Time: 6:05p
 * Updated in $/rd_2/project/SoC/Components/VIC/Device_Driver/VIC
 * 1. MODIFICATION: Add device number information into the interface
 * between VIC and sensor device - DONE
 * 2. MODIFICATION: Fix compiling-warnings - DONE
 *
 * *****************  Version 143  *****************
 * User: Rikchang     Date: 10/05/28   Time: 11:50a
 * Updated in $/rd_2/project/SoC/Simulation_Models/ObjDtct_C_Simulation/ObjDtct_Lib
 *
 * *****************  Version 142  *****************
 * User: Jaja         Date: 10/05/21   Time: 9:53a
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/H264Enc/H264Enc_Lib
 *
 * *****************  Version 139  *****************
 * User: Evelyn       Date: 10/04/30   Time: 4:46p
 * Updated in $/rd_common/common_header
 *
 * *****************  Version 138  *****************
 * User: Albert.shen  Date: 10/04/28   Time: 9:13p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/VideoDisp/VideoDisp_Lib
 *
 * *****************  Version 137  *****************
 * User: Rikchang     Date: 10/04/20   Time: 10:22a
 * Updated in $/rd_2/project/SoC/Simulation_Models/ObjDtct_C_Simulation/ObjDtct_Lib
 * MODIFICATION: Add VIDEO_OD_SET_ROI_MASK video option - DONE.
 *
 * *****************  Version 136  *****************
 * User: Jaja         Date: 10/04/09   Time: 11:02a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/H264Enc/H264Enc_Lib
 *
 * *****************  Version 135  *****************
 * User: Ycchang      Date: 10/04/08   Time: 11:18p
 * Updated in $/rd_2/project/SoC/Components/VIC/Device_Driver/VIC
 *
 * *****************  Version 134  *****************
 * User: Jaja         Date: 10/04/07   Time: 1:37p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/H264Enc/H264Enc_Lib
 *
 * *****************  Version 133  *****************
 * User: Jaja         Date: 10/04/02   Time: 4:30p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/H264Enc/H264Enc_Lib
 *
 * *****************  Version 132  *****************
 * User: Jasonyang    Date: 10/04/02   Time: 1:54p
 * Updated in $/rd_2/project/SoC/Simulation_Models/Sample_C_Simulation/PatGen_Lib
 *
 * *****************  Version 132  *****************
 * User: Jasonyang    Date: 10/04/02   Time: 1:54p
 * Updated in $/rd_2/project/SoC/Simulation_Models/Sample_C_Simulation/PatGen_Lib
 *
 * *****************  Version 131  *****************
 * User: Rikchang     Date: 10/03/15   Time: 12:31p
 * Updated in $/rd_common/common_header
 * MODIFICATION: Add VIDEO_FD_SET_DB and VIDEO_LPD_SET_DB video options -
 * DONE.
 *
 * *****************  Version 130  *****************
 * User: Albert.shen  Date: 10/01/20   Time: 2:44p
 * Updated in $/rd_2/project/SoC/Components/VOC/Device_Driver/VOC
 *
 * UPDATE : Add ARGB32 and ARGB16 format in EPixelFormatFlags - DONE.
 *
 * *****************  Version 129  *****************
 * User: Kensuke.chen Date: 10-01-19   Time: 18:48
 * Updated in $/rd_2/project/SoC/Components/IRE/Device_Driver/IRE
 *
 * *****************  Version 127  *****************
 * User: Albert.shen  Date: 09/11/05   Time: 4:13p
 * Updated in $/rd_2/project/SoC/Components/VOC/Simulation_Model/VPL_VOC_Lib
 *
 * UPDATE : Add VIDEO_SIGNAL_FORMAT_RGB24, VIDEO_SIGNAL_FORMAT_INTERLACE_RAW,
 * and VIDEO_SIGNAL_FORMAT_INTERLACE_RAW_16BITS three formats to
 * EVideoSignalFormat. - DONE
 *
 * *****************  Version 126  *****************
 * User: Aniki        Date: 09/11/04   Time: 10:33p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/VideoCap/VideoCap_Lib
 *
 * *****************  Version 125  *****************
 * User: Alan         Date: 09/10/08   Time: 3:41p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/VideoCap/VideoCap_Lib
 * UPDATE: Add VIDEO_SIGNAL_OPTION_SET_AUTO_COLOR_SUPPRESSION option to
 * EVideoSignalOptionFlags - DONE.
 *
 * *****************  Version 124  *****************
 * User: Aniki        Date: 09/10/05   Time: 10:13p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/VideoCap/VideoCap_Lib
 *
 * *****************  Version 123  *****************
 * User: Rikchang     Date: 09/09/29   Time: 4:37p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/VideoCap/VideoCap_Lib
 * MODIFICATION: Add VIDEO_SIGNAL_OPTION_SET_TONE_MAPPING and
 * VIDEO_SIGNAL_OPTION_SET_CONTRAST_ENHANCEMENT options.
 *
 * *****************  Version 122  *****************
 * User: Alan         Date: 09/07/09   Time: 2:38p
 * Updated in $/rd_common/common_header
 *
 * *****************  Version 121  *****************
 * User: Alan         Date: 09/06/19   Time: 4:20p
 * Updated in $/rd_common/common_header
 * MODIFICATION: Add FOURCC_H264 and FOURCC_MDAT - DONE.
 *
 * *****************  Version 120  *****************
 * User: Albert.shen  Date: 09/06/09   Time: 9:23a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/VideoCap/VideoCap_Lib
 *
 * *****************  Version 119  *****************
 * User: Ycchang      Date: 09/05/05   Time: 4:11p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/VideoCap/VideoCap_Lib
 *
 * *****************  Version 118  *****************
 * User: Albert.shen  Date: 09/02/23   Time: 11:48a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/VideoCap/VideoCap_Lib
 *
 * *****************  Version 117  *****************
 * User: Albert.shen  Date: 08/12/19   Time: 5:41p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/VideoCap/VideoCap_Lib
 *
 * *****************  Version 116  *****************
 * User: Albert.shen  Date: 08/12/19   Time: 4:34p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/VideoCap/VideoCap_Lib
 *
 * *****************  Version 115  *****************
 * User: Albert.shen  Date: 08/12/19   Time: 3:46p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/VideoCap/VideoCap_Lib
 *
 * *****************  Version 114  *****************
 * User: Albert.shen  Date: 08/11/19   Time: 2:24p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/VideoDisp/VideoDisp_Lib
 * MODIFICATION: Add video signal sizes VIDEO_SIGNAL_SIZE_480x272 in
 * EVideoSignalSize - DONE.
 *
 * *****************  Version 113  *****************
 * User: Albert.shen  Date: 08/11/19   Time: 11:34a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/VideoDisp/VideoDisp_Lib
 * MODIFICATION: Add video signal sizes from VIDEO_SIGNAL_SIZE_768x576 to
 * VIDEO_SIGNAL_SIZE_2560x2048 in EVideoSignalSize - DONE.
 *
 * *****************  Version 112  *****************
 * User: Albert.shen  Date: 08/11/14   Time: 4:59p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/VideoDisp/VideoDisp_Lib
 * MODIFICATION: Add VIDEO_DISP_OPTION_SET_OUTPUT_CTRL option in
 * EVideoDispOptionFlags - DONE.
 *
 * *****************  Version 111  *****************
 * User: Alan         Date: 08/11/10   Time: 1:01a
 * Updated in $/rd_2/image/DeInterlacer/C/DeInterlacer/DeInterlacer_Lib
 * MODIFICATION: Add VIDEO_DELETE_ALL_WINDOWS option in EVideoOptionFlags
 * - DONE.
 *
 * *****************  Version 110  *****************
 * User: Ycchang      Date: 08/09/30   Time: 4:57p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * Add VIDEO_SIGNAL_FORMAT_PROGRESSIVE_RAW_16BITS in EVideoSignalFormat.
 *
 * *****************  Version 109  *****************
 * User: Alan         Date: 08/04/12   Time: 12:25a
 * Updated in $/rd_2/misc/PBENGINE/C/PBEngine/PBEngine_Lib
 * When defined __TCS__, the platform is TriMedia and the callback
 * functions only support 32-bit.
 * Add __x86_64__, __IA64__, _M_IA64, _M_X64 and _WIN64 macro to detect
 * 64-bit platform and modify ALIGN_ADDRESS() macro to fit the
 * architecture.
 *
 * *****************  Version 108  *****************
 * User: Iker         Date: 08/03/21   Time: 4:14p
 * Updated in $/rd_common/common_header
 *
 * *****************  Version 107  *****************
 * User: Alan         Date: 08/02/04   Time: 4:00p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/VideoCap/VideoCap_Lib
 *
 * *****************  Version 106  *****************
 * User: Alan         Date: 08/01/23   Time: 3:26p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/VideoDisp/VideoDisp_Lib
 *
 * *****************  Version 105  *****************
 * User: Alan         Date: 08/01/23   Time: 1:21p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/VideoDisp/VideoDisp_Lib
 * MODIFICATION: Add EVideoDispOptionFlags for video display flags - DONE.
 *
 * *****************  Version 104  *****************
 * User: Alan         Date: 08/01/21   Time: 10:30p
 * Updated in $/rd_2/Peripherals/TW2815/TW2815_Lib
 *
 * *****************  Version 103  *****************
 * User: Rey          Date: 07/11/15   Time: 3:37p
 * Updated in $/rd_common/common_header
 * MODIFICATION: Add FOURCC_729A and FOURCC_729B
 *
 * *****************  Version 102  *****************
 * User: Albert.shen  Date: 07/10/08   Time: 2:21p
 * Updated in $/rd_2/Peripherals/VICAM3/VICAM3_Lib
 *
 * MODIFICATION: Change VIDEO_SIGNAL_OPTION_SET_IRIS_LEVEL
 * to VIDEO_SIGNAL_OPTION_SET_EXPOSURE_LEVEL
 *
 * *****************  Version 101  *****************
 * User: Albert.shen  Date: 07/07/20   Time: 12:03p
 * Updated in $/rd_2/Peripherals/MT9M111/MT9M111_Lib
 *
 * *****************  Version 100  *****************
 * User: Albert.shen  Date: 07/06/21   Time: 3:47p
 * Updated in $/rd_2/Peripherals/VICAM3/VICAM3_Lib
 *
 * *****************  Version 99  *****************
 * User: Vincent      Date: 07/06/07   Time: 5:13p
 * Updated in $/rd_common/common_header
 *
 * *****************  Version 98  *****************
 * User: Vincent      Date: 07/06/07   Time: 5:08p
 * Updated in $/rd_common/common_header
 * MODIFICATION: Add FOURCC for ALAW and ULAW
 *
 * *****************  Version 97  *****************
 * User: Albert.shen  Date: 07/06/05   Time: 12:03p
 * Updated in $/rd_2/Peripherals/VICAM3/VICAM3_Lib
 *
 * *****************  Version 96  *****************
 * User: Albert.shen  Date: 07/06/04   Time: 10:01a
 * Updated in $/rd_2/Peripherals/VICAM3/VICAM3_Lib
 * MODIFICATION: Add VIDEO_SIGNAL_OPTION_SET_BACKLIGHT_COMPENSATION and
 * VIDEO_SIGNAL_OPTION_SET_AUTO_IRIS_CTRL options to EVideoSignalOptionFlags
 * - DONE.
 *
 * *****************  Version 95  *****************
 * User: Albert.shen  Date: 07/05/30   Time: 6:07p
 * Updated in $/rd_2/Peripherals/MT9V111/MT9V111_Lib
 * MODIFICATION: Add VIDEO_SIGNAL_OPTION_SET_CONFIG option to
 * EVideoSignalOptionFlags - DONE.
 *
 * *****************  Version 94  *****************
 * User: Ronald       Date: 07/04/03   Time: 6:15p
 * Updated in $/rd_2/Peripherals/MT9V111/MT9V111_Lib
 *
 * *****************  Version 93  *****************
 * User: Alan         Date: 07/03/30   Time: 11:47p
 * Updated in $/rd_2/project/Vivaldi/Linux_Libraries/VideoCap/VideoCap_Lib
 * MODIFICATION: Add VIDEO_SIGNAL_OPTION_RESET_FRAME_RATE_CTRL to
 * EVideoSignalOptionFlags - DONE.
 *
 * *****************  Version 92  *****************
 * User: Alan         Date: 07/03/23   Time: 4:18p
 * Updated in $/rd_2/project/Vivaldi/Linux_Libraries/VideoCap/VideoCap_Lib
 * MODIFICATION: Add VIDEO_SIGNAL_OPTION_SET_FIELD_INVERSE option to
 * EVideoSignalOptionFlags - DONE.
 *
 * *****************  Version 91  *****************
 * User: Ycchang      Date: 07/03/21   Time: 3:08p
 * Updated in $/rd_2/Peripherals/MT9M111/MT9M111_Lib
 * MODIFICATION: Add VIDEO_SIGNAL_OPTION_SET_EXPOSURE_TIME option to
 * EVideoSignalOptionFlags - DONE.
 *
 * *****************  Version 90  *****************
 * User: Sor          Date: 06/12/01   Time: 5:41p
 * Updated in $/rd_2/video/MPEG4/C/Encoder/MP4VENC/Mp4VEnc_Lib
 *
 * *****************  Version 89  *****************
 * User: Ycchang      Date: 06/11/07   Time: 2:43p
 * Updated in $/rd_2/Peripherals/MT9V111/MT9V111_Lib
 * MODIFICATION: Add VIDEO_SIGNAL_OPTION_SET_AUTO_TRACK_WHITE option to
 * EVideoSignalOptionFlags - DONE.
 *
 * *****************  Version 88  *****************
 * User: Ycchang      Date: 06/11/02   Time: 3:43p
 * Updated in $/rd_2/Peripherals/PV480/PV480_Lib
 * MODIFICATION: Add VIDEO_SIGNAL_OPTION_SET_ZOOM and
 * VIDEO_SIGNAL_OPTION_SET_PRIVACY_MASK option to
 *  * EVideoSignalOptionFlags - DONE.
 *
 * *****************  Version 87  *****************
 * User: Sor          Date: 06/10/12   Time: 10:16a
 * Updated in $/rd_2/image/ImgProcess/MMX/ImgProcess/ImgProcess_Lib
 *
 * *****************  Version 86  *****************
 * User: Sor          Date: 06/10/11   Time: 6:25p
 * Updated in $/rd_2/image/JPEG/C/Decoder/JPEGDec/GBEngine_Lib
 *
 * *****************  Version 85  *****************
 * User: Sor          Date: 06/10/11   Time: 5:49p
 * Updated in $/rd_2/image/JPEG/C/Decoder/JPEGDec/GBEngine_Lib
 * FEATURE: Use PVOID data type for all cross platform variables - DONE.
 *
 * *****************  Version 84  *****************
 * User: Sor          Date: 06/10/05   Time: 9:11p
 * Updated in $/rd_2/image/JPEG/C/Decoder/JPEGDec/GBEngine_Lib
 * MODIFICATION: Change FOnEnterCriticalSec, FOnLeaveCriticalSec, and
 * FOnSendData function type definition - DONE.
 *
 * *****************  Version 83  *****************
 * User: Sor          Date: 06/08/15   Time: 7:53p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Block2Frame/Block2Frame_Lib
 *
 * *****************  Version 82  *****************
 * User: Sor          Date: 06/08/15   Time: 7:36p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Mp4VDec/Mp4VDec_Lib
 *
 * *****************  Version 81  *****************
 * User: Ycchang      Date: 06/08/09   Time: 4:29p
 * Updated in $/rd_2/Peripherals/SAA7113H/SAA7113H_Lib
 * UPDATE: Add VIDEO_SIGNAL_OPTION_SET_OUTPUT_TYPE option to
 * EVideoSignalOptionFlags - DONE.
 *
 *
 * *****************  Version 80  *****************
 * User: Vincent      Date: 06/07/26   Time: 5:42p
 * Updated in $/rd_common/common_header
 * MODIFICATION: Add FOURCC for JPEG and HINT
 *
 * *****************  Version 79  *****************
 * User: Sor          Date: 06/07/05   Time: 10:45a
 * Updated in $/rd_2/project/Vivaldi/Linux_Libraries/Mp4VEnc/Mp4VEnc_Lib
 * MODIFICATION: Add ERateCtrlFlags enumerator - DONE.
 *
 * *****************  Version 78  *****************
 * User: Bernard      Date: 06/05/15   Time: 7:46p
 * Updated in $/rd_2/misc/PBENGINE/C/PBEngine/PBEngine_Lib
 * MODIFICATION: Add VIDEO_CHANGE_QUANT_TABLE - DONE.
 *
 * *****************  Version 77  *****************
 * User: Sor          Date: 06/04/21   Time: 11:56a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Block2Frame/Block2Frame_Lib
 *
 * *****************  Version 76  *****************
 * User: Sean         Date: 06/03/10   Time: 12:57p
 * Updated in $/rd_2/Training/Library_Development/Sean/MtnDtct/MtnDtct_Lib
 * MODIFICATION: Add VIDEO_ADD_WINDOW and
 * VIDEO_DELETE_WINDOW options - DONE.
 *
 * *****************  Version 75  *****************
 * User: Sean         Date: 06/03/07   Time: 4:23p
 * Updated in $/rd_2/Training/Library_Development/Sean/MtnDtct/MtnDtct_Lib
 *
 * *****************  Version 74  *****************
 * User: Sor          Date: 06/03/01   Time: 1:13p
 * Updated in $/rd_2/project/Vivaldi/Libraries/Mp4VEnc/Mp4VEnc_Lib
 * UPDATE: Add VIDEO_CHANGE_IN_MODE option to EVideoOPtionFlags - DONE.
 *
 * *****************  Version 73  *****************
 * User: Sor          Date: 06/02/21   Time: 6:14p
 * Updated in $/rd_2/Training/Library_Development/Sean/MtnDtct/MtnDtct_Lib
 * MODIFICATION: Add VIDEO_CHANGE_WINDOW_POSITION and
 * VIDEO_CHANGE_WINDOW_SIZE options - DONE.
 *
 * *****************  Version 72  *****************
 * User: Sor          Date: 06/01/13   Time: 4:29p
 * Updated in $/rd_2/project/SoC/Simulation_Models/Block2Frame_C_Simulation/Block2Frame_Lib
 *
 * *****************  Version 71  *****************
 * User: Alan         Date: 05/12/13   Time: 5:46p
 * Updated in $/rd_2/project/SoC/Simulation_Models/VideoCap_C_Simulation/VideoCap_Lib
 * UPDATE: Add VIDEO_SIGNAL_FORMAT_BAYER_PATTERN.
 *
 * *****************  Version 70  *****************
 * User: Cchuang      Date: 05/11/29   Time: 11:28
 * Updated in $/rd_common/common_header
 * 1. MODIFICATION: Add FOURCC for VIVO, TEXT and CONF
 *
 * *****************  Version 69  *****************
 * User: Sor          Date: 05/10/31   Time: 10:57a
 * Updated in $/rd_2/image/ImgProcess/C/ImgProcess/ImgProcess_Lib
 * MODIFICATION: Add BGR16555, BGR16565, BGR24, and BGR32 to
 * EPixelFormatFlags enumerator - DONE.
 *
 * *****************  Version 68  *****************
 * User: Sor          Date: 05/08/20   Time: 4:41p
 * Updated in $/rd_2/project/SoC/Simulation_Models/Sample_C_Simulation/MemMgr_Lib
 *
 * *****************  Version 67  *****************
 * User: Sor          Date: 05/08/20   Time: 4:38p
 * Updated in $/rd_2/project/SoC/Simulation_Models/Sample_C_Simulation/MemMgr_Lib
 *
 * *****************  Version 66  *****************
 * User: Sor          Date: 05/08/20   Time: 4:27p
 * Updated in $/rd_2/project/SoC/Simulation_Models/Mp4VEnc_C_Simulation/Mp4VEnc_Lib
 *
 * *****************  Version 65  *****************
 * User: Sor          Date: 05/08/10   Time: 11:09a
 * Updated in $/rd_2/project/Vivaldi/Libraries/IICCtrl/IICCtrl_Lib
 * MODIFICATION: Modify FOnReadReg and FOnWriteReg definition - DONE.
 *
 * *****************  Version 64  *****************
 * User: Sor          Date: 05/04/21   Time: 4:50p
 * Updated in $/rd_2/image/ImgProcess/C/ImgProcess/ImgProcess_Lib
 *
 * *****************  Version 63  *****************
 * User: Sor          Date: 05/04/20   Time: 2:29p
 * Updated in $/rd_2/video/H263/C/Decoder/H263Dec/ImgProcess_Lib
 *
 * *****************  Version 62  *****************
 * User: Sor          Date: 05/04/20   Time: 2:29p
 * Updated in $/rd_2/video/H263/C/Decoder/H263Dec/ImgProcess_Lib
 *
 * *****************  Version 61  *****************
 * User: Sor          Date: 05/04/20   Time: 1:40p
 * Updated in $/rd_2/video/H263/C/Decoder/H263Dec/H263Dec_Lib
 *
 * *****************  Version 60  *****************
 * User: Chikuang     Date: 05/03/03   Time: 7:29p
 * Updated in $/rd_2/project/Vivaldi/Libraries/Mp4VEnc/Mp4VEnc_Lib
 *
 * *****************  Version 59  *****************
 * User: Sor          Date: 05/02/17   Time: 3:35p
 * Updated in $/rd_2/misc/PBENGINE/C/PBEngine/PBEngine_Lib
 *
 * *****************  Version 58  *****************
 * User: Sor          Date: 05/01/10   Time: 11:45a
 * Updated in $/rd_2/project/Vivaldi/Libraries/VideoCap/VideoCap_Lib
 *
 * *****************  Version 57  *****************
 * User: Sor          Date: 04/11/19   Time: 10:56a
 * Updated in $/rd_2/project/Vivaldi/Libraries/Mp4VEnc/PBEngine_Lib
 *
 * *****************  Version 56  *****************
 * User: Chikuang     Date: 04/11/18   Time: 10:28a
 * Updated in $/rd_2/project/Handel/Simulation_Models/ImgProcess_C_Simulation/ImgProcess_Lib
 *
 * *****************  Version 55  *****************
 * User: Chikuang     Date: 04/11/18   Time: 10:00a
 * Updated in $/rd_2/project/Handel/Simulation_Models/ImgProcess_C_Simulation/VMA_IPE_Lib
 *
 * *****************  Version 54  *****************
 * User: Sor          Date: 04/11/17   Time: 12:05p
 * Updated in $/rd_2/misc/PBENGINE/C/PBEngine/PBEngine_Lib
 *
 * *****************  Version 53  *****************
 * User: Sor          Date: 04/11/17   Time: 10:55a
 * Updated in $/rd_2/Peripherals/OV7620/OV7620_Lib
 *
 * *****************  Version 52  *****************
 * User: Sor          Date: 04/09/17   Time: 1:36p
 * Updated in $/rd_2/Peripherals/OV7648/OV7648_Lib
 *
 * *****************  Version 51  *****************
 * User: Sor          Date: 04/09/01   Time: 9:23p
 * Updated in $/rd_2/video/MPEG4/C/Decoder/MP4VDEC/Mp4VDec_Lib
 *
 * *****************  Version 50  *****************
 * User: Sor          Date: 04/09/01   Time: 5:12p
 * Updated in $/rd_2/image/JPEG/C/Decoder/JPEGDec/JPEGDec_Lib
 *
 * *****************  Version 48  *****************
 * User: Sor          Date: 04/09/01   Time: 4:55p
 * Updated in $/rd_2/image/JPEG/C/Decoder/JPEGDec/JPEGDec_Lib
 *
 * *****************  Version 47  *****************
 * User: Sor          Date: 04/09/01   Time: 4:53p
 * Updated in $/rd_2/image/JPEG/C/Decoder/JPEGDec/JPEGDec_Lib
 *
 * *****************  Version 46  *****************
 * User: Sor          Date: 04/09/01   Time: 3:00p
 * Updated in $/rd_2/image/JPEG/C/Decoder/JPEGDec/JPEGDec_Lib
 *
 * *****************  Version 45  *****************
 * User: Sor          Date: 04/09/01   Time: 1:17p
 * Updated in $/rd_2/video/MPEG4/C/Decoder/MP4VDEC/Mp4VDec_Lib
 *
 * *****************  Version 44  *****************
 * User: Sor          Date: 04/08/31   Time: 10:32p
 * Updated in $/rd_2/image/JPEG/C/Decoder/JPEGDec/JPEGDec_Lib
 *
 * *****************  Version 43  *****************
 * User: Sor          Date: 04/08/31   Time: 10:29p
 * Updated in $/rd_2/image/JPEG/C/Decoder/JPEGDec/JPEGDec_Lib
 *
 * *****************  Version 42  *****************
 * User: Sor          Date: 04/08/31   Time: 10:22p
 * Updated in $/rd_2/image/JPEG/C/Decoder/JPEGDec/JPEGDec_Lib
 *
 * *****************  Version 41  *****************
 * User: Sor          Date: 04/08/31   Time: 9:23p
 * Updated in $/rd_2/video/H263/C/Decoder/H263Dec/H263Dec_Lib
 *
 * *****************  Version 40  *****************
 * User: Sor          Date: 04/08/31   Time: 2:23p
 * Updated in $/rd_2/image/ImgProcess/C/ImgProcess/ImgProcess_Lib
 *
 * *****************  Version 39  *****************
 * User: Sor          Date: 04/08/31   Time: 1:40p
 * Updated in $/rd_2/image/ImgProcess/C/ImgProcess/ImgProcess_Lib
 *
 * *****************  Version 37  *****************
 * User: Sor          Date: 04/08/20   Time: 7:51p
 * Updated in $/rd_2/Peripherals/OV7648/OV7648_Lib
 * MODIFICATION: Change VIDEO_SIGNAL_OPTION_SET_COLOR_TEMPERATURE option -
 * DONE.
 *
 * *****************  Version 36  *****************
 * User: Chikuang     Date: 04/07/22   Time: 4:29p
 * Updated in $/rd_2/project/Vivaldi/Libraries/VideoCap/SAA7113H_Lib
 *
 * *****************  Version 35  *****************
 * User: Sor          Date: 04/06/24   Time: 7:46p
 * Updated in $/rd_2/project/Vivaldi/Libraries/RealTimeClk/RealTimeClk_Lib
 * UPDATE: Update version number from 1.0.0.0 to 2.0.0.0 - DONE.
 *
 * *****************  Version 34  *****************
 * User: Sor          Date: 04/06/24   Time: 4:21p
 * Updated in $/rd_2/Peripherals/OV7648/OV7648_Lib
 *
 * *****************  Version 33  *****************
 * User: Vicky        Date: 04/06/10   Time: 4:24p
 * Updated in $/rd_2/project/Vivaldi/Libraries/VideoCap/OV7648_Lib
 *
 * *****************  Version 32  *****************
 * User: Chikuang     Date: 04/06/09   Time: 11:27a
 * Updated in $/rd_2/image/JPEG/C/Encoder/JPEGEnc/JPEGEnc_Lib
 * MODIFICATION: Rename YUV to YUV420 and add YUV422 in TPixelFormatFlags
 *
 * *****************  Version 31  *****************
 * User: Sor          Date: 04/05/07   Time: 3:02p
 * Updated in $/rd_2/project/Vivaldi/Libraries/VideoCap/CPE/VideoCap_Lib
 *
 * *****************  Version 30  *****************
 * User: Sor          Date: 04/05/07   Time: 2:31p
 * Updated in $/rd_2/project/Vivaldi/Libraries/VideoCap/CPE/VideoCap_Lib
 *
 * *****************  Version 29  *****************
 * User: Sor          Date: 04/05/07   Time: 1:48p
 * Updated in $/rd_2/project/Vivaldi/Libraries/VideoCap/CPE/VideoCap_Lib
 *
 * *****************  Version 28  *****************
 * User: Sor          Date: 04/05/07   Time: 11:51a
 * Updated in $/rd_2/project/Vivaldi/Libraries/VideoCap/CPE/VideoCap_Lib
 *
 * *****************  Version 27  *****************
 * User: Sor          Date: 04/05/06   Time: 8:07p
 * Updated in $/rd_2/project/Vivaldi/Libraries/VideoCap/CPE/VideoCap_Lib
 * MODIFICATION: Add alias to some definition with old naming rule - DONE.
 *
 * *****************  Version 26  *****************
 * User: Sor          Date: 04/05/06   Time: 7:57p
 * Updated in $/rd_2/project/Vivaldi/Libraries/VideoCap/CPE/VideoCap_Lib
 *
 * *****************  Version 25  *****************
 * User: Chikuang     Date: 04/05/05   Time: 3:40p
 * Updated in $/rd_2/project/Vivaldi/Libraries/Mp4VEnc/Mp4VEnc_Lib
 *
 * *****************  Version 24  *****************
 * User: Sor          Date: 04/05/05   Time: 9:32a
 * Updated in $/rd_2/Peripherals/OV7648/OV7648_Lib
 *
 * *****************  Version 23  *****************
 * User: Sor          Date: 04/05/04   Time: 11:32p
 * Updated in $/rd_2/Peripherals/OV7648/OV7648_Lib
 *
 * *****************  Version 22  *****************
 * User: Sor          Date: 04/05/04   Time: 10:53p
 * Updated in $/rd_2/project/Vivaldi/Libraries/MemMgr/MemMgr_Lib
 * MODIFICATION: Add video signal enumerations - DONE.
 *
 * *****************  Version 21  *****************
 * User: Sor          Date: 04/04/30   Time: 5:23p
 * Updated in $/rd_2/project/Vivaldi/Libraries/VideoCap/CPE/VideoCap_Lib
 * MODIFICATION: Add request memory callback function definition - DONE.
 *
 * *****************  Version 20  *****************
 * User: Sor          Date: 04/04/30   Time: 1:42p
 * Updated in $/rd_2/project/Vivaldi/Libraries/Mp4VEnc/Mp4VEnc_Lib
 * MODIFICATION: Add version number structure - DONE.
 *
 * *****************  Version 19  *****************
 * User: Sor          Date: 03/12/22   Time: 4:49p
 * Updated in $/rd_2/project/SOC/Mp4VEnc_C_Simulation/Mp4VEnc_Lib
 *
 * *****************  Version 18  *****************
 * User: Sor          Date: 03/11/20   Time: 4:29p
 * Updated in $/rd_2/video/MPEG4/TM1300/Encoder/Mp4VEnc/Mp4VEnc_Lib
 * FEATURE: Add Mp4VEnc_Config.c to library - DONE.
 *
 * *****************  Version 17  *****************
 * User: Aren         Date: 03/07/09   Time: 5:18p
 * Updated in $/rd_2/speech/G723_1/ARM9/comm_lib
 * MODIFICATION: Remove superfluous ',' in TPixelFormatFlags - DONE.
 *
 * *****************  Version 16  *****************
 * User: Sor          Date: 03/07/02   Time: 9:32a
 * Updated in $/rd_2/video/H263/C/Decoder/H263Dec/H263Dec_Lib
 * FEATURE: Add RGB16555, RGB16565, and FLIP_RGB24 to TPixelFormatFlags -
 * DONE.
 *
 * *****************  Version 15  *****************
 * User: Sor          Date: 03/05/30   Time: 4:24p
 * Updated in $/rd_2/project/SOC/Mp4VEnc_C_Simulation/Mp4VEnc_Lib
 * FEATURE: Add TColorFlags - DONE.
 *
 * *****************  Version 14  *****************
 * User: Sor          Date: 03/05/06   Time: 4:12p
 * Updated in $/rd_2/video/MPEG4/C/Encoder/MP4VENC/Mp4VEnc_Lib
 * MODIFICATION: Add VIDEO_CHANGE_FRAME_SIZE to TVideoOptionFlags
 * structure - DONE.
 *
 * *****************  Version 13  *****************
 * User: Sor          Date: 03/05/06   Time: 1:57p
 * Updated in $/rd_2/video/MPEG4/C/Encoder/MP4VENC/Mp4VEnc_Lib
 *
 * *****************  Version 12  *****************
 * User: Sor          Date: 03/05/06   Time: 1:55p
 * Updated in $/rd_2/video/MPEG4/C/Encoder/MP4VENC/Mp4VEnc_Lib
 * MODIFICATION: Original TVideoOptionFlags can't be modified - RECOVERED.
 *
 * *****************  Version 11  *****************
 * User: Sor          Date: 03/05/06   Time: 1:37p
 * Updated in $/rd_2/video/MPEG4/C/Encoder/MP4VENC/Mp4VEnc_Lib
 * MODIFICATION: Modify TVideoOptionFlags structure - DONE.
 *
 * *****************  Version 10  *****************
 * User: Sor          Date: 03/02/14   Time: 3:39p
 * Updated in $/rd_2/image/JPEG/C/Encoder/JPEGEnc/JPEGEnc_Lib
 * MODIFICATION: Modify DLLAPI definition - DONE.
 *
 * *****************  Version 9  *****************
 * User: Sor          Date: 03/02/14   Time: 3:25p
 * Updated in $/rd_2/image/JPEG/C/Encoder/JPEGEnc/JPEGEnc_Lib
 * MODIFICATION: Add DLLAPI definition - DONE.
 *
 * *****************  Version 8  *****************
 * User: Sor          Date: 03/02/14   Time: 9:55a
 * Updated in $/rd_2/image/JPEG/C/Encoder/JPEGEnc/JPEGEnc_Lib
 * FEATURE: Add new video option flag - DONE.
 *
 * *****************  Version 7  *****************
 * User: Sor          Date: 03/01/22   Time: 4:50p
 * Updated in $/rd_2/video/MPEG4/C/Decoder/MP4VDEC/Mp4VDec_Lib
 * MODIFICATION: Add VIDEO_CHANGE_PIXEL_FORMAT and VIDEO_SET_DEBLOCKING in
 * TVideoOptionFlags - DONE.
 *
 * *****************  Version 6  *****************
 * User: Sor          Date: 03/01/20   Time: 3:58p
 * Updated in $/rd_2/video/MPEG4/TM1300/Encoder/Mp4VEnc/Mp4VEnc_Lib
 * REFINEMENT: Change TMp4VEncOptionFlags to TVideoOptionFlags and move it
 * to global_codec.h - DONE.
 * REFINEMENT: Remove TMp4VEncProfileLevel field in TMp4VEncInitOptions -
 * DONE.
 *
 * *****************  Version 5  *****************
 * User: Sor          Date: 03/01/15   Time: 1:56p
 * Updated in $/rd_2/video/MPEG4/C/Decoder/MP4VDEC/Mp4VDec_Lib
 * Change to fit the VNDP requirement.
 *
 * *****************  Version 4  *****************
 * User: Sor          Date: 02/12/02   Time: 11:08a
 * Updated in $/rd_2/video/MPEG4/C/Decoder/MP4VDEC/Mp4VDec_Lib
 * Added output pixel format enumerator for video decoders.
 *
 * *****************  Version 3  *****************
 * User: Sor          Date: 02/11/29   Time: 8:23a
 * Updated in $/rd_2/misc/CODINGRULE/codec_proto/comm_lib
 * Add GB and PB engine buffer type enumeration.
 *
 * *****************  Version 2  *****************
 * User: Vicky        Date: 02/10/28   Time: 12:01p
 * Updated in $/rd_2/misc/CODINGRULE/codec_proto/comm_lib
 * remove "\" for complier error on linux OS
 *
 * *****************  Version 1  *****************
 * User: Sor          Date: 02/07/23   Time: 1:46p
 * Created in $/rd_2/misc/CODINGRULE/codec_proto/comm_lib
 *
 * *****************  Version 3  *****************
 * User: Joe          Date: 02/04/26   Time: 4:16p
 * Updated in $/rd_2/misc/CODINGRULE/codec_proto/comm_lib
 * update 80 word wrapping
 *
 * *****************  Version 2  *****************
 * User: Joe          Date: 02/04/25   Time: 4:58p
 * Updated in $/rd_2/misc/CODINGRULE/codec_proto/comm_lib
 * Update callback function definitions
 *
 * *****************  Version 1  *****************
 * User: Joe          Date: 02/03/06   Time: 10:25a
 * Created in $/rd_2/misc/CODINGRULE/CODEC_PROTO/comm_lib
 * codec prototype
 *
 */

/*!
 *******************************************************************************
 *
 * Copyright 2008 VN Inc. All rights reserved.
 *
 * \file
 * global_codec.h
 *
 * \brief
 * VN's Audio/Video codec common callback function definitions
 *
 * \date
 * 2002/04/25
 *
 * \author
 * Joe Wu
 *
 **************************************************************************** */

#ifndef _VIVO_CODEC_H_
#define _VIVO_CODEC_H_

#include "typedef.h"
#include "errordef.h"

/*! Make from four character codes to one 32-bits DWORD */
#ifndef MAKEFOURCC
    #define MAKEFOURCC(ch0, ch1, ch2, ch3)  ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) | ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif //defined(MAKEFOURCC)

/*! Use for generate dynamic link library for Windows */
#ifndef DLLAPI
#ifdef _WIN32
  #ifdef _USRDLL
    #define DLLAPI __declspec(dllexport) __stdcall
  #else
    #define DLLAPI __stdcall
  #endif //defined(_USRDLL)
#else
  #define DLLAPI
#endif //defined(_WIN32)
#endif //defined(DLLAPI)

/*!
 *******************************************************************************
 * \brief
 * This callback function provides the codec to enter an OS dependent
 * critical section.
 *
 * \param dwSemaphore
 * The custom data for this callback function. It may be the address of the
 * semaphore or any other data you want to passed to the callback function
 * when enter the critical section.
 *
 * \return
 * NONE
 *
 * \note
 * This callback function will be called whenever the codec need to enter
 * critical section.
 *
 **************************************************************************** */
#if defined(__TCS__)
typedef void (* FOnEnterCriticalSec)(DWORD dwSemaphore);
#else
typedef void (* FOnEnterCriticalSec)(PVOID pvSemaphore);
#endif
#define TOnEnterCriticalSec FOnEnterCriticalSec

/*!
 *******************************************************************************
 * \brief
 * This callback function provides the codec to leave an OS dependent
 * critical section.
 *
 * \param dwSemaphore
 * The custom data for this callback function. It may be the address of the
 * semaphore or any other data you want to passed to the callback function when
 * enter the critical section.
 *
 * \return
 * NONE
 *
 * \note
 * This callback function will be called whenever the codec need to leave
 * critical section. The semaphore are the same as that in TOnEnterCriticalSec.
 *
 **************************************************************************** */
#if defined(__TCS__)
typedef void (* FOnLeaveCriticalSec)(DWORD dwSemaphore);
#else
typedef void (* FOnLeaveCriticalSec)(PVOID pvSemaphore);
#endif
#define TOnLeaveCriticalSec FOnLeaveCriticalSec

/*!
 *******************************************************************************
 * \brief
 * This callback function is called when the encoder sends data from the
 * internal bitstream buffer and the bitstream buffer is full.
 *
 * \param dwUserData
 * The custom data for this callback function. It may be the address of a
 * structure or the delay time the encoder sends data from the internal buffer
 * again.
 *
 * \retval S_OK
 * The data is sent ok
 * \retval S_FAIL
 * The data is unable to process, and the encoder's task will return immediately
 * with return value - \a S_FAIL.
 *
 * \note
 * This callback function has two different usages. In single thread mode, if
 * you set this callback function, it will be called when the internal bitstream
 * buffer is full. You can call the \b AVEnc_TakeData function to retrieve the
 * bitstream data from internal buffer. In multithread mode, you can just call
 * the \b Sleep function, or any system call to let OS switch to another thread.
 *
 **************************************************************************** */
#if defined(__TCS__)
typedef SCODE (* FOnSendData)(DWORD dwUserData);
#else
typedef SCODE (* FOnSendData)(PVOID pvUserData);
#endif
#define TOnSendData FOnSendData

/*!
 *******************************************************************************
 * \brief
 * This callback function is called when the decoder acquires the data from the
 * internal bitstream buffer and the bitstream buffer is empty.
 *
 * \param dwUserData
 * The custom data for this callback function. It may be the address of a
 * structure or the delay time the decoder acquires the data from the internal
 * buffer again.
 *
 * \retval S_OK
 * The data is sent ok
 * \retval S_FAIL
 * The data is unable to process, and the decoder's task will return immediately
 * with return value - \a S_FAIL.
 *
 * \note
 * This callback function has two different usages. In single thread mode, if
 * you set this callback function, it will be called when the internal bitstream
 * buffer is empty. You can call the \b AVDec_FeedData function to put the
 * bitstream data to the internal buffer. In multithread mode, you can just call
 * the \b Sleep function, or any system call to let OS switch to another thread.
 *
 **************************************************************************** */
#if defined(__TCS__)
typedef SCODE (* FOnRequestData)(DWORD dwUserData);
#else
typedef SCODE (* FOnRequestData)(PVOID pvUserData);
#endif
#define TOnRequestData FOnRequestData

/* ====================================================================================== */
#if defined(__x86_64__) || defined(__IA64__) || defined(_M_IA64) || defined (_M_X64) || defined(_WIN64)
#define ALIGN_ADDRESS(a) ((BYTE *)a+ALIGN_PADDING-(((DWORD)(QWORD)a+ALIGN_PADDING)&(~ALIGN_MASK)));
#else //TriMedia or 32-bit PC
#define ALIGN_ADDRESS(a) ((BYTE *)a+ALIGN_PADDING-(((DWORD)a+ALIGN_PADDING)&(~ALIGN_MASK)));
#endif
/* ====================================================================================== */
typedef enum mem_type
{
    MEM_TYPE_CACHED = 0,
    MEM_TYPE_NONCACHED = 1
} EMemType;

typedef enum align_type
{
    ALIGN_TYPE_1_BYTE = 0,
    ALIGN_TYPE_2_BYTE = 1,
    ALIGN_TYPE_4_BYTE = 2,
    ALIGN_TYPE_8_BYTE = 3,
    ALIGN_TYPE_16_BYTE = 4,
    ALIGN_TYPE_32_BYTE = 5,
    ALIGN_TYPE_64_BYTE = 6,
    ALIGN_TYPE_128_BYTE = 7,
    ALIGN_TYPE_256_BYTE = 8,
    ALIGN_TYPE_512_BYTE = 9,
    ALIGN_TYPE_1024_BYTE = 10,
    ALIGN_TYPE_2048_BYTE = 11,
    ALIGN_TYPE_4096_BYTE = 12,
    ALIGN_TYPE_8192_BYTE = 13,
    ALIGN_TYPE_16384_BYTE = 14,
    ALIGN_TYPE_32768_BYTE = 15
} EAlignType;

typedef SCODE (* FOnRequestMem)(DWORD dwUserData,
                                DWORD dwSize,
                                EMemType eMemType,
                                EAlignType eAlignType,
                                DWORD *pdwUserBaseAddr,
                                DWORD *pdwPhyBaseAddr);
typedef SCODE (* FOnRequestNonCachedMem)(DWORD dwUserData,
                                         DWORD dwSize,
                                         DWORD *pdwUserBaseAddr,
                                         DWORD *pdwPhyBaseAddr);
typedef SCODE (* FOnRequestCachedMem)(DWORD dwUserData,
                                      DWORD dwSize,
                                      DWORD *pdwUserBaseAddr,
                                      DWORD *pdwPhyBaseAddr);

/* ====================================================================================== */
typedef SCODE (* FOnReadReg)(DWORD dwUserData, DWORD dwDeviceAddr, DWORD dwRegAddr, DWORD dwSpecRegAddr, DWORD *pdwData);
typedef SCODE (* FOnWriteReg)(DWORD dwUserData, DWORD dwDeviceAddr, DWORD dwRegAddr, DWORD dwSpecRegAddr, DWORD dwData);
typedef SCODE (* FOnReadBuf)(DWORD dwUserData, DWORD dwDeviceAddr, DWORD dwCmdRegAddr, DWORD dwLength, BYTE *pbyData);
typedef SCODE (* FOnWriteBuf)(DWORD dwUserData, DWORD dwDeviceAddr, DWORD dwCmdRegAddr, DWORD dwLength, BYTE *pbyData);

/* ====================================================================================== */
/*! FOURCC for H263 video codec */
#define FOURCC_H263 (MAKEFOURCC('H','2','6','3'))
/*! FOURCC for MPEG-4 video codec */
#define FOURCC_MP4V (MAKEFOURCC('M','P','4','V'))
/*! FOURCC for MPEG-2 video codec */
#define FOURCC_MP2V (MAKEFOURCC('M','P','2','V'))
/*! FOURCC for JPEG image codec */
#define FOURCC_JPEG (MAKEFOURCC('J','P','E','G'))
/*! FOURCC for H264 video codec */
#define FOURCC_H264 (MAKEFOURCC('H','2','6','4'))

/*! FOURCC for MPEG-1 layer 3 audio codec */
#define FOURCC_MP3A (MAKEFOURCC('M','P','3','A'))
/*! FOURCC for MPEG-2 AAC audio codec */
#define FOURCC_AAC2 (MAKEFOURCC('A','A','C','2'))
/*! FOURCC for MPEG-4 AAC audio codec */
#define FOURCC_AAC4 (MAKEFOURCC('A','A','C','4'))

/*! FOURCC for GSM-AMR speech codec */
#define FOURCC_GAMR (MAKEFOURCC('G','A','M','R'))
/*! FOURCC for G.711 codec */
#define FOURCC_G711 (MAKEFOURCC('G','7','1','1'))
/*! FOURCC for G.726 codec */
#define FOURCC_G726 (MAKEFOURCC('G','7','2','6'))
/*! FOURCC for a-LAW codec */
#define FOURCC_ALAW (MAKEFOURCC('A','L','A','W'))
/*! FOURCC for u-LAW codec */
#define FOURCC_ULAW (MAKEFOURCC('U','L','A','W'))
/*! FOURCC for G.722 speech codec */
#define FOURCC_G722 (MAKEFOURCC('G','7','2','2'))
/*! FOURCC for G.722.1 speech codec */
#define FOURCC_G721 (MAKEFOURCC('G','7','2','1'))
/*! FOURCC for G.723.1 speech codec */
#define FOURCC_G723 (MAKEFOURCC('G','7','2','3'))
/*! FOURCC for G.728 speech codec */
#define FOURCC_G728 (MAKEFOURCC('G','7','2','8'))
/*! FOURCC for G.729 speech codec */
#define FOURCC_G729 (MAKEFOURCC('G','7','2','9'))
/*! FOURCC for G.729AB speech codec */
#define FOURCC_729A (MAKEFOURCC('7','2','9','A'))
/*! FOURCC for G.729B speech codec */
#define FOURCC_729B (MAKEFOURCC('7','2','9','B'))

/*! FOURCC for VIVO codec */
#define FOURCC_VIVO (MAKEFOURCC('V','I','V','O'))
/*! FOURCC for TEXT codec */
#define FOURCC_TEXT (MAKEFOURCC('T','E','X','T'))
/*! FOURCC for HINT codec */
#define FOURCC_HINT (MAKEFOURCC('H','I','N','T'))
/*! FOURCC for EVNT */
#define FOURCC_EVNT (MAKEFOURCC('E','V','N','T'))
/*! FOURCC for Motion Event */
#define FOURCC_MOTN (MAKEFOURCC('M','O','T','N'))
/*! FOURCC for Digital Input Event */
#define FOURCC_DIGI (MAKEFOURCC('D','I','G','I'))
/*! FOURCC for Meta Data */
#define FOURCC_MDAT (MAKEFOURCC('M','D','A','T'))


/*! FOURCC for configuration */
#define FOURCC_CONF (MAKEFOURCC('C','O','N','F'))

/*! FOURCC for proprietary codec */
#define FOURCC_PROP (MAKEFOURCC('P','R','O','P'))

/*! GB and PB engine buffer type enumeration */
typedef enum buffer_type_flags
{
    JPEGNONCIRCULAR = 1,
    JPEGCIRCULAR = 2,
    MPEGNONCIRCULAR = 3,
    MPEGCIRCULAR = 4,
    H264NONCIRCULAR = 5,
	H26LNONCIRCULAR = 5,
    H264CIRCULAR = 6,
    H26LCIRCULAR = 6,
    H264IOVECTOR = 7,
    H26LIOVECTOR = 7
} EBufTypeFlags;
#define TBufTypeFlags EBufTypeFlags

/*! GB and PB engine io vector mode enumeration */
typedef enum io_vector_mode
{
    IO_VECTOR_BYTE = 0,
    IO_VECTOR_WORD = 1,
    IO_VECTOR_DWORD = 2,
	IO_VECTOR_NONE = 4
} EIOVectorMode;

/* ====================================================================================== */
/*! Video signal format enumeration */
typedef enum video_signal_format
{
    VIDEO_SIGNAL_FORMAT_PROGRESSIVE_RAW = 1,
    VIDEO_SIGNAL_FORMAT_INTERLACE_CCIR656 = 2,
    VIDEO_SIGNAL_FORMAT_PROGRESSIVE_CCIR656 = 3,
    VIDEO_SIGNAL_FORMAT_BAYER_PATTERN = 4,
    VIDEO_SIGNAL_FORMAT_PROGRESSIVE_RAW_16BITS = 5,
    VIDEO_SIGNAL_FORMAT_PROGRESSIVE_BT1120 = 6,
    VIDEO_SIGNAL_FORMAT_INTERLACE_BT1120 = 7,
	VIDEO_SIGNAL_FORMAT_RGB24 = 8,
	VIDEO_SIGNAL_FORMAT_INTERLACE_RAW = 9,
	VIDEO_SIGNAL_FORMAT_INTERLACE_RAW_16BITS = 10,
	VIDEO_SIGNAL_FORMAT_RGB16 = 11,
	VIDEO_SIGNAL_FORMAT_INTERLEAVE_RGB16 = 12,
	VIDEO_SIGNAL_FORMAT_INTERLEAVE_RGB24 = 13
} EVideoSignalFormat;

typedef enum video_signal_size
{
    VIDEO_SIGNAL_SIZE_160x120   = 1,
    VIDEO_SIGNAL_SIZE_176x120   = 2,
    VIDEO_SIGNAL_SIZE_176x144   = 3,
    VIDEO_SIGNAL_SIZE_320x240   = 4,
    VIDEO_SIGNAL_SIZE_352x240   = 5,
    VIDEO_SIGNAL_SIZE_352x288   = 6,
    VIDEO_SIGNAL_SIZE_640x240   = 7,
    VIDEO_SIGNAL_SIZE_704x240   = 8,
    VIDEO_SIGNAL_SIZE_704x288   = 9,
    VIDEO_SIGNAL_SIZE_720x240   = 10,
    VIDEO_SIGNAL_SIZE_720x288   = 11,
    VIDEO_SIGNAL_SIZE_640x480   = 12,
    VIDEO_SIGNAL_SIZE_704x480   = 13,
    VIDEO_SIGNAL_SIZE_704x576   = 14,
    VIDEO_SIGNAL_SIZE_720x480   = 15,
    VIDEO_SIGNAL_SIZE_720x576   = 16,
    VIDEO_SIGNAL_SIZE_480x272   = 17,
    VIDEO_SIGNAL_SIZE_768x576   = 18,
    VIDEO_SIGNAL_SIZE_800x480   = 19,
    VIDEO_SIGNAL_SIZE_800x600   = 20,
    VIDEO_SIGNAL_SIZE_854x480   = 21,
    VIDEO_SIGNAL_SIZE_1024x600  = 22,
    VIDEO_SIGNAL_SIZE_1024x768  = 23,
    VIDEO_SIGNAL_SIZE_1152x768  = 24,
    VIDEO_SIGNAL_SIZE_1280x720  = 25,
    VIDEO_SIGNAL_SIZE_1280x768  = 26,
    VIDEO_SIGNAL_SIZE_1280x800  = 27,
    VIDEO_SIGNAL_SIZE_1280x854  = 28,
    VIDEO_SIGNAL_SIZE_1280x960  = 29,
    VIDEO_SIGNAL_SIZE_1280x1024 = 30,
    VIDEO_SIGNAL_SIZE_1368x768  = 31,
    VIDEO_SIGNAL_SIZE_1400x1050 = 32,
    VIDEO_SIGNAL_SIZE_1440x900  = 33,
    VIDEO_SIGNAL_SIZE_1440x960  = 34,
    VIDEO_SIGNAL_SIZE_1600x1200 = 35,
    VIDEO_SIGNAL_SIZE_1680x1050 = 36,
    VIDEO_SIGNAL_SIZE_1920x1080 = 37,
    VIDEO_SIGNAL_SIZE_1920x1200 = 38,
    VIDEO_SIGNAL_SIZE_2048x1080 = 39,
    VIDEO_SIGNAL_SIZE_2048x1536 = 40,
    VIDEO_SIGNAL_SIZE_2560x1600 = 41,
    VIDEO_SIGNAL_SIZE_2560x2048 = 42

} EVideoSignalSize;

/*! Video signal color temperature enumeration */
typedef enum video_signal_color_temperature
{
    VIDEO_SIGNAL_COLOR_TEMPERATURE_2856K = 1,
    VIDEO_SIGNAL_COLOR_TEMPERATURE_3200K = 1,
    VIDEO_SIGNAL_COLOR_TEMPERATURE_4150K = 2,
    VIDEO_SIGNAL_COLOR_TEMPERATURE_5500K = 2,
    VIDEO_SIGNAL_COLOR_TEMPERATURE_GT_5500K = 3,
    VIDEO_SIGNAL_COLOR_TEMPERATURE_6500K = 3
} EVideoSignalColorTemperature;

/*! Video signal frequency enumeration */
typedef enum video_signal_frequency
{
    VIDEO_SIGNAL_FREQUENCY_50HZ = 1,
    VIDEO_SIGNAL_FREQUENCY_60HZ = 2,
    VIDEO_SIGNAL_FREQUENCY_24HZ = 3,
    VIDEO_SIGNAL_FREQUENCY_30HZ = 4
} EVideoSignalFrequency;

/*! Video signal option flag enumeration */
typedef enum video_signal_option_flags
{
    VIDEO_SIGNAL_OPTION_RESET = 1,
    VIDEO_SIGNAL_OPTION_SET_SIZE = 2,
    VIDEO_SIGNAL_OPTION_SET_BRIGHTNESS = 3,
    VIDEO_SIGNAL_OPTION_SET_CONTRAST = 4,
    VIDEO_SIGNAL_OPTION_SET_HUE = 5,
    VIDEO_SIGNAL_OPTION_SET_SATURATION = 6,
    VIDEO_SIGNAL_OPTION_SET_COLOR_TEMPERATURE = 7,
    VIDEO_SIGNAL_OPTION_SET_AUTO_EXPOSURE_CTRL = 8,
    VIDEO_SIGNAL_OPTION_SET_AUTO_GAIN_CTRL = 9,
    VIDEO_SIGNAL_OPTION_SET_AUTO_WHITE_BALANCE_CTRL = 10,
    VIDEO_SIGNAL_OPTION_SET_AUTO_BRIGHTNESS_CTRL = 11,
    VIDEO_SIGNAL_OPTION_SET_FREQUENCY = 12,
    VIDEO_SIGNAL_OPTION_SET_FLIP = 13,
    VIDEO_SIGNAL_OPTION_SET_MIRROR = 14,
    VIDEO_SIGNAL_OPTION_SET_MONO = 15,
    VIDEO_SIGNAL_OPTION_SET_LOW_PASS_FILTER = 16,
    VIDEO_SIGNAL_OPTION_SET_CAPTURE_AREA = 17,
    VIDEO_SIGNAL_OPTION_SET_START_PIXEL = 18,
    VIDEO_SIGNAL_OPTION_SET_NIGHT_MODE = 19,
    VIDEO_SIGNAL_OPTION_SET_FRAME_RATE = 20,
    VIDEO_SIGNAL_OPTION_SET_OUTPUT_TYPE = 21,
    VIDEO_SIGNAL_OPTION_SET_ZOOM = 22,
    VIDEO_SIGNAL_OPTION_SET_PRIVACY_MASK = 23,
    VIDEO_SIGNAL_OPTION_SET_AUTO_TRACK_WHITE = 24,
    VIDEO_SIGNAL_OPTION_SET_EXPOSURE_TIME = 25,
    VIDEO_SIGNAL_OPTION_SET_FIELD_INVERSE = 26,
    VIDEO_SIGNAL_OPTION_RESET_FRAME_RATE_CTRL = 27,
    VIDEO_SIGNAL_OPTION_SET_SHUTTER_CTRL = 28,
    VIDEO_SIGNAL_OPTION_SET_GLOBALGAIN_CTRL = 29,
    VIDEO_SIGNAL_OPTION_SET_CONFIG = 30,
    VIDEO_SIGNAL_OPTION_SET_BACKLIGHT_COMPENSATION = 31,
    VIDEO_SIGNAL_OPTION_SET_AUTO_IRIS_CTRL = 32,
    VIDEO_SIGNAL_OPTION_SET_EXPOSURE_LEVEL = 33,
    VIDEO_SIGNAL_OPTION_SET_SHARPNESS = 34,
    VIDEO_SIGNAL_OPTION_SET_HALF_SIZED_OUTPUT = 35,
    VIDEO_SIGNAL_OPTION_SET_COLOR_CORRECTION = 36,
    VIDEO_SIGNAL_OPTION_SET_GAMMA_TABLE = 37,
	VIDEO_SIGNAL_OPTION_SET_TONE_MAPPING = 39,
	VIDEO_SIGNAL_OPTION_SET_CONTRAST_ENHANCEMENT = 40,
	VIDEO_SIGNAL_OPTION_PHOTO_LDC_CALIBRATE = 41,
	VIDEO_SIGNAL_OPTION_SET_PHOTO_LDC_TABLE = 42,
	VIDEO_SIGNAL_OPTION_SET_AUTO_COLOR_SUPPRESSION = 43,
	VIDEO_SIGNAL_OPTION_SET_PHOTO_LDC_EN = 44,
	VIDEO_SIGNAL_OPTION_SET_OBJECT_MASK = 45,
	VIDEO_SIGNAL_OPTION_AUTO_DETECT_STD = 52,
	VIDEO_SIGNAL_OPTION_SET_WDR = 53,
	VIDEO_SIGNAL_OPTION_SET_AUTO_EXPOSURE_WINDOWS = 38,
	VIDEO_SIGNAL_OPTION_SET_AUTO_EXPOSURE_WINDOW_PRIORITY = 46,
	VIDEO_SIGNAL_OPTION_SET_AUTO_EXPOSURE_MAX_SHUTTER = 47,
	VIDEO_SIGNAL_OPTION_SET_AUTO_EXPOSURE_MAX_GAIN = 48,
	VIDEO_SIGNAL_OPTION_SET_AUTO_EXPOSURE_TARGET_LUMINANCE = 49,
	VIDEO_SIGNAL_OPTION_SET_AUTO_EXPOSURE_MIN_SHUTTER = 50,
	VIDEO_SIGNAL_OPTION_SET_AUTO_EXPOSURE_MIN_GAIN = 51,
	VIDEO_SIGNAL_OPTION_SET_AUTO_EXPOSURE_MODE = 54,
	VIDEO_SIGNAL_OPTION_SET_AUTO_IRIS_EN = 55,
	VIDEO_SIGNAL_OPTION_SET_AUTO_FOCUS_WINDOW = 56,
	VIDEO_SIGNAL_OPTION_SET_FOCUS_POSITION = 57,
	VIDEO_SIGNAL_OPTION_SET_FOCUS_SPEED = 58,
	VIDEO_SIGNAL_OPTION_SET_ZOOM_POSITION = 59,
	VIDEO_SIGNAL_OPTION_SET_ZOOM_SPEED = 60,
	VIDEO_SIGNAL_OPTION_SET_FOCUS_NOISE_THRES = 61,
	VIDEO_SIGNAL_OPTION_SET_ZOOMTRACKING_FOCUS_EN = 62,
	VIDEO_SIGNAL_OPTION_GET_AUTO_FOCUS_TABLE_SIZE = 63,
	VIDEO_SIGNAL_OPTION_SET_AUTO_FOCUS_CALIBRATE = 64,
	VIDEO_SIGNAL_OPTION_SET_AUTO_FOCUS_TABLE = 65,
	VIDEO_SIGNAL_OPTION_SET_ANTI_ALIASING = 66,
	VIDEO_SIGNAL_OPTION_SET_AUTO_EXPOSURE_SPEED = 67,
	VIDEO_SIGNAL_OPTION_SET_AUTO_IRIS_ACTIVE_TIME = 68,
	VIDEO_SIGNAL_OPTION_SET_AUTO_SCENE = 69,
	VIDEO_SIGNAL_OPTION_SET_BLACK_CLAMP = 70,
	VIDEO_SIGNAL_OPTION_SET_IMPULSE_NOISE_REMOVAL = 71,
	VIDEO_SIGNAL_OPTION_SET_AUTO_WHITE_BALANCE_WINDOW_PRIORITY = 72,
	VIDEO_SIGNAL_OPTION_SET_CACHE_COHERENCE = 73,
	VIDEO_SIGNAL_OPTION_GET_COLOR_TEMPERATURE = 74,
	VIDEO_SIGNAL_OPTION_SET_COLOR_TRANSFORM = 75,
	VIDEO_SIGNAL_OPTION_SENSOR_DIRECT_ACCESS = 76,
	VIDEO_SIGNAL_OPTION_SET_COMPRESS_FORMAT = 77,
	// [NOTE]!! MAXIMUM NUMBER is 77
} EVideoSignalOptionFlags;

/* ! Video display option flag enumeration */
typedef enum video_display_option_flags
{
    VIDEO_DISP_OPTION_SET_INPUT = 1,
    VIDEO_DISP_OPTION_SET_OVERLAY = 2,
    VIDEO_DISP_OPTION_SET_ALPHA_VALUE = 3,
    VIDEO_DISP_OPTION_SET_KEY = 4,
    VIDEO_DISP_OPTION_SET_CURSOR = 5,
    VIDEO_DISP_OPTION_SET_PIXEL_FORMAT = 6,
    VIDEO_DISP_OPTION_SET_PAL = 7,
    VIDEO_DISP_OPTION_SET_BRIGHTNESS = 8,
    VIDEO_DISP_OPTION_SET_CONTRAST = 9,
    VIDEO_DISP_OPTION_SET_SATURATION = 10,
    VIDEO_DISP_OPTION_SET_GAMMA = 11,
    VIDEO_DISP_OPTION_SET_OVERLAY_POSITION = 12,
    VIDEO_DISP_OPTION_SET_DISPLAY_BUFFER = 13,
    VIDEO_DISP_OPTION_SET_INPUT_OFFSET = 14,
    VIDEO_DISP_OPTION_SET_OUTPUT_CTRL = 15,
	VIDEO_DISP_OPTION_SET_VIEW = 16,
    VIDEO_DISP_OPTION_SET_OUTPUT_SIZE = 17,
    VIDEO_DISP_OPTION_CABLE_DETECT = 18,
} EVideoDispOptionFlags;

/* ====================================================================================== */
/* ! Encoding rate control type enumeration */
typedef enum rate_control_flags
{
    RATE_CTRL_NONE = 0,         // constant quality and variable bitrate
    RATE_CTRL_VQCB = 1,         // variable quality and constant bitrate
    RATE_CTRL_CQCB = 2,         // constant quality and constant bitrate
    RATE_CTRL_STRICT_VQCB = 3,  // strict variable quality and constant bitrate, never overflow
    RATE_CTRL_CVBR = 4,          // constant quality when bitrate meet the requirement.
	RATE_CTRL_ADPT =5           //Adaptive control for different conditions
} ERateCtrlFlags;

/* ! Encoding frame color components enumeration */
typedef enum color_flags
{
    COLOR = 1,
    MONO = 2
} EColorFlags;
#define TColorFlags EColorFlags

/* ! Video output pixel format enumeration */
typedef enum pixel_format_flags
{
    YUV420 = 1,
    YUV422 = 2,
    YUY2 = 3,
    UYVY = 4,
    RGB16555 = 5,
    RGB16565 = 6,
    RGB24 = 7,
    FLIP_RGB24 = 8,                 // obsolete
    RGB32 = 9,
    NONE = 10,
    YUV444 = 11,
    BGR16555 = 12,
    BGR16565 = 13,
    BGR24 = 14,
    BGR32 = 15,
    SP420 = 16,                     // semi-planar 420, aka NV12
    SP422 = 17,                     // semi-planar 422
    ARGB32 = 18,
    ARGB16 = 19
} EPixelFormatFlags;
#define TPixelFormatFlags EPixelFormatFlags

typedef enum nal_type_flags
{
	NAL_TYPE_NON_IDR_SLICE = 1,
	NAL_TYPE_DATA_PARTITION_A_SLICE = 2,
	NAL_TYPE_DATA_PARTITION_B_SLICE = 3,
	NAL_TYPE_DATA_PARTITION_C_SLICE = 4,
	NAL_TYPE_IDR_SLICE = 5,
	NAL_TYPE_SEI = 6,
	NAL_TYPE_SEQUENCE_PARAMETER_SET = 7,
	NAL_TYPE_PICTURE_PARAMETER_SET = 8,
	NAL_TYPE_ACCESS_UNIT_DELIMITER = 9,
	NAL_TYPE_END_OF_SEQUENCE = 10,
	NAL_TYPE_END_OF_STREAM = 11,
	NAL_TYPE_FILLER_DATA = 12,
	NAL_TYPE_SEQUENCE_PARAMETER_SET_EXTENSION = 13,
	NAL_TYPE_PREFIX_NAL_UNIT = 14
} ENALTypeFlags;

/* ! Video and image codec option enumeration */
typedef enum video_option_flags
{
    VIDEO_RESTART = 1,
    VIDEO_CHANGE_FRAME_BUFFER = 2,  // should be replaced with option 17 and 18
    VIDEO_CHANGE_BITSTREAM_BUFFER = 3,
    VIDEO_FLUSH_BITSTREAM_BUFFER = 4,
    VIDEO_CHANGE_USERDATA_BUFFER = 5,
    VIDEO_FORCE_INTRA = 6,
    VIDEO_CHANGE_QUANT = 7,
    VIDEO_CHANGE_PIXEL_FORMAT = 8,  // should be replaced with option 19 and 20
    VIDEO_SET_DEBLOCKING = 9,
    VIDEO_CHANGE_COLOR = 10,
    VIDEO_CHANGE_INTRA_INTERVAL = 11,
    VIDEO_CHANGE_BITRATE = 12,
    VIDEO_SET_RATE_CONTROL = 13,
    VIDEO_CHANGE_FRAME_SIZE = 14,   // should be replaced with option 21 and 22
    VIDEO_CHANGE_INTRA_MB_REFRESH_NUM = 15,
    VIDEO_CHANGE_MOTION_DETECTION_THRESHOLD = 16,
    VIDEO_CHANGE_IN_FRAME_BUFFER = 17,
    VIDEO_CHANGE_OUT_FRAME_BUFFER = 18,
    VIDEO_CHANGE_IN_PIXEL_FORMAT = 19,
    VIDEO_CHANGE_OUT_PIXEL_FORMAT = 20,
    VIDEO_CHANGE_IN_FRAME_SIZE = 21,
    VIDEO_CHANGE_OUT_FRAME_SIZE = 22,
    VIDEO_SET_OUTPUT = 23,
    VIDEO_SET_DERINGING = 24,
    VIDEO_SET_VERTICAL_DOUBLE = 25,
    VIDEO_CHANGE_DISPOSITION = 26,  // Frame-based or block-based
    VIDEO_CHANGE_OVERLAY_FRAME_BUFFER = 27,
    VIDEO_CHANGE_OVERLAY_FRAME_SIZE = 28,
    VIDEO_CHANGE_OVERLAY_CTRL = 29,
    VIDEO_CHANGE_IN_FRAME_OFFSET = 30,
    VIDEO_CHANGE_ENCODE_MODE = 31,
    VIDEO_CHANGE_ROTATE_DEGREE = 32,
    VIDEO_CHANGE_SUB_SAMPLE_RATIO = 33,
    VIDEO_CHANGE_WINDOW_POSITION = 34,
    VIDEO_CHANGE_WINDOW_SIZE = 35,
    VIDEO_CHANGE_IN_MODE = 36,
    VIDEO_ADD_WINDOW = 37,
	VIDEO_DELETE_WINDOW = 38,
    VIDEO_CHANGE_QUANT_TABLE = 39,
    VIDEO_DELETE_ALL_WINDOWS = 40,
    VIDEO_FD_SET_DB = 41,
    VIDEO_LPD_SET_DB = 42,
	VIDEO_OD_SET_FB_AREA = 43,
	VIDEO_CHANGE_OBJECT_INFO_BUFFER = 44,
	VIDEO_CHANGE_OBJECT_QP_DELTA = 45,
	VIDEO_OD_SET_ROI_MASK = 46,
	VIDEO_CHANGE_SEARCH_RANGE = 47,
	VIDEO_CHANGE_REFERENCE_FRAME_NUM = 48,
	VIDEO_OD_SET_FLAME_PARAM = 49,
	VIDEO_OD_SET_MB = 50,
	VIDEO_OD_SET_FG_BLK_TYPE = 51,
    VIDEO_CHANGE_BITSTREAM_IO_VECTOR_BUFFERS = 52,
    VIDEO_CHANGE_SCALABILITY_TEMPORAL_LAYER_NUM = 53,
    VIDEO_CHANGE_REGION_OF_INTEREST_INFO = 54,
    VIDEO_CHANGE_SCALABILITY_HEADER_EN = 55,
    VIDEO_CHANGE_ENCODE_MAXIMUM_FRAME_RATE = 56,
	VIDEO_SET_DECODE_FRAME_NUM = 57,
	VIDEO_CHANGE_REGION_OF_INTEREST_WINDOWS_INFO = 58,
	VIDEO_CHANGE_I_SLICE_QUALITY_STRATEGY = 59,
	VIDEO_GET_FRAME_QUANT = 60,
	VIDEO_CHANGE_FRAME_RATE = 61,
	VIDEO_CLEAN_INTERNAL_BUFFER = 62,
	VIDEO_GET_MB_INFO_BUFF = 63,
	VIDEO_CHANGE_FRAME_TIME_TICKS_INFO = 64,
	VIDEO_CHANGE_H264_ENCODE_DEBLOCKING_FLAG = 65,
	VIDEO_CHANGE_SVC_ENCODE_DEBLOCKING_FLAG = 66,
	VIDEO_CHANGE_COMPRESS_FORMAT_FLAG  =67
} EVideoOptionFlags;
#define TVideoOptionFlags EVideoOptionFlags

/* =========================================================================================== */
typedef enum sub_sample_ratio
{
    SSR_ORG = 0,
    SSR_HALF = 1,
    SSR_QUARTER = 2,
    SSR_OCT = 3
} ESubSampleRatio;

/* ====================================================================================== */
typedef struct video_signal_options
{
    EVideoSignalOptionFlags eOptionFlags;
    DWORD adwUserData[3];
} TVideoSignalOptions;

/* ! Version number structure */
typedef struct version_num
{
    BYTE byMajor;
    BYTE byMinor;
    BYTE byBuild;
    BYTE byRevision;
} TVersionNum;

typedef SCODE (* FOnVertDouble)(DWORD dwUserData, BYTE *pbyInYFrame, BYTE *pbyInCbFrame, BYTE *pbyInCrFrame, BYTE *pbyOutYFrame, BYTE *pbyOutCbFrame, BYTE *pbyOutCrFrame, EPixelFormatFlags ePixelFormat, DWORD dwStride, DWORD dwWidth, DWORD dwHeight);
typedef SCODE (* FOnDeblocking)(DWORD dwUserData, BYTE *pbyInYFrame, BYTE *pbyInCbFrame, BYTE *pbyInCrFrame, EPixelFormatFlags ePixelFormat, DWORD dwStride, DWORD dwWidth, DWORD dwHeight, DWORD dwQuant);
typedef SCODE (* FOnDeringing)(DWORD dwUserData, BYTE *pbyInYFrame, BYTE *pbyInCbFrame, BYTE *pbyInCrFrame, BYTE *pbyOutYFrame, BYTE *pbyOutCbFrame, BYTE *pbyOutCrFrame, EPixelFormatFlags ePixelFormat, DWORD dwStride, DWORD dwWidth, DWORD dwHeight, DWORD dwQuant);
typedef SCODE (* FOnMoving)(DWORD dwUserData, BYTE *pbyInYFrame, BYTE *pbyInCbFrame, BYTE *pbyInCrFrame, BYTE *pbyOutYFrame, BYTE *pbyOutCbFrame, BYTE *pbyOutCrFrame, EPixelFormatFlags ePixelFormat, DWORD dwClockwiseAngle, DWORD dwInStride, DWORD dwOutStride, DWORD dwWidth, DWORD dwHeight);
typedef SCODE (* FOnConverting)(DWORD dwUserData, BYTE *pbyInYFrame, BYTE *pbyInCbFrame, BYTE *pbyInCrFrame, BYTE *pbyOutYFrame, BYTE *pbyOutCbFrame, BYTE *pbyOutCrFrame, EPixelFormatFlags eInPixelFormat, EPixelFormatFlags eOutPixelFormat, DWORD dwClockwiseAngle, DWORD dwInStride, DWORD dwOutStride, DWORD dwWidth, DWORD dwHeight);
typedef SCODE (* FOnBlock2Frame)(DWORD , BYTE *, BYTE *, BYTE *, BYTE *, BYTE *, BYTE *, EPixelFormatFlags , DWORD , DWORD , DWORD , ESubSampleRatio, ESubSampleRatio);

typedef enum autoiris_en
{
		AUTOIRIS_DISABLE = 0,
		AUTOIRIS_ENABLE = 1,
}EAutoIrisEn;

typedef enum autoexposure_mode
{
		AEMODE_AUTO	= 0,
		AEMODE_BACKLIGHT	= 1,
		AEMODE_CUSTOMIZED = 2,
}EAEMode;

typedef enum tw2866_standard
{
		TVSTD_NTSC	= 0,
		TVSTD_PAL	= 1,
		TVSTD_UNKNOWN = 2,
}ETVStd;

typedef struct video_sensor_initial_param
{
	DWORD dwVideoSensorVersion;
	/* Sensor light frequency (50 Hz or 60 Hz) */
	DWORD dwVideoSensorFrequency;
} TVideoSensorInitialParam;

typedef struct auto_detect_std_info {
	BOOL bDataLost;
	ETVStd eDetectStd;
}TAutoDetectStdInfo;

typedef struct video_sensor_statistic
{
	DWORD dwLumaHorSubWindowNum;
	DWORD dwLumaVerSubWindowNum;
	DWORD *pdwStatisticY;

	DWORD dwWBHorSubWindowNum;
	DWORD dwWBVerSubWindowNum;
	DWORD *pdwStatisticR;
	DWORD *pdwStatisticG;
	DWORD *pdwStatisticB;

	DWORD dwHistogramBinNum;
	DWORD *pdwHistogram;
} TVideoSensorStatistic;

/* Window/crop information structrue */
typedef struct window_info
{
    DWORD dwHorzStart;
    DWORD dwVertStart;
    DWORD dwWidth;
    DWORD dwHeight;
} TWindowInfo;

/* sensor call back function */
typedef struct video_sensor_device
{
	/* open function */
	int (*open)(TVideoSensorInitialParam* ptParam, DWORD dwDevNum);
	/* release function */
	void (*release)(DWORD dwDevNum);
	/* ioctl function */
	int (*ioctl)( TVideoSignalOptions* ptArg, DWORD dwDevNum);
	/* getting current shutter value function */
	DWORD (*get_shutter_value)(void);
	/* getting sensor remaining line function */
	DWORD (*get_remaining_line_num)(void);
	/* grouping access function */
	void (*group_access)(int status);
	/* detect NTSC/PAL standard */
	void (*detect_standard)(DWORD dwDevNum, TAutoDetectStdInfo *ptInfo);
	/* setup NTSC/PAL standard */
	void (*setup_standard)(DWORD dwDevNum, ETVStd eStd);
	/* get ae statistic from sensor */
	int (*get_exp_statistic)(TVideoSensorStatistic *ptParam, DWORD dwDevNum);
	/* get wb statistic from sensor */
	int (*get_wb_statistic)(TVideoSensorStatistic *ptParam, DWORD dwDevNum);
	/* getting sensor max gain function */
	DWORD (*get_max_gain)(void);
} TVideoSensorDevice;

/* Iris motor call back function */
typedef struct iris_motor_device
{
	int (*open)(void);
	void (*release)(void);
	int  (*control)(DWORD dwCmd,  DWORD dwArg);
	int (*set_options)(TVideoSignalOptions *ptArg);
} TIrisMotorDevice;

/* Iris motor control info */
typedef struct iris_ctrl_info
{
	DWORD dwSpeed;
	DWORD dwStep;
	BOOL bWaitCmpt;
} TIrisCtrlInfo;
/* ====================================================================================== */
// With respective to CMOS sensor
typedef enum lens_move_direction
{
	LENS_NO_MOVE = 0,
	LENS_MOVE_NEAR = 1,
	LENS_MOVE_FAR = 2,
} ELensMoveDirection;

/* Lens motor control options */
typedef enum lens_control_flags
{
	LENS_CTRL_FOCUS_STOP = 1,
	LENS_CTRL_FOCUS_RESET = 2,
	LENS_CTRL_FOCUS_GET_POSITION = 3,
	LENS_CTRL_FOCUS_GET_RANGE = 4,
	LENS_CTRL_FOCUS_MOVE_TARGET = 5,
	LENS_CTRL_FOCUS_MOVE_NEAR_LIMIT = 6,
	LENS_CTRL_FOCUS_MOVE_FAR_LIMIT = 7,
	LENS_CTRL_ZOOM_STOP = 8,
	LENS_CTRL_ZOOM_RESET = 9,
	LENS_CTRL_ZOOM_GET_POSITION = 10,
	LENS_CTRL_ZOOM_GET_RANGE = 11,
	LENS_CTRL_ZOOM_MOVE_TARGET = 12,
	LENS_CTRL_ZOOM_MOVE_TELE = 13,
	LENS_CTRL_ZOOM_MOVE_WIDE = 14,
	LENS_CTRL_ZOOM_GET_MAXRATIO = 15,
	LENS_CTRL_FOCUS_ZOOM_STOP = 16,
	LENS_CTRL_FOCUS_ZOOM_MOVE_TARGET = 17,
	LENS_CTRL_IRCUT_ENABLE = 18,
	LENS_CTRL_IRCUT_DISABLE = 19,
	LENS_CTRL_IRIS_STOP = 20,
	LENS_CTRL_IRIS_OPEN_LARGEST = 21,
	LENS_CTRL_IRIS_OPEN = 22,
	LENS_CTRL_IRIS_CLOSE = 23,
	LENS_CTRL_IRIS_CLOSE_SMALLEST = 24,
	LENS_CTRL_IRIS_GET_POSITION = 25,
	LENS_CTRL_IRIS_GET_RANGE = 26,
} ELensControlFlags;

/* Auto-Focus motor call back function */
typedef struct autofocus_motor_device
{
	int (*open)(void);
	void (*release)(void);
	int (*control)(unsigned int dwCmd, DWORD dwArg);
} TAutoFocusMotorDevice;

/* Auto-Focus motor control info */
typedef struct autofocus_control_info
{
	DWORD dwFocusDirection;
	DWORD dwFocusSpeed;
	DWORD dwFocusStep;
	DWORD dwZoomDirection;
	DWORD dwZoomSpeed;
	DWORD dwZoomStep;
	BOOL bWaitCmpt;	// 1: TRUE, 0:FALSE
} TAutoFocusCtrlInfo;

#endif // codec

/* ====================================================================================== */

