/*
 *******************************************************************************
 * $Header: $
 *
 *  Copyright (c) 2011-2015 VN Inc. All rights reserved.
 *
 *  +-----------------------------------------------------------------+
 *  | THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY ONLY BE USED |
 *  | AND COPIED IN ACCORDANCE WITH THE TERMS AND CONDITIONS OF SUCH  |
 *  | A LICENSE AND WITH THE INCLUSION OF THE THIS COPY RIGHT NOTICE. |
 *  | THIS SOFTWARE OR ANY OTHER COPIES OF THIS SOFTWARE MAY NOT BE   |
 *  | PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY OTHER PERSON. THE   |
 *  | OWNERSHIP AND TITLE OF THIS SOFTWARE IS NOT TRANSFERRED.        |
 *  |                                                                 |
 *  | THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT   |
 *  | ANY PRIOR NOTICE AND SHOULD NOT BE CONSTRUED AS A COMMITMENT BY |
 *  | VN INC.                                                     |
 *  +-----------------------------------------------------------------+
 *
 * $History: $
 *
 *******************************************************************************
 */

/*
 *******************************************************************************
 * Copyright 2011-2015 VN, Inc. All rights reserved.
 *
 * File: vlist.h
 *
 * Brief: 
 *
 * Creation Date: 2011-07-29
 *
 * Author: bruce.hsu (bruce.hsu@VN.com)
 *
 *******************************************************************************
 */

#ifndef __VLIST_H__
#define __VLIST_H__

#include <stdio.h>
#include <stdlib.h>

#include "typedef.h"
#include "errordef.h"
#include "global_codec.h"

#define VLIST_VERSION MAKEFOURCC(1, 0, 0, 1)

typedef size_t (*PFVListElemMeter)(const void *pvElem);
typedef void *(*PFVListElemAlloc) (void);
typedef void (*PFVListElemDeAlloc) (void *pvElem);
typedef int (*PFVListElemCopier) (void *pvElemCopier, const void *pvElemCopiee);  //dst, src

typedef enum vlist_type
{
    vtNoCopy    =0,
    vtSmpleCopy =1,
    vtCmplxCopy =2
} EVListType;

typedef struct vlist_conf
{
    DWORD               dwVersion;
    EVListType          eType;          //copy data or assign data
    PFVListElemMeter    pfElemMeter;    //if you want copy data(simple copy), you have to assign this func.
    PFVListElemAlloc    pfElemAlloc;    //if you want copy data(complex copy), you have to assign this func.
    PFVListElemDeAlloc  pfElemDeAlloc;  //if you want copy data(complex copy), you have to assign this func.
    PFVListElemCopier   pfElemCopier;   //if you want copy data(complex copy), you have to assign this func.
} TVListConf;

SCODE vlist_init(HANDLE *phVList, TVListConf *ptConf);
SCODE vlist_rls(HANDLE *phVList);
//property
int vlist_size(HANDLE hVList);
BOOL vlist_is_empty(HANDLE hVList);
//insert
int vlist_insert_at(HANDLE hVList, const void *pvData, DWORD dwPos);
int vlist_push_back(HANDLE hVList, const void *pvData);
int vlist_push_front(HANDLE hVList, const void *pvData);
//access
void *vlist_get_at(HANDLE hVList, DWORD dwPos);
void *vlist_back(HANDLE hVList);
void *vlist_front(HANDLE hVList);
//delete
int vlist_remove_at(HANDLE hVList, DWORD dwPos);
int vlist_clear(HANDLE hVList);
int vlist_pop_back(HANDLE hVList);
int vlist_pop_front(HANDLE hVList);
//iterator
int vlist_iterator_start(HANDLE hVList);
void *vlist_iterator_next(HANDLE hVList);
BOOL vlist_iterator_has_next(HANDLE hVList);
int vlist_iterator_stop(HANDLE hVList);

//ready-made meter functions
//meter functions
size_t vlist_meter_int8_t(const void *pvElem);
size_t vlist_meter_int16_t(const void *pvElem);
size_t vlist_meter_int32_t(const void *pvElem);
size_t vlist_meter_int64_t(const void *pvElem);
size_t vlist_meter_uint8_t(const void *pvElem);
size_t vlist_meter_uint16_t(const void *pvElem);
size_t vlist_meter_uint32_t(const void *pvElem);
size_t vlist_meter_uint64_t(const void *pvElem);
size_t vlist_meter_float(const void *pvElem);
size_t vlist_meter_double(const void *pvElem);
size_t vlist_meter_string(const void *pvElem);

#endif//__VLIST_H__

