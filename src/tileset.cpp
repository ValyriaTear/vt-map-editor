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
*** \file    tileset.cpp
*** \author  Philip Vorsilak, gorzuate@allacrost.org
*** \author  Yohann Ferreira, yohann ferreira orange fr
*** \brief   Source file for editor's tileset, used for maintaining a visible
***          "list" of tiles to select from for painting on the map.
*** **************************************************************************/

#include "utils/utils_pch.h"
#include "tileset.h"

#include "script/script_read.h"
#include "script/script_write.h"

#include <QHeaderView>
#include <QFile>
#include <QImage>

using namespace vt_script;

const uint32_t num_rows = 16;
const uint32_t num_cols = 16;

namespace vt_editor
{

////////////////////////////////////////////////////////////////////////////////
// Tileset class -- all functions
////////////////////////////////////////////////////////////////////////////////

Tileset::Tileset() :
    _initialized(false)
{} // Tileset constructor


Tileset::~Tileset()
{
    tiles.clear();
} // Tileset destructor


QString Tileset::CreateTilesetName(const QString &filename)
{
    QString tname = filename;
    // Remove everything up to and including the final '/' character
    tname.remove(0, tname.lastIndexOf("/") + 1);
    // Chop off the appended four characters (the filename extension)
    tname.chop(4);
    return tname;
}

bool Tileset::New(const QString &img_filename, const QString& root_folder, bool one_image)
{
    if (img_filename.isEmpty())
        return false;

    _initialized = false;

    // Retrieve the tileset name from the image filename
    _tileset_name = CreateTilesetName(img_filename);
    _tileset_definition_filename = "data/tilesets/" + _tileset_name + ".lua";

    // Check existence of a previous lua definition file
    if (QFile::exists(_tileset_definition_filename)) {
        _tileset_definition_filename.clear();
        _tileset_name.clear();
        qDebug("Failed to create tileset, it already exists: %s",
                _tileset_definition_filename.toStdString().c_str());
        return false;
    }

    _tileset_image_filename = "data/tilesets/" + _tileset_name + img_filename.mid(img_filename.length() - 4, 4);

    // Prepare the tile vector and load the tileset image
    tiles.clear();
    tiles.resize(256);

    QRect rectangle;
    QImage entire_tileset;
    QString tileset_full_path = root_folder + _tileset_image_filename;
    if (!entire_tileset.load(tileset_full_path, "png")) {
        qDebug("Failed to load tileset image: %s",
                tileset_full_path.toStdString().c_str());
        return false;
    }

    if (one_image) {
        tiles[0].convertFromImage(entire_tileset);
    }
    else {

        for(uint32_t row = 0; row < num_rows; ++row) {
            for(uint32_t col = 0; col < num_cols; ++col) {
                rectangle.setRect(col * TILE_WIDTH, row * TILE_HEIGHT, TILE_WIDTH,
                                TILE_HEIGHT);
                QImage tile = entire_tileset.copy(rectangle);
                if(!tile.isNull()) {
                    // linearize the tile index
                    uint32_t i = num_rows * row + col;
                    tiles[i].convertFromImage(tile);
                } else {
                    qDebug("Image loading error!");
                }
            }
        }
    }

    // Initialize the rest of the tileset data
    std::vector<int32_t> blank_entry(4, 0);
    for(uint32_t i = 0; i < 16; ++i)
        for(uint32_t j = 0; j < 16; ++j)
            walkability.insert(std::make_pair(i * 16 + j, blank_entry));

    autotileability.clear();
    _animated_tiles.clear();

    _initialized = true;
    return true;
} // Tileset::New(...)


bool Tileset::Load(const QString &def_filename, const QString& root_folder, bool one_image)
{
    if (def_filename.isEmpty())
        return false;

    _initialized = false;

    // Reset container data
    autotileability.clear();
    walkability.clear();
    _animated_tiles.clear();

    // Create filenames from the tileset name
    _tileset_name = CreateTilesetName(def_filename);
    _tileset_definition_filename = def_filename;

    // Set up for reading the tileset definition file.
    ReadScriptDescriptor read_data;
    if(!read_data.OpenFile(root_folder.toStdString() + def_filename.toStdString())) {
        _initialized = false;
        return false;
    }

    if (!read_data.OpenTable("tileset")) {
        read_data.CloseFile();
        qDebug("Failed to open the 'tileset' table");
        return false;
    }

    _tileset_image_filename = QString::fromStdString(read_data.ReadString("image"));

    // Prepare the tile vector and load the tileset image
    tiles.clear();
    tiles.resize(256);

    QRect rectangle;
    QImage entire_tileset;
    QString tileset_full_path = root_folder + _tileset_image_filename;
    if (!entire_tileset.load(tileset_full_path, "png")) {
        qDebug("Failed to load tileset image: %s",
                tileset_full_path.toStdString().c_str());
        return false;
    }

    if (one_image) {
        tiles[0].convertFromImage(entire_tileset);
    }
    else {

        for(uint32_t row = 0; row < num_rows; ++row) {
            for(uint32_t col = 0; col < num_cols; ++col) {
                rectangle.setRect(col * TILE_WIDTH, row * TILE_HEIGHT, TILE_WIDTH,
                                TILE_HEIGHT);
                QImage tile = entire_tileset.copy(rectangle);
                if(!tile.isNull()) {
                    // linearize the tile index
                    uint32_t i = num_rows * row + col;
                    tiles[i].convertFromImage(tile);
                } else {
                    qDebug("Image loading error!");
                }
            }
        }
    }

    // Read in autotiling information.
    if(read_data.DoesTableExist("autotiling") == true) {
        // Contains the keys (indeces, if you will) of this table's entries
        std::vector<int32_t> keys;
        uint32_t table_size = read_data.GetTableSize("autotiling");
        read_data.OpenTable("autotiling");

        read_data.ReadTableKeys(keys);
        for(uint32_t i = 0; i < table_size; ++i)
            autotileability[keys[i]] = read_data.ReadString(keys[i]);
        read_data.CloseTable();
    } // make sure table exists first

    // Read in walkability information.
    if(read_data.DoesTableExist("walkability") == true) {
        std::vector<int32_t> vect;  // used to read in vectors from the data file
        read_data.OpenTable("walkability");

        for(int32_t i = 0; i < 16; ++i) {
            read_data.OpenTable(i);
            // Make sure that at least one row exists
            if(read_data.IsErrorDetected() == true) {
                read_data.CloseTable();
                read_data.CloseTable();
                read_data.CloseFile();
                _initialized = false;
                return false;
            }

            for(int32_t j = 0; j < 16; ++j) {
                read_data.ReadIntVector(j, vect);
                if(read_data.IsErrorDetected() == false)
                    walkability[i * 16 + j] = vect;
                vect.clear();
            } // iterate through all tiles in a row
            read_data.CloseTable();
        } // iterate through all rows of the walkability table
        read_data.CloseTable();
    } // make sure table exists first

    // Read in animated tiles.
    if(read_data.DoesTableExist("animated_tiles") == true) {
        uint32_t table_size = read_data.GetTableSize("animated_tiles");
        read_data.OpenTable("animated_tiles");

        for(uint32_t i = 1; i <= table_size; ++i) {
            _animated_tiles.push_back(std::vector<AnimatedTileData>());
            std::vector<AnimatedTileData>& tiles = _animated_tiles.back();
            // Calculate loop end: an animated tile is comprised of a tile id
            // and a time, so the loop end is really half the table size.
            uint32_t tile_count = read_data.GetTableSize(i) / 2;
            read_data.OpenTable(i);
            for(uint32_t index = 1; index <= tile_count; index++) {
                AnimatedTileData anim_tile;
                anim_tile.tile_id = read_data.ReadUInt(index * 2 - 1);
                anim_tile.time    = read_data.ReadUInt(index * 2);
                tiles.push_back(anim_tile);
            } // iterate through all tiles in one animated tile
            read_data.CloseTable();
        } // iterate through all animated tiles in the table
        read_data.CloseTable();
    } // make sure table exists first

    read_data.CloseTable();
    read_data.CloseFile();

    _initialized = true;
    return true;
} // Tileset::Load(...)


bool Tileset::Save(const QString& root_folder)
{
    WriteScriptDescriptor write_data;

    if (!write_data.OpenFile(QString(root_folder + _tileset_definition_filename).toStdString()))
        return false;

    // Write the main table for the tileset file
    write_data.BeginTable("tileset");
    write_data.InsertNewLine();

    // Write basic tileset properties
    write_data.WriteString("image", _tileset_image_filename.toStdString());
    write_data.WriteInt("num_tile_cols", 16);
    write_data.WriteInt("num_tile_rows", 16);
    write_data.InsertNewLine();

    // Write autotiling data
    if(autotileability.empty() == false) {
        write_data.BeginTable("autotiling");
        for(std::map<int, std::string>::iterator it = autotileability.begin();
                it != autotileability.end(); ++it)
            write_data.WriteString((*it).first, (*it).second);
        write_data.EndTable();
        write_data.InsertNewLine();
    } // data must exist in order to save it

    // Write walkability data
    write_data.WriteComment("The general walkability of the tiles in the tileset. Zero indicates walkable. One tile has four walkable quadrants listed as: NW corner, NE corner, SW corner, SE corner.");
    write_data.BeginTable("walkability");
    for(uint32_t row = 0; row < 16; row++) {
        write_data.BeginTable(row);
        for(uint32_t col = 0; col < 16; col++)
            write_data.WriteIntVector(col, walkability[row * 16 + col]);
        write_data.EndTable();
    } // iterate through all rows of the tileset
    write_data.EndTable();
    write_data.InsertNewLine();

    // Write animated tile data
    if(_animated_tiles.empty() == false) {
        write_data.WriteComment("The animated tiles table has one row per animated tile, with each entry in a row indicating which tile in the tileset is the next part of the animation, followed by the time in ms that the tile will be displayed for.");
        write_data.BeginTable("animated_tiles");
        std::vector<uint32_t> vect;
        for(uint32_t anim_tile = 0; anim_tile < _animated_tiles.size(); ++anim_tile) {
            for(uint32_t i = 0; i < _animated_tiles[anim_tile].size(); ++i) {
                vect.push_back(_animated_tiles[anim_tile][i].tile_id);
                vect.push_back(_animated_tiles[anim_tile][i].time);
            } // iterate through all tiles in one animated tile
            write_data.WriteUIntVector(anim_tile + 1, vect);
            vect.clear();
        } // iterate through all animated tiles of the tileset
        write_data.EndTable(); // animated_tiles
    } // data must exist in order to save it

    write_data.EndTable(); // tileset

    if(write_data.IsErrorDetected()) {
        PRINT_ERROR << "Errors were detected when saving tileset file. The errors include: "
                    << std::endl << write_data.GetErrorMessages() << std::endl;
        write_data.CloseFile();
        return false;
    }

    write_data.CloseFile();
    return true;
} // Tileset::Save()


///////////////////////////////////////////////////////////////////////////////
// TilesetTable class -- all functions
///////////////////////////////////////////////////////////////////////////////

TilesetTable::TilesetTable() :
    Tileset()
{
    // Set up the QT table
    table = new QTableWidget(num_rows, num_cols);
    table->setShowGrid(false);
    table->setSelectionMode(QTableWidget::ContiguousSelection);
    table->setEditTriggers(QTableWidget::NoEditTriggers);
    table->setContentsMargins(0, 0, 0, 0);
    table->setDragEnabled(false);
    table->setAcceptDrops(false);
    table->setHorizontalHeaderLabels(QStringList());
    table->setVerticalHeaderLabels(QStringList());
    table->verticalHeader()->hide();
    table->verticalHeader()->setContentsMargins(0, 0, 0, 0);
    table->horizontalHeader()->hide();
    table->horizontalHeader()->setContentsMargins(0, 0, 0, 0);

    for(uint32_t i = 0; i < num_rows; ++i)
        table->setRowHeight(i, TILE_HEIGHT);
    for(uint32_t i = 0; i < num_cols; ++i)
        table->setColumnWidth(i, TILE_WIDTH);
} // TilesetTable constructor


TilesetTable::~TilesetTable()
{
    delete table;
} // TilesetTable destructor


bool TilesetTable::Load(const QString &def_filename, const QString& root_folder)
{
    if (!Tileset::Load(def_filename, root_folder))
        return false;

    // Read in tiles and create table items.
    QRect rectangle;
    QImage entire_tileset;
    entire_tileset.load(root_folder + _tileset_image_filename, "png");
    for(uint32_t row = 0; row < num_rows; ++row) {
        for(uint32_t col = 0; col < num_cols; ++col) {
            rectangle.setRect(col * TILE_WIDTH, row * TILE_HEIGHT, TILE_WIDTH,
                              TILE_HEIGHT);
            QVariant variant = entire_tileset.copy(rectangle);
            if(!variant.isNull()) {
                QTableWidgetItem *item = new QTableWidgetItem(QTableWidgetItem::UserType);
                item->setData(Qt::DecorationRole, variant);
                item->setFlags(item->flags() &~ Qt::ItemIsEditable);

                table->setItem(row, col, item);
            } else
                qDebug("Image loading error!");
        } // iterate through the columns of the tileset
    } // iterate through the rows of the tileset


    // Select the top left item
    table->setCurrentCell(0, 0);

    return true;
}

} // namespace vt_editor
