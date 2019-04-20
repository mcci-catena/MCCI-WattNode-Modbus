/*

Module:  Mcci-WattNode-Modbus-Registers.h

Function:
	C++ register definition and manipulation functions
	for WattNode Modbus.

Copyright notice:
	See LICENSE file accompanying this work.

Author:
	Terry Moore, MCCI Corporation	April, 2018

*/

#ifndef _MCCI_WATTNODE_MODBUS_REGISTERS_H_		/* prevent multiple includes */
#define _MCCI_WATTNODE_MODBUS_REGISTERS_H_

#pragma once

#include <stdint.h>
#include <cstring>

#include "MCCI-WattNode-Modbus-tuple.h"

namespace McciWattNodeModbus {

// create a version number for comparison
static constexpr uint32_t makeVersion(
		uint8_t major, uint8_t minor, uint8_t patch, uint8_t local = 0
		)
		{
		return ((uint32_t)major << 24u) | ((uint32_t)minor << 16u) | ((uint32_t)patch << 8u) | (uint32_t)local;
		}

// extract major number from version
static constexpr uint8_t getMajor(uint32_t v)
		{
		return uint8_t(v >> 24u);
		}

// extract minor number from version
static constexpr uint8_t getMinor(uint32_t v)
		{
		return uint8_t(v >> 16u);
		}

// extract patch number from version
static constexpr uint8_t getPatch(uint32_t v)
		{
		return uint8_t(v >> 8u);
		}

// extract local number from version
static constexpr uint8_t getLocal(uint32_t v)
		{
		return uint8_t(v);
		}

// version of library, for use in static_asserts
static constexpr uint32_t kVersion = makeVersion(0,2,0,0);

class WattNodeModbus_Framework
	{
public:
	// convert WattNodeModbus::Register into equivalent address.
	// Addresses on the bus are origin 0; but Modbus documentation
	// is always origin 1; hence the discrepancy.
	template <typename TRegister>
	static constexpr uint16_t getAddress(TRegister r)
		{
		return static_cast<uint16_t>(r) - 1;
		}

	// convert address into a WattNodeModbus::Register
	template <typename TRegister>
	static constexpr TRegister getRegister(uint16_t address)
		{
		return TRegister(address + 1);
		}


	static float regval_getfloat(int16_t vLow, int16_t vHigh, bool *pError = nullptr)
		{
		const uint32_t uImage = (static_cast<uint16_t>(vHigh) << 16u) 
				      | static_cast<uint16_t>(vLow);

		/* reject NANs */
		if ((uImage & 0x7F800000u) == 0x7F800000u)
			{
			if (pError != nullptr)
				*pError = true;
			return 0.0;
			}

		/* otherwise the answer is the bit pattern as a float */
		float fpResult;
		static_assert(sizeof(fpResult) == sizeof(uImage), "float / uin32_t mismatch");
		std::memcpy(&fpResult, &uImage, sizeof(fpResult));
		return fpResult;
		}

	// Convert float to external form for Modbus.
	// Convenient use: 
	// 	int16_t vLow, vHigh;
	//	std::tie(vLow, vHigh) = WattNodeModbus::float_getregval(vFloat);
	static std::tuple<int16_t, int16_t> float_getregval(float vFloat)
		{
		uint32_t uImage;
		static_assert(sizeof(vFloat) == sizeof(uImage), "float / uint32_t mismatch");
		std::memcpy(&uImage, &vFloat, sizeof(vFloat));
		const int16_t vLow = static_cast<int16_t>(uImage);
		const int16_t vHigh = static_cast<int16_t>(uImage >> 16);
		return std::make_tuple(vLow, vHigh); 
		}

	static int32_t regval_getint32(int16_t vLow, int16_t vHigh)
		{
		const uint32_t uImage = (static_cast<uint16_t>(vHigh) << 16u) 
				      | static_cast<uint16_t>(vLow);
		return (int32_t)(uImage);		
		}

