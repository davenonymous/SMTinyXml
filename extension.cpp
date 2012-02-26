/**
 * vim: set ts=4 :
 * =============================================================================
 * SourceMod Sample Extension
 * Copyright (C) 2004-2008 AlliedModders LLC.  All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id$
 */

#include "extension.h"
#include "tinyxml.h"
#include "tinystr.h"

TinyXmlHandler g_TinyXmlHandler;
HandleType_t g_TinyXmlHandle=0;


static cell_t TinyXml_CreateDocument(IPluginContext *pCtx, const cell_t *params)
{
	TiXmlDocument *x = new TiXmlDocument();
	return g_pHandleSys->CreateHandle(g_TinyXmlHandle, (void*)x, pCtx->GetIdentity(), myself->GetIdentity(), NULL);
}

static cell_t TinyXml_CreateElement(IPluginContext *pCtx, const cell_t *params)
{
	char *title;
	pCtx->LocalToString(params[1], &title);

	TiXmlElement *x = new TiXmlElement(title);
	return g_pHandleSys->CreateHandle(g_TinyXmlHandle, (void*)x, pCtx->GetIdentity(), myself->GetIdentity(), NULL);
}

static cell_t TinyXml_CreateText(IPluginContext *pCtx, const cell_t *params)
{
	char *text;
	pCtx->LocalToString(params[1], &text);

	TiXmlText *x = new TiXmlText(text);
	return g_pHandleSys->CreateHandle(g_TinyXmlHandle, (void*)x, pCtx->GetIdentity(), myself->GetIdentity(), NULL);
}

static cell_t TinyXml_CreateComment(IPluginContext *pCtx, const cell_t *params)
{
	char *cmt;
	pCtx->LocalToString(params[1], &cmt);

	TiXmlComment *x = new TiXmlComment(cmt);
	return g_pHandleSys->CreateHandle(g_TinyXmlHandle, (void*)x, pCtx->GetIdentity(), myself->GetIdentity(), NULL);
}

static cell_t TinyXml_CreateDeclaration(IPluginContext *pCtx, const cell_t *params)
{
	char *version;
	pCtx->LocalToString(params[1], &version);

	char *encoding;
	pCtx->LocalToString(params[2], &encoding);

	char *standalone;
	pCtx->LocalToString(params[3], &standalone);

	TiXmlDeclaration *x = new TiXmlDeclaration(version,encoding,standalone);
	return g_pHandleSys->CreateHandle(g_TinyXmlHandle, (void*)x, pCtx->GetIdentity(), myself->GetIdentity(), NULL);
}

static cell_t TinyXml_LoadFile(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	TiXmlDocument *x;

	if ((err=g_pHandleSys->ReadHandle(hndl, g_TinyXmlHandle, &sec, (void **)&x)) != HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid TinyXml handle %x (error %d)", hndl, err);
	}

	if (!x)
	{
		pCtx->ThrowNativeError("TinyXml data not found\n");
		return 0;
	}

	char *xmlfile;
	pCtx->LocalToString(params[2], &xmlfile);

	bool ok = x->LoadFile(xmlfile);

	return ok;
}

static cell_t TinyXml_SaveFile(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	TiXmlDocument *x;

	if ((err=g_pHandleSys->ReadHandle(hndl, g_TinyXmlHandle, &sec, (void **)&x)) != HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid TinyXml handle %x (error %d)", hndl, err);
	}

	if (!x)
	{
		pCtx->ThrowNativeError("TinyXml data not found\n");
		return 0;
	}

	char *xmlfile;
	pCtx->LocalToString(params[2], &xmlfile);

	bool ok = x->SaveFile(xmlfile);

	return ok;
}

static cell_t TinyXml_RootElement(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	TiXmlDocument *x;

	if ((err=g_pHandleSys->ReadHandle(hndl, g_TinyXmlHandle, &sec, (void **)&x)) != HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid TinyXml handle %x (error %d)", hndl, err);
	}

	if (!x)
	{
		pCtx->ThrowNativeError("TinyXml data not found\n");
		return 0;
	}

	TiXmlNode *root = x->RootElement();

	if(root == NULL) {
		return 0;
	} else {
		return g_pHandleSys->CreateHandle(g_TinyXmlHandle, (void*)root, pCtx->GetIdentity(), myself->GetIdentity(), NULL);
	}
}

