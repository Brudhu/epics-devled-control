/*******************************************************************************
 * Copyright (C) 2015 Florian Feldbauer <feldbaue@kph.uni-mainz.de>
 *                    - Helmholtz-Institut Mainz
 *
 * This file is part of devGpio
 *
 * devGpio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * devGpio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * version 1.0.0; Aug 13, 2015
 *
*******************************************************************************/

/**
 * @file devGpioBo.c
 * @author F.Feldbauer
 * @date 13 Aug 2015
 * @brief Device Support implementation for bo records
 */

/*_____ I N C L U D E S ______________________________________________________*/

/* ANSI C includes  */
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/* EPICS includes */
#include <mbboRecord.h>
#include <alarm.h>
#include <dbAccess.h>
#include <errlog.h>
#include <epicsExport.h>
#include <epicsTypes.h>
#include <iocLog.h>
#include <iocsh.h>
#include <recGbl.h>

/* local includes */
#include "devGpio.h"

/*_____ D E F I N I T I O N S ________________________________________________*/
static long devGpioInitRecord_bo( mbboRecord *prec );
static long devGpioWrite_bo( mbboRecord *prec );

/*_____ G L O B A L S ________________________________________________________*/
devGpio_dset_t devGpioMbbo = {
  6,
  NULL,
  devGpioControlInit,
  devGpioInitRecord_bo,
  NULL,
  devGpioWrite_bo,
  NULL
};
epicsExportAddress( dset, devGpioMbbo );

/*_____ L O C A L S __________________________________________________________*/

/*_____ F U N C T I O N S ____________________________________________________*/

/**-----------------------------------------------------------------------------
 * @brief   Initialization of bo records
 *
 * @param   [in]  prec   Address of the record calling this function
 *
 * @return  In case of error return -1, otherwise return 0
 *----------------------------------------------------------------------------*/
static long devGpioInitRecord_bo( mbboRecord *prec ){
  prec->pact = (epicsUInt8)true; /* disable record */

  devGpio_rec_t conf = { &prec->out, true, 0 };
  long status = devGpioControlInitRecord( (dbCommon*)prec, &conf );
  if( status != 0 ) return ERROR;

  prec->pact = (epicsUInt8)false; /* enable record */

  return OK;
}

/**-----------------------------------------------------------------------------
 * @brief   Write routine of bo records
 *
 * @param   [in]  prec   Address of the record calling this function
 *
 * @return  In case of error return -1, otherwise return 0
 *----------------------------------------------------------------------------*/
long devGpioWrite_bo( mbboRecord *prec ) {
  devGpio_info_t *pinfo = (devGpio_info_t *)prec->dpvt;
  pinfo->value = prec->rval;

  switch (prec->rval) {
  case 0:
      pinfo->strValue = prec->zrst;
  case 1:
      pinfo->strValue = prec->onst;
      break;
  case 2:
      pinfo->strValue = prec->twst;
      break;
  case 3:
      pinfo->strValue = prec->thst;
      break;
  case 4:
      pinfo->strValue = prec->frst;
      break;
  case 5:
      pinfo->strValue = prec->fvst;
      break;
  case 6:
      pinfo->strValue = prec->sxst;
      break;
  case 7:
      pinfo->strValue = prec->svst;
      break;
  case 8:
      pinfo->strValue = prec->eist;
      break;
  case 9:
      pinfo->strValue = prec->nist;
      break;
  case 10:
      pinfo->strValue = prec->test;
      break;
  case 11:
      pinfo->strValue = prec->elst;
      break;
  default:
      break;
  }

  long status = devGpioControlWrite( pinfo );
  if( ERROR == status ) {
    fprintf( stderr, "\033[31;1m%s: Could not write value: %s\033[0m\n",
             prec->name, pinfo->errmsg );
    recGblSetSevr( prec, WRITE_ALARM, INVALID_ALARM );
  }
  return status;
}

