@echo off

call "%~dp0..\config.bat"

"%RunUATPath%" BuildCookRun ^
 -project="%ProjectPath%" ^
 -platform="%Platform%" ^
 -serverconfig="%Configuration%" ^
 -archivedirectory="%ArchivePath%" ^
 -build -cook -package -archive -stage -pac -allmaps -server -noclient