static cell_t TinyXml_FirstChild(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	TiXmlNode *x;

	if ((err=g_pHandleSys->ReadHandle(hndl, g_TinyXmlHandle, &sec, (void **)&x)) != HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid TinyXml handle %x (error %d)", hndl, err);
	}

	if (!x)
	{
		pCtx->ThrowNativeError("TinyXml not found\n");
		return 0;
	}

	char *search;
	pCtx->LocalToString(params[2], &search);
	TiXmlNode *child;
	if(strcmp(search, "") == 0) {
		child = x->FirstChild();
	} else {
		child = x->FirstChild(search);
	}

	if(child == NULL) {
		return 0;
	} else {
		return g_pHandleSys->CreateHandle(g_TinyXmlHandle, (void*)child, pCtx->GetIdentity(), myself->GetIdentity(), NULL);
	}
}

static cell_t TinyXml_FirstChildElement(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	TiXmlNode *x;

	if ((err=g_pHandleSys->ReadHandle(hndl, g_TinyXmlHandle, &sec, (void **)&x)) != HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid TinyXml handle %x (error %d)", hndl, err);
	}

	if (!x)
	{
		pCtx->ThrowNativeError("TinyXml not found\n");
		return 0;
	}

	char *search;
	pCtx->LocalToString(params[2], &search);
	TiXmlNode *child;
	if(strcmp(search, "") == 0) {
		child = x->FirstChildElement();
	} else {
		child = x->FirstChildElement(search);
	}

	if(child == NULL) {
		return 0;
	} else {
		return g_pHandleSys->CreateHandle(g_TinyXmlHandle, (void*)child, pCtx->GetIdentity(), myself->GetIdentity(), NULL);
	}
}

static cell_t TinyXml_NextSibling(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	TiXmlNode *x;

	if ((err=g_pHandleSys->ReadHandle(hndl, g_TinyXmlHandle, &sec, (void **)&x)) != HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid TinyXml handle %x (error %d)", hndl, err);
	}

	if (!x)
	{
		pCtx->ThrowNativeError("TinyXml not found\n");
		return 0;
	}

	char *search;
	pCtx->LocalToString(params[2], &search);
	TiXmlNode *sibling;
	if(strcmp(search, "") == 0) {
		sibling = x->NextSibling();
	} else {
		sibling = x->NextSibling(search);
	}

	if(sibling == NULL) {
		return 0;
	} else {
		return g_pHandleSys->CreateHandle(g_TinyXmlHandle, (void*)sibling, pCtx->GetIdentity(), myself->GetIdentity(), NULL);
	}
}

static cell_t TinyXml_NextSiblingElement(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	TiXmlNode *x;

	if ((err=g_pHandleSys->ReadHandle(hndl, g_TinyXmlHandle, &sec, (void **)&x)) != HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid TinyXml handle %x (error %d)", hndl, err);
	}

	if (!x)
	{
		pCtx->ThrowNativeError("TinyXml not found\n");
		return 0;
	}

	char *search;
	pCtx->LocalToString(params[2], &search);
	TiXmlNode *sibling;
	if(strcmp(search, "") == 0) {
		sibling = x->NextSiblingElement();
	} else {
		sibling = x->NextSiblingElement(search);
	}

	if(sibling == NULL) {
		return 0;
	} else {
		return g_pHandleSys->CreateHandle(g_TinyXmlHandle, (void*)sibling, pCtx->GetIdentity(), myself->GetIdentity(), NULL);
	}
}

