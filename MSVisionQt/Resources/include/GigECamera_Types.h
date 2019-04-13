#ifndef GENICAM_TYPES_H
#define GENICAM_TYPES_H

#if ! defined(_MSC_VER_)	// Modified by Sny

  #include <stdint.h>

  //#define interface struct	// Modified by Sny

  typedef unsigned short USHORT;
  typedef unsigned char UCHAR;
  typedef unsigned long ULONG;
  typedef int BOOL;

#else // if defined(_MSC_VER_)

  //! \addtogroup Base_PublicUtilities
  //! \{

  //! makes int 8 portable across different platforms
  typedef char int8_t;
  //! makes char 8 portable across different platforms
  typedef char char8_t;
  //! makes unsigned char portable across different platforms
  typedef unsigned char uint8_t;
  //! makes char 8 portable across different platforms
  typedef unsigned char uchar8_t;

  //! makes short portable across different platforms
  typedef short int16_t;
  //! makes unsigned short portable across different platforms
  typedef unsigned short uint16_t;

  //! makes __int32 portable across different platforms
  typedef __int32 int32_t;
  //! makes unsigned int32 portable across different platforms
  typedef unsigned __int32 uint32_t;

  //! makes int64 portable across different platforms
  typedef __int64 int64_t;
  //! makes unsigned int64 portable across different platforms
  typedef unsigned __int64 uint64_t;

  //! use a universal platform dependend int
  typedef __int64 int_t;
  //! use a universal platform dependend unsigned int
  typedef unsigned __int64 uint_t;

  #ifndef _SIZE_T_DEFINED
    #ifdef  _WIN64
      //! use a universal platform dependend unsigned int
      typedef unsigned __int64 size_t;
    #else
      //! use a universal platform dependend unsigned int
      typedef unsigned int size_t;
    #endif
    #define _SIZE_T_DEFINED
  #endif

  #ifndef _INTPTR_T_DEFINED
    #ifdef  _WIN64
      //! use a universal platform dependend int
      typedef __int64 intptr_t;
    #else
      //! use a universal platform dependend int
      typedef int intptr_t;
    #endif
    #define _INTPTR_T_DEFINED
  #endif
#endif

#ifndef INT64_MAX
#define INT64_MAX     0x7fffffffffffffffLL  /*maximum signed __int64 value */
#define INT64_MIN     0x8000000000000000LL  /*minimum signed __int64 value */
#define UINT64_MAX    0xffffffffffffffffULL  /*maximum unsigned __int64 value */

#define INT32_MAX     0x000000007fffffffLL  /*maximum signed __int32 value */
#define INT32_MIN     0xffffffff80000000LL  /*minimum signed __int32 value */
#define UINT32_MAX    0x00000000ffffffffULL  /*maximum unsigned __int32 value */

#define INT8_MAX     0x000000000000007fLL  /*maximum signed __int8 value */
#define INT8_MIN     0xffffffffffffff80LL  /*minimum signed __int8 value */
#define UINT8_MAX    0x00000000000000ffULL  /*maximum unsigned __int8 value */
#endif
  //! \}


/* 
*  \brief 回调函数得到的数据的结构
*/
typedef struct _IMAGE_INFO
{
	uint64_t	nTimeStamp;		///< 时间戳，采集到图像的时刻，精度为0.01us
	USHORT		nBlockId;		///< 帧号，从开始采集开始计数
	UCHAR		*pImageBuffer;	///< 图像指针，即指向(0,0)像素所在内存位置的指针，通过该指针可以访问整个图像
	ULONG		nImageSizeAcq;	///< 采集到的图像大小[字节]
	UCHAR		nMissingPackets;///< 传输过程中丢掉的包数量
	uint64_t	nPixelType;		///< 图像格式
	uint32_t	nSizeX;			///< 图像宽度
	uint32_t	nSizeY;         ///< 图像高度
	uint32_t	nOffsetX;		///< 图像水平坐标
	uint32_t	nOffsetY;       ///< 图像垂直坐标
} MV_IMAGE_INFO, * pMV_IMAGE_INFO ;

