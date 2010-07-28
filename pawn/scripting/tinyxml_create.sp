#pragma semicolon 1
#include <sourcemod>
#include <tinyxml>

public OnPluginStart() {
	CreateTestXML();
}

stock CreateTestXML() {
	new Handle:hTxDoc = TinyXml_CreateDocument();
	new Handle:hTxDecl = TinyXml_CreateDeclaration("1.0","","");
	new Handle:hTxCmt = TinyXml_CreateComment("This is a comment");
	new Handle:hTxEle = TinyXml_CreateElement("Hello");
	new Handle:hTxEleSub = TinyXml_CreateElement("Sublevel");
	new Handle:hTxText = TinyXml_CreateText("HEEEEEEEEELO WORLD");
	TinyXml_LinkEndChild(hTxEleSub, hTxText);
	TinyXml_LinkEndChild(hTxEle, hTxEleSub);

	TinyXml_LinkEndChild(hTxDoc, hTxDecl);
	TinyXml_LinkEndChild(hTxDoc, hTxEle);
	TinyXml_LinkEndChild(hTxDoc, hTxCmt);

	TinyXml_SaveFile(hTxDoc, "./test_sm.xml");

	CloseHandle(hTxDoc);
}