#ifndef GETSTEAMLIB_H
#define GETSTEAMLIB_H

#include <string>
#include <vector>

// º¯ÊýÉùÃ÷
std::string getSteamInstallPath();
std::vector<std::string> getSteamLibraryFolders(const std::string& steamPath);
bool directoryExists(const std::string& path);
std::string findGameInstallPath(const std::string& gameName);
bool createDirectory(const std::string& path);
bool createDirectoriesRecursively(const std::string& path);

#endif // GETSTEAMLIB_H
