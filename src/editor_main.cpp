////////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2004-2011 by The Allacrost Project
//            Copyright (C) 2012-2015 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 2. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See http://www.gnu.org/copyleft/gpl.html for details.
////////////////////////////////////////////////////////////////////////////////

/** ****************************************************************************
*** \file    editor_main.cpp
*** \author  Philip Vorsilak, gorzuate@allacrost.org
*** \author  Yohann Ferreira, yohann ferreira orange fr
*** \brief   Source file for editor's main() function.
*** ***************************************************************************/

#include "utils/utils_pch.h"
#include "editor.h"

// Used to permit compilation as qmake will create a makefile requiring qMain
// as the main app function, and the aliasing of main prevents this to work
// with Qt 4.8.3.
#if defined(main)
#undef main
#endif

using namespace vt_script;
using namespace vt_editor;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    Editor *editor = new Editor();
    editor->setWindowTitle("Map Editor");
    //app.setMainWidget(editor); // Now useless according to the Qt 4.8.3 documentation.
    editor->show();

    return app.exec();
}
