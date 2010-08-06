enum NodeType
{
	TINYXML_DOCUMENT = 0,
	TINYXML_ELEMENT,
	TINYXML_COMMENT,
	TINYXML_UNKNOWN,
	TINYXML_TEXT,
	TINYXML_DECLARATION,
	TINYXML_TYPECOUNT
};


native Handle:TinyXml_CreateDocument();
native Handle:TinyXml_CreateElement(String:name[]);
native Handle:TinyXml_CreateText(String:text[]);
native Handle:TinyXml_CreateComment(String:text[]);
native Handle:TinyXml_CreateDeclaration(String:version[]="1.0",String:encoding[]="",String:standalone[]="");

native TinyXml_Parse(Handle:doc, String:xmldata[]);
native TinyXml_LoadFile(Handle:doc, String:filename[]);
native TinyXml_SaveFile(Handle:doc, String:filename[]);

native TinyXml_LinkEndChild(Handle:blob, Handle:node);

native Handle:TinyXml_RootElement(Handle:doc);
native Handle:TinyXml_FirstChildElement(Handle:node, String:search[]="");
native Handle:TinyXml_NextSiblingElement(Handle:node, String:search[]="");
native Handle:TinyXml_FirstChild(Handle:node, String:search[]="");
native Handle:TinyXml_NextSibling(Handle:node, String:search[]="");

native Handle:TinyXml_FirstAttribute(Handle:node);
native Handle:TinyXml_NextAttribute(Handle:node);

native TinyXml_Type(Handle:node);
native TinyXml_Value(Handle:node, String:dest[], length);
native TinyXml_GetText(Handle:node, String:dest[], length);


native TinyXml_GetAttribute(Handle:element, String:key[], String:dest[], length);
native TinyXml_SetAttribute(Handle:element, String:key[], String:value[]);
native TinyXml_SetAttributeNum(Handle:element, String:key[], value);
native TinyXml_SetAttributeFloat(Handle:element, String:key[], Float:value);
native TinyXml_AttributeName(Handle:node, String:dest[], length);
native TinyXml_AttributeValue(Handle:node, String:dest[], length);
native TinyXml_AttributeValueNum(Handle:node);
native Float:TinyXml_AttributeValueFloat(Handle:node);

native TinyXml_Version(Handle:declaration, String:dest[], length);
native TinyXml_Encoding(Handle:declaration, String:dest[], length);
native TinyXml_Standalone(Handle:declaration, String:dest[], length);

native bool:TinyXml_IsCDATA(Handle:text);

stock KeyValuesToXML(Handle:kv, String:fileout[], bool:bPairsAsAttributes = true) {
	//Create Header
	new Handle:hTxDoc = TinyXml_CreateDocument();
	new Handle:hTxDecl = TinyXml_CreateDeclaration("1.0","","");
	new Handle:hTxCmt = TinyXml_CreateComment(" This file is an import from a keyvalues structure ");
	TinyXml_LinkEndChild(hTxDoc, hTxDecl);
	TinyXml_LinkEndChild(hTxDoc, hTxCmt);

	//Traverse the keyvalues structure
	IterateKeyValues(kv, hTxDoc, bPairsAsAttributes);

	//Save file
	TinyXml_SaveFile(hTxDoc, fileout);
	CloseHandle(hTxDecl);
	CloseHandle(hTxCmt);
	CloseHandle(hTxDoc);
}

IterateKeyValues(&Handle:kv, &Handle:doc, bool:bPairsAsAttributes = true)
{
	do
	{
		new String:sSection[255];
		KvGetSectionName(kv, sSection, sizeof(sSection));

		new String:sValue[255];
		KvGetString(kv, "", sValue, sizeof(sValue));
		if(!StrEqual(sValue, "")) {
			//We have a value, so this is a pair
			if(bPairsAsAttributes) {
				TinyXml_SetAttribute(doc, sSection, sValue);
			} else {
				new Handle:hTxEle = TinyXml_CreateElement(sSection);
				new Handle:hTxText = TinyXml_CreateText(sValue);
				TinyXml_LinkEndChild(hTxEle, hTxText);
				TinyXml_LinkEndChild(doc, hTxEle);
				CloseHandle(hTxEle);
				CloseHandle(hTxText);
			}
		} else {
			//We have no value, this must be another section
			new Handle:hTxEle = TinyXml_CreateElement(sSection);
			if (KvGotoFirstSubKey(kv, false))
			{

				IterateKeyValues(kv, hTxEle, bPairsAsAttributes);
				KvGoBack(kv);
			}
			TinyXml_LinkEndChild(doc, hTxEle);
			CloseHandle(hTxEle);
		}

	} while (KvGotoNextKey(kv, false));
}

/**
 * Do not edit below this line!
 */
public Extension:__ext_tinyxml = 
{
	name = "TinyXml",
	file = "tinyxml.ext",
#if defined AUTOLOAD_EXTENSIONS
	autoload = 1,
#else
	autoload = 0,
#endif
#if defined REQUIRE_EXTENSIONS
	required = 1,
#else
	required = 0,
#endif
};