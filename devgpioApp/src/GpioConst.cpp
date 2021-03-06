//******************************************************************************
// Copyright (C) 2015 Florian Feldbauer <feldbaue@kph.uni-mainz.de>
//                    - Helmholtz-Institut Mainz
//
// This file is part of devGpio
//
// devGpio is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// devGpio is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// version 1.0.0; Aug 13, 2015
//
//******************************************************************************

//! @file GpioConst.cpp
//! @author F.Feldbauer
//! @date 13 Aug 2015
//! @brief Implementation look-up table get GPIOs by name or key

//_____ I N C L U D E S ________________________________________________________

// ANSI C/C++ includes
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>

// EPICS includes

// local includes
#include "GpioConst.h"
#include "devGpioErrors.h"

//_____ D E F I N I T I O N S __________________________________________________

//_____ G L O B A L S __________________________________________________________
GpioConst *GpioConst::_pinstance = NULL;

//_____ L O C A L S ____________________________________________________________
static const char ERR_BEGIN[] = "\033[31;1m";
static const char ERR_END[] = "\033[0m\n";

//_____ F U N C T I O N S ______________________________________________________

//------------------------------------------------------------------------------
//! @brief   Standard Constructor
//------------------------------------------------------------------------------
GpioConst::GpioConst( BOARD board )
 : _selection( board )
{
  switch( board ) {
    case RASPI_B_REV2:     init_raspi_b_rev2();    break;
    case RASPI_BP:         init_raspi_bp();        break;
    case BEAGLEBONE_BLACK: init_beagleboneblack(); break;
  }
}

//------------------------------------------------------------------------------
//! @brief   Standard Destructor
//------------------------------------------------------------------------------
GpioConst::~GpioConst(){
  _gpioByKey.clear();
  _gpioByName.clear();
  _gpioByNumber.clear();
}

//------------------------------------------------------------------------------
//! @brief   Check if instance already exists
//------------------------------------------------------------------------------
bool GpioConst::exists(){
  if( _pinstance ) return true;
  return false;
}

//------------------------------------------------------------------------------
//! @brief   Get Instance
//------------------------------------------------------------------------------
GpioConst* GpioConst::instance(){
  if( !_pinstance ) throw GpioConstError( "GpioConst::instance: No instance was created" );
  return _pinstance;
}

//------------------------------------------------------------------------------
//! @brief   Create instance
//------------------------------------------------------------------------------
void GpioConst::create( BOARD board ) {
  if( _pinstance ) {
    std::cerr << "GpioConst::create: Instance already created" << std::endl;
    return;
  }
  _pinstance = new GpioConst( board );
}

//------------------------------------------------------------------------------
//! @brief   Get GPIO number from name or key
//------------------------------------------------------------------------------
epicsUInt32 GpioConst::findGPIO( std::string const& keyOrName ) {
  std::map< std::string, epicsUInt32 >::const_iterator it;

  // first try to find GPIO by key
  it = _gpioByKey.find( keyOrName );
  if( it != _gpioByKey.end() ) return it->second;

  // if not found try to find by name
  it = _gpioByName.find( keyOrName );
  if( it != _gpioByName.end() ) return it->second;

  // if still not found, interpret keyOrName as GPIO number
  it = _gpioByNumber.find( keyOrName );
  if( it != _gpioByNumber.end() ) return it->second;

  // if we end here, keyOrName is invalid
  std::stringstream errmsg;
  errmsg << ERR_BEGIN
         << "GpioConst::findGPIO: Error: Cannot find GPIO " << keyOrName
         << ERR_END;
  throw GpioConstError( errmsg.str() );

}

