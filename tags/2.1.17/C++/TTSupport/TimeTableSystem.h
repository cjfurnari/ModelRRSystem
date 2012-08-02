/* 
 * ------------------------------------------------------------------
 * TimeTableSystem.h - Time Table System class definition
 * Created by Robert Heller on Mon Dec 19 21:01:00 2005
 * ------------------------------------------------------------------
 * Modification History: $Log$
 * Modification History: Revision 1.11  2007/09/03 14:39:28  heller
 * Modification History: Rev 2.1.9 Lockdown
 * Modification History:
 * Modification History: Revision 1.10  2007/05/06 12:49:38  heller
 * Modification History: Lock down  for 2.1.8 release candidate 1
 * Modification History:
 * Modification History: Revision 1.9  2007/04/19 17:23:22  heller
 * Modification History: April 19 Lock Down
 * Modification History:
 * Modification History: Revision 1.8  2007/02/21 21:03:10  heller
 * Modification History: SWIG Hackery
 * Modification History:
 * Modification History: Revision 1.7  2007/02/21 20:19:22  heller
 * Modification History: SWIG Hackery
 * Modification History:
 * Modification History: Revision 1.5  2006/08/04 01:59:25  heller
 * Modification History: Aug 3 Lockdown
 * Modification History:
 * Modification History: Revision 1.4  2006/05/18 17:03:24  heller
 * Modification History: CentOS 4.3 updates
 * Modification History:
 * Modification History: Revision 1.3  2006/05/17 23:42:37  heller
 * Modification History: May 17, 2006 Lock down
 * Modification History:
 * Modification History: Revision 1.2  2006/05/16 19:27:46  heller
 * Modification History: May162006 Lockdown
 * Modification History:
 * Modification History: Revision 1.1  2006/01/03 15:30:21  heller
 * Modification History: Lockdown
 * Modification History:
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

#ifndef _TIMETABLESYSTEM_H_
#define _TIMETABLESYSTEM_H_

#ifndef SWIG
#include <Common.h>
#include <PathName.h>
#include <Station.h>
#include <Cab.h>
#include <Train.h>
#include <list>

#ifdef __GNUC__
#if __GNUC__ < 3
#include <hash_map.h>
        namespace Sgi { using ::hash_map; }; // inherit globals
#else
#include <ext/hash_map>
#if __GNUC_MINOR__ == 0
          namespace Sgi = std;               // GCC 3.0
#else
          namespace Sgi = ::__gnu_cxx;       // GCC 3.1 and later
#endif
#endif
#else      // ...  there are other compilers, right?
        namespace Sgi = std;
#endif

/** @name  Main Time Table class and support types.
  * @doc  \TEX{\typeout{Generated from $Id$.}}
  */

//@{


/**  A Vector of doubles.  Used as a vector of layover times. */
typedef vector<double> doubleVector;

/** Equality structure.  Used with the hash map used for Print Options */
struct eqstr
{
	bool operator()(const char* s1, const char* s2) const
	{
		return strcmp(s1, s2) == 0;
	}
};

/** Option hash map, used for Print options. */
typedef Sgi::hash_map<const char*, string, Sgi::hash<const char*>, eqstr> OptionHashMap;


/** List of trains. Simple linked list of trains, used for passing train lists
  * around. */
typedef list<Train*> TrainList;

/** Station times class, used by the \TEX{\LaTeX\ } generator methods.
  * This class holds time table information used in the code that generates 
  * the \TEX{\LaTeX\ } tables.  Each StationTimes item contains one table 
  * element in the form of an arrival time and a departure time.  The flag
  * member indicates if only the arrival time, departure time, or both times
  * are valid.  An originating train has no arrival time and a terminating
  * train has no departure time.
  *
  * This class is actually used to hold the information for a single cell in
  * a formatted time table.  Each cell contains an arrivial time and a 
  * departure time.  Each row in the table contains the information for a
  * specific station and each column contains the information for a single
  * train.
  *
  * See also: {@link TrainStationTimes} and {@link TrainTimesAtStation}.
  */
class StationTimes {
public:
	/** Constructor: create an entry for a time table cell.
	  *   @param a The arrival time.
	  *   @param d The departure time.
	  *   @param f The stop flag: Origin, Terminate, or Transit.
	  */
	StationTimes(double a=-1,double d=-1,Stop::FlagType f=Stop::Transit) {
		arrival = a;
		departure = d;
		flag = f;
	}
	/** Copy constructor: create an entry for a time table cell from an
	  * existing StationTimes object.
	  *    @param other The other StationTimes object.
	  */
	StationTimes(const StationTimes &other) {
		arrival = other.arrival;
		departure = other.departure;
		flag = other.flag;
	}
	/** Assignment operator: copy the fields from another StationTimes
	  * object.
	  *    @param other The other StationTimes object.
	  */
	StationTimes &operator= (const StationTimes &other) {
		arrival = other.arrival;
		departure = other.departure;
		flag = other.flag;
		return *this;
	}
	/** Accessor for the arrival time.
	  */
	double Arrival() const {return arrival;}
	/** Accessor for the departure time.
	  */
	double Departure() const {return departure;}
	/** Accessor for the type of stop flag.
	  */
	Stop::FlagType Flag() const {return flag;}
private:
	/** The arrival time, in scale time units. Only used for trains in
	  * transit and for terminating trains. 
	  */
	double arrival;
	/** The departure time, in scale time units. Only used for trains in
	  * transit and for originating trains. 
	  */
	double departure;
	/** The stop flag: Origin, Terminate, or Transit. 
	  */
	Stop::FlagType flag;
};

/** Map of station times, indexed by train number. These are the individual
  * rows of the time table. The train number (symbol) is the column index.
  * Each of these rows is for a single station.  This is a sparse vector, since
  * not all trains stop at (or go past) all stations.  The ommited elements
  * result in blank cells in the output table.
  */
typedef map<string,StationTimes,less<string> > TrainStationTimes;

/** Map of maps of station times, indexed by station index.  This is the whole
  * time table.  The station index is the row index.  This is a sparse vector,
  * since not all trains stop at (or go past) all stations.  The ommited
  * elements result in blank cells in the output table.
  */
typedef map<int,TrainStationTimes,less<int> > TrainTimesAtStation;

