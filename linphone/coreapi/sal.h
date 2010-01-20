/*
linphone
Copyright (C) 2010  Simon MORLAT (simon.morlat@free.fr)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

/** 
 This header files defines the Signaling Abstraction Layer.
 The purpose of this layer is too allow experiment different call signaling 
 protocols and implementations under linphone, for example SIP, JINGLE...
**/

#ifndef sal_h
#define sal_h

#include "mediastreamer2/mscommon.h"

struct Sal;

typedef struct Sal Sal;

struct SalOp;

typedef struct SalOp SalOp;

Sal * sal_init();
void sal_uninit(Sal* sal);

typedef enum {
	SAL_TRANSPORT_DATAGRAM,
	SAL_TRANSPORT_STREAM
}SalTransport;

typedef enum {
	SAL_AUDIO,
	SAL_VIDEO,
	SAL_OTHER
} SalStreamType;

typedef enum{
	SAL_PROTO_UNKNOWN,
	SAL_PROTO_RTP_AVP,
	SAL_PROTO_RTP_SAVP
}SalMediaProto;

typedef struct SalStreamDescription{
	SalMediaProto proto;
	SalStreamType type;
	char addr[64];
	int port;
	MSList *payloads; //<list of PayloadType
	int bandwidth;
	int ptime;
} SalStreamDescription;

#define SAL_MEDIA_DESCRIPTION_MAX_STREAMS 4

typedef struct SalMediaDescription{
	char addr[64];
	char username[64];
	int nstreams;
	SalStreamDescription streams[SAL_MEDIA_DESCRIPTION_MAX_STREAMS];
} SalMediaDescription;

void sal_media_description_free(SalMediaDescription *md);

typedef enum SalError{
	SalErrorNetwork,
	SalErrorMedia,
	SalErrorAuth,
	SalErrorForbidden
} SalError;

typedef void (*SalOnCallReceived)(SalOp *op);
typedef void (*SalOnCallRinging)(SalOp *op);
typedef void (*SalOnCallAccepted)(SalOp *op);
typedef void (*SalOnCallTerminated)(SalOp *op);
typedef void (*SalOnCallFailure)(SalOp *op, SalError error, const char *details);

int sal_listen_port(Sal *ctx, const char *addr, int port, SalTransport tr, int is_secure);
void sal_set_user_agent(Sal *ctx, const char *user_agent);
void sal_use_session_timers(Sal *ctx, int expires);

SalOp * sal_call_create(Sal *sal, const char *from, const char *to, const char *route, const char *contact);
int sal_call_set_local_media_description(SalOp *h, const SalMediaDescription *desc);
int sal_call(SalOp *h);
int sal_call_accept(SalOp*h);
int sal_call_get_final_media_description(SalOp *h, SalMediaDescription *result);
int sal_call_terminate(SalOp *h);

int sal_iterate(Sal *sal);

SalOp *sal_register_create(Sal *ctx, const char *from, const char *contact, int expires);
int sal_register(SalOp *h);

void sal_op_release(SalOp *h);

#define payload_type_set_number(pt,n)	(pt)->user_data=(void*)((long)n);
#define payload_type_get_number(pt)		((int)(long)(pt)->user_data)

#endif
