#pragma once

/*
    ###################################################################################
    winproof88.h
    My way of including windows.h, for my projects.
    The idea is the following:
    unlike when including windows.h where macro definitions need to be used to SKIP
    including a lot of usually unneeded definitions, using my header files requires
    macro definitions to ALLOW including the usually unneeded definitions.
    For example, by default I'm defining WIN32_LEAN_AND_MEAN and NOMINMAX macros before
    including windows.h, and whoever needs extra functionality is needed to explicitly
    enable them by defining my WINPROOF88_ALLOW_... macros before including my header.
    This file is part of PFL.
    Made by PR00F88, West Whiskhyll Entertainment
    2023
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/


#ifndef WINPROOF88_ALLOW_THICK_HEADER
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#endif

#ifndef NOGDICAPMASKS
#define NOGDICAPMASKS     /* - CC_*, LC_*, PC_*, CP_*, TC_*, RC_ */
#endif

#ifndef WINPROOF88_ALLOW_VIRTUALKEYCODES
#ifndef NOVIRTUALKEYCODES
#define NOVIRTUALKEYCODES /* - VK_* */
#endif
#endif

#ifndef WINPROOF88_ALLOW_MSG_USER_WINMESSAGES
    #ifndef NOMSG
        #define NOMSG             /* - typedef MSG and associated routines */
    #endif
    #ifndef NOUSER
        #define NOUSER            /* - All USER defines and routines */
    #endif
    #ifndef NOWINMESSAGES
        #define NOWINMESSAGES     /* - WM_*, EM_*, LB_*, CB_* */
    #endif
#endif  // WINPROOF88_ALLOW_MSG_USER_WINMESSAGES

#ifndef WINPROOF88_ALLOW_WINSTYLES
#ifndef NOWINSTYLES
#define NOWINSTYLES       /* - WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_* */
#endif
#endif

#ifndef WINPROOF88_ALLOW_SYSMETRICS
#ifndef NOSYSMETRICS
#define NOSYSMETRICS      /* - SM_* */
#endif
#endif

#ifndef NOMENUS
#define NOMENUS           /* - MF_* */
#endif

#ifndef NOICONS
#define NOICONS           /* - IDI_* */
#endif

#ifndef NOKEYSTATES
#define NOKEYSTATES       /* - MK_* */
#endif

#ifndef WINPROOF88_ALLOW_SYSCOMMANDS_E_G_SCREENSAVER
#ifndef NOSYSCOMMANDS
#define NOSYSCOMMANDS     /* - SC_* */
#endif
#endif

#ifndef WINPROOF88_ALLOW_RASTERPOS
#ifndef NORASTEROPS
#define NORASTEROPS       /* - Binary and Tertiary raster ops */
#endif
#endif

#ifndef WINPROOF88_ALLOW_SHOWWINDOW
#ifndef NOSHOWWINDOW
#define NOSHOWWINDOW      /* - SW_* */
#endif
#endif

#ifndef OEMRESOURCE
#define OEMRESOURCE       /* - OEM Resource values */
#endif

#ifndef NOATOM
#define NOATOM            /* - Atom Manager routines */
#endif

#ifndef NOCLIPBOARD
#define NOCLIPBOARD       /* - Clipboard routines */
#endif

#ifndef NOCOLOR
#define NOCOLOR           /* - Screen colors */
#endif

#ifndef WINPROOF88_ALLOW_CONTROLS_AND_DIALOGS
#ifndef NOCTLMGR
#define NOCTLMGR          /* - Control and Dialog routines */
#endif
#endif

#ifndef NODRAWTEXT
#define NODRAWTEXT        /* - DrawText() and DT_* */
#endif

#ifndef WINPROOF88_ALLOW_GDI
#ifndef NOGDI
#define NOGDI             /* - All GDI defines and routines */
#endif
#endif

#ifndef NOKERNEL
#define NOKERNEL          /* - All KERNEL defines and routines */
#endif

#ifndef WINPROOF88_ALLOW_NATIONAL_LANGUAGE_SUPPORT
#ifndef NONLS
#define NONLS             /* - All NLS defines and routines */
#endif
#endif

#ifndef WINPROOF88_ALLOW_MESSAGEBOX
#ifndef NOMB
#define NOMB              /* - MB_* and MessageBox() */
#endif
#endif

#ifndef NOMEMMGR
#define NOMEMMGR          /* - GMEM_*, LMEM_*, GHND, LHND, associated routines */
#endif

#ifndef NOMETAFILE
#define NOMETAFILE        /* - typedef METAFILEPICT */
#endif

// note that still somehow IntelliSense parses the min and max macros from minwindef.h, so expect that
// in my projects I explicitly add NOMINMAX to the preprocessor definitions in project settings!
#ifndef NOMINMAX
#define NOMINMAX          /* - Macros min(a,b) and max(a,b) */
#endif

#ifndef NOOPENFILE
#define NOOPENFILE        /* - OpenFile(), OemToAnsi, AnsiToOem, and OF_* */
#endif

#ifndef NOSCROLL
#define NOSCROLL          /* - SB_* and scrolling routines */
#endif

#ifndef NOSERVICE
#define NOSERVICE         /* - All Service Controller routines, SERVICE_ equates, etc. */
#endif

#ifndef WINPROOF88_ALLOW_SOUND
#ifndef NOSOUND
#define NOSOUND           /* - Sound driver routines */
#endif
#endif

#ifndef NOTEXTMETRIC
#define NOTEXTMETRIC      /* - typedef TEXTMETRIC and associated routines */
#endif

#ifndef NOWH
#define NOWH              /* - SetWindowsHook and WH_* */
#endif

#ifndef WINPROOF88_ALLOW_WINOFFSETS
#ifndef NOWINOFFSETS
#define NOWINOFFSETS      /* - GWL_*, GCL_*, associated routines */
#endif
#endif

#ifndef NOCOMM
#define NOCOMM            /* - COMM driver routines */
#endif

#ifndef NOKANJI
#define NOKANJI           /* - Kanji support stuff. */
#endif

#ifndef NOHELP
#define NOHELP            /* - Help engine interface. */
#endif

#ifndef NOPROFILER
#define NOPROFILER        /* - Profiler interface. */
#endif

#ifndef NODEFERWINDOWPOS
#define NODEFERWINDOWPOS  /* - DeferWindowPos routines */
#endif

#ifndef NOMCX
#define NOMCX             /* - Modem Configuration Extensions */
#endif

#include <windows.h>