	// Convert int32_t to external form for Modbus.
	// Convenient use: 
	// 	int16_t vLow, vHigh;
	//	std::tie(vLow, vHigh) = WattNodeModbus::int32_getregval(vInt32);
	static std::tuple<int16_t, int16_t> int32_getregval(int32_t vInt32)
		{
		const auto uImage = (uint32_t)(vInt32);
		const int16_t vLow = static_cast<int16_t>(uImage);
		const int16_t vHigh = static_cast<int16_t>(uImage >> 16);
		return std::make_tuple(vLow, vHigh); 
		}

	}; // class WattNodeModbus_Framework

// chose a long name because in the Arduino world, it's all too common for
// strange upper-case symbols to be #define'd.
class WattNode_WNC : public WattNodeModbus_Framework 
	{
public:
	enum class Register : uint16_t;

	// It would be nice to write `auto` here, but some GCCs
	// fail. See https://stackoverflow.com/questions/24617679/workaround-for-debug-symbol-error-with-auto-member-function
	// The error manifests as:
	//	internal compiler error: in gen_type_die_with_usage, at dwarf2out.c:19484
	//	 class WattNode_WNC : public WattNodeModbus_Framework
	//	       ^
	static constexpr uint16_t getAddress(Register r)
		{
		return WattNodeModbus_Framework::getAddress<Register>(r);
		}
	static constexpr Register getRegister(uint16_t address)
		{
		return WattNodeModbus_Framework::getRegister<Register>(address);
		}

