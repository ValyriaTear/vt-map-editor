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

#include "utils/utils_common.h"
#include "editor.h"

using namespace vt_script;
using namespace vt_editor;

int main(int argc, char **argv)
{
    Q_INIT_RESOURCE(data);

    QApplication app(argc, argv);
    app.setOrganizationName("QtProject");
    app.setApplicationName("VT - Map Editor");

    Editor *editor = new Editor();
    editor->setWindowTitle("Map Editor");
    //app.setMainWidget(editor); // Now useless according to the Qt 4.8.3 documentation.
    editor->show();

    return app.exec();
}
