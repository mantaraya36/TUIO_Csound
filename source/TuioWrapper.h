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

#ifndef INCLUDED_TuioWrapper_H
#define INCLUDED_TuioWrapper_H

#include "TuioListener.h"
#include "TuioClient.h"

using namespace TUIO;

typedef void (*addObjectCallback_t)(void *, TuioObject *);
typedef void (*updateObjectCallback_t)(void *, TuioObject *);
typedef void (*removeObjectCallback_t)(void *, TuioObject *);

class TuioWrapper : public TuioListener { 
	
	public:
		TuioWrapper(int p);
		~TuioWrapper() {}

		void setAddObjectCallback(addObjectCallback_t cb, void *addData);
		void setUpdateObjectCallback(updateObjectCallback_t cb, void *updateData);
		void setRemoveObjectCallback(removeObjectCallback_t cb, void *removeData);
		
		void start();
		void stop();
		bool isRunning() { return running; }

		int getPort() { return udp_port; }
	
		void addTuioObject(TuioObject *tuioObject);
		void updateTuioObject(TuioObject *tuioObject);
		void removeTuioObject(TuioObject *tuioObject);
	
		void addTuioCursor(TuioCursor *tuioCursor);
		void updateTuioCursor(TuioCursor *tuioCursor);
		void removeTuioCursor(TuioCursor *tuioCursor);
	
		void refresh(TuioTime bundleTime);

	private:

		TuioClient *client;
		addObjectCallback_t add_cb;
		void *add_data;
		updateObjectCallback_t update_cb;
		void *update_data;
		removeObjectCallback_t remove_cb;
		void *remove_data;

		int udp_port;
		bool running;
};

#endif /* INCLUDED_TuioWrapper_H */