static cell_t TinyXml_LinkEndChild(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	TiXmlNode *x;

	if ((err=g_pHandleSys->ReadHandle(hndl, g_TinyXmlHandle, &sec, (void **)&x)) != HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid TinyXml handle %x (error %d)", hndl, err);
	}

	if (!x)
	{
		pCtx->ThrowNativeError("TinyXml not found\n");
		return 0;
	}

	Handle_t hndlb = static_cast<Handle_t>(params[2]);
	HandleError errb;
	HandleSecurity secb;
	secb.pOwner = NULL;
	secb.pIdentity = myself->GetIdentity();

	TiXmlNode *y;

	if ((errb=g_pHandleSys->ReadHandle(hndlb, g_TinyXmlHandle, &secb, (void **)&y)) != HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid TinyXml handle %x (error %d)", hndlb, errb);
	}

	if (!y)
	{
		pCtx->ThrowNativeError("TinyXml not found\n");
		return 0;
	}

	x->LinkEndChild(y);

	return true;
}

static cell_t TinyXml_FirstAttribute(IPluginContext *pCtx, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	TiXmlNode *x;

	if ((err=g_pHandleSys->ReadHandle(hndl, g_TinyXmlHandle, &sec, (void **)&x)) != HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid TinyXml handle %x (error %d)", hndl, err);
	}

	if (!x)
	{
		pCtx->ThrowNativeError("TinyXml not found\n");
		return 0;
	}

	if(x->Type() == TiXmlNode::TINYXML_ELEMENT) {
		TiXmlAttribute *attribute = x->ToElement()->FirstAttribute();

		if(attribute == NULL) {
			return 0;
		} else {
			return g_pHandleSys->CreateHandle(g_TinyXmlHandle, (void*)attribute, pCtx->GetIdentity(), myself->GetIdentity(), NULL);
		}
	} else {
		return 0;
	}
}

static cell_t TinyXml_NextAttribute(IPluginContext *pCtx, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	TiXmlAttribute *x;

	if ((err=g_pHandleSys->ReadHandle(hndl, g_TinyXmlHandle, &sec, (void **)&x)) != HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid TinyXml handle %x (error %d)", hndl, err);
	}

	if (!x)
	{
		pCtx->ThrowNativeError("TinyXml not found\n");
		return 0;
	}

	TiXmlAttribute *attribute = x->Next();

	if(attribute == NULL) {
		return 0;
	} else {
		return g_pHandleSys->CreateHandle(g_TinyXmlHandle, (void*)attribute, pCtx->GetIdentity(), myself->GetIdentity(), NULL);
	}
}

static cell_t TinyXml_GetText(IPluginContext *pCtx, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	TiXmlNode *x;

	if ((err=g_pHandleSys->ReadHandle(hndl, g_TinyXmlHandle, &sec, (void **)&x)) != HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid TinyXml handle %x (error %d)", hndl, err);
	}

	if (!x)
	{
		pCtx->ThrowNativeError("TinyXml not found\n");
		return 0;
	}

	if(x->Type() == TiXmlNode::TINYXML_ELEMENT) {
		TiXmlElement *y = x->ToElement();
		const char *text;
		text = y->GetText();
		if(text == NULL)
			return 0;

		char buffer[2048];
		snprintf(buffer, sizeof(buffer), "%s", text);
		pCtx->StringToLocal(params[2], params[3], buffer);

		return strlen(text);
	} else {
		return 0;
	}
}

static cell_t TinyXml_Type(IPluginContext *pCtx, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	TiXmlNode *x;

	if ((err=g_pHandleSys->ReadHandle(hndl, g_TinyXmlHandle, &sec, (void **)&x)) != HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid TinyXml handle %x (error %d)", hndl, err);
	}

	if (!x)
	{
		pCtx->ThrowNativeError("TinyXml not found\n");
		return 0;
	}

	return x->Type();
}

static cell_t TinyXml_SetAttribute(IPluginContext *pCtx, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	TiXmlNode *x;

	if ((err=g_pHandleSys->ReadHandle(hndl, g_TinyXmlHandle, &sec, (void **)&x)) != HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid TinyXml handle %x (error %d)", hndl, err);
	}

	if (!x)
	{
		pCtx->ThrowNativeError("TinyXml not found\n");
		return 0;
	}

	if(x->Type() == TiXmlNode::TINYXML_ELEMENT) {
		TiXmlElement *y = x->ToElement();

		char *key;
		pCtx->LocalToString(params[2], &key);

		char *value;
		pCtx->LocalToString(params[3], &value);

		y->SetAttribute(key,value);
		return 1;
	}

	return 0;
}

