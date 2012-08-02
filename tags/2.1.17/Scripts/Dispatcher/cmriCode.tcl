#* 
#* ------------------------------------------------------------------
#* cmriCode.tcl - CM/RI Library
#* Created by Robert Heller on Sun Apr 13 18:02:03 2008
#* ------------------------------------------------------------------
#* Modification History: $Log$
#* Modification History: Revision 1.1  2002/07/28 14:03:50  heller
#* Modification History: Add it copyright notice headers
#* Modification History:
#* ------------------------------------------------------------------
#* Contents:
#* ------------------------------------------------------------------
#*  
#*     Model RR System, Version 2
#*     Copyright (C) 1994,1995,2002-2005  Robert Heller D/B/A Deepwoods Software
#* 			51 Locke Hill Road
#* 			Wendell, MA 01379-9728
#* 
#*     This program is free software; you can redistribute it and/or modify
#*     it under the terms of the GNU General Public License as published by
#*     the Free Software Foundation; either version 2 of the License, or
#*     (at your option) any later version.
#* 
#*     This program is distributed in the hope that it will be useful,
#*     but WITHOUT ANY WARRANTY; without even the implied warranty of
#*     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#*     GNU General Public License for more details.
#* 
#*     You should have received a copy of the GNU General Public License
#*     along with this program; if not, write to the Free Software
#*     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#* 
#*  
#* 

# $Id$

package require Cmri;#          Load the CMR/I package.
package require grsupport 2.0

# Snit type to implement a CMR/I node. 
snit::type CMriNode {
  typecomponent CMriBusObject;#		CMR/I I/O object
  typeconstructor {set CMriBusObject {}}
  typemethod open {port speed retries} {
    set CMriBusObject [::new_CMri $port $speed $retries]
  }
  typemethod close {} {
    if {[string length "$CMriBusObject"] > 0} {$CMriBusObject -delete}
    set CMriBusObject {}
  }
  # Define the integer verify method.
  GRSupport::VerifyIntegerMethod
  # options used by CMR/I nodes.
  option -ns -default 0 -readonly yes \
	 -validatemethod _VerifyInteger
  option -ni -default 0 -readonly yes \
	 -validatemethod _VerifyInteger
  option -no -default 0 -readonly yes \
	 -validatemethod _VerifyInteger
  option -dl -default 0 -readonly yes \
	 -validatemethod _VerifyInteger
  option -ct -default {} -readonly yes
  variable UA;#				This node's address
    
  constructor {_UA NodeType args} {
    set UA $_UA;#			Stash our address
    # Determine type of node.
    switch -exact -- $NodeType {
      SUSIC {set ctype X}
      USIC  {set ctype N}
      SMINI {set ctype M
	     set options(-ni) 3
	     set options(-no) 6
	     set options(-dl) 0}
      default {
	error "Unknown Node Type: $NodeType"
      }
    }
    # Initialize our board.
    $CMriBusObject InitBoard $options(-ct) $options(-ni) \
			       $options(-no) $options(-ns) \
			       $UA $ctype $options(-dl)
  }
  # Input method -- poll our input ports
  method inputs {} {
    return [$CMriBusObject Inputs $UA]
  }
  # Output method -- write to our output ports
  method outputs {args} {
    return [$CMriBusObject Outputs $args $UA]
  }
}


  

