/* 
 * ------------------------------------------------------------------
 * mrd.h - Azatrax MRD series Model Railroad Sensor / Relay units
 * Created by Robert Heller on Sun Oct 23 10:28:59 2011
 * ------------------------------------------------------------------
 * Modification History: $Log$
 * Modification History: Revision 1.1  2002/07/28 14:03:50  heller
 * Modification History: Add it copyright notice headers
 * Modification History:
 * ------------------------------------------------------------------
 * Contents:
 * ------------------------------------------------------------------
 *  
 *     Model RR System, Version 2
 *     Copyright (C) 1994,1995,2002-2005  Robert Heller D/B/A Deepwoods Software
 * 			51 Locke Hill Road
 * 			Wendell, MA 01379-9728
 * 
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with this program; if not, write to the Free Software
 *     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * 
 *  
 */

#ifndef _MRD_H_
#define _MRD_H_

#ifdef HAVE_LIBUSB1
#include <libusb.h>
#endif

/** @defgroup MRD MRD
  * @brief MRD C++ LibUSB 1.0 Interface.
  *
  * This library implements a class that interfaces to Azatrax's MRD2-S
  * and MRD2-U modules.  Both contain a pair a IR sensors, a stopwatch,
  * and the MRD2-S contains relays.  These units can sense trains, either
  * reflectively (typically `burried' in the track roadbed) or across the
  * track.  The sensors trigger the stopwatch and can also trigger the
  * relays.
  *
  * I was contacted by John Parsons of Azatrax (http://www.azatrax.com) and
  * he kindly sent some samples of the MRD2-U and MRD2-S modules.  This code
  * is the result. The samples allowed me to test the code.
  *
  * @author Robert Heller @<heller\@deepsoft.com@>
  *
  * @{
  */

/**
  * @brief MRD C++ LibUSB 1.0 Interface.
  *
  * This is the basic cross-platform class library that uses the libusb-1.0
  * API to communicate with MRD2-U and MRD2-S modules from Azatrax over the
  * USB bus.  This library contains a class that encapsulates logic to
  * communicate with these devices.  Each class instance connects to a specific
  * device instance.
  *
  * @author Robert Heller @<heller\@deepsoft.com@>
  *
  */

namespace mrd {

#ifndef SWIGTCL8
#define ErrorCode int
#endif


/** @brief MRD I/O Class
  *
  * MRD interface class.
  *
  * This class implements the interface logic for a MRD2-S or MRD2-U device.
  *
  * The constructor opens a connection to a MRD2-S or MRD2-U device, given
  * its serial number.  Each MRD2-S or MRD2-U device has a unique, factory
  * defined serial number, which is printed on a sticker attached to the
  * module. This serial number is much like the MAC address of an Ethernet
  * interface. The destructor closes the connection to the device and frees
  * any resources allocated.
  *
  * The class provides methods to send commands to the device, read back its
  * state and interrogate the state read back.  This way each class instance
  * encapsulates each device instance.
  *
  * There are two static methods, one that return the number of open devices
  * (active instances) and another that returns a list of discovered serial
  * numbers.
  *
  * @author Robert Heller \<heller\@deepsoft.com\>
  */
  
class MRD {
public:
	/** @brief Open device count.
	  *
	  * Return the number of open devices.
	  */
	static int NumberOfOpenDevices() {return deviceOpenCount;}
	/** @brief List serial numbers of all connected devices.
	  *
	  * Return a vector of serial number strings.
	  */
	static char ** AllConnectedDevices();
#ifdef SWIGTCL8
	enum OperatingMode_Type { NonTurnoutSeparate,
				  NonTurnoutDirectionSensing, 
				  TurnoutSolenoid, 
				  TurnoutMotor };
#else
	/** Operating Mode codes. */
	enum OperatingMode_Type {
	/** Non Turnout, separate (-U always reports this). */
	NonTurnoutSeparate=0x31,
	/** Non Turnout, Direction Sensing */
	NonTurnoutDirectionSensing=0x32,
	/** Turnout, Solenoid (momentary action) */
	TurnoutSolenoid=0x34,
	/** Turnout, Motor (sustained action) */
	TurnoutMotor=0x37
	};
	/** Command codes. */
	enum commands {
		/** Set channel 1 */
		cmd_SetChan1=0x31,
		/** Set channel 2*/
		cmd_SetChan2=0x32,
		/** Clear Externally Changed */
		cmd_ClearExternallyChanged=0x34,
		/** Disable External Changes */
		cmd_DisableExternal=0x37,
		/** Enable External Changes */
		cmd_EnableExternal=0x38,
		/** Restore LED Function */
		cmd_RestoreLEDFunction=0x50,
		/** Identify LED 1 */
		cmd_Identify_1=0x51,
		/** Identify LED 2 */
		cmd_Identify_2=0x52,
		/** Identify both LEDs */
		cmd_Identify_1_2=0x53,
		/** Reset Stopwatch */
		cmd_ResetStopwatch=0x54,
		/**  Get State Data */
		cmd_GetStateData=0x57
	};
	/** Device connection list constants. */
	enum DeviceConnectionList {
		_InitSize = 10,
		_GrowSize = 10
	};
#endif
#ifdef SWIGTCL8
	MRD(const char *serialnumber, char **outmessage);
#else
	/** @brief   Constructor.
	  *
	  * The argument is the (unique) serial number of the device.
	  *
	  *   @param serialnumber The serial number of the device to open.
	  *   @param outmessage This holds a pointer to an error message,
	  *	if any.
	  */
	MRD(const char *serialnumber, char **outmessage=NULL);
#endif
	/** @brief   Destructor. */
	~MRD();
	/** @brief Set channel 1 relays and status bits.
	  *
	  * Sets the relays and status bits as it a train activated
	  * channel 1 (-S, turnour mode only).
	  */
	ErrorCode SetChan1() const {return sendByte(cmd_SetChan1);}
		
