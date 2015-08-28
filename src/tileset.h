///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2004-2011 by The Allacrost Project
//            Copyright (C) 2012-2015 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 2. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See http://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

/** ***************************************************************************
*** \file    tileset.h
*** \author  Philip Vorsilak, gorzuate@allacrost.org
*** \author  Yohann Ferreira, yohann ferreira orange fr
*** \brief   Header file for editor's tileset, used for maintaining a visible
***          "list" of tiles to select from for painting on a map.
*** **************************************************************************/

#ifndef __TILESET_HEADER__
#define __TILESET_HEADER__

#include <QImageReader>
#include <QRect>
#include <QTableWidget>
#include <QVariant>

#include "script/script.h"

//! All calls to the editor are wrapped in this namespace.
namespace vt_editor
{

//! \brief Standard tile dimensions in number of pixels.
//@{
const unsigned int TILE_WIDTH  = 32;
const unsigned int TILE_HEIGHT = 32;
//@}


/** ***************************************************************************
*** \brief Represents an animated tile
*** **************************************************************************/
struct AnimatedTileData {
    //! \brief Index into tileset represents tile which will be part of the
    //         animation sequence.
    uint32 tile_id;
    //! \brief Time in milliseconds to display this particular tile.
    uint32 time;
};


/** ***************************************************************************
*** \brief Represents a tileset and retains the tileset's image and properties
***
*** This is a container of tileset data. The tileset's properties are contained
*** within a Lua file specific to the tileset. The Lua file is located in a
*** separate path from the tileset's image file. Currently this class assumes
*** and only supports a standard tileset of 512x512 pixels with
*** 32x32 pixel tiles (256 total tiles in one tileset file).
***
*** \todo Add support for animated tiles (display, editing)
*** **************************************************************************/
class Tileset
{
public:
    Tileset();

    virtual ~Tileset();

    //! \brief Returns the filename of a tileset image given the tileset's name
    QString GetImageFilename() {
        return _tileset_image_filename;
    }

    //! \brief Returns the filename of a tileset definition file given the tileset's name
    QString GetDefintionFilename() {
        return _tileset_definition_filename;
    }

    //! \brief Returns the filename of a tileset definition file given the tileset's name
    QString GetTilesetName() {
        return _tileset_name;
    }

    /** \brief Returns the tileset name that corresponds to either an image or
    ***        data filename
    *** \param filename The name of the file, which may or may not include the
    ***                 path
    **/
    static QString CreateTilesetName(const QString &filename);

    //! \brief Class member accessor functions
    //@{
    bool IsInitialized() const {
        return _initialized;
    }
    //@}

    /** \brief Creates a new tileset object using only a tileset image
    *** \param img_filename The path + name of the image file to use for the
    ***                     tileset
    *** \param one_image If true, the tiles vector will contain a single image
    ***                  for the entire tileset
    *** \return True if the tileset image was loaded successfully
    *** \note A tileset image is required to use this function, but nothing else
    **/
    virtual bool New(const QString& img_filename, const QString& root_folder, bool one_image = false);

    /** \brief Loads the tileset definition file and stores its data in the
    ***        class containers
    *** \param def_filename The tileset definition filename.
    *** \param one_image If true, the tiles vector will contain a single image
    ***                  for the entire tileset
    *** \return True if the tileset was loaded successfully
    *** \note This function will clear the previously loaded contents when it
    ***       is called
    **/
    virtual bool Load(const QString& def_filename, const QString& root_folder, bool one_image = false);

    /** \brief Saves the tileset data to its tileset definition file
    *** \return True if the save operation was successful
    **/
    bool Save(const QString& root_folder);

    //! \brief Contains the QPixmap tiles of the tileset, used in grid.cpp.
    //! \note The QPixmap class is optimized to show pictures on screen,
    //! but QImage is used at load times at it is better in it.
    std::vector<QPixmap> tiles;

    //! \brief Contains walkability information for each tile.
    std::map<int, std::vector<int32> > walkability;

    //! \brief Contains autotiling information for any autotileable tile.
    std::map<int, std::string> autotileability;

protected:
    //! \brief tileset image and definition filenames.
    QString _tileset_image_filename;
    QString _tileset_definition_filename;

    //! \brief The tileset name and namespace
    QString _tileset_name;

    //! \brief True if the class is holding valid, loaded tileset data.
    bool _initialized;

    //! \brief Contains animated tile information for any animated tile.
    std::vector<std::vector<AnimatedTileData> > _animated_tiles;
}; // class Tileset


/** ***************************************************************************
*** \brief Used to visually represent a tileset via a QT table
*** **************************************************************************/
class TilesetTable : public Tileset
{
public:
    TilesetTable();

    ~TilesetTable();

    //! \brief Loads a tileset, using the given rootFolder for relative filenames.
    bool Load(const QString& def_filename, const QString& root_folder);

    //! Reference to the table implementation of this tileset
    QTableWidget *table;
}; // class TilesetTable : public Tileset

} // namespace vt_editor

#endif // __TILESET_HEADER__