static cell_t TinyXml_SetAttributeNum(IPluginContext *pCtx, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	TiXmlNode *x;

	if ((err=g_pHandleSys->ReadHandle(hndl, g_TinyXmlHandle, &sec, (void **)&x)) != HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid TinyXml handle %x (error %d)", hndl, err);
	}

	if (!x)
	{
		pCtx->ThrowNativeError("TinyXml not found\n");
		return 0;
	}

	if(x->Type() == TiXmlNode::TINYXML_ELEMENT) {
		TiXmlElement *y = x->ToElement();

		char *key;
		pCtx->LocalToString(params[2], &key);

		y->SetAttribute(key,params[3]);
		return 1;
	}

	return 0;
}

static cell_t TinyXml_SetAttributeFloat(IPluginContext *pCtx, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	TiXmlNode *x;

	if ((err=g_pHandleSys->ReadHandle(hndl, g_TinyXmlHandle, &sec, (void **)&x)) != HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid TinyXml handle %x (error %d)", hndl, err);
	}

	if (!x)
	{
		pCtx->ThrowNativeError("TinyXml not found\n");
		return 0;
	}

	if(x->Type() == TiXmlNode::TINYXML_ELEMENT) {
		TiXmlElement *y = x->ToElement();

		char *key;
		pCtx->LocalToString(params[2], &key);

		float value = sp_ctof(params[3]);

		y->SetDoubleAttribute(key,value);
		return 1;
	}

	return 0;
}

static cell_t TinyXml_AttributeName(IPluginContext *pCtx, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	TiXmlAttribute *x;

	if ((err=g_pHandleSys->ReadHandle(hndl, g_TinyXmlHandle, &sec, (void **)&x)) != HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid TinyXml handle %x (error %d)", hndl, err);
	}

	if (!x)
	{
		pCtx->ThrowNativeError("TinyXml not found\n");
		return 0;
	}

	char buffer[2048];
	snprintf(buffer, sizeof(buffer), "%s", x->Name());
	pCtx->StringToLocal(params[2], params[3], buffer);

	return strlen(x->Name());
}

static cell_t TinyXml_AttributeValue(IPluginContext *pCtx, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	TiXmlAttribute *x;

	if ((err=g_pHandleSys->ReadHandle(hndl, g_TinyXmlHandle, &sec, (void **)&x)) != HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid TinyXml handle %x (error %d)", hndl, err);
	}

	if (!x)
	{
		pCtx->ThrowNativeError("TinyXml not found\n");
		return 0;
	}

	char buffer[2048];
	snprintf(buffer, sizeof(buffer), "%s", x->Value());
	pCtx->StringToLocal(params[2], params[3], buffer);

	return strlen(x->Name());
}

static cell_t TinyXml_AttributeValueNum(IPluginContext *pCtx, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	TiXmlAttribute *x;

	if ((err=g_pHandleSys->ReadHandle(hndl, g_TinyXmlHandle, &sec, (void **)&x)) != HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid TinyXml handle %x (error %d)", hndl, err);
	}

	if (!x)
	{
		pCtx->ThrowNativeError("TinyXml not found\n");
		return 0;
	}
	int iValue;
	if(x->QueryIntValue(&iValue) == TIXML_SUCCESS) {
		return iValue;
	}

	return 0;
}

static cell_t TinyXml_AttributeValueFloat(IPluginContext *pCtx, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	TiXmlAttribute *x;

	if ((err=g_pHandleSys->ReadHandle(hndl, g_TinyXmlHandle, &sec, (void **)&x)) != HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid TinyXml handle %x (error %d)", hndl, err);
	}

	if (!x)
	{
		pCtx->ThrowNativeError("TinyXml not found\n");
		return 0;
	}

	double dValue;
	if(x->QueryDoubleValue(&dValue) == TIXML_SUCCESS) {
		float fValue = (float) dValue;
		return sp_ftoc(fValue);
	}

	return 0;
}