	/** @brief Set channel 2 relays and status bits.
	  *
	  * Sets the relays and status bits as it a train activated
	  * channel 2 (-S, turnour mode only).
	  */
	ErrorCode SetChan2() const {return sendByte(cmd_SetChan2);}
	/** @brief Clear `ExternallyChanged' status bit.
	  *
	  * Clear `ExternallyChanged' status bit (data packet byte 2).
	  */
	ErrorCode ClearExternallyChanged() const {return sendByte(cmd_ClearExternallyChanged);}
	/** @brief Disable external changes of turnout state.
	  *
	  * Disable external changes of turnout state (-S only).
	  */
	ErrorCode DisableExternal() const {return sendByte(cmd_DisableExternal);}
	/** @brief Enable external changes of turnout state.
	  *
	  * Enable external changes of turnout state (-S only).
	  */
	ErrorCode EnableExternal() const {return sendByte(cmd_EnableExternal);}
	/** @brief Restore LED function.
	  *
	  * Restore LED function - On-board LEDs return to their normal
	  * function of indicating status of sensors 1 and 2.
	  */
	ErrorCode RestoreLEDFunction() const {return sendByte(cmd_RestoreLEDFunction);}
	/** @brief Identify 1.
	  *
	  * Identify 1 - Flashes sensor 1's LED.
	  */
	ErrorCode Identify_1() const {return sendByte(cmd_Identify_1);}
	/** @brief Identify 2.
	  *
	  * Identify 2 - Flashes sensor 2's LED.
	  */
	ErrorCode Identify_2() const {return sendByte(cmd_Identify_2);}
	/** @brief Identify 1 and 2.
	  *
	  * Identify 1 and 2 - Flashes sensor 1 and 2 LEDs.
	  */
	ErrorCode Identify_1_2() const {return sendByte(cmd_Identify_1_2);}
	/** @brief Reset Stopwatch.
	  *
	  * ResetStopwatch - Stops the stopwatch and resets time to 0.
	  */
	ErrorCode ResetStopwatch() const {return sendByte(cmd_ResetStopwatch);}
	/** @brief Get State Data.
	  *
	  * Get State Data - retrieve 16 byte data packet from sensor to
	  * in memory copy.
	  */
	ErrorCode GetStateData();
	/** Packet count. */
	uint8_t PacketCount() const {return stateDataPacket.packetCount;}
	/** Sensor one active. */
	bool Sense_1() const {
		status1_union b;
		b.theByte = stateDataPacket.status1;
		return b.theBits.sense_1 == 1;
	}
	/** Sensor two active. */
	bool Sense_2() const {
		status1_union b;
		b.theByte = stateDataPacket.status1;
		return b.theBits.sense_2 == 1;
	}
	/** Latch one. */
	bool Latch_1() const {
		status1_union b;
		b.theByte = stateDataPacket.status1;
		return b.theBits.latch_1 == 1;
	}
	/** Latch two. */
	bool Latch_2() const {
		status1_union b;
		b.theByte = stateDataPacket.status1;
		return b.theBits.latch_2 == 1;
	}
	/** Has Relays? */
	bool HasRelays() const {
		status1_union b;
		b.theByte = stateDataPacket.status1;
		return b.theBits.modtype == 1;
	}
	/** Reset status? */
	bool ResetStatus() const {
		status2_union b;
		b.theByte = stateDataPacket.status2;
		return b.theBits.resetStatus == 1;
	}
	/** Stopwatch Ticking? */
	bool StopwatchTicking() const {
		status2_union b;
		b.theByte = stateDataPacket.status2;
		return b.theBits.stopwatchTicking == 1;
	}
	/** Externally changed? */
	bool ExternallyChanged() const {
		status2_union b;
		b.theByte = stateDataPacket.status2;
		return b.theBits.externallyChanged == 1;
	}
	/** Allowing External Changes? */
	bool AllowingExternalChanges() const {
		status2_union b;
		b.theByte = stateDataPacket.status2;
		return b.theBits.allowExternalChanges == 1;
	}
	/** @brief Operating mode.
	  *
	  * Returns the operating mode.
	  */
	OperatingMode_Type OperatingMode() const {
		return (OperatingMode_Type)stateDataPacket.operatingMode;
	}
	/** @brief Stopwatch time.
	  *
	  * Returns the current Stopwatch time.
	  *   @param fract 1/100s of a second.
	  *   @param seconds Whole seconds.
	  *   @param minutes Whole minutes.
	  *   @param hours   Whole hours.
	  */
	void Stopwatch(uint8_t &fract, uint8_t &seconds, uint8_t &minutes,
			uint8_t &hours) const {
		fract = stateDataPacket.stopwatchFract;
		seconds = stateDataPacket.stopwatchSeconds;
		minutes = stateDataPacket.stopwatchMinutes;
		hours = stateDataPacket.stopwatchHours;
	}
	/** @brief Serial Number.
	  *
	  * Return our serial number.
	  */
	const char *SerialNumber() const {return mySerialNumber;}
#ifndef SWIGTCL8
private:
	/** @brief Is this the device we want?
	  *
	  * Check to see if this device is a MRD device and if it has
	  * the serial number we want.
	  *
	  * @param dev libusb_device struct pointer
	  *	(from libusb_get_device_list).
	  * @param serialnumber the serial number we are looking for.
	  */
	bool IsTheMRDWeAreLookingFor(libusb_device *dev,
					const char *serialnumber);
	/** @brief Send a command byte.
	  *
	  * Sends one command byte on endpoint 01
	  * @param commandByte the command byte to send.
	  */
	ErrorCode sendByte(uint8_t commandByte) const;
	/** @brief  MRD vendor code.
	  *
	  *  This is the vendor code (Microchip Technology)
	  * was granted for their USB products.
	  */
	static const unsigned short int idVendor;
	/** @brief  MRD product code.
	  *
	  * This is the product Azatrax
	  * uses for their MRD products
	  */
	static const unsigned short int idProduct;
	/** @brief Device open count.
	  *
	  * This is used to decide if and when to call libusb_init
	  * and libusb_exit.
	  */
	static int deviceOpenCount;
	/** @brief USB Device Handle.
	  *
	  *  This is the USB device handle for the
	  * device.
	  */
	struct libusb_device_handle *handle;
	/** @brief serial number buffer.
	  *
	  * This holds the serial number of the device.
	  */
	char mySerialNumber[10];
	/** @brief USB Data Packet.
	  *
	  * This is the USB Data Packet returned in response
	  * to the GetStateData command.
	  */
	struct StateDataPacket {
		/** Command Echo byte. */
		uint8_t commandEcho;		// byte 0
		/** Packet counter */
		uint8_t packetCount;		// byte 1
		/** Status byte 1 */
		uint8_t status1;		// byte 2
		/** Status byte 2 */
		uint8_t status2;		// byte 3
		/** Stopwatch 1/100ths of a second */
		uint8_t stopwatchFract;		// byte 4
		/** Stopwatch seconds */
		uint8_t stopwatchSeconds;	// byte 5
		/** Stopwatch minutes */
		uint8_t stopwatchMinutes;	// byte 6
		/** Stopwatch hours */
		uint8_t stopwatchHours;		// byte 7
		/** Operating mode */
		uint8_t operatingMode;		// byte 8
		/** End of data */
		uint8_t endOfData;		// byte 9
		/** Reserved bytes */
		uint8_t reserved[6];		// bytes 10-15
	} stateDataPacket;
	/** Status byte 1 union type */
	union status1_union {
		/** Status byte as a byte */
		uint8_t theByte;
		/** Status byte as bit fields */
		struct {
			/** Sense 1 */
			unsigned int sense_1:1;
			/** Sense 2 */
			unsigned int sense_2:1;
			/** Latch 1 */
			unsigned int latch_1:1;
			/** Latch 2 */
			unsigned int latch_2:1;
			/** Module type */
			unsigned int modtype:1;
			/** Reserved bits */
			unsigned int reserved:3;
		} theBits;
	};
	/** Status byte 2 union type */
	union status2_union {
		/** Status byte as a byte */
		uint8_t theByte;
		/** Status byte as bit fields */
		struct {
			/** Reset Status */
			unsigned int resetStatus:1;
			/** Stopwatch Ticking */
			unsigned int stopwatchTicking:1;
			/** Externally Changed */
			unsigned int externallyChanged:1;
			/** Allow External Changes */
			unsigned int allowExternalChanges:1;
			/** Reserved bits */
			unsigned int reserved:4;
		} theBits;
	};

#endif
};

};

/*! @} */


#endif // _MRD_H_