//------------------------------------------------------------------------------
//! @brief   Init maps for Raspberry Pi B Revision 2
//------------------------------------------------------------------------------
void GpioConst::init_raspi_b_rev2() {
  std::cout << "GpioConst: Loading Look-Up table for Raspberry Pi B Rev2" << std::endl;
  // Key list
  _gpioByKey.insert( std::make_pair( "P1-08", 14 ) );
  _gpioByKey.insert( std::make_pair( "P1-10", 15 ) );
  _gpioByKey.insert( std::make_pair( "P1-12", 18 ) );
  _gpioByKey.insert( std::make_pair( "P1-16", 23 ) );
  _gpioByKey.insert( std::make_pair( "P1-18", 24 ) );
  _gpioByKey.insert( std::make_pair( "P1-22", 25 ) );
  _gpioByKey.insert( std::make_pair( "P1-24", 8 ) );
  _gpioByKey.insert( std::make_pair( "P1-26", 7 ) );
  _gpioByKey.insert( std::make_pair( "P1-03", 2 ) );
  _gpioByKey.insert( std::make_pair( "P1-05", 3 ) );
  _gpioByKey.insert( std::make_pair( "P1-07", 4 ) );
  _gpioByKey.insert( std::make_pair( "P1-11", 17 ) );
  _gpioByKey.insert( std::make_pair( "P1-13", 27 ) );
  _gpioByKey.insert( std::make_pair( "P1-15", 22 ) );
  _gpioByKey.insert( std::make_pair( "P1-19", 10 ) );
  _gpioByKey.insert( std::make_pair( "P1-21", 9 ) );
  _gpioByKey.insert( std::make_pair( "P1-23", 11 ) );
  _gpioByKey.insert( std::make_pair( "P5-03", 28 ) );
  _gpioByKey.insert( std::make_pair( "P5-05", 30 ) );
  _gpioByKey.insert( std::make_pair( "P5-04", 29 ) );
  _gpioByKey.insert( std::make_pair( "P5-06", 31 ) );

  // Name list
  _gpioByName.insert( std::make_pair( "GPIO14", 14 ) );
  _gpioByName.insert( std::make_pair( "GPIO15", 15 ) );
  _gpioByName.insert( std::make_pair( "GPIO18", 18 ) );
  _gpioByName.insert( std::make_pair( "GPIO23", 23 ) );
  _gpioByName.insert( std::make_pair( "GPIO24", 24  ) );
  _gpioByName.insert( std::make_pair( "GPIO25", 25 ) );
  _gpioByName.insert( std::make_pair( "GPIO08", 8 ) );
  _gpioByName.insert( std::make_pair( "GPIO07", 7 ) );
  _gpioByName.insert( std::make_pair( "GPIO2", 2  ) );
  _gpioByName.insert( std::make_pair( "GPIO3", 3 ) );
  _gpioByName.insert( std::make_pair( "GPIO4", 4 ) );
  _gpioByName.insert( std::make_pair( "GPIO17", 17 ) );
  _gpioByName.insert( std::make_pair( "GPIO27", 27 ) );
  _gpioByName.insert( std::make_pair( "GPIO22", 22 ) );
  _gpioByName.insert( std::make_pair( "GPIO10", 10 ) );
  _gpioByName.insert( std::make_pair( "GPIO9", 9 ) );
  _gpioByName.insert( std::make_pair( "GPIO11", 11 ) );
  _gpioByName.insert( std::make_pair( "GPIO28", 28 ) );
  _gpioByName.insert( std::make_pair( "GPIO30", 30 ) );
  _gpioByName.insert( std::make_pair( "GPIO29", 29 ) );
  _gpioByName.insert( std::make_pair( "GPIO31", 31 ) );

  // Number list
  _gpioByNumber.insert( std::make_pair( "14", 14 ) );
  _gpioByNumber.insert( std::make_pair( "15", 15 ) );
  _gpioByNumber.insert( std::make_pair( "18", 18 ) );
  _gpioByNumber.insert( std::make_pair( "23", 23 ) );
  _gpioByNumber.insert( std::make_pair( "24", 24  ) );
  _gpioByNumber.insert( std::make_pair( "25", 25 ) );
  _gpioByNumber.insert( std::make_pair( "08", 8 ) );
  _gpioByNumber.insert( std::make_pair( "07", 7 ) );
  _gpioByNumber.insert( std::make_pair( "2", 2  ) );
  _gpioByNumber.insert( std::make_pair( "3", 3 ) );
  _gpioByNumber.insert( std::make_pair( "4", 4 ) );
  _gpioByNumber.insert( std::make_pair( "17", 17 ) );
  _gpioByNumber.insert( std::make_pair( "27", 27 ) );
  _gpioByNumber.insert( std::make_pair( "22", 22 ) );
  _gpioByNumber.insert( std::make_pair( "10", 10 ) );
  _gpioByNumber.insert( std::make_pair( "9", 9 ) );
  _gpioByNumber.insert( std::make_pair( "11", 11 ) );
  _gpioByNumber.insert( std::make_pair( "28", 28 ) );
  _gpioByNumber.insert( std::make_pair( "30", 30 ) );
  _gpioByNumber.insert( std::make_pair( "29", 29 ) );
  _gpioByNumber.insert( std::make_pair( "31", 31 ) );

}

