#ifndef HTTPFS_UTILS
#define HTTPFS_UTILS

#include <stdio.h>
#include "simplesocket.h"

#define MIN(x,y) ((x) < (y)? (x): (y))

#define TIMEOUT_SECS 30

//static const char* gIndexPageHead = "<HTML><META HTTP-EQUIV=\"Pragma\" CONTENT=\"no-cache\"><BODY BGCOLOR=\"CBCBCB\">";
static const char* gUploadFileBuffer1 = "<BR><BR><HR><P>Upload file:<FORM name='theform' enctype='multipart/form-data' method='POST' action='./upload_file'><INPUT type='hidden' name='path' value='";
static const char* gUploadFileBuffer2 = "'><INPUT type='file' name='upload_file' size=30><BR><INPUT type='button' value='Upload' onclick='this.form.submit()'></FORM><HR>\n \
                                        <P>Admin: <A HREF=\"./auth\">(Click here to login)</A>, or <A HREF=\"./auth_logout\">(here to logout)</A><FORM name='theform2' enctype='multipart/form-data' method='POST' action='./admin_command'><SELECT name='cmd'><OPTION value='del'>Delete\n<OPTION value='mkdir'>Create Directory<OPTION value='exec'>exec\n</SELECT>&nbsp;&nbsp; name:<INPUT type='text' name='admin_filename'><BR>optional arguments:<INPUT type='text' name='admin_args'><BR><INPUT type='button' value='Go' onclick='this.form.submit()'></FORM>";
static const char* gAuthPageBuf = "<HTML><META HTTP-EQUIV=\"Pragma\" CONTENT=\"no-cache\"><BODY BGCOLOR=\"CBCBCB\"><P>Password:<FORM name='theform2' enctype='multipart/form-data' method='POST' action='./auth'><INPUT type='password' name='auth_password'> <INPUT type='button' value='Login' onclick='this.form.submit()'></FORM></HTML>";
static const char* gIndexCMSPageHead = 
""
"<script src='files.js'></script>\n"
"<script language='javascript'>\n"
"\n"
"var tableJournalBuffer = '';\n"
"function onPreview(btn, imgId, file) {\n"
"    document.getElementById(imgId).src=file;\n"
"    document.getElementById(imgId).hidden=false;\n"
"    btn.hidden=true;\n"
"}\n"
"function tableJournalAdd(title, log, file, count, thumb, tags) {\n"
"    var imgId = file;\n"
"    tableJournalBuffer += \n"
"        '<tr>' +\n"
"        '<td>'+title+'</td><td>'+log+'</td>'+'<td><a href='+file+'>download</a></td><td ><img id=\"'+imgId+'\" width=\"64\" height=\"64\" hidden=true src=\"http_icon0.gif\"><button onclick=\"onPreview(this, \\\''+imgId+'\\\', \\\''+file+'\\\');\">preview</button></td>'+\n"
"        '<td><button onClick=\"onDelete(\\\''+title+'\\\');\">delete</button></td>'+\n"
"        '<td>'+tags+'</td>'+\n"
"        '</tr>';\n"
"}\n"
"function tableJournalClear() {\n"
"    var t = document.getElementById(\"tableMain\");\n"
"    t.innerHTML = '';\n"
"    tableJournalBuffer = '';\n"
"}\n"
"function tableJournalDone() {\n"
"    t.innerHTML = tableJournalBuffer;\n"
"}\n"
"function onLoad() {\n"
"    document.getElementById('locationId').innerHTML='<H2>'+document.location+'</H2><br>';\n"
"    tableJournalClear();\n"
"    var i = 0;\n"
"    for(i = fileList.length-1; i >= 0; i--) {\n"
"        tableJournalAdd(fileList[i].name,\n"
"                        'No log entry',\n"
"                        fileList[i].url, 1,\n"
"                        fileList[i].thumbnail,\n"
"                        '');\n"
"    }\n"
"    tableJournalDone();\n"
"}\n"
"function onDelete(fileName) {\n"
    "if(confirm('Delete '+fileName+'?')) {\n"
    "    document.theform2.cmd.value = 'del';\n"
    "    document.theform2.admin_filename.value = fileName;\n"
    "    document.theform2.submit();\n"
    "}\n"
"}\n"
"function onMkdir(fileName) {\n"
    "document.theform2.cmd.value = 'del';\n"
    "document.theform2.admin_filename.value = fileName;\n"
    "document.theform2.submit();\n"
"}\n"
"\n</script>\n\n"
"<html>\n"
"<body onload='onLoad();'>\n"
"<div id='locationId'></div>\n"
"<br>\n"
"<button onclick='window.location = \"..\";'>Up one level</button>\n"
"<table border=1 id='tableMain'>\n"
"<tr><td><p>Loading...</td></tr>\n"
"</table>\n"
"<button onclick='window.open(\"./auth\");'>Upload/Remove/Login</button>\n"
"<br>\n"
"<table><tr><td><form name='mkdirForm'><input name='dirname'></input></td><td><button onclick='onMkdir(document.mkdirForm.dirname.value);'>mkdir</button></form></td></tr></table>\n"
"</body>\n"
"</html>\n"
"\n";
static const char* gIndexPageHead = gIndexCMSPageHead;

#ifdef _BWINDOWS_
typedef HANDLE MutexType;
#endif
#ifdef _UNIX_
typedef pthread_mutex_t MutexType;
#endif

typedef struct {
    MutexType mutex1;
} Semaphore;


void ConvertSlashes(char* str);
void CleanString(char* str);
void EscapeHttp(char* str, int len);
bool isWhiteSpc(char c);
char ContainsAnyChar(char* haystack, char* needles);
void Send404(SimpleSocket* s);
void Send404WithMsg(SimpleSocket* s, char *msg);
void SendRedirect(SimpleSocket* s, char *location);
void SendAuth(SimpleSocket* s, char* cookie, bool success);
void SendAuthPage(SimpleSocket* s);
void SendContinue(SimpleSocket* s);
int ReadAndBufferUntil(SimpleSocket* s, char* str, bool matchCase, char* destbuf, long destbuf_size, unsigned long* count);
void LookupContentTypeByExt(char* str, /* at most 64 chars in length */ char* dest);
int MkDir(char* pathFileName);
int DelLocalFile(char* pathFileName);
int SystemExec(char* pathFileName, char* args);
void MutexSemInit(Semaphore* sem);
void MutexSemDestroy(Semaphore* sem);
void MutexSemTake(Semaphore* sem);
void MutexSemGive(Semaphore* sem);

void CreateDirFile(char* path);
char *DirOfPath(char* path);
int SendDirectory(SimpleSocket* s, char* path);
int SendDirectoryJS(SimpleSocket* s, char* path);
int TestDirectory(char *localpath);
int getFileModTime(char *path, char *strdest);
int getDateTime(char *strdest);
void bwm_sleep(int msec);
int simple_parse(char *buf, char *beginkey, char *endkey, char* dest, int dest_len);
void pipe_to_devnull(FILE *fp);
#endif /* HTTPFS_UTILS */