static cell_t TinyXml_Value(IPluginContext *pCtx, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	TiXmlNode *x;

	if ((err=g_pHandleSys->ReadHandle(hndl, g_TinyXmlHandle, &sec, (void **)&x)) != HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid TinyXml handle %x (error %d)", hndl, err);
	}

	if (!x)
	{
		pCtx->ThrowNativeError("TinyXml not found\n");
		return 0;
	}

	const char *text;
	text = x->Value();

	if(text == NULL)
		return 0;

	char buffer[2048];
	snprintf(buffer, sizeof(buffer), "%s", text);
	pCtx->StringToLocal(params[2], params[3], buffer);

	return strlen(text);
}

static cell_t TinyXml_Version(IPluginContext *pCtx, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	TiXmlNode *x;

	if ((err=g_pHandleSys->ReadHandle(hndl, g_TinyXmlHandle, &sec, (void **)&x)) != HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid TinyXml handle %x (error %d)", hndl, err);
	}

	if (!x)
	{
		pCtx->ThrowNativeError("TinyXml not found\n");
		return 0;
	}

	if(x->Type() == TiXmlNode::TINYXML_DECLARATION) {
		TiXmlDeclaration *y = x->ToDeclaration();

		char buffer[2048];
		snprintf(buffer, sizeof(buffer), "%s", y->Version());
		pCtx->StringToLocal(params[2], params[3], buffer);

		return strlen(y->Version());
	}

	return 0;
}

static cell_t TinyXml_Encoding(IPluginContext *pCtx, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	TiXmlNode *x;

	if ((err=g_pHandleSys->ReadHandle(hndl, g_TinyXmlHandle, &sec, (void **)&x)) != HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid TinyXml handle %x (error %d)", hndl, err);
	}

	if (!x)
	{
		pCtx->ThrowNativeError("TinyXml not found\n");
		return 0;
	}

	if(x->Type() == TiXmlNode::TINYXML_DECLARATION) {
		TiXmlDeclaration *y = x->ToDeclaration();

		char buffer[2048];
		snprintf(buffer, sizeof(buffer), "%s", y->Encoding());
		pCtx->StringToLocal(params[2], params[3], buffer);

		return strlen(y->Encoding());
	}

	return 0;
}

static cell_t TinyXml_Standalone(IPluginContext *pCtx, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	TiXmlNode *x;

	if ((err=g_pHandleSys->ReadHandle(hndl, g_TinyXmlHandle, &sec, (void **)&x)) != HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid TinyXml handle %x (error %d)", hndl, err);
	}

	if (!x)
	{
		pCtx->ThrowNativeError("TinyXml not found\n");
		return 0;
	}

	if(x->Type() == TiXmlNode::TINYXML_DECLARATION) {
		TiXmlDeclaration *y = x->ToDeclaration();

		char buffer[2048];
		snprintf(buffer, sizeof(buffer), "%s", y->Standalone());
		pCtx->StringToLocal(params[2], params[3], buffer);

		return strlen(y->Standalone());
	}

	return 0;
}

static cell_t TinyXml_IsCDATA(IPluginContext *pCtx, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	TiXmlNode *x;

	if ((err=g_pHandleSys->ReadHandle(hndl, g_TinyXmlHandle, &sec, (void **)&x)) != HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid TinyXml handle %x (error %d)", hndl, err);
	}

	if (!x)
	{
		pCtx->ThrowNativeError("TinyXml not found\n");
		return 0;
	}

	if(x->Type() == TiXmlNode::TINYXML_TEXT) {
		TiXmlText *y = x->ToText();
		return y->CDATA();
	}

	return 0;
}

static cell_t TinyXml_Parse(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	TiXmlDocument *x;

	if ((err=g_pHandleSys->ReadHandle(hndl, g_TinyXmlHandle, &sec, (void **)&x)) != HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid TinyXml handle %x (error %d)", hndl, err);
	}

	if (!x)
	{
		pCtx->ThrowNativeError("TinyXml data not found\n");
		return 0;
	}

	char *xmlstring;
	pCtx->LocalToString(params[2], &xmlstring);

	x->Parse(xmlstring);

	return 1;
}

