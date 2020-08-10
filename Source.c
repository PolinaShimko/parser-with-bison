#define _CRT_SECURE_NO_WARNINGS
#include "malloc.h"
#include <locale.h>
#include "string.h"
#include "y_tab.h"
#include "stdio.h"
#include "stdlib.h"
#define alloca malloc
#define STRING 10000
#define TAGNUM 122
#define NESTEDNUM 30
#define SINGLENUM 20

int line = 0;
int flag_tag_start[TAGNUM], flag_tag_end[TAGNUM];
FILE *f, *f2;
char str[STRING];
int m;
int SCRIPT = 0;
int flagHTML = 0, flagHEAD = 0, flagTITLE = 0, flagBODY = 0;
int htmlerror = 0, headerror = 0, titleerror = 0, bodyerror = 0;

typedef struct list
{
	char tag[25];
	struct list *ptr;
} lst;
lst *head = NULL;
char* single[] =
{
	"area",
	"base",
	"basefont",
	"bgsound",
	"br",
	"col",
	"DOCTYPE",
	"command",
	"embed",
	"hr",
	"img",
	"input",
	"isindex",
	"keygen",
	"link",
	"meta",
	"param",
	"source",
	"track",
	"wbr"
};


//char* (nested[][2]) = //теги (1), которые обязательно должны быть вложены в тег (2)
//{
//	{ "area","map" },
//	{ "base","head" },
//	{ "meta","head" },
//	{ "meta","script" },
//	{ "title","head" },
//	{ "caption","table" },
//	{ "col","colgroup" },
//	{ "dd","dl" },
//	{ "dt","dl" },
//	{ "input","form" },
//	{ "input","div" },
//	{ "legend","fieldset" },
//	{ "li","ul" }, 
//	{ "li","ol" },
//	{ "li","menu" },
//	{ "param","object" },
//	{ "tbody","table" },
//	{ "td","table" },
//	{ "tfoot","table" },
//	{ "th","table" },
//	{ "thead","table" },
//	{ "tr","table" },
//	{ "td","tr" },
//	{ "th","tr" },
//	{ "tr","tbody" },
//	{ "tr","tfoot" },
//	{ "tr","thead" },
//	{ "b","p" },
//	{ "b","div" },
//	{ "b","th" },
//	{ "b","tr" },
//	{ "b","td" }
//};

