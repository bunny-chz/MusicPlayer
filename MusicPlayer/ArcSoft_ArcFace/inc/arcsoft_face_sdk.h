/*******************************************************************************
* Copyright(c) ArcSoft, All right reserved.
*
* This file is ArcSoft's property. It contains ArcSoft's trade secret, proprietary
* and confidential information.
*
* DO NOT DISTRIBUTE, DO NOT DUPLICATE OR TRANSMIT IN ANY FORM WITHOUT PROPER
* AUTHORIZATION.
*
* If you are not an intended recipient of this file, you must not copy,
* distribute, modify, or take any action in reliance on it.
*
* If you have received this file in error, please immediately notify ArcSoft and
* permanently delete the original and any copy of any file and any printout
* thereof.
*********************************************************************************/

#ifndef _ARCSOFT_SDK_ASF_H_
#define _ARCSOFT_SDK_ASF_H_

#include "amcomdef.h"
#include "asvloffscreen.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ASF_NONE				0x00000000
#define ASF_FACE_DETECT			0x00000001	//�˴�detect������tracking����detection��������֮һ�������ѡ����detect mode ȷ��
#define ASF_FACERECOGNITION		0x00000004	
#define ASF_AGE					0x00000008
#define ASF_GENDER				0x00000010
#define ASF_FACE3DANGLE			0x00000020

#define ASF_DETECT_MODE_VIDEO 	0x00000000		//Videoģʽ��һ�����ڶ�֡�������
#define ASF_DETECT_MODE_IMAGE 	0xFFFFFFFF		//Imageģʽ��һ�����ھ�̬ͼ�ĵ��μ��


//�汾��Ϣ
typedef struct {
	MPChar Version;							// �汾��
	MPChar BuildDate;						// ��������
	MPChar CopyRight;						// Copyright
}ASF_VERSION;

//���ʱ�� Orientation ���ȼ���
typedef MInt32 ASF_OrientPriority;
enum ArcSoftFace_OrientPriority {
	ASF_OP_0_ONLY = 0x1,			// 0, 0, ...
	ASF_OP_90_ONLY = 0x2,			// 90, 90, ...
	ASF_OP_270_ONLY = 0x3,			// 270, 270, ...
	ASF_OP_180_ONLY = 0x4,			// 180, 180, ...
	ASF_OP_0_HIGHER_EXT = 0x5,		// 0, 90, 270, 180, 0, 90, 270, 180, ...
};

//orientation �Ƕȣ���ʱ�뷽��	
typedef MInt32 ASF_OrientCode;
enum ArcSoftFace_OrientCode {
	ASF_OC_0 = 0x1,			// 0 degree 
	ASF_OC_90 = 0x2,		// 90 degree 
	ASF_OC_270 = 0x3,		// 270 degree 
	ASF_OC_180 = 0x4,   	// 180 degree 
	ASF_OC_30 = 0x5,		// 30 degree 
	ASF_OC_60 = 0x6,		// 60 degree 
	ASF_OC_120 = 0x7,		// 120 degree 
	ASF_OC_150 = 0x8,		// 150 degree 
	ASF_OC_210 = 0x9,		// 210 degree 
	ASF_OC_240 = 0xa,		// 240 degree 
	ASF_OC_300 = 0xb,		// 300 degree 
	ASF_OC_330 = 0xc		// 330 degree 
};

//��������Ϣ
typedef struct {
	MRECT		faceRect;		// ��������Ϣ
	MInt32		faceOrient;		// ����ͼ��ĽǶȣ����Բο� ArcFaceCompare_OrientCode
} ASF_SingleFaceInfo, *LPASF_SingleFaceInfo;

//��������Ϣ
typedef struct {
	MRECT* 		 faceRect;			// ��������Ϣ
	MInt32*		 faceOrient;		// ����ͼ��ĽǶȣ����Բο� ArcFaceCompare_OrientCode . 
	MInt32		 faceNum;			// ��⵽����������
}ASF_MultiFaceInfo, *LPASF_MultiFaceInfo;

/*******************************************************************************************
* �����ȶ�SDK����
*******************************************************************************************/
MRESULT ASFActivation(
	MPChar				AppId,			// [in]  APPID
	MPChar				SDKKey			// [in]  SDKKEY
);

/************************************************************************
* ��ʼ������
************************************************************************/
MRESULT ASFInitEngine(
	MLong				detectMode,					// [in]	AF_DETECT_MODE_VIDEO ��Ƶģʽ������������ͷԤ������Ƶ�ļ�ʶ��
													//		AF_DETECT_MODE_IMAGE ͼƬģʽ�������ھ�̬ͼƬ��ʶ��
	ASF_OrientPriority	detectFaceOrientPriority,	// [in]	��������ĽǶ�����ֵ���ο� ArcFaceCompare_OrientPriority
	MInt32				detectFaceScaleVal,			// [in] ������ֵ����ʾ����С�����ߴ磬�óߴ���������ߴ������ͼƬ���ߵ�ռ�ȡ�
													//		ͼ�����ݳߴ�Ϊ1280x720������nscaleΪ8�����⵽����С��������Ϊ1280/8 = 160	//		���磬�û����⵽����С�����ߴ���ͼƬ���ߵ� 1/8����scaleVal����Ϊ 8
													//		video ģʽ��Чֵ��Χ[2,16], Image ģʽ��Чֵ��Χ[2,32],�Ƽ�ֵΪ 16
	MInt32				detectFaceMaxNum,			// [in] �����Ҫ������������
	MInt32				combinedMask,				// [in] �û�ѡ����Ҫ���Ĺ�����ϣ��ɵ�������
	MHandle*			hEngine						// [out] ��ʼ�����ص�����handle
);