/** List of strings.
  * This is a simple linked list of strings, used in various places.
  */
typedef list<string> StringList;

/** Convert a list of strings to a flat string. The result is comma
  * separated and each string is enclosed in quote characters 
  * (#"#).  If a string contains a quote character or a
  * backslash, the character is quoted with a backslash.
  *   @param list The list of strings.
  */
const char *StringListToString(const StringList &list);

/** Convert a flat string to a list of strings. Returns false if there was
  * a syntax error.
  *    @param strlinList The input string.
  *    @param result The output list.
  */
bool StringListFromString(string strlinList,StringList &result);

#endif


#ifdef SWIG
/*
 * Type map to handle error messages.  Hide this parameter from Tcl, but return
 * it as a second result, returning TCL_ERROR, if there is an error message.
 */

/*
 * On input: allocate a pointer to receive a possible error message.
 */
 
%typemap(tcl8,in,numinputs=0) char **outmessage {
	$1 = new char*;
	*$1 = NULL;
}

/*
 * On output: If there was an error message, capture it and return TCL_ERROR.
 * Free up used space.
 */
%typemap(tcl8,argout) char **outmessage {
	Tcl_Obj * tcl_result = $result;
	if (*$1 != NULL) {
		int mlen = strlen(*$1);
		if (Tcl_ListObjAppendElement(interp,tcl_result,Tcl_NewStringObj(*$1,mlen)) != TCL_OK) {
			delete *$1;
			delete $1;
			return TCL_ERROR;
		}
		delete *$1;
		delete $1;
		return TCL_ERROR;
	}
	delete $1;
}

/*
 * Type map for doubleVector: convert from a Tcl list of doubles on input.
 */

/*
 * On input: allocate a doubleVector and fill it from the input list argument.
 */
%typemap(tcl8,in) const doubleVector * {
	Tcl_Obj **objvPtr;
	int       objcPtr,i;
	double	  v;
	if (Tcl_ListObjGetElements(interp,$input,&objcPtr,&objvPtr) != TCL_OK)
		return(TCL_ERROR);
	$1 = new doubleVector;
#ifdef DEBUG
	cerr << "*** Passing list of " << objcPtr << " doubles as a doubleVector" << endl;
#endif
	for (i = 0; i < objcPtr; i++) {
		if (Tcl_GetDoubleFromObj(interp,objvPtr[i],&v) != TCL_OK) {
			if (strcmp(Tcl_GetString(objvPtr[i]),"-") != 0) {
				delete $1;
				return TCL_ERROR;
			}
			else v = 0.0;
		}
		$1->push_back(v);
	}
}

/*
 * Free allocated space.
 */

%typemap(tcl8,freearg) const doubleVector * {
	delete $1;
}

%typemap(tcl8,in) const doubleVector {
	Tcl_Obj **objvPtr;
	int       objcPtr,i;
	double	  v;
	if (Tcl_ListObjGetElements(interp,$input,&objcPtr,&objvPtr) != TCL_OK)
		return(TCL_ERROR);
#ifdef DEBUG
	cerr << "*** Passing list of " << objcPtr << " doubles as a doubleVector" << endl;
#endif
	for (i = 0; i < objcPtr; i++) {
		if (Tcl_GetDoubleFromObj(interp,objvPtr[i],&v) != TCL_OK) {
			if (strcmp(Tcl_GetString(objvPtr[i]),"-") != 0) {
				return TCL_ERROR;
			}
			else v = 0.0;
		}
		$1.push_back(v);
	}
}

/*
 * Type map for stringVector: convert from a Tcl list on input.
 */
 
/*
 * On input: allocate a stringVector and fill it from the input list argument.
 */
%typemap(tcl8,in) const stringVector * {
	Tcl_Obj **objvPtr;
	int       objcPtr,i;
	if (Tcl_ListObjGetElements(interp,$input,&objcPtr,&objvPtr) != TCL_OK)
		return(TCL_ERROR);
	$1 = new stringVector;
#ifdef DEBUG
	cerr << "*** Passing list of " << objcPtr << " strings as a stringVector" << endl;
#endif
	for (i = 0; i < objcPtr; i++) {
		$1->push_back(string(Tcl_GetString(objvPtr[i])));
	}
}

/*
 * Free allocated space.
 */

%typemap(tcl8,freearg) const stringVector * {
	delete $1;
}

%typemap(tcl8,in) const stringVector {
	Tcl_Obj **objvPtr;
	int       objcPtr,i;
	if (Tcl_ListObjGetElements(interp,$input,&objcPtr,&objvPtr) != TCL_OK)
		return(TCL_ERROR);
#ifdef  DEBUG
	cerr << "*** Passing list of " << objcPtr << " strings as a doubleVector" << endl;
#endif
	for (i = 0; i < objcPtr; i++) {
		$1.push_back(string(Tcl_GetString(objvPtr[i])));
	}
}

#endif
/** \TEX{\typeout{Generated from $Id$}}
  * This is the main Time Table Class.  It implements all of the basic data
  * and algorithms used in the Time Table program.
  *
  * This class includes code to load a set of stations and the trains that
  * run between these stations, along with code to read and write a time table
  * file and code to create a formatted time table, suitable for printing (by
  * way of \TEX{\LaTeX\ }).
  */