/* 
*  \brief Bayer颜色模式
*/
typedef enum {	
	BayerRG,	//< 颜色模式RGGB
	BayerBG,	//< 颜色模式BGGR
	BayerGR,	//< 颜色模式GRBG
	BayerGB,	//< 颜色模式GBRG
	BayerGRW,	//< 颜色模式GRW
	BayerInvalid
} MV_BAYER_MODE;

/* 
*  \brief 图像像素格式
*/
typedef enum {	
	PixelFormat_Mono8 = 0x01080001,	//!<8Bit灰度
	PixelFormat_BayerBG8=0x0108000B,	//!<8Bit Bayer图,颜色模式为BGGR
	PixelFormat_BayerRG8=0x01080009,	//!<8Bit Bayer图,颜色模式为RGGB
	PixelFormat_BayerGB8=0x0108000A,	//!<8Bit Bayer图,颜色模式为GBRG
	PixelFormat_BayerGR8=0x01080008,	//!<8Bit Bayer图,颜色模式为GRBG
	PixelFormat_BayerGRW8=0x0108000C,	//!<8Bit Bayer图,颜色模式为GRW8
	PixelFormat_Mono16=0x01100007,		//!<16Bit灰度
	PixelFormat_BayerGR16=0x0110002E,	//!<16Bit Bayer图,颜色模式为GR
	PixelFormat_BayerRG16=0x0110002F,	//!<16Bit Bayer图,颜色模式为RG
	PixelFormat_BayerGB16=0x01100030,	//!<16Bit Bayer图,颜色模式为GB
	PixelFormat_BayerBG16=0x01100031	//!<16Bit Bayer图,颜色模式为BG
} MV_PixelFormatEnums;

/* 
*  \brief 错误返回值类型
*/
typedef enum  
{
	MVST_SUCCESS                = 0,      ///< 没有错误      
	MVST_ERROR                  = -1001,  ///< 一般错误
	MVST_ERR_NOT_INITIALIZED    = -1002,  //!< 没有初始化
	MVST_ERR_NOT_IMPLEMENTED    = -1003,  //!< 没有实现
	MVST_ERR_RESOURCE_IN_USE    = -1004,  //!< 资源被占用
	MVST_ACCESS_DENIED          = -1005,  ///< 无法访问
	MVST_INVALID_HANDLE         = -1006,  ///< 错误句柄
	MVST_INVALID_ID             = -1007,  ///< 错误ID
	MVST_NO_DATA                = -1008,  ///< 没有数据
	MVST_INVALID_PARAMETER      = -1009,  ///< 错误参数
	MVST_FILE_IO                = -1010,  ///< IO错误
	MVST_TIMEOUT                = -1011,  ///< 超时
	MVST_ERR_ABORT              = -1012,  ///< 退出
	MVST_INVALID_BUFFER_SIZE    = -1013,  ///< 缓冲区尺寸错误
	MVST_ERR_NOT_AVAILABLE      = -1014,  ///< 无法访问
	MVST_INVALID_ADDRESS        = -1015,  ///< 地址错误
}MVSTATUS_CODES;

typedef struct  
{
	unsigned char mIpAddr[4];	//!<相机的IP地址
	unsigned char mEthernetAddr[6];	//!<相机的MAC地址
	char mMfgName[32];	//!<相机厂商名称
	char mModelName[32]; //!<相机型号
	char mSerialNumber[16];	//!<相机序列号
	char mUserDefinedName[16];	//!<用户设置的相机名称
	unsigned char m_IfIp[4];	//!<计算机和相机连接的网卡IP地址
	unsigned char m_IfMAC[6];	//!<计算机和相机连接的网卡MAC地址
}MVCamInfo;

typedef enum 
{
	TriggerSource_Software=0,//!<触发模式下，由软触发(软件指令)来触发采集
	TriggerSource_Line1=2 //!<触发模式下，有外触发信号来触发采集
}TriggerSourceEnums;