	enum class Register : uint16_t
		{
		DummyReg_i32 = 1,
		EnergySum_f32 = 1001,
		EnergyPosSum_f32 = 1003,
		EnergySumNR_f32 = 1005,
		EnergyPosSumNR_f32 = 1007,
		PowerSum_f32 = 1009,
		PowerA_f32 = 1011,
		PowerB_f32 = 1013,
		PowerC_f32 = 1015,
		VoltAvgLN_f32 = 1017,
		VoltA_f32 = 1019,
		VoltB_f32 = 1021,
		VoltC_f32 = 1023,
		VoltAvgLL_f32 = 1025,
		VoltAB_f32 = 1027,
		VoltBC_f32 = 1029,
		VoltAC_f32 = 1031,
		Freq_f32 = 1033,
		EnergyA_f32 = 1101,
		EnerygB_f32 = 1103,
		EnergyC_f32 = 1105,
		EnergyPosA_f32 = 1107,
		EnergyPosB_f32 = 1109,
		EnergyPosC_f32 = 1111,
		EnergyNegSum_f32 = 1113,
		EnergyNegSumNR_f32 = 1115,
		EnergyNegA_f32 = 1117,
		EnergyNegB_f32 = 1119,
		EnergyNegC_f32 = 1121,
		EnergyReacSum_f32 = 1123,
		EnergyReacA_f32 = 1125,
		EnergyReacB_f32 = 1127,
		EnergyReacC_f32 = 1129,
		EnergyAppSum_f32 = 1131,
		EnergyAppA_f32 = 1133,
		EnergyAppB_f32 = 1135,
		EnergyAppC_f32 = 1137,
		PowerFactorAvg_f32 = 1139,
		PowerFactorA_f32 = 1141,
		PowerFactorB_f32 = 1143,
		PowerFactorC_f32 = 1145,
		PowerReacSum_f32 = 1147,
		PowerReacA_f32 = 1149,
		PowerReacB_f32 = 1151,
		PowerReacC_f32 = 1153,
		PowerAppSum_f32 = 1155,
		PowerAppA_f32 = 1157,
		PowerAppB_f32 = 1159,
		PowerAppC_f32 = 1161,
		CurrentA_f32 = 1163,
		CurrentB_f32 = 1165,
		CurrentC_f32 = 1167,
		Demand_f32 = 1169,
		DemandMin_f32 = 1171,
		DemandMax_f32 = 1173,
		DemandApp_f32 = 1175,
		DemandA_f32 = 1177,
		DemandB_f32 = 1179,
		DemandC_f32 = 1181,
		EnergySum_i32 = 1201,
		EnergyPosSum_i32 = 1203,
		EnergySumNR_i32 = 1205,
		EnergyPosSumNR_i32 = 1207,
		PowerSum_i16 = 1209,
		PowerA_i16 /* = 1210 */,
		PowerB_i16 /* = 1211 */,
		PowerC_i16 /* = 1212 */,
		VoltAvgLN_i16 /* = 1213 */,
		VoltA_i16 /* = 1214 */,
		VoltB_i16 /* = 1215 */,
		VoltC_i16 /* = 1216 */,
		VoltAvgLL_i16 /* = 1217 */,
		VoltAB_i16 /* = 1218 */,
		VoltBC_i16 /* = 1219 */,
		VoltAC_i16 /* = 1220 */,
		Freq_i16 /* = 1221 */,
		EnergyA_i32 = 1301,
		EnerygB_i32 = 1303,
		EnergyC_i32 = 1305,
		EnergyPosA_i32 = 1307,
		EnergyPosB_i32 = 1309,
		EnergyPosC_i32 = 1311,
		EnergyNegSum_i32 = 1313,
		EnergyNegSumNR_i32 = 1315,
		EnergyNegA_i32 = 1317,
		EnergyNegB_i32 = 1319,
		EnergyNegC_i32 = 1321,
		EnergyReacSum_i32 = 1323,
		EnergyReacA_i32 = 1325,
		EnergyReacB_i32 = 1327,
		EnergyReacC_i32 = 1329,
		EnergyAppSum_i32 = 1331,
		EnergyAppA_i32 = 1333,
		EnergyAppB_i32 = 1335,
		EnergyAppC_i32 = 1337,
		PowerFactorAvg_i16 = 1339,
		PowerFactorA_i16 /* = 1340 */,
		PowerFactorB_i16 /* = 1341 */,
		PowerFactorC_i16 /* = 1342 */,
		PowerReacSum_i16 /* = 1343 */,
		PowerReacA_i16 /* = 1344 */,
		PowerReacB_i16 /* = 1345 */,
		PowerReacC_i16 /* = 1346 */,
		PowerAppSum_i16 /* = 1347 */,
		PowerAppA_i16 /* = 1348 */,
		PowerAppB_i16 /* = 1349 */,
		PowerAppC_i16 /* = 1350 */,
		CurrentA_i16 /* = 1351 */,
		CurrentB_i16 /* = 1352 */,
		CurrentC_i16 /* = 1353 */,
		Demand_i16 /* = 1354 */,
		DemandMin_i16 /* = 1355 */,
		DemandMax_i16 /* = 1356 */,
		DemandApp_i16 /* = 1357 */,
		DemandA_i16 /* = 1358 */,
		DemandB_i16 /* = 1359 */,
		DemandC_i16 /* = 1360 */,
		IoPinState_i16 /* = 1361 */,
		PulseCount_i32 /* = 1362 */,
		ConfigPasscode_i32 = 1601,
		CtAmps_i16 = 1603,
		CtAmpsA_i16 /* = 1604 */,
		CtAmpsB_i16 /* = 1605 */,
		CtAmpsC_i16 /* = 1606 */,
		CtDirections_i16 /* = 1607 */,
		Averaging_i16 /* = 1608 */,
		PowerIntScale_i16 /* = 1609 */,
		DemPerMins_i16 /* = 1610 */,
		DemSubints_i16 /* = 1611 */,
		GainAdjustA_i16 /* = 1612 */,
		GainAdjustB_i16 /* = 1613 */,
		GainAdjustC_i16 /* = 1614 */,
		PhaseAdjustA_i16 /* = 1615 */,
		PhaseAdjustB_i16 /* = 1616 */,
		PhaseAdjustC_i16 /* = 1617 */,
		CreepLimit_i16 /* = 1618 */,
		PhaseOffset_i16 /* = 1619 */,
		ZeroEnergy_i16 /* = 1620 */,
		ZeroDemand_i16 /* = 1621 */,
		CurrentIntScale_i16 /* = 1622 */,
		IoPinMode_i16 /* = 1623 */,
		ApplyComConfig_i16 = 1651,
		Address_i16 /* = 1652 */,
		BaudRate_i16 /* = 1653 */,
		ParityMode_i16 /* = 1654 */,
		ModbusMode_i16 /* = 1655 */,
		ReplyDelay_i16 /* = 1656 */,
		SerialNumber_i32 = 1701,
		UptimeSecs_i32 = 1703,
		TotalSecs_i32 = 1705,
		Model_i16 = 1707,
		Version_i16 /* = 1708 */,
		Options_i16 /* = 1709 */,
		ErrorStatus_i16 /* = 1710 */,
		PowerFailCount_i16 /* = 1711 */,
		CrcErrorCount_i16 /* = 1712 */,
		FrameErrorCount_i16 /* = 1713 */,
		PacketErrorCount_i16 /* = 1714 */,
		OverrunCount_i16 /* = 1715 */,
		ErrorStatus1_i16 /* = 1716 */,
		ErrorStatus2_i16 /* = 1717 */,
		ErrorStatus3_i16 /* = 1718 */,
		ErrorStatus4_i16 /* = 1719 */,
		ErrorStatus5_i16 /* = 1720 */,
		ErrorStatus6_i16 /* = 1721 */,
		ErrorStatus7_i16 /* = 1722 */,
		ErrorStatus8_i16 /* = 1723 */,
		CStartAddress_u16 = 9501,
		CRegisterCount_u16 /*= 9502 */,
		CRegisterList_vu16 /*= 9503 */,
		CRegisterList0_u16 = CRegisterList_vu16 + 0,
		CRegisterList124_u16 = CRegisterList_vu16 + 124,
		CRegisterListLast_u16 = CRegisterList124_u16,
		}; // enum Register
	};

class WattNode_WND : public WattNodeModbus_Framework
	{
public:
	enum class Register : uint16_t;