class TimeTableSystem {
#ifndef SWIG
protected:
	/** The default constructor.  This is protected to prevent the
	  * creation of an uninitialized class instance, which would be
	  * an error.  Making the default constructor a protected method
	  * will cause a compiler error when application code attempts to
	  * create a TimeTableSystem instance without using one of the
	  * parameterized constructor.  There is no meaningful way to
	  * create a proper TimeTableSystem without supplying some parameters.
	  */
	TimeTableSystem() {}
#endif
public:
#ifndef SWIG
	/** The constructor that creates a time table system from an existing
	  * file.  The file is read in and the class is properly initialized 
	  * from the data in the file.
	  *   @param filename The name of the file to load.
	  *   @param outmessage Pointer to a pointer to receive any error 
	  *     messages for any errors that might occur.
	  */
	TimeTableSystem(string filename,char **outmessage = NULL);
	/** The constructor that creates a new, empty time table system from
	  * stratch, given a set of esentual parameters.
	  *  @param name The name of the time table system.
	  *  @param timescale Number of time units per 24 hours.  There are
	  *	1440 minutes in 24 hours.
	  *  @param timeinterval The tick frequency in time units.
	  */
	TimeTableSystem(string name,int timescale,int timeinterval);
#endif
	/** Destructor. Properly clean up and free up all used space.
	  */
	~TimeTableSystem();
#ifdef SWIG
	int AddStation(const char *name,double smile);
#else
	/** Add a new station to the system. Creates a new Station class
	  * instance and adds it to the station vector.  Stations must be
	  * added in order of their scale mile location.  If the new station
	  * is out of order, -1 is returned and the station is not added!
	  *
	  *  @param name The name of the station.
	  *  @param smile The scale mile along the route where the station is
	  *	located.
	  */
	int AddStation(string name,double smile);
#endif
#ifdef SWIG
	int FindStationByName(const char *name);
#else
	/** Find a station by name.  Returns the index of the station or -1 if
	  * the station cannot be found.
	  @param name The name of the station.
	 */
	int FindStationByName(string name);
#endif
	/** Number of stations. Returns the number of stations in the system.
	  */
	int NumberOfStations() const {return stations.size();}
	/** Return Ith station object.  Returns the NULL pointer if the index
	  * is out of range.
	  *   @param i The index of the station.
	  */
	Station *IthStation(int i) {
		if (i < 0 || i >= stations.size()) return NULL;
		else return &(stations[i]);
	}
	/** Return the Ith station name. Returns the NULL pointer if the index
	  * is out of range.
	  *    @param i The index of the station.
	  */
	const char *StationName(int i) const {
		if (i < 0 || i >= stations.size()) return NULL;
		else return stations[i].Name();
	}
	/** Return the Ith station's scale mile location.  Returns -1.0 if
	  * the index is out of range.
	  *    @param i The index of the station.
	  */
	double SMile(int i) const {
		if (i < 0 || i >= stations.size()) return -1.0;
		else return stations[i].SMile();
	}
	/** The total length of the route in scale miles.  This is just the
	  * scale mile location of the last station along the route.
	  */
	double TotalLength() const {
		if (stations.size() == 0) return 0.0;
		else return stations[stations.size()-1].SMile();
	}
	/** The duplicate station index for a given station.  Only meaningful
	  * for out and back type layouts or layouts that have shared trackage.
	  * This would be stations along shared trackage.  Returns -1 if
	  * the index is out of range or if there is not duplicate station for
	  * the ith station.
	  *    @param i The index of the station.
	  */
	int DuplicateStationIndex(int i) const {
		if (i < 0 || i >= stations.size()) return -1;
		else return stations[i].DuplicateStationIndex();
	}
	/** Set the duplicate station index for a given station.  Only 
	  * meaningful for out and back type layouts or layouts that have 
	  * shared trackage. This would be stations along shared trackage.
	  * setting the duplicate station index indicates there is no
	  * duplicate station
	  *    @param i The index of the station to be updated.
	  *    @param dup The other station index sharing this station 
	  *      location.
	  */
	void SetDuplicateStationIndex(int i,int dup) {
		if (i < 0 || i >= stations.size()) return;
		else stations[i].SetDuplicateStationIndex(dup);
	}
#ifdef SWIG
	StorageTrack *AddStorageTrack(int i,const char *name);
#else
	/** Add a storage track to a station.  Sometimes stations, especially
	  * major terminals, have extra tracks for storing terminating and
	  * originating trains.  Returns the NULL pointer if the index is
	  * out of range.  Otherwise returns the pointer to the new 
	  * StorageTrack object.
	  *    @param i The index of the station to be updated.
	  *    @param name The name for the new storage track.
	  */
	StorageTrack *AddStorageTrack(int i,string name) {
		if (i < 0 || i >= stations.size()) return NULL;
		else return stations[i].AddStorageTrack(name);
	}
#endif
#ifdef SWIG
	StorageTrack *FindStorageTrack(int i,const char *name);
#else
	/** Find a storage track at a station.  Sometimes stations, especially
	  * major terminals, have extra tracks for storing terminating and
	  * originating trains. Returns the NULL pointer if the index is
	  * out of range or if there is no storage track with the specified
	  * name.  Otherwise the StorageTrack object pointer is returned.
	  *    @param i The index of the station to be updated.
	  *    @param name The name of the storage track.
	  */
	StorageTrack *FindStorageTrack(int i,string name) {
		if (i < 0 || i >= stations.size()) return NULL;
		else return stations[i].FindStorageTrack(name);
	}
#endif
#ifdef SWIG
	Cab *AddCab(const char *name, const char *color);
#else
	/** Add a new cab to the system.  With DC systems this would be an
	  * actual cab.  With DCC systems, this can be used to define a
	  * logical operator for the train.  The color is used for visual
	  * distintion.  A pointer to the new cab object is returned.
	  *   @param name The name of the cab.
	  *   @param color The color of the cab.
	  */
	Cab *AddCab(string name, string color);
#endif
	/** The nymber of cabs.
	  */
	int NumberOfCabs() const {return cabs.size();}
#ifdef SWIG
	Train *AddTrain(const char *name, const char *number, int speed,
			int classnumber,int departure,
			int start=0,int end=-1);
#else
	/** Add a train to the system, short version.  Creates a new Train
	  * opject and adds it to the train map.  The short version assumes
	  * that the train does not layover at any of the stops.  Layover
	  * times can be added later.  Returns a pointer to the new Train
	  * object.
	  *   @param name The name of the train.
	  *   @param number The number (or symbol) of the train.
	  *   @param speed The trains maximum speed.
	  *   @param classnumber The class (inverse priority) of the train.
	  *   @param start The train's origin station index.  Defaults to the
	  *	first station.
	  *   @param end The train's destination station index. Defaults to
	  *	the last station.
	  */
	Train *AddTrain(string name, string number, int speed, int classnumber,
			int departure,
			int start=0,int end=-1);
#endif
#ifdef SWIG
	Train *AddTrainLongVersion(const char *name, const char *number,
				   int speed, int classnumber,int departure,
				   int start,int end,
				   const doubleVector * layoverVector,
				   const stringVector * cabnameVector,
				   const stringVector * storageTrackVector,
				   char **outmessage = NULL);
				   
#else
	/** Add a train to the system, long version (includes storage track
	  * checking).  This version includes layover times, cabnames, and
	  * storage track assignments.  Returns a pointer to the new Train
	  * object or the NULL pointer if there was an error, in which case
	  * the error message will be stored in the pointer provided.
	  *  @param name The name of the train.
	  *  @param number The number (or symbol) of the train.
	  *  @param speed The trains maximum speed.
	  *  @param classnumber The class (inverse priority) of the train.
	  *  @param start The train's origin station index.
	  *  @param end The train's destination station index.
	  *  @param layoverVector The train's layover vector.
	  *  @param cabnameVector The train's departure cab name vector.
	  *  @param storageTrackVector The train's storage track name vector.
	  *  @param outmessage Pointer to a pointer to receive any error 
	  *	messages for any errors that might occur.
	  */
	Train *AddTrainLongVersion(string name, string number, int speed,
				   int classnumber,int departure,
				   int start,int end,
				   const doubleVector * layoverVector,
				   const stringVector * cabnameVector,
				   const stringVector * storageTrackVector,
				   char **outmessage = NULL);
#endif
#ifdef SWIG
	bool DeleteTrain(const char *number,char **outmessage=NULL);
#else
	/**
	  * Delete a train.  Returns true if the train was successfully deleted
	  * and false if not.  If the train was not deleted, an error message
	  * will be provided in the pointer provided.
	  *  @param number The train number or symbol.
	  *  @param outmessage Pointer to a pointer to receive any error messages
	  *  	for any errors that might occur.
	  */
	bool DeleteTrain(string number,char **outmessage=NULL);
#endif
#ifdef SWIG
	Cab *FindCab(const char *name) const;
#else
	/** Find a cab (by name).  Returns the pointer to the named cab or NULL
	  * if the cab was not found.
	  *   @param name The cab name to look for.
	  */
	Cab *FindCab(string name) const;
#endif
#ifdef SWIG
	Train *FindTrainByName(const char *name) const;
#else
	/** Find a train by name. Returns the pointer to the named train or
	  * NULL if the train was not found.
	  *   @param name The train name to look for.
	  */
	Train *FindTrainByName(string name) const;
#endif
#ifdef SWIG
	Train *FindTrainByNumber(const char *number) const;
#else
	/** Find a train by number (or symbol). Returns the pointer to the 
	  * train or NULL if the train was not found.
	  *   @param number The train number (or symbol) to look for.
	  */
	Train *FindTrainByNumber(string number) const;
#endif
	/** Return the number of trains.
	  */
	int NumberOfTrains() const {return trains.size();}
	/** Return the number of notes.
	  */
	int NumberOfNotes() const {return notes.size();}
	/** Return the ith note (1-based!) as a string.  Returns the NULL
	  * pointer if the index is out of range.
	  *   @param i The note index.  The first note is at index 1, not 0!.
	  */
	const char *Note(int i) {
		if (i <= 0 || i > notes.size()) return NULL;
		else return notes[i-1].c_str();
	}
#ifdef SWIG
	int AddNote(const char *newnote);
#else
	/** Add a note to the notes vector.
	  *   @param newnote The text of the new note.
	  */
	int AddNote(string newnote) {
		notes.push_back(newnote);
		return notes.size();
	}
#endif	
#ifdef SWIG
	bool SetNote(int i,const char *note);
#else
	/** Set the ith note (1-based!).  Updates the text of the specificed
	  * note.  Returns true if the note was updated or false if the
	  * index was out of range.
	  *   @param i The note index.  The first note is at index 1, not 0!.
	  *   @param note The new text for the note.
	  */
	bool SetNote(int i,string note) {
		if (i <= 0 || i > notes.size()) return false;
		else {
			notes[i-1] = note;
			return true;
		}
	}
#endif	
	/** Fetch a print option.  Returns the value of a specified print
	  * option or the empty string if the print option was not found.
	  *   @param key The name of the print option.
	  */
	const char *GetPrintOption(const char *key) const
	{
		OptionHashMap::const_iterator element = printOptions.find(key);
#ifdef DEBUG
		fprintf(stderr,"*** TimeTableSystem::GetPrintOption: element->first is '%s'.\n",(element->first));
#endif
		if (element == printOptions.end()) {
#ifdef DEBUG
		  fprintf(stderr,"*** TimeTableSystem::GetPrintOption: %s not in table, returning empty string.\n",key);
#endif
		  return "";
		}
		else {
#ifdef DEBUG
		  fprintf(stderr,"*** TimeTableSystem::GetPrintOption: %s => '%s'\n.",
		  	  key,(element->second).c_str());
#endif
		  return (element->second).c_str();
		}
	}
#ifdef SWIG
	void SetPrintOption(const char *key,const char *value);
#else
	/** Set a print option.  Sets the value of a print option.  Creates a
	  * new hash table element if the specified print option does not
	  * already exist.
	  *  @param key The name of the print option to be set.
	  *  @param value The value to set the print option to.
	  */
	void SetPrintOption(const char *key,string value)
	{
		OptionHashMap::iterator element = printOptions.find(key);
		if (element == printOptions.end()) {
#ifdef DEBUG
		  fprintf(stderr,"*** TimeTableSystem::SetPrintOption: %s is new.\n",key);
#endif
		  char *localKey = new char[strlen(key)+1];
		  strcpy(localKey,key);
		  printOptions[localKey] = value;
		} else {
#ifdef DEBUG
		  fprintf(stderr,"*** TimeTableSystem::SetPrintOption: %s is old, updating.\n",key);
#endif
		  element->second = value;
		}
	}
#endif
#ifdef SWIG
	bool WriteNewTimeTableFile(const char *filename = "",
				bool setfilename = false,
				char **outmessage = NULL);
#else
	/** Write out a Time Table System to a new file.  The current contents
	  * of the time table is written to a new time table file. Returns 
	  * true if successful and false if not.
	  *  @param filename The name of the file to write (if empty, use
	  *	existing name, if available).
	  *  @param setfilename Change the filename if true.
	  *  @param outmessage Pointer to a pointer to receive any error
	  *	messages for any errors that might occur.
	  */
	bool WriteNewTimeTableFile(string filename = "TimeTableFile.tt",
				bool setfilename = false,
				char **outmessage = NULL);
#endif
	/** Write an old time table file.  The current contents of the time
	  * table is written to the file name stored in the time table
	  * object.  This method just calls the WriteNewTimeTableFile
	  * method with the old file name. Returns true if successful and 
	  * false if not.
	  *  @param outmessage Pointer to a pointer to receive any error
	  *	messages for any errors that might occur.
	  */
	bool WriteOldTimeTableFile(char **outmessage = NULL) {
		return WriteNewTimeTableFile(filepath.FullPath(),false,
					     outmessage);
	}
	/** Return time scale.
	  */
	int TimeScale() const {return timescale;}
	/** Return time interval.
	  */
	int TimeInterval() const {return timeinterval;}
	/** Return the name of the system.
	  */
	const char * Name() const {return name.c_str();}
	/** Return file pathname.
	  */
	const char * Filename() const {return filepath.FullPath().c_str();}
#ifdef SWIG
	bool CreateLaTeXTimetable(const char *filename,
				  char **outmessage = NULL);
#else
	/** Create a \TEX{\LaTeX\ } file for generating a (hard copy) Employee
	  * Timetable. This method create a \TEX{\LaTeX\ } source file from
	  * the information in the time table structure.  It access various
	  * print options to control how the \TEX{\LaTeX\ } file is generated.
	  *  @param filename The name of the  \TEX{\LaTeX\ } file to create.
	  *  @param outmessage Pointer to a pointer to receive any error
	  *	messages for any errors that might occur.
	  */
	bool CreateLaTeXTimetable(string filename,char **outmessage = NULL);
#endif
#ifdef SWIG
	%extend {
		%apply int MyTcl_Result { int CabNameList };
		/** @memo Returns the list of cab names.
		  * @args none
		  * @type list
		  * Returns the list of cab names.  Only available to Tcl code.
		  * C++ code should iterate with FirstCab() and LastCab().
		  */
		int CabNameList (Tcl_Interp *interp) {
			CabNameMap::const_iterator Sx;
			string indx;
			Tcl_Obj *tcl_result = Tcl_NewListObj(0,NULL);
			for (Sx = self->FirstCab(); Sx != self->LastCab(); Sx++) {
				if (Sx->second == NULL) continue;
				indx = Sx->first;
				if (Tcl_ListObjAppendElement(interp,tcl_result,
								Tcl_NewStringObj(indx.c_str(),-1))
					!= TCL_OK) return TCL_ERROR;
			}
			Tcl_SetObjResult(interp,tcl_result);
			return TCL_OK;
		}
		%apply int MyTcl_Result { int TrainNumberList };
		/** @memo Returns the list of train numbers (symbols).
		  * @args none
		  * @type list
		  * Returns the list of train numbers (symbols). Only 
		  * available to Tcl code. C++ code should iterate with 
		  * FirstTrain() and LastTrain().
		  */
		int TrainNumberList (Tcl_Interp *interp) {
			TrainNumberMap::const_iterator Sx;
			string indx;
			Tcl_Obj *tcl_result = Tcl_NewListObj(0,NULL);
			for (Sx = self->FirstTrain(); Sx != self->LastTrain(); Sx++) {
				if (Sx->second == NULL) continue;
				indx = Sx->first;
				if (Tcl_ListObjAppendElement(interp,tcl_result,
								Tcl_NewStringObj(indx.c_str(),-1))
					!= TCL_OK) return TCL_ERROR;
			}
			Tcl_SetObjResult(interp,tcl_result);
			return TCL_OK;
		}
	}
#else
	/** First cab. Return a const iterator for the first cab.
	  */
	CabNameMap::const_iterator FirstCab() const {
		return cabs.begin();
	}
	/** Last cab. Return a const iterator for the last cab.
	  */
	CabNameMap::const_iterator LastCab() const {
		return cabs.end();
	}
	/** First train. Return a const iterator for the first train.
	  */
	TrainNumberMap::const_iterator FirstTrain() const {
		return trains.begin();
	}
	/** Last train. Return a const iterator for the last train.
	  */
	TrainNumberMap::const_iterator LastTrain() const {
		return trains.end();
	}
	/** First Print option. Return a const iterator for the first print
	  * option.
	  */
	OptionHashMap::const_iterator FirstPrintOption() const {
		return printOptions.begin();
	}
	/** Last Print option. Return a const iterator for the last print
	  * option.
	  */
	OptionHashMap::const_iterator LastPrintOption() const {
		return printOptions.end();
	}
private:
	/** Read in a note. Reads a note from a stream.  Returns the note text.
	  *   @param in Stream to read from.
	  */  
	string ReadNote(istream &in) const;
	/** Write out a note.  Writes the note text to a stream. Returns the
	  * stream.
	  *   @param out Stream to write to.
	  *   @oaram note The note text.
	  */
	ostream & WriteNote(ostream &out,string note) const;
	/** Make a time table grouped by class. Writes a time table
	  * \TEX{\LaTeX\ } file grouped by train class.  Each class will
	  * have its own table in its own section.  Returns true if
	  * successfull and false if there were problems (errors).
	  *   @param out The \TEX{\LaTeX\ } output stream.
	  *   @param allTrains A list of all of the trains.
	  *   @param forwardTrains A list of all forward moving trains.
	  *   @param backwardTrains A list of all backward moving trains.
	  *   @param outmessage Pointer to a pointer to receive any error
	  *     messages for any errors that might occur.
	  */
	bool MakeTimeTableGroupByClass(ostream &out,TrainList &allTrains,
				       TrainList &forwardTrains,
				       TrainList &backwardTrains,
				       char **outmessage = NULL);
	/** Make a time table grouped manually. Writes a time table
	  * \TEX{\LaTeX\ } file grouped manually. Returns true if
	  * successfull and false if there were problems (errors).
	  *   @param out The \TEX{\LaTeX\ } output stream.
	  *   @param maxTrains The maximum number of trains per table.
	  *   @param allTrains A list of all of the trains.
	  *   @param forwardTrains A list of all forward moving trains.
	  *   @param backwardTrains A list of all backward moving trains.
	  *   @param outmessage Pointer to a pointer to receive any error
	  *     messages for any errors that might occur.
	  */
	bool MakeTimeTableGroupManually(ostream &out,int maxTrains,
					TrainList &allTrains,
				        TrainList &forwardTrains,
				        TrainList &backwardTrains,
					char **outmessage = NULL);
	/** Make a time table as a single table. Writes a time table
	  * \TEX{\LaTeX\ } file with all trains in a single table.  This
	  * only makes sense if the total number of trains is small.
	  *   @param out The \TEX{\LaTeX\ } output stream.
	  *   @param allTrains A list of all of the trains.
	  *   @param forwardTrains A list of all forward moving trains.
	  *   @param backwardTrains A list of all backward moving trains.
	  *   @param header String (\TEX{\LaTeX\ } code) to use for the time 
	  *	table header.
	  *   @param sectionTOP String (\TEX{\LaTeX\ } code) to use for the 
	  *	section start.
	  *   @param outmessage Pointer to a pointer to receive any error
	  *     messages for any errors that might occur.
	  */
	bool MakeTimeTableOneTable(ostream &out,TrainList &allTrains,
				   TrainList &forwardTrains,
				   TrainList &backwardTrains,
				   string header,string sectionTOP,
				   char **outmessage = NULL);
	/** Make a time table as a single table, with the stations on the
	  * left (single direction trains).
	  *   @param out The \TEX{\LaTeX\ } output stream.
	  *   @param trains A list of the trains.
	  *   @param header String (\TEX{\LaTeX\ } code) to use for the time 
	  *	table header.
	  *   @param sectionTOP String (\TEX{\LaTeX\ } code) to use for the 
	  *	section start.
	  *   @param outmessage Pointer to a pointer to receive any error
	  *     messages for any errors that might occur.
	  */
	bool MakeTimeTableOneTableStationsLeft(ostream &out,TrainList &trains,
					       string header,string sectionTOP,
					       char **outmessage = NULL);
	/** Make a time table as a single table, with the stations in the
	  *  center (bi-directional trains).
	  *   @param out The \TEX{\LaTeX\ } output stream.
	  *   @param forwardTrains A list of all forward moving trains.
	  *   @param backwardTrains A list of all backward moving trains.
	  *   @param header String (\TEX{\LaTeX\ } code) to use for the time 
	  *	table header.
	  *   @param sectionTOP String (\TEX{\LaTeX\ } code) to use for the 
	  *	section start.
	  *   @param outmessage Pointer to a pointer to receive any error
	  *     messages for any errors that might occur.
          */
	bool MakeTimeTableOneTableStationsCenter(ostream &out,
						 TrainList &forwardTrains,
						 TrainList &backwardTrains,
						 string header,
						 string sectionTOP,
						 char **outmessage = NULL);
	/** Precompute station times, given a list of trains. This helper
	  * function creates the table cell information for a time table.
	  *   @param timesAtStations The time table matrix to be filled in.
	  *   @param trains A list of trains to process.
	  */
	void ComputeTimes(TrainTimesAtStation &timesAtStations,
			  TrainList &trains);
	/** The name of the time table system.
	  */
	string name;
	/** The pathname of the file the system was loaded from.
	  */
	PathName filepath;
	/** Time scale.
	  */
	int timescale;
	/** Time interval.
	  */
	int timeinterval;
	/** Station stop vector.
	  */
	StationVector stations;
	/** Cap name map.
	  */
	CabNameMap cabs;
	/** Train number/symbol map.
	  */
	TrainNumberMap trains;
	/** Notes.
	  */
	vector<string> notes;
	/** Print option hash table.
	  */
	OptionHashMap printOptions;
	/** Table Of Contents?	Used by print functions.
	  */
	bool TOCP;
	/** Direction Name.  Used by print functions.
	  */
	string DirectionName;
#endif	
};

