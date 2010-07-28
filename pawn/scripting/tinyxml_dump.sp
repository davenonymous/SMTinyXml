#pragma semicolon 1
#include <sourcemod>
#include <tinyxml>

public OnPluginStart() {
	DumpXML();
}

stock DumpXML() {
	new Handle:hTxDoc = TinyXml_CreateDocument();
	TinyXml_LoadFile(hTxDoc, "./example4.xml");

	RecursiveDump(hTxDoc);
}

stock RecursiveDump(Handle:hNode, level = 0) {
	new String:padding[32];
	for(new i = 0; i < level; i++) {
		StrCat(padding, sizeof(padding), "  ");
	}

	while(hNode != INVALID_HANDLE) {
		new NodeType:iType = NodeType:TinyXml_Type(hNode);
		new String:sType[16];
		TypeToString(iType, sType, sizeof(sType));

		new String:sValue[64];

		if(iType == TINYXML_ELEMENT) {
			TinyXml_Value(hNode, sValue, sizeof(sValue));
			LogMessage("%s%s: %s", padding, sType, sValue);
			DumpAttributes(hNode, level+1);
			RecursiveDump(TinyXml_FirstChild(hNode), level+1);
		}

		if(iType == TINYXML_TEXT) {
			TinyXml_Value(hNode, sValue, sizeof(sValue));
			LogMessage("%s%s: %s", padding, sType, sValue);
		}

		if(iType == TINYXML_DOCUMENT) {
			RecursiveDump(TinyXml_RootElement(hNode));
		}

		if(iType == TINYXML_COMMENT) {
			TinyXml_Value(hNode, sValue, sizeof(sValue));
			LogMessage("%s%s: %s", padding, sType, sValue);
		}

		hNode = TinyXml_NextSibling(hNode);
	}
}

stock DumpAttributes(Handle:hElement, level = 0) {
	new String:padding[32];
	for(new i = 0; i < level; i++) {
		StrCat(padding, sizeof(padding), "  ");
	}

	new Handle:hAttribute = TinyXml_FirstAttribute(hElement);

	while(hAttribute != INVALID_HANDLE) {
		new String:sName[64];
		new String:sValue[64];

		TinyXml_AttributeName(hAttribute, sName, sizeof(sName));
		TinyXml_AttributeValue(hAttribute, sValue, sizeof(sValue));

		LogMessage("%s%s: %s", padding, sName, sValue);

		hAttribute = TinyXml_NextAttribute(hAttribute);
	}
}

stock TypeToString(NodeType:iType, String:buff[], sizebuff) {
	if(iType == TINYXML_DOCUMENT)
		strcopy(buff,sizebuff, "Document");
	else if(iType == TINYXML_ELEMENT)
		strcopy(buff,sizebuff, "Element");
	else if(iType == TINYXML_COMMENT)
		strcopy(buff,sizebuff, "Comment");
	else if(iType == TINYXML_UNKNOWN)
		strcopy(buff,sizebuff, "Unknown");
	else if(iType == TINYXML_TEXT)
		strcopy(buff,sizebuff, "Text");
	else if(iType == TINYXML_DECLARATION)
		strcopy(buff,sizebuff, "Declaration");
	else if(iType == TINYXML_TYPECOUNT)
		strcopy(buff,sizebuff, "Typecount");
}