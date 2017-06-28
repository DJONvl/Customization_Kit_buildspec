@set VER_MAJOR=7
@rem default to 1107, updated in following code
@set VER_MINOR=1107
@rem default to 01, updated in following code
@set VER_BUILD=01
@set VER_PATCH=00

@for /f "tokens=*" %%a in ('%MINOR%') do @(set VER_MINOR=%%a)
@for /f "tokens=*" %%a in ('%MINOR% -w') do @(set VER_BUILD=%%a)

@IF "%VER_PATCH%" == "00" (
	@set VERSION_STRING=%VER_MAJOR%.%VER_MINOR%.%VER_BUILD%
	@set RELEASE_TYPE=Official-Build
) ELSE (
	@set VERSION_STRING=%VER_MAJOR%.%VER_MINOR%.%VER_BUILD%.%VER_PATCH%
	@set RELEASE_TYPE=Patch-Build
)

@echo Import version info by setver.bat
@echo BROM_DLL Version: %VERSION_STRING% [%RELEASE_TYPE%]