#ifdef SWIG

/** @name Tcl Helper functions.
  * @doc These are top level Tcl support functions for the TimeTableSystem
  * class.  They are only available from Tcl, C++ programs have other API
  * functions, including overloaded constructors and iterator methods.
  */

//@{

/** @memo Tcl constructor to create a new TimeTable.
  * @args name timescale timeinterval
  * @type TimeTableSystem
  * Tcl constructor to create a new TimeTable.  Calls the new time table 
  * constructor.
  *  @param name The name of the time table system.
  *  @param timescale Number of time units per 24 hours.  There are
  *	1440 minutes in 24 hours.
  *  @param timeinterval The tick frequency in time units.
  */

TimeTableSystem *NewCreateTimeTable(const char *name,int timescale,int timeinterval);
/** @memo Tcl constructor to create a time table system from an existing file.
  * @args filename
  * @type TimeTableSystem
  * Tcl constructor to create a time table system from an existing file. The 
  * file is read in and the class is properly initialized 
  * from the data in the file.
  *   @param filename The name of the file to load. 
  */
TimeTableSystem *OldCreateTimeTable(const char *filename,char **outmessage = NULL);


%{
static TimeTableSystem *NewCreateTimeTable(const char *name,int timescale,int timeinterval)
{
	return new TimeTableSystem(name,timescale,timeinterval);
}
static TimeTableSystem *OldCreateTimeTable(const char *filename,char **outmessage = NULL)
{
	return new TimeTableSystem(filename,outmessage);
}
%}