// "accesskey", "class", "contenteditable", "contextmenu", "draggablr", "dropzone", "dir", "hidden", "id", "lang", "spellcheck", "style", "tabindex", "title", "translate", "onblur", "onchange", "onclick", "ondblclick", "onfocus", "onkeydown", "onkeypress", "onkeyup", "onmousedown", "onmousemove", "onmouseout", "onmouseover", "onmouseup" },
extern char* (tag_attribute[150][60]) = {
	{ "DOCTYPE","html" },
	{ "a","download","href","target","hreflang","rel","type","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "abbr","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },//
	{ "address", "accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },//
	{ "area","alt","coords","download","href","hreflang","rel","shape","target","type","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },//
	{ "article","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },//
	{ "aside","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },//
	{ "audio","autoplay","crossorigin","controls","loop","preload","mediagroup","muted","src","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onclick","ondblclick","onerror","onfocus","onkeydown","onkeypress","onkeyup","onload","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup","onreset","onselect","onsubmit","onunload" },//
	{ "base","href","target","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },//
	{ "b","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },//
	{ "bdi","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },//
	{ "bdo","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },//
	{ "blockquote","cite","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },//
	{ "body","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onafterprint","onmessage","onoffline","ononline","onpagehide","onpageshow","onpopstate","onstorage","onbeforeprint","onhashchange","onbeforeunload","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onunload","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "br","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "button","autofocus","disabled","form","formaction","formenctype","formmethod","formnovalidate","formtarget","name","type","value","menu","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "canvas","height","width","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "caption","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "cite","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "code","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "col","span","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "colgroup","span","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "data","value","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "datalist","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "dd","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "del","cite","datetime","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "details","open","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "dfn","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "dialog","open","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "div","accesskey","class","draggable","controlwidth","controlheight","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "dl","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "dt","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "em","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "embed","height","src","type","width","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "fieldset","face","color","disabled","form","name","size","accesskey","class","dir","id","lang","style","onblur","onclick","ondblclick","onerror","onfocus","onkeydown","onkeypress","onkeyup","onload","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup","onreset","onselect","onsubmit","onunload" },
	{ "figure","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "figcaption","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "footer","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "form","accept-charset","action","autocomplete","enctype","method","name","novalidate","target","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "h1","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "h2","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "h3","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "h4","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "h5","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "h6","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "header","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "head","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "hr","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "html","manifest","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "i","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "iframe","frameborder","height","name","src","srcdoc","sandbox","width","allowtransparency","hspace","marginheight","marginwidth","scrolling","style","title","vspace","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup"},
	{ "img","alt","border","draggable","crossorigin","height","ismap","name","src","usemap","width","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "input","accept","alt","autocomplete","autofocus","checked","dirname","disabled","form","formaction","formenctype","formmethod","formnovalidate","formtarget","height","inputmode","list","max","min","maxlength","multiple","name","pattern","placeholder","readonly","required","size","src","step","type","value","width","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "ins","cite","datetime","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "keygen","autofocus","challenge","disabled","form","keytype","name","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "kbd","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "label","form","for","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "legend","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "li","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "link","href","crossorigin","hreflang","media","rel","sizes","type","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate" },
	{ "main","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate" },
	{ "mark","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "map","name","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "meta","content","http-equiv","name","charset","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
//	{ "menu","disabled","type","compact","class","dir","id","lang","style","title","onblur","onclick","ondblclick","onerror","onfocus","onkeydown","onkeypress","onkeyup","onload","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup","onreset","onselect","onsubmit","onunload" },
	{ "meter","value","min","max","low","high","optimum","accesskey", "class", "contenteditable", "contextmenu", "draggablr", "dropzone", "dir", "hidden", "id", "lang", "spellcheck", "style", "tabindex", "title", "translate", "onblur", "onchange", "onclick", "ondblclick", "onfocus", "onkeydown", "onkeypress", "onkeyup", "onmousedown", "onmousemove", "onmouseout", "onmouseover", "onmouseup" },
	{ "nav","accesskey","class","contenteditable","contextmenu","dir","hidden","id","lang","style","spellcheck","tabindex","title","onblur","onblur","onclick","ondblclick","onerror","onfocus","onkeydown","onkeypress","onkeyup","onload","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup","onreset","onselect","onsubmit","onunload" },
	{ "noscript","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "object","typemustmatch","usemap","form","data","height","name","type","width","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "ol","reversed","start","type","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "optgroup","label","disabled","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "option","selected","value","label","disabled","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "output","for","form","name","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "p","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "param","name","value","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "pre","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "progress","value","max","accesskey", "class", "contenteditable", "contextmenu", "draggablr", "dropzone", "dir", "hidden", "id", "lang", "spellcheck", "style", "tabindex", "title", "translate", "onblur", "onchange", "onclick", "ondblclick", "onfocus", "onkeydown", "onkeypress", "onkeyup", "onmousedown", "onmousemove", "onmouseout", "onmouseover", "onmouseup" },
	{ "rb","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{"rt","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "rtc","accesskey", "class", "contenteditable", "contextmenu", "draggablr", "dropzone", "dir", "hidden", "id", "lang", "spellcheck", "style", "tabindex", "title", "translate", "onblur", "onchange", "onclick", "ondblclick", "onfocus", "onkeydown", "onkeypress", "onkeyup", "onmousedown", "onmousemove", "onmouseout", "onmouseover", "onmouseup" },
	{"rp","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "ruby","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "q","cite","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "samp","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "script","src","type","charset","crossorigin","async","defer","runat","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "s","accesskey", "class", "contenteditable", "contextmenu", "draggablr", "dropzone", "dir", "hidden", "id", "lang", "spellcheck", "style", "tabindex", "title", "translate", "onblur", "onchange", "onclick", "ondblclick", "onfocus", "onkeydown", "onkeypress", "onkeyup", "onmousedown", "onmousemove", "onmouseout", "onmouseover", "onmouseup" },
	{ "section","accesskey", "class", "contenteditable", "contextmenu", "draggablr", "dropzone", "dir", "hidden", "id", "lang", "spellcheck", "style", "tabindex", "title", "translate", "onblur", "onchange", "onclick", "ondblclick", "onfocus", "onkeydown", "onkeypress", "onkeyup", "onmousedown", "onmousemove", "onmouseout", "onmouseover", "onmouseup" },
	{ "select","autofocus","disabled","form","required","multiple","name","size","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "small","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "source","src","type","accesskey", "class", "contenteditable", "contextmenu", "draggablr", "dropzone", "dir", "hidden", "id", "lang", "spellcheck", "style", "tabindex", "title", "translate", "onblur", "onchange", "onclick", "ondblclick", "onfocus", "onkeydown", "onkeypress", "onkeyup", "onmousedown", "onmousemove", "onmouseout", "onmouseover", "onmouseup" },
	{ "span","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "strong","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "style","media","type","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "sub","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "sup","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "table","border","sortable","width","background","bgcolor","bordercolor","bordercolordark","bordercolorlight","cellpadding","cellspacing","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "tbody","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "td","colspan","rowspan","headers","valign","background","bordercolor","bordercolordark","bordercolorlight","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "template","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "textarea","autocomplete","autofocus","cols","dirname","disabled","form","name","inputmode","maxlenght","minlenght","rows","placeholder","readonly","required","wrap","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "tfoot","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "th","colspan","rowspan","headers","scope","abbr","sorted","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "title","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "time", "datetime","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "thead","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "tr","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "track","kind","src","srclang","label","default","accesskey", "class", "contenteditable", "contextmenu", "draggablr", "dropzone", "dir", "hidden", "id", "lang", "spellcheck", "style", "tabindex", "title", "translate", "onblur", "onchange", "onclick", "ondblclick", "onfocus", "onkeydown", "onkeypress", "onkeyup", "onmousedown", "onmousemove", "onmouseout", "onmouseover", "onmouseup" },
	{ "u","accesskey", "class", "contenteditable", "contextmenu", "draggablr", "dropzone", "dir", "hidden", "id", "lang", "spellcheck", "style", "tabindex", "title", "translate", "onblur", "onchange", "onclick", "ondblclick", "onfocus", "onkeydown", "onkeypress", "onkeyup", "onmousedown", "onmousemove", "onmouseout", "onmouseover", "onmouseup" },
	{ "ul","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "var","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup" },
	{ "video","crossorigin","autoplay","controls","height","loop","muted","poster","preload","mediagroup","width","src","accesskey","class","contenteditable","contextmenu","draggablr","dropzone","dir","hidden","id","lang","spellcheck","style","tabindex","title","translate""onblur","onclick","ondblclick","onerror","onfocus","onkeydown","onkeypress","onkeyup","onload","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup","onreset","onselect","onsubmit","onunload" },
	{ "wbr", "accesskey", "class", "contenteditable", "contextmenu", "draggablr", "dropzone", "dir", "hidden", "id", "lang", "spellcheck", "style", "tabindex", "title", "translate", "onblur", "onchange", "onclick", "ondblclick", "onfocus", "onkeydown", "onkeypress", "onkeyup", "onmousedown", "onmousemove", "onmouseout", "onmouseover", "onmouseup" },
	{ "svg", "x", "y","width", "height", "viewBox", "version",  "accesskey", "class", "contenteditable", "contextmenu", "draggablr", "dropzone", "dir", "hidden", "id", "lang", "spellcheck", "style", "tabindex", "title", "translate", "onblur", "onchange", "onclick", "ondblclick", "onfocus", "onkeydown", "onkeypress", "onkeyup", "onmousedown", "onmousemove", "onmouseout", "onmouseover", "onmouseup" },
	{ "desc" },
	{ "path","d","fill","stroke","stroke-width","accesskey", "class", "contenteditable", "contextmenu", "draggablr", "dropzone", "dir", "hidden", "id", "lang", "spellcheck", "style", "tabindex", "title", "translate", "onblur", "onchange", "onclick", "ondblclick", "onfocus", "onkeydown", "onkeypress", "onkeyup", "onmousedown", "onmousemove", "onmouseout", "onmouseover", "onmouseup" },
	{ "rect","x","y","height","width","fill","stroke","stroke-width","accesskey", "class", "contenteditable", "contextmenu", "draggablr", "dropzone", "dir", "hidden", "id", "lang", "spellcheck", "style", "tabindex", "title", "translate", "onblur", "onchange", "onclick", "ondblclick", "onfocus", "onkeydown", "onkeypress", "onkeyup", "onmousedown", "onmousemove", "onmouseout", "onmouseover", "onmouseup" },
	{ "circle","cx","cy","r","fill","stroke","stroke-width","accesskey", "class", "contenteditable", "contextmenu", "draggablr", "dropzone", "dir", "hidden", "id", "lang", "spellcheck", "style", "tabindex", "title", "translate", "onblur", "onchange", "onclick", "ondblclick", "onfocus", "onkeydown", "onkeypress", "onkeyup", "onmousedown", "onmousemove", "onmouseout", "onmouseover", "onmouseup" },
	{ "ellipse","cx","cy","rx","ry","r","fill","stroke","stroke-width","transform","accesskey", "class", "contenteditable", "contextmenu", "draggablr", "dropzone", "dir", "hidden", "id", "lang", "spellcheck", "style", "tabindex", "title", "translate", "onblur", "onchange", "onclick", "ondblclick", "onfocus", "onkeydown", "onkeypress", "onkeyup", "onmousedown", "onmousemove", "onmouseout", "onmouseover", "onmouseup" },
	{ "line","x","y","x1","x2","y1","y2","fill","stroke","stroke-width","accesskey", "class", "contenteditable", "contextmenu", "draggablr", "dropzone", "dir", "hidden", "id", "lang", "spellcheck", "style", "tabindex", "title", "translate", "onblur", "onchange", "onclick", "ondblclick", "onfocus", "onkeydown", "onkeypress", "onkeyup", "onmousedown", "onmousemove", "onmouseout", "onmouseover", "onmouseup" },
	{ "polyline","points","fill","stroke","stroke-width","accesskey", "class", "contenteditable", "contextmenu", "draggablr", "dropzone", "dir", "hidden", "id", "lang", "spellcheck", "style", "tabindex", "title", "translate", "onblur", "onchange", "onclick", "ondblclick", "onfocus", "onkeydown", "onkeypress", "onkeyup", "onmousedown", "onmousemove", "onmouseout", "onmouseover", "onmouseup" },
	{ "polygon","points","fill","stroke","stroke-width","accesskey", "class", "contenteditable", "contextmenu", "draggablr", "dropzone", "dir", "hidden", "id", "lang", "spellcheck", "style", "tabindex", "title", "translate", "onblur", "onchange", "onclick", "ondblclick", "onfocus", "onkeydown", "onkeypress", "onkeyup", "onmousedown", "onmousemove", "onmouseout", "onmouseover", "onmouseup" },
	{ "text","x","y","height","width","fill","stroke","stroke-width","accesskey", "class", "contenteditable", "contextmenu", "draggablr", "dropzone", "dir", "hidden", "id", "lang", "spellcheck", "style", "tabindex", "title", "translate", "onblur", "onchange", "onclick", "ondblclick", "onfocus", "onkeydown", "onkeypress", "onkeyup", "onmousedown", "onmousemove", "onmouseout", "onmouseover", "onmouseup" },
	{ "g","fill","stroke","stroke-width","accesskey", "class", "contenteditable", "contextmenu", "draggablr", "dropzone", "dir", "hidden", "id", "lang", "spellcheck", "style", "tabindex", "title", "translate", "onblur", "onchange", "onclick", "ondblclick", "onfocus", "onkeydown", "onkeypress", "onkeyup", "onmousedown", "onmousemove", "onmouseout", "onmouseover", "onmouseup" }
};
int yyerror(const char *err)
{
	printf("\nERROR: %s. Line: %d\n", err, line + 1);
	return -1;
}

extern int TestAttribute(char* a, char* b)
{
	int i = 0, j = 1;
	if (b[i] == 'd' && b[i + 1] == 'a' && b[i + 2] == 't' && b[i + 3] == 'a' && b[i + 4] == '-') return 1; //для атрибута data-
	if (b[i] == 'a' && b[i + 1] == 'r' && b[i + 2] == 'i' && b[i + 3] == 'a' && b[i + 4] == '-') return 1; //для атрибута area-
	if (b[i] == 'f' && b[i + 1] == 'o' && b[i + 2] == 'n' && b[i + 3] == 't' && b[i + 4] == '-') return 1; //для атрибута font-
	if (b[i] == 'x' && b[i + 1] == 'm' && b[i + 2] == 'l' && b[i + 3] == 'n' && b[i + 4] == 's') return 1; //для атрибута xmlns:
	if (b[i] == 'x' && b[i + 1] == 'm' && b[i + 2] == 'l') return 1; //для атрибута xml:
	if (!strcmp("role", (const char*)b)) return 1;
	while (i < TAGNUM)
	{
		if (strcmp(tag_attribute[i][0], (const char*)a) == 0) //если тег в списке тегов найден
		{
			while (tag_attribute[i][j] > 0)
			{

				if (strcmp(tag_attribute[i][j], (const char*)b) == 0)
					return 1;
				j++;
			}
			return 0;
		}
		i++;
	}
	return 0;
}

extern void print_error_line()
{
	printf("Line: %d\n\n", line+1);
}
extern void fprint_error_line()
{
	fprintf(f2, "Line: %d\n\n", line+1);
}

void comment()
{
	int i = 0;
	char c3;
	c3 = (char)fgetc(f);
	while (c3 != EOF)
	{
		if (c3 == '\n')
			line++;
		if (c3 == '-')
		{
			int i = 0;
			while (c3 == '-')
			{
				i++;
				c3 = (char)fgetc(f);
			}
			if (i > 1 && c3 == '>')
				return;
		}
		else
			c3 = (char)fgetc(f); //пропускаем, что внутри комментария
	}

}


void ParseTagAndAttribute(char* str)
{
	int flag = 0, flag2 = 0; 
	int i = 0, j;
	while (str[i] != '>')
	{
		if (((str[i] >= 'a') && (str[i] <= 'z')) || ((str[i] >= '1') && (str[i] <= '9')) || (str[i] == '!') /*Для Doctype*/) //если просто тег
		{
			if (str[i] == '!') i++; //пропускаем ! знак
			int j = i;
			flag2 = 1;
			while ((str[i] >= 'a') && (str[i] <= 'z') || (str[i] >= '1') && (str[i] <= '9'))
			{
				yylval.tagtype.name[i - j] = str[i]; //запоминаем имя тега
				i++;
				flag = 1;  // помечаем, что тег был, чтобы парсить атрибуты
			}
			
			i = i - 1;
		}
		else if (str[i] == '/') //если закрывающий тег 
		{
			//j = i;
			while (((str[i] >= 'a') && (str[i] <= 'z')) || ((str[i] >= '1') && (str[i] <= '9')) || (str[i] == '/'))
			{
				yylval.tagtype.name[i] = str[i];
				i++;
			}
			break;
		}
		else if (flag == 1) //тег уже пропарсили; парсим атрибуты, если они есть
		{
			yylval.tagtype.number = 0;
			while (str[i] != '>') //пока все атрибуты не пропарсим
			{

			//	flag2 = 0;
			//	int j = i;
			//	yylval.tagtype.number++;
			//	while (str[i] == ' ') i++;
				int v;
				int t = i; //запоминаем место, где начинается атрибут
							 //					
				while (str[i] != '=' && str[i] != '>')  //ищем равно или закрывающую скобку (когда атрибут без значения), чтобы считывать имя атрибута
					i++;
				if (str[i] == '=' || str[i] == ' ' || str[i] == '>')
				{
					flag2 = 0;
					yylval.tagtype.number++;
					while (str[t] == ' ' || str[t] == '\n')
					{
						if (str[t] == '\n') line++;
						t++; //пропускаем пробелы или перевод строки между атрибутами
					}
						
					i = t;
					j = i;
					while ((str[i] >= 'a') && (str[i] <= 'z') || (str[i] == '-') || (str[i] == '_') || (str[i] == ':'))
					{
						v = yylval.tagtype.number - 1;
						yylval.tagtype.attribute[v][i - j] = str[i]; //запоминаем имя атрибута
						i++;
					}
				//	if (str[i] == '=') i++; //пропускаем '='
					while (((str[i] != '"') && (str[i] != '>')) || (flag2 == 0)) //пропускаем само значение атрибута
					{
						if (str[i] == '"') flag2 = 1;
						i++;
					}
					v = yylval.tagtype.number - 1;
					if (yylval.tagtype.attribute[v][0] == 0) //если атрибутов нет
						yylval.tagtype.number--;

					i++; //переход к следующему символу
				}
			} 
			break;
		}
		i++;
	}
	if (!strcmp(yylval.tagtype.name,"script")) SCRIPT = 1;
	if (yylval.tagtype.name[0] == '/')
	{
		for (i = 0; i < TAGNUM; i++)
		{
			if (strcmp(tag_attribute[i][0], yylval.tagtype.name + 1) == 0)
			{
			//	flag_tag_end[i]++;
				break;
			}
		}
	}
	else
	{
		for (i = 0; i < TAGNUM; i++)
			if (strcmp(tag_attribute[i][0], yylval.tagtype.name) == 0)
			{
				break;
			}
	}
	if (i == TAGNUM)
	{
		printf_s("ERROR: Invalid tag %s. Line : %d\n\n", yylval.tagtype.name, line);
	}


}

void Parse(char c, int *flag)
{
	char c2 = (char)getc(f);
	char c3;
	if (c == '<' && c2 == '!') //встретили комментарий
	{
		if (SCRIPT==0)	
			comment();
		//if (c2 == '!') 
		//{
		//	c3 = fgetc(f);
	}
	if ((c == '<') && ((c2 == '/') || ((c2 >= 'a') && (c2 <= 'z')))) //встретили строку (тег с атрибутами или без)
	{
		//if (c2 == 'n' || c2 == 'i')
		//{
		//	c3 = (char)getc(f);
		//	if (c3 == '.') { *flag = 2; }
		//	else
		//	{
				int i = 0;
		//		ungetc(c3, f);
				ungetc(c2, f);
				for (;;)
				{
					c = (char)fgetc(f);
					if (c == '<') {
						ungetc(c, f);
						break;
					}
					str[i] = c; //записываем все, что между '<' и '>' (и '>' тоже) в массив для дальнейшей обработки тегов и атрибутов
					if (c == '\n')
						line++;
					i++;
					if (c == '>') break;
				}
				*flag = 1;
	}
	else
	{
		if (c == '\n')
			line++;
		ungetc(c2, f);
		*flag = 2;
	}
}

int yylex()
{
	int flag = 0, flag2 = 0;
	char c;
	int k;

	yylval.tagtype.number = 0;
	 m = 0;
	x:
		for (int i = 0; i < STRING; i++)
			str[i] = 0;
		if (SCRIPT == 1)
		{
			while (strcmp(str, "/script>"))
			{
				for (int i = 0; i < STRING; i++)
					str[i] = 0;
				c = (char)fgetc(f);
				if (c == EOF)	return 0;
				Parse(c, &flag);//тег или комментарий?
			} 
			goto y;
		}
		c = (char)fgetc(f);
		if (c == EOF)	return 0;
		Parse(c, &flag);//тег или комментарий?


	y:
	SCRIPT = 0;
	if (flag == 1) //обработка тегов и атрибутов
	{
		//каждый раз необходимо обнулить массивы имен тегов и атрибутов
		for (int i = 0; i< 25; i++)
			yylval.tagtype.name[i] = 0;
		for (k = 0; k < 50; k++)
			for (int i = 0; i < 100; i++)
				yylval.tagtype.attribute[k][i] = 0;


	ParseTagAndAttribute(str);
	


	if ((!strcmp("html", (const char*)yylval.tagtype.name)) && flagHTML)
	{
		printf_s("ERROR: Invalid 'html' tag location. Line: %d\n\n", line + 1);
		htmlerror = 1;
		goto x;
	}
	else if (!strcmp("head", (const char*)yylval.tagtype.name) && flagHEAD)
	{
		printf_s("ERROR: Invalid 'head' tag location. Line: %d\n\n", line + 1);
		headerror = 1;
		goto x;
	}
	else if (!strcmp("title", (const char*)yylval.tagtype.name) && flagTITLE)
	{
		printf_s("ERROR: Invalid 'title' tag location. Line: %d\n\n", line + 1);
		titleerror = 1;
		goto x;
	}
	else if (!strcmp("body", (const char*)yylval.tagtype.name) && flagBODY)
	{
		printf_s("ERROR: Invalid 'body' tag location. Line: %d\n\n", line + 1);
		bodyerror = 1;
		goto x;
	}


	if ((!strcmp("/html", (const char*)yylval.tagtype.name)) && flagHTML && htmlerror)
	{
		printf_s("ERROR: Invalid '/html' tag location. Line: %d\n\n", line + 1);
		goto x;
	}
	else if (!strcmp("/head", (const char*)yylval.tagtype.name) && flagHEAD && headerror)
	{
		printf_s("ERROR: Invalid '/head' tag location. Line: %d\n\n", line + 1);
		goto x;
	}
	else if (!strcmp("/title", (const char*)yylval.tagtype.name) && flagTITLE && titleerror)
	{
		printf_s("ERROR: Invalid '/title' tag location. Line: %d\n\n", line + 1);
		goto x;
	}
	else if (!strcmp("/body", (const char*)yylval.tagtype.name) && flagBODY && bodyerror)
	{
		printf_s("ERROR:Invalid '/body' tag location. Line: %d\n\n", line + 1);
		goto x;
	}

	if (!strcmp("html", (const char*)yylval.tagtype.name) && !flagHTML)
		if (!flagHEAD && !flagTITLE && !flagBODY)  flagHTML = 1;

	if (!strcmp("head", (const char*)yylval.tagtype.name) && !flagHEAD)
		if (flagHTML && !flagTITLE && !flagBODY)  flagHEAD = 1;

	if (!strcmp("title", (const char*)yylval.tagtype.name) && !flagTITLE)
		if (flagHEAD && flagHTML && !flagBODY)  flagTITLE = 1;

	if (!strcmp("body", (const char*)yylval.tagtype.name) && !flagBODY)
		if (flagHEAD && flagTITLE && flagHTML)  flagBODY = 1;

	

	if (yylval.tagtype.number == 0) //если тег без атрибутов
		{
			int k, i;
			int R1 = 0;
		
			flag2 = 0;
			
			for (int i = 0; i < SINGLENUM; i++) 
				if (strcmp((const char*)yylval.tagtype.name, single[i]) == 0) //если тег одиночный
				{
					R1 = Single_Tag_No_Attribute;
					flag2 = 1;
				}

			//если не закрывающий, не одиночный и не DOCTYPE (т.е. если это новые открывающие теги)
			// тогда заполняем список подряд открывающимися тегами
			if (yylval.tagtype.name[0] != '/' && strcmp((const char*)yylval.tagtype.name, "DOCTYPE") && !flag2) 
			{
				if (head == NULL) //если список еще пуст
				{
					int i = 0;
					head = (lst*)malloc(sizeof(lst)); //выделение памяти под голову списка
					for (i = 0; i < 25; i++)
						head->tag[i] = yylval.tagtype.name[i];
					head->ptr = NULL;
				}
				else //иначе там уже что-то лежит, записываем в список тег в качестве следующего элемента
					 //и т.д.
				{
					lst *temp, *t;
					temp = head;
					while (temp->ptr != NULL) temp = temp->ptr; //записываем в temp все теги, которые уже есть в списке 
					temp->ptr = (lst*)malloc(sizeof(lst));
					t = temp;
					temp->ptr->ptr = NULL;
					int i = 0;
					for (i = 0; i < 25; i++) temp->ptr->tag[i] = yylval.tagtype.name[i];
					t->ptr = temp->ptr;
				}
			}
			else if (strcmp((const char*)yylval.tagtype.name, "DOCTYPE")) //если закрывающий или одиночный и НЕ DOCTYPE
			{
				lst *temp = head, *t = head;
				while (temp->ptr != NULL) //на каждом шаге извлекаем в обратном порядке из списка теги, пока не извлечем все
				{
					t = temp;
					temp = temp->ptr;
				}
				if (flag2) //если одиночный
				{
				/*	i = 0;
					char test = NULL;
					while (i++ < NESTEDNUM)
						if (strcmp((const char*)yylval.tagtype.name, nested[i - 1][0]) == 0)
						{
							if (!strcmp((const char*)t->tag, nested[i - 1][1]))
								break;
							else
							{
								char test = nested[i - 1][1];
								//i++;
							}
						}
					if (test != NULL) printf_s("ERROR: Nesting of tags: '%s', '%s'. Line %d\n\n", test, yylval.tagtype.name + 1, line + 1);
*/
				}
				else //если закрывающий
				{
					if (strcmp((const char*)temp->tag, (const char*)(yylval.tagtype.name + 1)) == 0) // проверка, что считанный закрытый тег = соответствующему открывающему, тогда переходим к следующим тегам
					{
					//	if (strcmp((const char*)t->tag, "html") == 0 || strcmp((const char*)t->tag, "body") == 0) {} //так как html и body самые внешние теги, в них вложено все
					//	else
					//	{
					/*		i = 0;
							char test = NULL;
							while (i++ < NESTEDNUM)
								if (strcmp((const char*)yylval.tagtype.name, nested[i - 1][0]) == 0)
								{
									if (!strcmp((const char*)t->tag, nested[i - 1][1]))
										break;
									else
									{
										char test = nested[i - 1][1];
										//i++;
									}
								}
							if (test != NULL) printf_s("ERROR: Nesting of tags: '%s', '%s'. Line %d\n\n", test, yylval.tagtype.name + 1, line + 1); */
					//	}
						t->ptr = NULL;
						free(temp);
						temp = NULL;
					} //иначе ошибка
					else
					{
						printf_s("Error: unclosed tag: %s. Line %d\n", temp->tag,  line+1);
					}
				}

			} 
					
			if (R1 != 0) //если тег одиночный
			{
				return R1;
			}
			else
			{
				if (yylval.tagtype.name[0] != '/') //иначе если это не закрывающий тег двойного тега
					return Pair_Tag_No_Attribute_Start;
				else
					return Tag_End; // иначе это закрывающий тег двойного тега
			}
		}
		else //тег с атрибутами
		{
			int R2 = 0;
			int R3 = Pair_Tag_Start_With_Attribute;
			flag2 = 0;

			
			for (int i = 0; i < SINGLENUM; i++)
				if (strcmp((const char*)yylval.tagtype.name, single[i]) == 0) //если тег одиночный
				{
					R2 = Single_Tag_With_Attribute; //помечаем, если одиночный
				flag2 = 1;
					break;
				}

			if (head == NULL) //если список еще пуст
			{
				int i = 0;
				head = (lst*)malloc(sizeof(lst));
				for (i = 0; i < 25; i++)
					head->tag[i] = yylval.tagtype.name[i];
				head->ptr = NULL;
			}
			else
			{
				lst *temp, *t;
				temp = head;
				while (temp->ptr != NULL) //считываем все теги в обратном порядке из списка
					temp = temp->ptr;


				if (flag2) //если одиночный
				{
				/*	int i = 0;
					char test = NULL;
					while (i++ < NESTEDNUM)
						if (strcmp((const char*)yylval.tagtype.name, nested[i - 1][0]) == 0)
						{
							if (!strcmp((const char*)temp->tag, nested[i - 1][1]))
								break;
							else
							{
								char test = nested[i - 1][1];
							}								
						}
					if (test != NULL) printf_s("ERROR:  Nesting of tags: '%s', '%s'. Line %d\n\n", test, yylval.tagtype.name, line + 1); */

				}
				else
				{
					temp->ptr = (lst*)malloc(sizeof(lst));
					t = temp;
					temp->ptr->ptr = NULL;
				int i = 0;
					for (i = 0; i < 25; i++)
						temp->ptr->tag[i] = yylval.tagtype.name[i];
					t->ptr = temp->ptr;
				}
			}
			if (R2 != 0) //если одиночный тег
				return R2;
			else //иначе это первая часть двойного тега
			{
				return R3;
			}
		}
	}
	else goto x; //иначе парсим дальше после комментария
}
int main(int argc, char *argv[])
{
	char t[20];
	char input[10], output[10];
	setlocale(LC_ALL, "Rus");
	if (argc == 1)
	{
		f = fopen("input.txt", "rb");

	}
	else
	{
		strcpy(t, argv[1]);
		f = fopen(t, "rb");

	}
	yyparse();
//	if (head != NULL) printf_s("Error: unclosed tag!: %s. Line %d\n", head->tag, line + 1);

/*	for (int i = 0; i < TAGNUM; i++)
	{
		if (flag_tag_start[i] != flag_tag_end[i])
		{
			printf_s("ERROR: Unclosed tag %s. Line : %d\n\n", tag_attribute[i][0], line);
		}
	} */
	printf("\n");
	return 0;
}