typedef enum 
{
	TriggerMode_Off,  //!<触发模式关，即FreeRun模式，相机连续采集
	TriggerMode_On	//!<触发模式开，相机等待软触发或外触发信号再采集图像
}TriggerModeEnums;

typedef enum 
{
	TriggerActivation_RisingEdge,//!<上升沿触发
	TriggerActivation_FallingEdge//!<下降沿触发
}TriggerActivationEnums;

typedef enum 
{
	LineSource_Off=0,  //!<关闭
	LineSource_ExposureActive=5,  //!<和曝光同时
	LineSource_Timer1Active=6,	//!<由定时器控制
	LineSource_UserOutput0=12	//!<直接由软件控制
}LineSourceEnums;

typedef struct  
{
	unsigned long m_nTotalBuf;	//!<从开始采集，总计成功收到的完整图像帧数
	unsigned long m_nFailedBuf;	//!<从开始采集，总计收到的不完整图像帧数
	unsigned long m_nTotalPacket;	//!<从开始采集，总计收到的图像数据包数
	unsigned long m_nFailedPacket;	//!<从开始采集，总计丢失的图像数据包数
	unsigned long m_nResendPacketReq;//!<从开始采集，总计重发请求的图像数据包数
	unsigned long m_nResendPacket;//!<从开始采集，总计重发成功的图像数据包数
}MVStreamStatistic;

typedef enum 
{
	UserSetSelector_Default,  //!<出厂设置
	UserSetSelector_UserSet1,  //!<用户设置1
	UserSetSelector_UserSet2   //!<用户设置2
}UserSetSelectorEnums;

typedef enum 
{
	SensorTaps_One,  //!<单通道
	SensorTaps_Two,  //!<双通道
	SensorTaps_Three,  //!<三通道
	SensorTaps_Four,  //!<四通道
}SensorTapsEnums;

typedef enum 
{
	AutoFunctionProfile_GainMinimum,  //!<Keep gain at minimum
	AutoFunctionProfile_ExposureMinimum   //!<Exposure Time at minimum
}AutoFunctionProfileEnums;

typedef enum 
{
	GainAuto_Off,  //!<Disables the Gain Auto function.
	GainAuto_Once,  //!<Sets operation mode to 'once'.
	GainAuto_Continuous   //!<Sets operation mode to 'continuous'.
}GainAutoEnums;

//! Valid values for ExposureAuto
typedef enum 
{
	ExposureAuto_Off,  //!<Disables the Exposure Auto function.
	ExposureAuto_Once,  //!<Sets operation mode to 'once'.
	ExposureAuto_Continuous   //!<Sets operation mode to 'continuous'.
}ExposureAutoEnums;

typedef enum 
{
	BalanceWhiteAuto_Off,  //!<Disables the Balance White Auto function.
	BalanceWhiteAuto_Once,  //!<Sets operation mode to 'once'.
	BalanceWhiteAuto_Continuous   //!<Sets operation mode to 'continuous'.
}BalanceWhiteAutoEnums;


//////////////////////////////////////////////////////////////////////////
typedef enum 
{
	FlipHorizontal = 0,  //!< 左右翻转
	FlipVertical = 1, //!< 上下翻转
	FlipBoth = 2 //!< 旋转180度
} ImageFlipType;

typedef enum 
{
	Rotate90DegCw = 0,       //!< 顺时针旋转90度
	Rotate90DegCcw = 1       //!< 逆时针旋转90度
}ImageRotateType;

typedef enum 
{
	TransferControlMode_Basic = 0,
	TransferControlMode_UserControlled = 2
}TransferControlModeEnums;

//! Valid values for EventID
typedef enum 
{
	EVID_LOST = 0,	//!< 事件ID，相机断开
	EVID_RECONNECT = 1	//! 事件ID，相机重新连上了
}EventIdEnums;

typedef int (__stdcall*MVStreamCB)(MV_IMAGE_INFO*, ULONG_PTR nUserVal);

#endif 