%apply int MyTcl_Result { int ForEveryStation };
%apply int MyTcl_Result { int ForEveryCab };
%apply int MyTcl_Result { int ForEveryTrain };
%apply int MyTcl_Result { int ForEveryNote };
%apply int MyTcl_Result { int ForEveryPrintOption };

/** @memo Tcl looping construct for Stations.
  * @args timetable variable body
  * @type empty string
  * Tcl looping construct that loops over the stations in timetable, setting 
  * variable to the Station pointer and evaluates body.
  *  @param timetable The time table object.
  *  @param variable The loop variable.
  *  @param body The body script.
  */
int ForEveryStation(Tcl_Interp *interp,TimeTableSystem *timetable,Tcl_Obj *variableName,Tcl_Obj *bodyPtr);
/** @memo Tcl looping construct for Cabs.
  * @args timetable variable body
  * @type empty string
  * Tcl looping construct that loops over the cabs in timetable, setting 
  * variable to the Cab pointer and evaluates body.
  *  @param timetable The time table object.
  *  @param variable The loop variable.
  *  @param body The body script.
  */
int ForEveryCab(Tcl_Interp *interp,TimeTableSystem *timetable,Tcl_Obj *variableName,Tcl_Obj *bodyPtr);
/** @memo Tcl looping construct for Trains.
  * @args timetable variable body
  * @type empty string
  * Tcl looping construct that loops over the stations in timetable, setting 
  * variable to the Train pointer and evaluates body.
  *  @param timetable The time table object.
  *  @param variable The loop variable.
  *  @param body The body script.
  */
