@echo off

rem Engine params
set EnginePath_4.26=C:\Program Files\Epic Games\UE_4.26
set EnginePath_5.2=C:\Program Files\Epic Games\UE_5.2
set EnginePath_4.26_InstallBuild=D:\JenkinsJobs\UE_Source\UE_4.26_InstallBuild

set UBTRelativePath4=Engine\Binaries\DotNET\AutomationTool.exe
set UBTRelativePath5=Engine\Binaries\DotNET\AutomationTool\AutomationTool.exe
set VersionSelector=C:\Program Files (x86)\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe

rem Engine version for packaging
set RunUATPath=%EnginePath_5.2%\Engine\Build\BatchFiles\RunUAT.bat

rem Project params
set ProjectRoot=D:\Epic Games\_Projects\TPS
set ProjectPureName=TPS
set ProjectName=%ProjectPureName%.uproject
set ProjectPath=%ProjectRoot%\%ProjectName%

rem Build params
set Platform=Win64
set Configuration=Development
set ArchivePath=%ProjectRoot%\Build

rem Other params
set SourceCodePath=%ProjectRoot%\Source
set dirsToRemove=Intermediate DerivedDataCache Saved Binaries .vs Build
set filesToRemove=*.sln .vsconfig

rem Target params
set COPYRIGHT_LINE=// Copyright EpicGames.
set EXTRA_MODULE_NAMES=%ProjectPureName%
set TargetTemplateFilePath=%ProjectRoot%\devops\targets\EditorModule.Target.cs.template

rem Run
set ClientExePath=%ProjectRoot%\Build\WindowsClient\%ProjectPureName%Client.exe
set ServerExePath=%ProjectRoot%\Build\WindowsServer\%ProjectPureName%Server.exe
set GameExeFile=%ProjectRoot%\Build\WindowsNoEditor\%ProjectPureName%.exe