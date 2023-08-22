@echo off

set EnginePath=%~1
set UBTRelativePath=%~2
set VersionSelector=%~3
set ProjectPath=%CD%\%~4

"%VersionSelector%" -switchversionsilent "%ProjectPath%" "%EnginePath%"

rem "%EnginePath%\%UBTRelativePath%" -projectfiles -progress -project="%ProjectPath%"
rem flags for builds from source: -game -engine

"%EnginePath%\%UBTRelativePath%" -projectFiles -progress -project="%ProjectPath%"