//------------------------------------------------------------------------------
//! @brief   Init maps for Raspberry Pi B+ and Raspberry Pi 2 B
//------------------------------------------------------------------------------
void GpioConst::init_raspi_bp() {
  std::cout << "GpioConst: Loading Look-Up table for Raspberry Pi B+" << std::endl;

  // Key list
  _gpioByKey.insert( std::make_pair( "J8-08", 14 ) );
  _gpioByKey.insert( std::make_pair( "J8-10", 15 ) );
  _gpioByKey.insert( std::make_pair( "J8-12", 18 ) );
  _gpioByKey.insert( std::make_pair( "J8-16", 23 ) );
  _gpioByKey.insert( std::make_pair( "J8-18", 24 ) );
  _gpioByKey.insert( std::make_pair( "J8-22", 25 ) );
  _gpioByKey.insert( std::make_pair( "J8-24", 8 ) );
  _gpioByKey.insert( std::make_pair( "J8-26", 7 ) );
  _gpioByKey.insert( std::make_pair( "J8-03", 2 ) );
  _gpioByKey.insert( std::make_pair( "J8-05", 3 ) );
  _gpioByKey.insert( std::make_pair( "J8-07", 4 ) );
  _gpioByKey.insert( std::make_pair( "J8-11", 17 ) );
  _gpioByKey.insert( std::make_pair( "J8-13", 27 ) );
  _gpioByKey.insert( std::make_pair( "J8-15", 22 ) );
  _gpioByKey.insert( std::make_pair( "J8-19", 10 ) );
  _gpioByKey.insert( std::make_pair( "J8-21", 9 ) );
  _gpioByKey.insert( std::make_pair( "J8-23", 11 ) );
  _gpioByKey.insert( std::make_pair( "J8-27", 0 ) );
  _gpioByKey.insert( std::make_pair( "J8-28", 1 ) );
  _gpioByKey.insert( std::make_pair( "J8-29", 5 ) );
  _gpioByKey.insert( std::make_pair( "J8-31", 6 ) );
  _gpioByKey.insert( std::make_pair( "J8-32", 12 ) );
  _gpioByKey.insert( std::make_pair( "J8-33", 13 ) );
  _gpioByKey.insert( std::make_pair( "J8-35", 19 ) );
  _gpioByKey.insert( std::make_pair( "J8-36", 16 ) );
  _gpioByKey.insert( std::make_pair( "J8-37", 26 ) );
  _gpioByKey.insert( std::make_pair( "J8-38", 20 ) );
  _gpioByKey.insert( std::make_pair( "J8-40", 21 ) );

  // Name list
  _gpioByName.insert( std::make_pair( "GPIO14", 14 ) );
  _gpioByName.insert( std::make_pair( "GPIO15", 15 ) );
  _gpioByName.insert( std::make_pair( "GPIO18", 18 ) );
  _gpioByName.insert( std::make_pair( "GPIO23", 23 ) );
  _gpioByName.insert( std::make_pair( "GPIO24", 24  ) );
  _gpioByName.insert( std::make_pair( "GPIO25", 25 ) );
  _gpioByName.insert( std::make_pair( "GPIO08", 8 ) );
  _gpioByName.insert( std::make_pair( "GPIO07", 7 ) );
  _gpioByName.insert( std::make_pair( "GPIO2", 2  ) );
  _gpioByName.insert( std::make_pair( "GPIO3", 3 ) );
  _gpioByName.insert( std::make_pair( "GPIO4", 4 ) );
  _gpioByName.insert( std::make_pair( "GPIO17", 17 ) );
  _gpioByName.insert( std::make_pair( "GPIO27", 27 ) );
  _gpioByName.insert( std::make_pair( "GPIO22", 22 ) );
  _gpioByName.insert( std::make_pair( "GPIO10", 10 ) );
  _gpioByName.insert( std::make_pair( "GPIO9", 9 ) );
  _gpioByName.insert( std::make_pair( "GPIO11", 11 ) );
  _gpioByName.insert( std::make_pair( "GPIO0", 0 ) );
  _gpioByName.insert( std::make_pair( "GPIO1", 1 ) );
  _gpioByName.insert( std::make_pair( "GPIO5", 5 ) );
  _gpioByName.insert( std::make_pair( "GPIO6", 6 ) );
  _gpioByName.insert( std::make_pair( "GPIO12", 12 ) );
  _gpioByName.insert( std::make_pair( "GPIO13", 13 ) );
  _gpioByName.insert( std::make_pair( "GPIO19", 19 ) );
  _gpioByName.insert( std::make_pair( "GPIO16", 16 ) );
  _gpioByName.insert( std::make_pair( "GPIO26", 26 ) );
  _gpioByName.insert( std::make_pair( "GPIO20", 20 ) );
  _gpioByName.insert( std::make_pair( "GPIO21", 21 ) );

  // Number list
  _gpioByNumber.insert( std::make_pair( "14", 14 ) );
  _gpioByNumber.insert( std::make_pair( "15", 15 ) );
  _gpioByNumber.insert( std::make_pair( "18", 18 ) );
  _gpioByNumber.insert( std::make_pair( "23", 23 ) );
  _gpioByNumber.insert( std::make_pair( "24", 24  ) );
  _gpioByNumber.insert( std::make_pair( "25", 25 ) );
  _gpioByNumber.insert( std::make_pair( "08", 8 ) );
  _gpioByNumber.insert( std::make_pair( "07", 7 ) );
  _gpioByNumber.insert( std::make_pair( "2", 2  ) );
  _gpioByNumber.insert( std::make_pair( "3", 3 ) );
  _gpioByNumber.insert( std::make_pair( "4", 4 ) );
  _gpioByNumber.insert( std::make_pair( "17", 17 ) );
  _gpioByNumber.insert( std::make_pair( "27", 27 ) );
  _gpioByNumber.insert( std::make_pair( "22", 22 ) );
  _gpioByNumber.insert( std::make_pair( "10", 10 ) );
  _gpioByNumber.insert( std::make_pair( "9", 9 ) );
  _gpioByNumber.insert( std::make_pair( "11", 11 ) );
  _gpioByNumber.insert( std::make_pair( "0", 0 ) );
  _gpioByNumber.insert( std::make_pair( "1", 1 ) );
  _gpioByNumber.insert( std::make_pair( "5", 5 ) );
  _gpioByNumber.insert( std::make_pair( "6", 6 ) );
  _gpioByNumber.insert( std::make_pair( "12", 12 ) );
  _gpioByNumber.insert( std::make_pair( "13", 13 ) );
  _gpioByNumber.insert( std::make_pair( "19", 19 ) );
  _gpioByNumber.insert( std::make_pair( "16", 16 ) );
  _gpioByNumber.insert( std::make_pair( "26", 26 ) );
  _gpioByNumber.insert( std::make_pair( "20", 20 ) );
  _gpioByNumber.insert( std::make_pair( "21", 21 ) );

}

//------------------------------------------------------------------------------
//! @brief   Init maps for BeagleBoneBlack
//------------------------------------------------------------------------------
void GpioConst::init_beagleboneblack() {
  std::cout << "GpioConst: Loading Look-Up table for BeagleBone Black" << std::endl;
  // Key list
  _gpioByKey.insert( std::make_pair( "USR0", 53 ) );
  _gpioByKey.insert( std::make_pair( "USR1", 54 ) );
  _gpioByKey.insert( std::make_pair( "USR2", 55 ) );
  _gpioByKey.insert( std::make_pair( "USR3", 56 ) );

  // Name list
  _gpioByKey.insert( std::make_pair( "led0", 0 ) );
  _gpioByKey.insert( std::make_pair( "led1", 1 ) );
  _gpioByKey.insert( std::make_pair( "led2", 2 ) );
  _gpioByKey.insert( std::make_pair( "led3", 3 ) );

  // Number list
  _gpioByNumber.insert( std::make_pair( "0", 0 ) );
  _gpioByNumber.insert( std::make_pair( "1", 1 ) );
  _gpioByNumber.insert( std::make_pair( "2", 2 ) );
  _gpioByNumber.insert( std::make_pair( "3", 3 ) );

}