int ForEveryTrain(Tcl_Interp *interp,TimeTableSystem *timetable,Tcl_Obj *variableName,Tcl_Obj *bodyPtr);
/** @memo Tcl looping construct for notes.
  * @args timetable variable body
  * @type empty string
  * Tcl looping construct that loops over the notes in timetable, setting 
  * variable to the note string and evaluates body.
  *  @param timetable The time table object.
  *  @param variable The loop variable.
  *  @param body The body script.
  */
int ForEveryNote(Tcl_Interp *interp,TimeTableSystem *timetable,Tcl_Obj *variableName,Tcl_Obj *bodyPtr);
/** @memo Tcl looping construct for print options.
  * @args timetable variable body
  * @type empty string
  * Tcl looping construct that loops over the stations in timetable, setting 
  * variable to the print option key and evaluates body.
  *  @param timetable The time table object.
  *  @param variable The loop variable.
  *  @param body The body script.
  */
int ForEveryPrintOption(Tcl_Interp *interp,TimeTableSystem *timetable,Tcl_Obj *variableName,Tcl_Obj *bodyPtr);

//@}

%{
	
static int ForEveryStation(Tcl_Interp *interp,TimeTableSystem *timetable,Tcl_Obj *variableName,Tcl_Obj *bodyPtr)
{
	int result = TCL_OK;
	int istation;

	for (istation = 0; istation < timetable->NumberOfStations(); istation++) {
	  Tcl_Obj *valuePtr, *varValuePtr;
	  valuePtr = SWIG_NewInstanceObj((void *) timetable->IthStation(istation),
					SWIGTYPE_p_Station,0);
	  varValuePtr = Tcl_ObjSetVar2(interp,variableName,NULL,valuePtr,0);
	  if (varValuePtr == NULL) {
	    Tcl_DecrRefCount(valuePtr);
	    Tcl_ResetResult(interp);
	    Tcl_AppendStringsToObj(Tcl_GetObjResult(interp),
	    	"couldn't set loop variable: \"",
		Tcl_GetString(variableName),"\"", (char *) NULL);
	    result = TCL_ERROR;
	    break;
	  }
	  result = Tcl_EvalObjEx(interp, bodyPtr, 0);
	  if (result != TCL_OK) {
	    if (result == TCL_CONTINUE) {
	      result = TCL_OK;
	    } else if (result == TCL_BREAK) {
	      result = TCL_OK;
	      break;
	    } else if (result == TCL_ERROR) {
	      char msg[64 + TCL_INTEGER_SPACE];
	      sprintf(msg, "\n    (\"ForEveryStation\" body line %d)",
	      		interp->errorLine);
	      Tcl_AddObjErrorInfo(interp, msg, -1);
	      break;
	    } else {
	      break;
	    }
	  }
	  
	}
	if (result == TCL_OK) {
	  Tcl_ResetResult(interp);
	}
	return result;
}

static int ForEveryCab(Tcl_Interp *interp,TimeTableSystem *timetable,Tcl_Obj *variableName,Tcl_Obj *bodyPtr)
{
	int result = TCL_OK;
	CabNameMap::const_iterator Cx;

	for (Cx = timetable->FirstCab();Cx != timetable->LastCab(); Cx++) {
	  Tcl_Obj *valuePtr, *varValuePtr;
	  valuePtr = SWIG_NewInstanceObj((void *) Cx->second, SWIGTYPE_p_Cab,0);
	  varValuePtr = Tcl_ObjSetVar2(interp,variableName,NULL,valuePtr,0);
	  if (varValuePtr == NULL) {
	    Tcl_DecrRefCount(valuePtr);
	    Tcl_ResetResult(interp);
	    Tcl_AppendStringsToObj(Tcl_GetObjResult(interp),
	    	"couldn't set loop variable: \"",
		Tcl_GetString(variableName),"\"", (char *) NULL);
	    result = TCL_ERROR;
	    break;
	  }
	  result = Tcl_EvalObjEx(interp, bodyPtr, 0);
	  if (result != TCL_OK) {
	    if (result == TCL_CONTINUE) {
	      result = TCL_OK;
	    } else if (result == TCL_BREAK) {
	      result = TCL_OK;
	      break;
	    } else if (result == TCL_ERROR) {
	      char msg[64 + TCL_INTEGER_SPACE];
	      sprintf(msg, "\n    (\"ForEveryCab\" body line %d)",
	      		interp->errorLine);
	      Tcl_AddObjErrorInfo(interp, msg, -1);
	      break;
	    } else {
	      break;
	    }
	  }
	  
	}
	if (result == TCL_OK) {
	  Tcl_ResetResult(interp);
	}
	return result;
}

static int ForEveryTrain(Tcl_Interp *interp,TimeTableSystem *timetable,Tcl_Obj *variableName,Tcl_Obj *bodyPtr)
{
	int result = TCL_OK;
	TrainNumberMap::const_iterator Tx;

	for (Tx = timetable->FirstTrain();Tx != timetable->LastTrain(); Tx++) {
	  Tcl_Obj *valuePtr, *varValuePtr;
	  valuePtr = SWIG_NewInstanceObj((void *) Tx->second, SWIGTYPE_p_Train,0);
	  varValuePtr = Tcl_ObjSetVar2(interp,variableName,NULL,valuePtr,0);
	  if (varValuePtr == NULL) {
	    Tcl_DecrRefCount(valuePtr);
	    Tcl_ResetResult(interp);
	    Tcl_AppendStringsToObj(Tcl_GetObjResult(interp),
	    	"couldn't set loop variable: \"",
		Tcl_GetString(variableName),"\"", (char *) NULL);
	    result = TCL_ERROR;
	    break;
	  }
	  result = Tcl_EvalObjEx(interp, bodyPtr, 0);
	  if (result != TCL_OK) {
	    if (result == TCL_CONTINUE) {
	      result = TCL_OK;
	    } else if (result == TCL_BREAK) {
	      result = TCL_OK;
	      break;
	    } else if (result == TCL_ERROR) {
	      char msg[64 + TCL_INTEGER_SPACE];
	      sprintf(msg, "\n    (\"ForEveryTrain\" body line %d)",
	      		interp->errorLine);
	      Tcl_AddObjErrorInfo(interp, msg, -1);
	      break;
	    } else {
	      break;
	    }
	  }
	  
	}
	if (result == TCL_OK) {
	  Tcl_ResetResult(interp);
	}
	return result;
}

static int ForEveryNote(Tcl_Interp *interp,TimeTableSystem *timetable,Tcl_Obj *variableName,Tcl_Obj *bodyPtr)
{
	int result = TCL_OK;
	int inote;

	for (inote = 1; inote <= timetable->NumberOfNotes(); inote++) {
	  Tcl_Obj *valuePtr, *varValuePtr;
	  valuePtr = Tcl_NewStringObj(timetable->Note(inote),-1);
	  varValuePtr = Tcl_ObjSetVar2(interp,variableName,NULL,valuePtr,0);
	  if (varValuePtr == NULL) {
	    Tcl_DecrRefCount(valuePtr);
	    Tcl_ResetResult(interp);
	    Tcl_AppendStringsToObj(Tcl_GetObjResult(interp),
	    	"couldn't set loop variable: \"",
		Tcl_GetString(variableName),"\"", (char *) NULL);
	    result = TCL_ERROR;
	    break;
	  }
	  result = Tcl_EvalObjEx(interp, bodyPtr, 0);
	  if (result != TCL_OK) {
	    if (result == TCL_CONTINUE) {
	      result = TCL_OK;
	    } else if (result == TCL_BREAK) {
	      result = TCL_OK;
	      break;
	    } else if (result == TCL_ERROR) {
	      char msg[64 + TCL_INTEGER_SPACE];
	      sprintf(msg, "\n    (\"ForEveryNote\" body line %d)",
	      		interp->errorLine);
	      Tcl_AddObjErrorInfo(interp, msg, -1);
	      break;
	    } else {
	      break;
	    }
	  }
	  
	}
	if (result == TCL_OK) {
	  Tcl_ResetResult(interp);
	}
	return result;
}

static int ForEveryPrintOption(Tcl_Interp *interp,TimeTableSystem *timetable,
			Tcl_Obj *variableName,Tcl_Obj *bodyPtr)
{
	int result = TCL_OK;
	OptionHashMap::const_iterator Ox;

	for (Ox = timetable->FirstPrintOption();Ox != timetable->LastPrintOption(); Ox++) {
#ifdef DEBUG
	  cerr << "*** ForEveryPrintOption: Ox->first = '" << Ox->first << "', Ox->second = '" << Ox->second << "'" << endl;
#endif
	  Tcl_Obj *valuePtr, *varValuePtr;
	  valuePtr = Tcl_NewStringObj(Ox->first,-1);
	  varValuePtr = Tcl_ObjSetVar2(interp,variableName,NULL,valuePtr,0);
	  if (varValuePtr == NULL) {
	    Tcl_DecrRefCount(valuePtr);
	    Tcl_ResetResult(interp);
	    Tcl_AppendStringsToObj(Tcl_GetObjResult(interp),
	    	"couldn't set loop variable: \"",
		Tcl_GetString(variableName),"\"", (char *) NULL);
	    result = TCL_ERROR;
	    break;
	  }
	  result = Tcl_EvalObjEx(interp, bodyPtr, 0);
	  if (result != TCL_OK) {
	    if (result == TCL_CONTINUE) {
	      result = TCL_OK;
	    } else if (result == TCL_BREAK) {
	      result = TCL_OK;
	      break;
	    } else if (result == TCL_ERROR) {
	      char msg[64 + TCL_INTEGER_SPACE];
	      sprintf(msg, "\n    (\"ForEveryPrintOption\" body line %d)",
	      		interp->errorLine);
	      Tcl_AddObjErrorInfo(interp, msg, -1);
	      break;
	    } else {
	      break;
	    }
	  }
	  
	}
	if (result == TCL_OK) {
	  Tcl_ResetResult(interp);
	}
	return result;
}

%}

