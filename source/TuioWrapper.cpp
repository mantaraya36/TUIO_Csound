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

#include "TuioWrapper.h"
	
TuioWrapper::TuioWrapper(int p) {

	udp_port = p;
	running = false;
}

void TuioWrapper::setAddObjectCallback(addObjectCallback_t cb, void *addData)
{
	add_cb = cb;
	add_data = addData;
}

void TuioWrapper::setUpdateObjectCallback(updateObjectCallback_t cb, void *updateData)
{
	update_cb = cb;
	update_data = updateData;
}

void TuioWrapper::setRemoveObjectCallback(removeObjectCallback_t cb, void *removeData)
{
	remove_cb = cb;
	remove_data = removeData;
}

void TuioWrapper::addTuioObject(TuioObject *tobj)
{
	add_cb(add_data, tobj);
}

void TuioWrapper::updateTuioObject(TuioObject *tobj)
{
	update_cb(update_data, tobj);
}

void TuioWrapper::removeTuioObject(TuioObject *tobj)
{
	remove_cb(remove_data, tobj);
}

void TuioWrapper::addTuioCursor(TuioCursor *tcur) {

//	t_atom message[4];
//	SETFLOAT(&message[0],(float)tcur->getSessionID());
//	SETFLOAT(&message[1],(float)tcur->getCursorID());
//	SETFLOAT(&message[2],tcur->getX());
//	SETFLOAT(&message[3],tcur->getY());
	
//	sys_lock();
//	for(std::list<t_outlet*>::iterator outlet = message_outlet.begin(); outlet!=message_outlet.end(); outlet++) {
//		outlet_anything(*outlet, gensym((char*)"addCursor"), 4, message);
//	}
//	sys_unlock();
}

void TuioWrapper::updateTuioCursor(TuioCursor *tcur) {

//	t_atom message[7];
//	SETFLOAT(&message[0],(float)tcur->getSessionID());
//	SETFLOAT(&message[1],(float)tcur->getCursorID());
//	SETFLOAT(&message[2],tcur->getX());
//	SETFLOAT(&message[3],tcur->getY());
//	SETFLOAT(&message[4],tcur->getXSpeed());
//	SETFLOAT(&message[5],tcur->getYSpeed());
//	SETFLOAT(&message[6],tcur->getMotionAccel());
	
//	if (sys_trylock()==0) {
//		for(std::list<t_outlet*>::iterator outlet = message_outlet.begin(); outlet!=message_outlet.end(); outlet++) {
//			outlet_anything(*outlet, gensym((char*)"updateCursor"), 7, message);
//		}
//		sys_unlock();
//	}
}

void TuioWrapper::removeTuioCursor(TuioCursor *tcur) {
	
//	t_atom message[2];
//	SETFLOAT(&message[0],(float)tcur->getSessionID());
//	SETFLOAT(&message[1],(float)tcur->getCursorID());
//	sys_lock();
//	for(std::list<t_outlet*>::iterator outlet = message_outlet.begin(); outlet!=message_outlet.end(); outlet++) {
//		outlet_anything(*outlet, gensym((char*)"removeCursor"), 2, message);
//	}
//	sys_unlock();
	
}

void TuioWrapper::refresh(TuioTime bundleTime)
{
	
//	if (sys_trylock()==0) {
//		for(std::list<t_outlet*>::iterator outlet = refresh_outlet.begin(); outlet!=refresh_outlet.end(); outlet++) {
//			outlet_bang(*outlet);
//		}
//		sys_unlock();
//	}
}

void TuioWrapper::start() {

	if (!running) {
		client = new TuioClient(udp_port);
		client->addTuioListener(this);
		client->connect();
		if (client->isConnected()) running = true;
	}
}

void TuioWrapper::stop() {
	
	if (running) {
		client->removeTuioListener(this);
		client->disconnect();
		delete client;
		running = false;
	}
}