	// It would be nice to write `auto` here, but some GCCs
	// fail. See https://stackoverflow.com/questions/24617679/workaround-for-debug-symbol-error-with-auto-member-function
	// The error manifests as:
	//	internal compiler error: in gen_type_die_with_usage, at dwarf2out.c:19484
	//	 class WattNode_WNC : public WattNodeModbus_Framework
	//	       ^
	static constexpr uint16_t getAddress(Register r)
		{
		return WattNodeModbus_Framework::getAddress<Register>(r);
		}
	static constexpr Register getRegister(uint16_t address)
		{
		return WattNodeModbus_Framework::getRegister<Register>(address);
		}

	enum class Register : uint16_t
		{
		DummyReg_i32 = 1,
		EnergySum_f32 = 1001,
		EnergyPosSum_f32 = 1003,
		EnergySumNR_f32 = 1005,
		EnergyPosSumNR_f32 = 1007,
		PowerSum_f32 = 1009,
		Power1_f32 = 1011,
		Power2_f32 = 1013,
		Power3_f32 = 1015,
		VoltAvgLN_f32 = 1017,
		VoltAN_f32 = 1019,
		VoltBN_f32 = 1021,
		VoltCN_f32 = 1023,
		VoltAvgLL_f32 = 1025,
		VoltAB_f32 = 1027,
		VoltBC_f32 = 1029,
		VoltCA_f32 = 1031,
		Freq_f32 = 1033,
		SecondsFast_f32 = 1035,
		PowerFastSum_f32 = 1037,
		PowerFast1_f32 = 1039,
		PowerFast2_f32 = 1041,
		PowerFast3_f32 = 1043,
		Energy1_f32 = 1101,
		Eneryg2_f32 = 1103,
		Energy3_f32 = 1105,
		EnergyPos1_f32 = 1107,
		EnergyPos2_f32 = 1109,
		EnergyPos3_f32 = 1111,
		EnergyNegSum_f32 = 1113,
		EnergyNegSumNR_f32 = 1115,
		EnergyNeg1_f32 = 1117,
		EnergyNeg2_f32 = 1119,
		EnergyNeg3_f32 = 1121,
		EnergyReacSum_f32 = 1123,
		EnergyReac1_f32 = 1125,
		EnergyReac2_f32 = 1127,
		EnergyReac3_f32 = 1129,
		EnergyAppSum_f32 = 1131,
		EnergyApp1_f32 = 1133,
		EnergyApp2_f32 = 1135,
		EnergyApp3_f32 = 1137,
		PowerFactorAvg_f32 = 1139,
		PowerFactor1_f32 = 1141,
		PowerFactor2_f32 = 1143,
		PowerFactor3_f32 = 1145,
		PowerReacSum_f32 = 1147,
		PowerReac1_f32 = 1149,
		PowerReac2_f32 = 1151,
		PowerReac3_f32 = 1153,
		PowerAppSum_f32 = 1155,
		PowerApp1_f32 = 1157,
		PowerApp2_f32 = 1159,
		PowerApp3_f32 = 1161,
		Current1_f32 = 1163,
		Current2_f32 = 1165,
		Current3_f32 = 1167,
		DemandSum_f32 = 1169,
		DemandSumMin_f32 = 1171,
		DemandSumMax_f32 = 1173,
		DemandAppSum_f32 = 1175,
		Demand1_f32 = 1177,
		Demand2_f32 = 1179,
		Demand3_f32 = 1181,
		EnergyReacPos1_f32 = 1183,
		EnergyReacPos2_f32 = 1185,
		EnergyReacPos3_f32 = 1187,
		EnergyReacPosSum_f32 = 1189,
		EnergyReacNeg1_f32 = 1191,
		EnergyReacNeg2_f32 = 1193,
		EnergyReacNeg3_f32 = 1195,
		EnergySum_i32 = 1201,
		EnergyPosSum_i32 = 1203,
		EnergySumNR_i32 = 1205,
		EnergyPosSumNR_i32 = 1207,
		PowerSum_i16 = 1209,
		Power1_i16 /* = 1210 */,
		Power2_i16 /* = 1211 */,
		Power3_i16 /* = 1212 */,
		VoltAvgLN_i16 /* = 1213 */,
		VoltAN_i16 /* = 1214 */,
		VoltBN_i16 /* = 1215 */,
		VoltCN_i16 /* = 1216 */,
		VoltAvgLL_i16 /* = 1217 */,
		VoltAB_i16 /* = 1218 */,
		VoltBC_i16 /* = 1219 */,
		VoltCA_i16 /* = 1220 */,
		Freq_i16 /* = 1221 */,
		Energy1_i32 = 1301,
		Eneryg2_i32 = 1303,
		Energy3_i32 = 1305,
		EnergyPos1_i32 = 1307,
		EnergyPos2_i32 = 1309,
		EnergyPos3_i32 = 1311,
		EnergyNegSum_i32 = 1313,
		EnergyNegSumNR_i32 = 1315,
		EnergyNeg1_i32 = 1317,
		EnergyNeg2_i32 = 1319,
		EnergyNeg3_i32 = 1321,
		EnergyReacSum_i32 = 1323,
		EnergyReac1_i32 = 1325,
		EnergyReac2_i32 = 1327,
		EnergyReac3_i32 = 1329,
		EnergyAppSum_i32 = 1331,
		EnergyApp1_i32 = 1333,
		EnergyApp2_i32 = 1335,
		EnergyApp3_i32 = 1337,
		PowerFactorAvg_i16 = 1339,
		PowerFactor1_i16 /* = 1340 */,
		PowerFactor2_i16 /* = 1341 */,
		PowerFactor3_i16 /* = 1342 */,
		PowerReacSum_i16 /* = 1343 */,
		PowerReac1_i16 /* = 1344 */,
		PowerReac2_i16 /* = 1345 */,
		PowerReac3_i16 /* = 1346 */,
		PowerAppSum_i16 /* = 1347 */,
		PowerApp1_i16 /* = 1348 */,
		PowerApp2_i16 /* = 1349 */,
		PowerApp3_i16 /* = 1350 */,
		Current1_i16 /* = 1351 */,
		Current2_i16 /* = 1352 */,
		Current3_i16 /* = 1353 */,
		DemandSum_i16 /* = 1354 */,
		DemandSumMin_i16 /* = 1355 */,
		DemandSumMax_i16 /* = 1356 */,
		DemandAppSum_i16 /* = 1357 */,
		Demand1_i16 /* = 1358 */,
		Demand2_i16 /* = 1359 */,
		Demand3_i16 /* = 1360 */,
		EnergyReacPos1_i32 = 1364,
		EnergyReacPos2_i32 = 1366,
		EnergyReacPos3_i32 = 1368,
		EnergyReacPosSum_i32 = 1370,
		EnergyReacNeg1_i32 = 1372,
		EnergyReacNeg2_i32 = 1374,
		EnergyReacNeg3_i32 = 1376,
		ConfigPasscode_i32 = 1601,
		CtAmps_i16 = 1603,
		CtAmps1_i16 /* = 1604 */,
		CtAmps2_i16 /* = 1605 */,
		CtAmps3_i16 /* = 1606 */,
		CtDirections_i16 /* = 1607 */,
		Averaging_i16 /* = 1608 */,
		PowerIntScale_i16 /* = 1609 */,
		DemPerMins_i16 /* = 1610 */,
		DemSubints_i16 /* = 1611 */,
		GainAdjust1_i16 /* = 1612 */,
		GainAdjust2_i16 /* = 1613 */,
		GainAdjust3_i16 /* = 1614 */,
		PhaseAdjust1_i16 /* = 1615 */,
		PhaseAdjust2_i16 /* = 1616 */,
		PhaseAdjust3_i16 /* = 1617 */,
		CreepLimit_i16 /* = 1618 */,
		// PhaseOffset_i16 /*= 1619  */, -- no longer used
		ZeroEnergy_i16 = 1620,
		ZeroDemand_i16 /* = 1621 */,
		CurrentIntScale_i16 /* = 1622 */,
		// IoPinMode_i16 /* = 1623 */,
		MeterConfig1_i16 = 1624,
		MeterConfig2_i16 /* = 1625 */,
		MeterConfig3_i16 /* = 1626 */,
		Reserved_i16 /* = 1627 */,
		ChangeCounter_u16 /* = 1628 */,
		NominalCtVolts1_f32 /* = 1629 */,
		NominalCtVolts2_f32 = 1631,
		NominalCtVolts3_f32 = 1633,
		// Reserved_i16 = 1635,
		ConnectionType_i16 = 1636,
		VoltsNoiseFloor_i16 /* = 1637 */,
		ApplyComConfig_i16 = 1651,
		Address_i16 /* = 1652 */,
		BaudRate_i16 /* = 1653 */,
		ParityMode_i16 /* = 1654 */,
		ModbusMode_i16 /* = 1655 */,
		ReplyDelay_i16 /* = 1656 */,
		SerialNumberKey_i32 /* = 1657 */,
		NewAddress_i16 = 1659,
		OptSignedCurrent_i16 = 1685,
		OptEnergySumMethod_i16 /* = 1686 */,
		SerialNumber_i32 = 1701,
		UptimeSecs_i32 = 1703,
		TotalSecs_i32 = 1705,
		Model_i16 = 1707,
		Version_i16 /* = 1708 */,
		Options_i16 /* = 1709 */,
		ErrorStatusQueue_i16 /* = 1710 */,
		PowerFailCount_i16 /* = 1711 */,
		CrcErrorCount_i16 /* = 1712 */,
		FrameErrorCount_i16 /* = 1713 */,
		PacketErrorCount_i16 /* = 1714 */,
		OverrunCount_i16 /* = 1715 */,
		ErrorStatus1_i16 /* = 1716 */,
		ErrorStatus2_i16 /* = 1717 */,
		ErrorStatus3_i16 /* = 1718 */,
		ErrorStatus4_i16 /* = 1719 */,
		ErrorStatus5_i16 /* = 1720 */,
		ErrorStatus6_i16 /* = 1721 */,
		ErrorStatus7_i16 /* = 1722 */,
		ErrorStatus8_i16 /* = 1723 */,
		OptCtAmps1_i16 /* = 1724 */,
		OptCtAmps2_i16 /* = 1725 */,
		OptCtAmps3_i16 /* = 1726 */,
		OptModbusMode_i16 /* = 1727 */,
		OptAddress_i16 /* = 1728 */,
		OptBaudRate_i16 /* = 1729 */,
		OptParityMode_i16 /* = 1730 */,
		OptLockedConfig_i16 = 1737,
		OptFastPower_i16 /* = 1738 */,
		CStartAddress_u16 = 9501,
		CRegisterCount_u16 /* = 9502 */,
		CRegisterList_vu16 /* = 9503 */,
		CRegisterList0_u16 = CRegisterList_vu16 + 0,
		CRegisterList124_u16 = CRegisterList_vu16 + 124,
		CRegisterListLast_u16 = CRegisterList124_u16,
		}; // enum Register
	};
} // namespace McciWattNodeModbus

#endif /* _MCCI_WATTNODE_MODBUS_REGISTERS_H_ */