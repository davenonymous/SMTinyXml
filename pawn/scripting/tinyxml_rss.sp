#pragma semicolon 1
#include <sourcemod>
#include <tinyxml>

public OnPluginStart() {
	ReadTestRSS();
}

stock ReadTestRSS() {
	new Handle:hTxDoc = TinyXml_CreateDocument();
	TinyXml_LoadFile(hTxDoc, "./rss.xml");
	new Handle:hTxRoot = TinyXml_RootElement(hTxDoc);
	new Handle:hChannel = TinyXml_FirstChildElement(hTxRoot);

	new Handle:hNode = TinyXml_FirstChildElement(hChannel);

	while(hNode != INVALID_HANDLE) {
		new String:sBuffer[2048];
		TinyXml_Value(hNode, sBuffer, sizeof(sBuffer));

		if(StrEqual(sBuffer, "item")) {
			ExtractItem(hNode);
		}

		hNode = TinyXml_NextSiblingElement(hNode);
	}

	CloseHandle(hTxDoc);
}

stock ExtractItem(Handle:hItem) {
	new String:sBufferK[2048];
	new String:sBufferV[2048];


	new Handle:hNode = TinyXml_FirstChildElement(hItem);

	while(hNode != INVALID_HANDLE) {
		TinyXml_Value(hNode, sBufferK, sizeof(sBufferK));

		if(StrEqual(sBufferK,"title")) {
			TinyXml_GetText(hNode, sBufferV, sizeof(sBufferV));
			LogMessage("Title: %s (%i)", sBufferV, TinyXml_Type(hNode));
		}

		if(StrEqual(sBufferK,"description")) {
			TinyXml_GetText(hNode, sBufferV, sizeof(sBufferV));
			LogMessage("Desc: %s", sBufferV);
		}

		if(StrEqual(sBufferK,"link")) {
			TinyXml_GetText(hNode, sBufferV, sizeof(sBufferV));
			LogMessage("Link: %s", sBufferV);
		}

		hNode = TinyXml_NextSiblingElement(hNode);
	}
}