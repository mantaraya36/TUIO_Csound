/*
	TUIO Pure Data External - part of the reacTIVision project
	http://reactivision.sourceforge.net/

	Copyright (c) 2005-2009 Martin Kaltenbrunner <mkalten@iua.upf.edu>
	Copyright (c) 2012 Andres Cabrera <mantaraya36@gmail.com>

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef TUIOCSOUND_H
#define TUIOCSOUND_H

#include "csdl.h"

/* Original signatures for messages are:

  addObject session_id symbol_id xpos ypos angle
  updateObject session_id symbol_id xpos ypos angle xspeed yspeed rspeed maccel raccel
  removeObject session_id symbol_id

But I've moved session to the end as it isn't often used */

/* List member types for message buffers */
typedef struct _add_data add_data_t;
typedef struct _update_data update_data_t;
typedef struct _remove_data remove_data_t;

typedef struct _tuio_listener {
	int port;
	void *tuio_wrapper;
	CSOUND *csound;
	void *add_mutex;
	add_data_t *add_list_head;
	void *update_mutex;
	update_data_t *update_list_head;
	void *remove_mutex;
	remove_data_t *remove_list_head;
} tuio_listener_t;

typedef struct _global_tuio {
	int nPorts; /* number of allocated listeners */
	tuio_listener_t *listeners;
} global_tuio_t;

typedef struct _tuio_init {
	OPDS h;
	/* outputs */
	MYFLT *handle;
	/* inputs */
	MYFLT *port;
} TUIO_INIT;

typedef struct _add_obj {
	OPDS h;
	/* outputs */
	MYFLT *trig, *symbol, *xpos, *ypos, *angle, *session;
	/* inputs */
	MYFLT *handle;
	/* internal data */
	tuio_listener_t *listener;
} ADD_OBJ;

typedef struct _update_obj {
	OPDS h;
	/* outputs */
	MYFLT *trig, *symbol, *xpos, *ypos, *angle, *xspeed, *yspeed;
	MYFLT *rspeed, *maccel, *raccel, *session;
	/* inputs */
	MYFLT *handle;
	/* internal data */
	tuio_listener_t *listener;
} UPDATE_OBJ;

typedef struct _remove_obj {
	OPDS h;
	/* outputs */
	MYFLT *trig, *symbol, *session;
	/* inputs */
	MYFLT *handle;
	/* internal data */
	tuio_listener_t *listener;
} REMOVE_OBJ;

typedef struct _add_data {
	MYFLT symbol, xpos, ypos, angle, session;
	struct _add_data *next;
} add_data_t;

typedef struct _update_data {
	MYFLT symbol, xpos, ypos, angle, xspeed, yspeed, rspeed, maccel, raccel, session;
	struct _update_data *next;
} update_data_t;

typedef struct _remove_data {
	MYFLT symbol, session;
	struct _remove_data *next;
} remove_data_t;


#endif // TUIOCSOUND_H