/******************************************************
* �������
******************************************************/
MRESULT ASFDetectFaces(
	MHandle				hEngine,				// [in] ����handle
	MInt32				width,					// [in] ͼƬ���
	MInt32				height,					// [in] ͼƬ�߶�
	MInt32				format,					// [in] ��ɫ�ռ��ʽ
	MUInt8*				imgData,				// [in] ͼƬ����
	LPASF_MultiFaceInfo	detectedFaces			// [out]��⵽��������Ϣ 
);
	
/************************************************************************
* ������Ϣ��⣨����/�Ա�/����3D�Ƕȣ�,���֧��4��������Ϣ��⣬�������ַ���δ֪
************************************************************************/
MRESULT ASFProcess(
	MHandle				hEngine,			// [in] ����handle
	MInt32				width,				// [in] ͼƬ���
	MInt32				height,				// [in] ͼƬ�߶�
	MInt32				format,				// [in] ��ɫ�ռ��ʽ
	MUInt8*				imgData,			// [in] ͼƬ����
	LPASF_MultiFaceInfo	detectedFaces,		// [in] ������Ϣ���û����ݴ����Ĺ��ܲü�ѡ����Ҫʹ�õ�������
	MInt32				combinedMask		// [in] ֻ֧�ֳ�ʼ��ʱ��ָ����Ҫ���Ĺ��ܣ���processʱ��һ��������Ѿ�ָ���Ĺ��ܼ��м���ɸѡ
											//      �����ʼ����ʱ��ָ�����������Ա� ��process��ʱ�����ֻ������䣬 
											//      ���ǲ��ܼ���������Ա�֮��Ĺ���
);

/************************************************************************
* ��������
************************************************************************/
MRESULT ASFUninitEngine(
	MHandle hEngine
);

/************************************************************************
* ��ȡ�汾��Ϣ
************************************************************************/
const ASF_VERSION* ASFGetVersion(
	MHandle hEngine
);

//******************************** ����ʶ����� *********************************************
typedef struct {
	MByte*		feature;		// ����������Ϣ
	MInt32		featureSize;	// ����������Ϣ����	
}ASF_FaceFeature, *LPASF_FaceFeature;

/************************************************************************
* ������������ȡ
************************************************************************/
MRESULT ASFFaceFeatureExtract(
	MHandle					hEngine,		// [in]	����handle
	MInt32					width,			// [in] ͼƬ���
	MInt32					height,			// [in] ͼƬ�߶�
	MInt32					format,			// [in] ��ɫ�ռ��ʽ
	MUInt8*					imgData,		// [in] ͼƬ����
	LPASF_SingleFaceInfo	faceInfo,		// [in] ��������λ�úͽǶ���Ϣ
	LPASF_FaceFeature		feature			// [out] ��������
);

/************************************************************************
* ���������ȶ�
************************************************************************/
MRESULT ASFFaceFeatureCompare(
	MHandle					hEngine,			// [in] ����handle
	LPASF_FaceFeature		feature1,		// [in] ���Ƚ���������1
	LPASF_FaceFeature		feature2,		// [in] ���Ƚ���������2
	MFloat*					confidenceLevel	// [out] �ȽϽ�������Ŷ���ֵ
);

//******************************** ������� **********************************************
typedef struct
{
	MInt32*	ageArray;				// "0" ����ȷ��������0����ֵ�����������������
	MInt32	num;					// ������������
}ASF_AgeInfo, *LPASF_AgeInfo;

/************************************************************************
* ��ȡ������Ϣ
************************************************************************/
MRESULT ASFGetAge(
	MHandle hEngine,				// [in] ����handle
	LPASF_AgeInfo ageInfo			// [out] ��⵽��������Ϣ
);

//******************************** �Ա���� **********************************************
typedef struct
{
	MInt32*	genderArray;			//"0" ��ʾ ����, "1" ��ʾ Ů��, "-1" ��ʾ��ȷ��
	MInt32	num;					// ������������	
}ASF_GenderInfo, *LPASF_GenderInfo;

/************************************************************************
* ��ȡ�Ա���Ϣ
************************************************************************/
MRESULT ASFGetGender(
	MHandle hEngine,				// [in] ����handle
	LPASF_GenderInfo genderInfo		// [out] ��⵽���Ա���Ϣ
);

//******************************** ����3D �Ƕ���Ϣ ***********************************
typedef struct
{
	MFloat* roll;
	MFloat* yaw;
	MFloat* pitch;
	MInt32* status;					//0: ������������ֵ������
	MInt32 num;
}ASF_Face3DAngle, *LPASF_Face3DAngle;

/************************************************************************
* ��ȡ3D�Ƕ���Ϣ
************************************************************************/
MRESULT ASFGetFace3DAngle(
	MHandle hEngine,					// [in] ����handle
	LPASF_Face3DAngle p3DAngleInfo		// [out] ��⵽����3D �Ƕ���Ϣ
);


#ifdef __cplusplus
}
#endif
#endif