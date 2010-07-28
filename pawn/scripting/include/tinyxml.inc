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


native TinyXml_Version();

native Handle:TinyXml_CreateDocument();
native Handle:TinyXml_CreateElement(String:name[]);
native Handle:TinyXml_CreateText(String:text[]);
native Handle:TinyXml_CreateComment(String:text[]);
native Handle:TinyXml_CreateDeclaration(String:version[]="1.0",String:encoding[]="",String:standalone[]="");

native TinyXml_LoadFile(Handle:doc, String:filename[]);
native TinyXml_SaveFile(Handle:doc, String:filename[]);

native TinyXml_LinkEndChild(Handle:blob, Handle:node);

native Handle:TinyXml_RootElement(Handle:doc);
native Handle:TinyXml_FirstChildElement(Handle:node);
native Handle:TinyXml_NextSiblingElement(Handle:node);
native Handle:TinyXml_FirstChild(Handle:node);
native Handle:TinyXml_NextSibling(Handle:node);

native Handle:TinyXml_FirstAttribute(Handle:node);
native Handle:TinyXml_NextAttribute(Handle:node);


native TinyXml_Type(Handle:node);
native TinyXml_Value(Handle:node, String:dest[], length);
native TinyXml_GetText(Handle:node, String:dest[], length);

native TinyXml_AttributeValue(Handle:node, String:dest[], length);
native TinyXml_AttributeName(Handle:node, String:dest[], length);


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