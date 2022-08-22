#include <glad/wgl.h>

#ifdef __cplusplus
extern "C" {
#endif

PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB = NULL;
PFNWGLGETEXTENSIONSSTRINGEXTPROC wglGetExtensionsStringEXT = NULL;

static int glad_wgl_has_extension(HDC hdc, const char *ext) {
    const char *terminator = NULL;
    const char *loc = NULL;
    const char *extensions = NULL;

    if(wglGetExtensionsStringEXT == NULL && wglGetExtensionsStringARB == NULL)
        return 0;

    if(wglGetExtensionsStringARB == NULL || hdc == INVALID_HANDLE_VALUE)
        extensions = wglGetExtensionsStringEXT();
    else
        extensions = wglGetExtensionsStringARB(hdc);

    if(extensions == NULL || ext == NULL)
        return 0;

    while(1) {
        loc = strstr(extensions, ext);
        if(loc == NULL)
            break;

        terminator = loc + strlen(ext);
        if((loc == extensions || *(loc - 1) == ' ') && (*terminator == ' ' || *terminator == '\0'))
        {
            return 1;
        }
        extensions = terminator;
    }

    return 0;
}

int gladLoadWGLUserPtr(HDC hdc, GLADuserptrloadfunc load, void *userptr) {
    wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC) load(userptr, "wglGetExtensionsStringARB");
    wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC) load(userptr, "wglGetExtensionsStringEXT");
    if(wglGetExtensionsStringARB == NULL && wglGetExtensionsStringEXT == NULL) return 0;

    if (!glad_wgl_has_extension(hdc, "WGL_ARB_create_context")) return 0;
    if (!glad_wgl_has_extension(hdc, "WGL_ARB_create_context_no_error")) return 0;
    if (!glad_wgl_has_extension(hdc, "WGL_ARB_create_context_profile")) return 0;
    if (!glad_wgl_has_extension(hdc, "WGL_ARB_extensions_string")) return 0;
    if (!glad_wgl_has_extension(hdc, "WGL_EXT_extensions_string")) return 0;
    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)load(userptr, "wglCreateContextAttribsARB");

    return GLAD_MAKE_VERSION(1, 0);
}

static GLADapiproc glad_wgl_get_proc(void *vuserptr, const char* name) 
{
    (void) vuserptr;
    return GLAD_GNUC_EXTENSION (GLADapiproc) wglGetProcAddress(name);
}

int gladLoaderLoadWGL(HDC hdc) {
    return gladLoadWGLUserPtr(hdc, glad_wgl_get_proc, NULL);
}

#ifdef __cplusplus
}
#endif