static cell_t TinyXml_GetAttribute(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	TiXmlNode *x;

	if ((err=g_pHandleSys->ReadHandle(hndl, g_TinyXmlHandle, &sec, (void **)&x)) != HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid TinyXml handle %x (error %d)", hndl, err);
	}

	if (!x)
	{
		pCtx->ThrowNativeError("TinyXml data not found\n");
		return 0;
	}

	if(x->Type() == TiXmlNode::TINYXML_ELEMENT) {
		TiXmlElement *y = x->ToElement();
		char *xmlstring;
		pCtx->LocalToString(params[2], &xmlstring);

		const char *text;
		text = y->Attribute(xmlstring);
		if(text == NULL)
			return 0;

		char buffer[2048];
		snprintf(buffer, sizeof(buffer), "%s", text);
		pCtx->StringToLocal(params[3], params[4], buffer);

		return strlen(text);
	}

	return 1;
}

bool SMTinyXML::SDK_OnLoad(char *error, size_t err_max, bool late)
{
	sharesys->AddNatives(myself, tinyxml_natives);
	sharesys->RegisterLibrary(myself, "tinyxml");

	g_TinyXmlHandle = g_pHandleSys->CreateType("TinyXml", &g_TinyXmlHandler, 0, NULL, NULL, myself->GetIdentity(), NULL);

	return true;
}

void SMTinyXML::SDK_OnUnload()
{
	g_pHandleSys->RemoveType(g_TinyXmlHandle, myself->GetIdentity());
}

void TinyXmlHandler::OnHandleDestroy(HandleType_t type, void *object)
{
	TiXmlNode *x = (TiXmlNode *)object;
	x->Clear();
	//delete x;
}

const sp_nativeinfo_t tinyxml_natives[] =
{
	//Documents
	{"TinyXml_CreateDocument",	TinyXml_CreateDocument},
	{"TinyXml_LoadFile",	TinyXml_LoadFile},
	{"TinyXml_SaveFile",	TinyXml_SaveFile},
	{"TinyXml_Parse",	TinyXml_Parse},

	//Declaration
	{"TinyXml_CreateDeclaration",	TinyXml_CreateDeclaration},
	{"TinyXml_Version",	TinyXml_Version},
	{"TinyXml_Encoding",	TinyXml_Encoding},
	{"TinyXml_Standalone",	TinyXml_Standalone},

	//Elements
	{"TinyXml_CreateElement",	TinyXml_CreateElement},
	{"TinyXml_GetText",	TinyXml_GetText},
	{"TinyXml_RootElement",	TinyXml_RootElement},
	{"TinyXml_FirstChildElement",	TinyXml_FirstChildElement},
	{"TinyXml_NextSiblingElement",	TinyXml_NextSiblingElement},
	{"TinyXml_GetAttribute",	TinyXml_GetAttribute},

	//Nodes
	{"TinyXml_Type",	TinyXml_Type},
	{"TinyXml_FirstChild",	TinyXml_FirstChild},
	{"TinyXml_NextSibling",	TinyXml_NextSibling},
	{"TinyXml_Value",	TinyXml_Value},
	{"TinyXml_LinkEndChild",	TinyXml_LinkEndChild},

	//Attributes
	{"TinyXml_SetAttribute",	TinyXml_SetAttribute},
	{"TinyXml_SetAttributeNum",	TinyXml_SetAttributeNum},
	{"TinyXml_SetAttributeFloat",	TinyXml_SetAttributeFloat},
	{"TinyXml_FirstAttribute",	TinyXml_FirstAttribute},
	{"TinyXml_NextAttribute",	TinyXml_NextAttribute},
	{"TinyXml_AttributeValue",	TinyXml_AttributeValue},
	{"TinyXml_AttributeValueNum",	TinyXml_AttributeValueNum},
	{"TinyXml_AttributeValueFloat",	TinyXml_AttributeValueFloat},
	{"TinyXml_AttributeName",	TinyXml_AttributeName},

	//Others
	{"TinyXml_CreateText",	TinyXml_CreateText},
	{"TinyXml_CreateComment",	TinyXml_CreateComment},
	{"TinyXml_IsCDATA",	TinyXml_IsCDATA},

	{NULL,			NULL},
};

SMTinyXML g_Sample;		/**< Global singleton for extension's main interface */

SMEXT_LINK(&g_Sample);