%apply int MyTcl_Result { int TT_ListToStringListString };

int TT_ListToStringListString(Tcl_Interp *interp,Tcl_Obj *list);

%{

static int TT_ListToStringListString(Tcl_Interp *interp,Tcl_Obj *list)
{
	int objc,iobj;
	Tcl_Obj **objv;
	StringList sl;
	int status = Tcl_ListObjGetElements(interp,list,&objc,&objv);
	if (status != TCL_OK) return status;
	for (iobj = 0; iobj < objc; iobj++) {
	  sl.push_back(Tcl_GetStringFromObj(objv[iobj],NULL));
	}
	string result = StringListToString(sl);
	Tcl_Obj *resultObj = Tcl_NewStringObj(result.c_str(),-1);
	Tcl_SetObjResult(interp,resultObj);
	return TCL_OK;	
}

%}

%apply int MyTcl_Result { int TT_StringListToList }

int TT_StringListToList(Tcl_Interp *interp,const char *stringList);

%{
static int TT_StringListToList(Tcl_Interp *interp,const char *stringList)
{
	StringList slist;
	StringList::const_iterator Sx;
	if (StringListFromString(stringList,slist)) {
	  Tcl_Obj *tcl_result = Tcl_NewListObj(0,NULL);
	  for (Sx = slist.begin(); Sx != slist.end(); Sx++) {
	    if (Tcl_ListObjAppendElement(interp,tcl_result,
	    				 Tcl_NewStringObj((*Sx).c_str(),-1))
	    	  != TCL_OK) return TCL_ERROR;
	  }
	  Tcl_SetObjResult(interp,tcl_result);
	  return TCL_OK;
	} else {
	  Tcl_ResetResult(interp);
	  Tcl_AppendStringsToObj(Tcl_GetObjResult(interp),
				 "Syntax error in string List: ",
				 stringList,NULL);
	  return TCL_ERROR;
	}
}
%}

#endif

//@}
	                    
#endif // _TIMETABLESYSTEM_H_

