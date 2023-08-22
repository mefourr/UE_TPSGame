@echo off
call "%~dp0devops\config.bat"

devops/misc/generate_project_files_master.bat "%EnginePath_5.2%" "%UBTRelativePath5%" "%VersionSelector%" "%ProjectName%"
