///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2004-2011 by The Allacrost Project
//            Copyright (C) 2012-2015 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 2. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See http://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

/** ****************************************************************************
*** \file    utils_files.cpp
*** \author  Tyler Olsen, roots@allacrost.org
*** \author  Yohann Ferreira, yohann ferreira orange fr
*** \brief   Source file for the utility code.
*** ***************************************************************************/

#include "utils/utils_pch.h"
#include "utils_files.h"

namespace vt_utils
{

bool DoesFileExist(const std::string& file_name)
{
    // Modified to use platform specific code because on windows stat does not work on directories,
    // but on POSIX compliant systems it does, and GetFileAttributes works for both folders and
    // directories on win32
#ifdef _WIN32
    return GetFileAttributesA(file_name.c_str()) != INVALID_FILE_ATTRIBUTES;
#else
    struct stat buf;
    if(stat(file_name.c_str(), &buf) == 0)
        return true;
    else
        return false;
#endif
}

bool MoveFile(const std::string &source_name, const std::string &destination_name)
{
    if(DoesFileExist(destination_name))
        remove(destination_name.c_str());
    return (rename(source_name.c_str(), destination_name.c_str()) == 0);
}

void CopyFile(const std::string &source, const std::string &destination)
{
    if(DoesFileExist(destination))
        remove(destination.c_str());
    std::ifstream src(source.c_str());
    std::ofstream dst(destination.c_str());
    dst << src.rdbuf();
}

bool MakeDirectory(const std::string &dir_name)
{
    // Don't do anything if the directory already exists.
    if (DoesFileExist(dir_name)) {
        return true;
    }

    // Create the directory with mkdir(). Note that Windows does not require file permissions to be set, but
    // all other operating systems do.

#ifdef _WIN32
    int32_t success = mkdir(dir_name.c_str());
#else
    int32_t success = mkdir(dir_name.c_str(), S_IRWXG | S_IRWXO | S_IRWXU);
#endif

    if(success == -1) {
        std::cerr << "UTILS ERROR: could not create directory: " << dir_name.c_str() << std::endl;
        return false;
    }

    return true;
}

bool CleanDirectory(const std::string &dir_name)
{
    // Don't do anything if the directory doesn't exist.
    if (!DoesFileExist(dir_name)) {
        return true;
    }

#ifdef _WIN32
    //--- WINDOWS --------------------------------------------------------------

    WIN32_FIND_DATAA info = { 0 };
    HANDLE handle = nullptr;

    char file_found[1024];
    memset(file_found, 0, sizeof(file_found));
    sprintf(file_found, "%s*.*", dir_name.c_str());

    handle = FindFirstFileA(file_found, &info);
    if (handle != INVALID_HANDLE_VALUE) {
        // Remove each file from the directory.
        do {
            sprintf(file_found, "%s%s", dir_name.c_str(), info.cFileName);
            DeleteFileA(file_found);
        } while (FindNextFileA(handle, &info));
    }

    FindClose(handle);
#else
    //--- NOT WINDOWS ----------------------------------------------------------

    DIR *parent_dir;
    struct dirent *dir_file;

    parent_dir = opendir(dir_name.c_str());   // open the directory we want to clean
    if(!parent_dir) {
        std::cerr << "UTILS ERROR: failed to clean directory: " << dir_name << std::endl;
        return false;
    }

    std::string base_dir = dir_name;
    if(base_dir[base_dir.length() - 1] != '/')
        base_dir += "/";

    // Remove each file found in the parent directory
    while((dir_file = readdir(parent_dir))) {
        std::string file_name = base_dir + dir_file->d_name;
        remove(file_name.c_str());
    }

    closedir(parent_dir);

#endif

    return true;
}

bool RemoveDirectory(const std::string &dir_name)
{
    // Don't do anything if the directory doesn't exist
    if (!DoesFileExist(dir_name)) {
        return true;
    }

    // Remove any files that still reside in the directory
    CleanDirectory(dir_name);

    // Finally, remove the folder itself with rmdir()
    int32_t success = rmdir(dir_name.c_str());

    if(success == -1) {
        std::cerr << "UTILS ERROR: could not delete directory: " << dir_name << std::endl;
        return false;
    }

    return true;
}

std::vector<std::string> ListDirectory(const std::string &dir_name, const std::string &filter)
{
    //create our vector
    std::vector<std::string> directoryList;

    // Don't try to list files if the directory does not exist.
    if (!DoesFileExist(dir_name)) {
        return directoryList;
    }

    //directory exists so lets list
#if defined _WIN32
    //--- WINDOWS --------------------------------------------------------------

    WIN32_FIND_DATAA info = { 0 };
    HANDLE handle = nullptr;

    char file_found[1024];
    memset(file_found, 0, sizeof(file_found));
    sprintf(file_found, "%s\\*.*", dir_name.c_str());

    if (handle != INVALID_HANDLE_VALUE) {
        // List each file from the directory.
        do {
            std::string file_name(file_found);
            if (filter == "") {
                directoryList.push_back(file_found);
            }
            else if (file_name.find(filter) != std::string::npos) {
                directoryList.push_back(file_found);
            }
        } while (FindNextFileA(handle, &info));
    }

    FindClose(handle);
#else
    //Not Windows
    DIR *dir;
    struct dirent *dir_file;
    dir = opendir(dir_name.c_str()); //open the directory for listing
    if(!dir) {
        std::cerr << "UTILS ERROR: Failed to list directory: " << dir_name << std::endl;
        return directoryList;
    }

    //List each file found in the directory as long as it end with .lua
    while((dir_file = readdir(dir))) {
        std::string fileName = dir_file->d_name;
        //contains a .lua ending so put it in the directory
        if(filter == "")
            directoryList.push_back(dir_file->d_name);
        else if(fileName.find(filter) != std::string::npos)
            directoryList.push_back(dir_file->d_name);
    }

    closedir(dir);

#endif

    return directoryList;
}

bool DeleteFile(const std::string &filename)
{
    if(DoesFileExist(filename)) {
        remove(filename.c_str());
        if(!DoesFileExist(filename))
            return true;
    }
    return false;
}

} // namespace utils
