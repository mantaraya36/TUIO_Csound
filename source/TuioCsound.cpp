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

#include "TuioCsound.h"
#include "TuioWrapper.h"

extern "C"{
	#include <stdio.h>
	#include <string.h>
	
//	static t_class *TuioClient_class;
	std::list<TuioWrapper*> TuioClientList;
	
	static void add_cb(void *data, TuioObject *obj)
	{
		tuio_listener_t *a = (tuio_listener_t *) data;

		a->csound->LockMutex(a->add_mutex);
		add_data_t *new_message = (add_data_t *) malloc(sizeof(add_data_t));
		add_data_t *last_message = a->add_list_head;
		while (last_message && last_message->next){
			last_message = last_message->next;
		}

		new_message->session = (MYFLT) obj->getSessionID();
		new_message->symbol = (MYFLT) obj->getSymbolID();
		new_message->xpos = (MYFLT) obj->getX();
		new_message->ypos = (MYFLT) obj->getY();
		new_message->angle = (MYFLT) obj->getAngle();
		new_message->next = NULL;

		if (last_message) {
			last_message->next = new_message;
		} else {
			a->add_list_head =  new_message;
		}
		a->csound->UnlockMutex(a->add_mutex);
	}

	static void update_cb(void *data, TuioObject *obj)
	{
		tuio_listener_t *a = (tuio_listener_t *) data;

		a->csound->LockMutex(a->update_mutex);
		update_data_t *new_message = (update_data_t *) malloc(sizeof(update_data_t));
		update_data_t *last_message = a->update_list_head;
		while (last_message && last_message->next){
			last_message = last_message->next;
		}

		new_message->session = (MYFLT) obj->getSessionID();
		new_message->symbol = (MYFLT) obj->getSymbolID();
		new_message->xpos = (MYFLT) obj->getX();
		new_message->ypos = (MYFLT) obj->getY();
		new_message->angle = (MYFLT) obj->getAngle();
		new_message->xspeed = (MYFLT) obj->getXSpeed();
		new_message->yspeed = (MYFLT) obj->getYSpeed();
		new_message->rspeed = (MYFLT) obj->getRotationSpeed();
		new_message->maccel = (MYFLT) obj->getMotionAccel();
		new_message->raccel = (MYFLT) obj->getRotationAccel();
		new_message->next = NULL;

		if (last_message) {
			last_message->next = new_message;
		} else {
			a->update_list_head =  new_message;
		}
		a->csound->UnlockMutex(a->update_mutex);
	}

	static void remove_cb(void *data, TuioObject *obj)
	{
		tuio_listener_t *a = (tuio_listener_t *) data;

		a->csound->LockMutex(a->remove_mutex);
		remove_data_t *new_message = (remove_data_t *) malloc(sizeof(remove_data_t));
		remove_data_t *last_message = a->remove_list_head;
		while (last_message && last_message->next){
			last_message = last_message->next;
		}

		new_message->session = (MYFLT) obj->getSessionID();
		new_message->symbol = (MYFLT) obj->getSymbolID();
		new_message->next = NULL;

		if (last_message) {
			last_message->next = new_message;
		} else {
			a->remove_list_head =  new_message;
		}
		a->csound->UnlockMutex(a->remove_mutex);
	}

	static int tuioinit_deinit(CSOUND *csound, TUIO_INIT *p)
	{
		global_tuio_t *pp;
		tuio_listener_t * ports;
		TuioWrapper *wr;
		pp = (global_tuio_t*) csound->QueryGlobalVariable(csound, "_TUIO_global");
		if (!pp) {
			csound->Warning(csound, Str("Failed to find TUIO globals for deinit."));
			return NOTOK;
		}

		int n = (int) *p->handle;
		ports = pp->listeners;
		if (ports == NULL || n > pp->nPorts) {
			csound->Warning(csound, Str("TUIO globals %i not allocated."), n);
			return NOTOK;
		}
		wr = (TuioWrapper *) ports[n].tuio_wrapper;
		wr->stop();
		delete wr;
		ports[n].add_list_head = NULL;
		ports[n].update_list_head = NULL;
		ports[n].remove_list_head = NULL;
		csound->DestroyMutex(ports[n].add_mutex);
		csound->DestroyMutex(ports[n].update_mutex);
		csound->DestroyMutex(ports[n].remove_mutex);
		csound->Warning(csound, "DEBUG: tuioinit_deinit");

		return OK;
	}

	static int tuioinit_init(CSOUND *csound, TUIO_INIT *p){
		global_tuio_t *pp;
		tuio_listener_t *ports;
		TuioWrapper *wr;
		int n;

		/* TODO check if port is alredy used by another instance or another application or can this be infered from the TuioWrapper instance*/
		pp = (global_tuio_t*) csound->QueryGlobalVariable(csound, "_TUIO_global");
		if (pp == NULL) { /* allocate if non existent */
			if (UNLIKELY(csound->CreateGlobalVariable(csound, "_TUIO_global",
													  sizeof(global_tuio_t)) != 0))
				csound->Die(csound, Str("TUIO: failed to allocate globals"));
			pp = (global_tuio_t*) csound->QueryGlobalVariable(csound, "_TUIO_global");
			pp->nPorts = 0;
		}
//	    pp->csound = csound;
//	    csound->RegisterResetCallback(csound, (void*) pp,
//	                                  (int (*)(CSOUND *, void *)) OSC_reset);

		/* allocate and initialise the globals structure */
		n = pp->nPorts;
		ports = (tuio_listener_t*) csound->ReAlloc(csound, pp->listeners,
											sizeof(tuio_listener_t) * (n + 1));
		ports[n].csound = csound;
		ports[n].add_mutex = csound->Create_Mutex(0);
		ports[n].update_mutex = csound->Create_Mutex(0);
		ports[n].remove_mutex = csound->Create_Mutex(0);
		ports[n].add_list_head = NULL;
		ports[n].update_list_head = NULL;
		ports[n].remove_list_head = NULL;

		wr = new TuioWrapper((int) *p->port);
		wr->setAddObjectCallback(add_cb, (void *) &(ports[n]) );
		wr->setUpdateObjectCallback(update_cb, (void *) &(ports[n]) );
		wr->setRemoveObjectCallback(remove_cb, (void *) &(ports[n]) );

		pp->listeners = ports;
		pp->nPorts = n + 1;
		*(p->handle) = (MYFLT) n;

//		csound->RegisterDeinitCallback(csound, p,
//									   (int (*)(CSOUND *, void *)) tuioinit_deinit);
		csound->RegisterResetCallback(csound, p,
									  (int (*)(CSOUND *, void *)) tuioinit_deinit);

		wr->start();
		if (!wr->isRunning()) {
			csound->InitError(csound,
							  Str("TUIO listener can't be started on port %i\n"),
							  (int) *p->port);
			delete wr;
			return NOTOK;
		}
		ports[n].tuio_wrapper = (void *) wr;
		csound->Message(csound, Str("TUIO listener #%d started on port %i\n"), (int) *p->handle, (int) *p->port);
		return OK;
	}

	static int add_obj_init(CSOUND *csound, ADD_OBJ *p)
	{
		global_tuio_t *pp;
		tuio_listener_t *listener;

		pp = (global_tuio_t*) csound->QueryGlobalVariable(csound, "_TUIO_global");
		if (pp == NULL) {
			csound->InitError(csound, "Invalid TUIO listener handle.");
			return NOTOK;
		}
		listener = & (pp->listeners[(int) *p->handle]);
		if (!listener) {
			csound->InitError(csound, "TUIO listener not allocated.");
			return NOTOK;
		}
		p->listener = listener;
		*p->trig = 0;
		*p->session = *p->symbol = *p->xpos = *p->ypos = *p->angle = 0;
		return OK;
	}

	static int add_obj_k(CSOUND *csound, ADD_OBJ *p){
		tuio_listener_t *l;
		add_data_t *data;

		l = p->listener;
		csound->LockMutex(l->add_mutex);
		/* TODO should CsoundLockMutex no wait be used to ensure better realtime performance but more chance of message jitter? */

		data = l->add_list_head;
		if (data) {
			*p->trig = 1.0;
			/* copy the data */
			*p->session = data->session;
			*p->symbol = data->symbol;
			*p->xpos = data->xpos;
			*p->ypos = data->ypos;
			*p->angle = data->angle;
			/* now pop from the list */
			add_data_t *next_message  = data->next;
			free(l->add_list_head);
			l->add_list_head = next_message;

		} else {
			*p->trig = 0.0;
		}
		csound->UnlockMutex(l->add_mutex);
		return OK;
	}

	static int update_obj_init(CSOUND *csound, UPDATE_OBJ *p)
	{
		global_tuio_t *pp;
		tuio_listener_t *listener;

		pp = (global_tuio_t*) csound->QueryGlobalVariable(csound, "_TUIO_global");
		if (pp == NULL) {
			csound->InitError(csound, "Invalid TUIO listener handle.");
			return NOTOK;
		}
		listener = & (pp->listeners[(int) *p->handle]);
		if (!listener) {
			csound->InitError(csound, "TUIO listener not allocated.");
			return NOTOK;
		}
		p->listener = listener;
		*p->trig = 0;
		*p->session = *p->symbol = *p->xpos = *p->ypos = *p->angle = 0;
		*p->xspeed = *p->yspeed = *p->rspeed = *p->maccel = *p->raccel = 0;
		return OK;
	}

	static int update_obj_k(CSOUND *csound, UPDATE_OBJ *p)
	{
		tuio_listener_t *l;
		update_data_t *data;

		l = p->listener;
		csound->LockMutex(l->update_mutex);
		/* TODO should CsoundLockMutex no wait be used to ensure better realtime performance but more chance of message jitter? */

		data = l->update_list_head;
		if (data) {
			*p->trig = 1.0;
			/* copy the data */
			*p->session = data->session;
			*p->symbol = data->symbol;
			*p->xpos = data->xpos;
			*p->ypos = data->ypos;
			*p->angle = data->angle;
			*p->xspeed = data->xspeed;
			*p->yspeed = data->yspeed;
			*p->rspeed = data->rspeed;
			*p->maccel = data->maccel;
			*p->raccel = data->raccel;
			/* now pop from the list */
			l->update_list_head = data->next;

		} else {
			*p->trig = 0.0;
		}
		csound->UnlockMutex(l->update_mutex);
		return OK;
	}

	static int remove_obj_init(CSOUND *csound, REMOVE_OBJ *p)
	{
		global_tuio_t *pp;
		tuio_listener_t *listener;

		pp = (global_tuio_t*) csound->QueryGlobalVariable(csound, "_TUIO_global");
		if (pp == NULL) {
			csound->InitError(csound, "Invalid TUIO listener handle.");
			return NOTOK;
		}
		listener = & (pp->listeners[(int) *p->handle]);
		if (!listener) {
			csound->InitError(csound, "TUIO listener not allocated.");
			return NOTOK;
		}
		p->listener = listener;
		*p->trig = 0;
		*p->session = *p->symbol = 0;
		return OK;
	}

	static int remove_obj_k(CSOUND *csound, REMOVE_OBJ *p)
	{
		tuio_listener_t *l;
		remove_data_t *data;

		l = p->listener;
		csound->LockMutex(l->remove_mutex);
		/* TODO should CsoundLockMutex no wait be used to ensure better realtime performance but more chance of message jitter? */

		data = l->remove_list_head;
		if (data) {
			*p->trig = 1.0;
			/* copy the data */
			*p->session = data->session;
			*p->symbol = data->symbol;
			/* now pop from the list */
			l->remove_list_head = data->next;

		} else {
			*p->trig = 0.0;
		}
		csound->UnlockMutex(l->remove_mutex);
		return OK;
	}

#define S(x)    sizeof(x)

	static OENTRY localops[] = {
		{ "TUIOinit", S(TUIO_INIT), 1, "i",
		  "i", (SUBR)tuioinit_init},
		{ "addObject", S(ADD_OBJ), 3, "kkkkkk",
		  "i", (SUBR)add_obj_init, (SUBR) add_obj_k},
		{ "updateObject", S(UPDATE_OBJ), 3, "kkkkkkkkkkk",
		  "i", (SUBR)update_obj_init, (SUBR) update_obj_k},
		{ "removeObject", S(REMOVE_OBJ), 3, "kkk",
		  "i", (SUBR)remove_obj_init, (SUBR) remove_obj_k},
	};
	LINKAGE
}
