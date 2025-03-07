 ; Script generated by the Inno Script Studio Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "navX-MXP"
#define MyAppVersion "0.0.000"
#define MyAppPublisher "KauaiLabs, Inc."
#define MyAppURL "http://navx-mxp.kauailabs.com"

[Setup]
SignTool=signtool
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{B8D8466D-9D83-4F6A-922B-9BE227E48E5B}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\Kauai Labs\navX-MXP
DisableDirPage=yes
DefaultGroupName=Kauai Labs\navX-MXP
DisableProgramGroupPage=yes
OutputBaseFilename=setup
Compression=lzma
SolidCompression=yes
OutputDir=.\Output_navX-MXP
SignTool=signtool

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Icons]
Name: "{group}\navXConfig"; Filename: "{app}\navXConfig\navXConfig.exe"; WorkingDir: "{app}\navXConfig"; IconFilename: "{app}\graphics\configuration.ico"
Name: "{group}\navXMagCalibrator"; Filename: "{app}\navXMagCalibrator\navXMagCalibrator.exe"; WorkingDir: "{app}\navXMagCalibrator"
Name: "{group}\navXFirmwareUpdater"; Filename: "{app}\navXFirmwareUpdater\navXFirmwareUpdater.exe"; WorkingDir: "{app}\navXFirmwareUpdater"; IconFilename: "{app}\graphics\download.ico"
Name: "{group}\navXUI"; Filename: "{app}\navXUI\navXUI"; WorkingDir: "{app}\navXUI"; IconFilename: "{app}\graphics\dashboard.ico"
Name: "{group}\Online Documentation"; Filename: "{app}\navx-mxp-software.url"; IconFilename: "{app}\graphics\information.ico"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{group}\RoboRIO Libaries & Samples"; Filename: "{win}\explorer"; Parameters: "{%HOMEPATH}\navx-mxp"

[Files]
Source: "..\drivers\windows\vcp\*.*"; DestDir: "{app}\installers\vcp"; Flags: recursesubdirs
Source: "..\drivers\windows\dfu\*.*"; DestDir: "{app}\installers\dfu"; Flags: recursesubdirs
Source: "..\c#\navXMagCalibrator\bin\Release\*.*"; DestDir: "{app}\navXMagCalibrator"; Flags: recursesubdirs
Source: "..\c#\navXConfig\bin\Release\*.*"; DestDir: "{app}\navXConfig"; Flags: recursesubdirs
Source: "..\c#\navXFirmwareUpdater\bin\Release\*.*"; DestDir: "{app}\navXFirmwareUpdater"; Flags: recursesubdirs
; Include additional 32-bit Visual Studio 2013 Redistributable files (MFC120.dll; MSVCR120.dll; vcredist_x86
; these files are referenced by the STMicroelectronics-provided STDUFiles.dll
Source: "..\installers\visual_studio\*.*"; DestDir: "{app}\navXFirmwareUpdater"; Flags: recursesubdirs
Source: "..\processing_output\*.*"; DestDir: "{app}\navXUI"; Flags: recursesubdirs
Source: "weblinks\navx-mxp-software.url"; DestDir: "{app}"

Source: "graphics\*.*"; DestDir: "{app}\graphics"; Flags: recursesubdirs
Source: "..\roborio\java\navx_frc\src\com\*.*"; DestDir: "{%HOMEPATH}\navx-mxp\java\src\com"; Flags: recursesubdirs
Source: "..\roborio\java\navx_frc\src\docs\*.*"; DestDir: "{%HOMEPATH}\navx-mxp\java\docs"; Flags: recursesubdirs
Source: "..\roborio\java\navx_frc\jar\navx_frc.jar"; DestDir: "{%HOMEPATH}\navx-mxp\java\lib"
Source: "..\roborio\java\navx_frc\jar\navx_frc.jar"; DestDir: "{%HOMEPATH}\wpilib\user\java\lib"
; Install maven rep contents to %PUBLIC%\wpilib\2020\maven, following new convention for FRC 2020 Season
Source: "..\..\..\..\.m2\repository\com\kauailabs\navx\*.*"; DestDir:  "{%PUBLIC}\wpilib\2020\maven\com\kauailabs\navx"; Flags:recursesubdirs
; Install vendordeps JSON file to %PUBLIC\wpilib\2020\vendordeps
Source: "..\build\vendordeps\*.json"; DestDir:  "{%PUBLIC}\wpilib\2020\vendordeps"
Source: "..\roborio\java\VSCodeExamples\navXMXP_Java_DataMonitor\*.*"; Excludes: "build,dist,bin"; DestDir: "{%HOMEPATH}\navx-mxp\java\examples\vscode\DataMonitor"; Flags: createallsubdirs recursesubdirs
Source: "..\roborio\java\VSCodeExamples\navXMXP_Java_FieldCentricDrive\*.*"; Excludes: "build,dist,bin"; DestDir: "{%HOMEPATH}\navx-mxp\java\examples\vscode\FieldCentricDrive"; Flags: createallsubdirs recursesubdirs
Source: "..\roborio\java\VSCodeExamples\navXMXP_Java_RotateToAngle\*.*"; Excludes: "build,dist,bin"; DestDir: "{%HOMEPATH}\navx-mxp\java\examples\vscode\RotateToAngle"; Flags: createallsubdirs recursesubdirs
Source: "..\roborio\java\VSCodeExamples\navXMXP_Java_AutoBalance\*.*"; Excludes: "build,dist,bin"; DestDir: "{%HOMEPATH}\navx-mxp\java\examples\vscode\AutoBalance"; Flags: createallsubdirs recursesubdirs
Source: "..\roborio\java\VSCodeExamples\navXMXP_Java_CollisionDetection\*.*"; Excludes: "build,dist,bin"; DestDir: "{%HOMEPATH}\navx-mxp\java\examples\vscode\CollisionDetection"; Flags: createallsubdirs recursesubdirs
Source: "..\roborio\java\VSCodeExamples\navXMXP_Java_MotionDetection\*.*"; Excludes: "build,dist,bin"; DestDir: "{%HOMEPATH}\navx-mxp\java\examples\vscode\MotionDetection"; Flags: createallsubdirs recursesubdirs
Source: "..\roborio\java\VSCodeExamples\navXMXP_ExpansionIO\*.*"; Excludes: "build,dist,bin"; DestDir: "{%HOMEPATH}\navx-mxp\java\examples\vscode\ExpansionIO"; Flags: createallsubdirs recursesubdirs

Source: "..\roborio\c++\navx_frc_cpp\Debug\*.a"; DestDir: "{%HOMEPATH}\navx-mxp\cpp\lib"
Source: "..\roborio\c++\navx_frc_cpp\Debug\*.a"; DestDir: "{%HOMEPATH}\wpilib\user\cpp\lib"
Source: "..\roborio\c++\navx_frc_cpp\include\*.*"; DestDir: "{%HOMEPATH}\navx-mxp\cpp\include"
Source: "..\roborio\c++\navx_frc_cpp\include\*.*"; DestDir: "{%HOMEPATH}\wpilib\user\cpp\include"
Source: "..\roborio\c++\navx_frc_cpp\src\*.*"; DestDir: "{%HOMEPATH}\navx-mxp\cpp\src"; Flags: createallsubdirs recursesubdirs
Source: "..\roborio\c++\doxygen\html\*.*"; DestDir: "{%HOMEPATH}\navx-mxp\cpp\docs"; Flags: createallsubdirs recursesubdirs
Source: "..\roborio\c++\VSCodeExamples\navXMXP_CPP_DataMonitor\*.*"; Excludes: "build,dist,bin"; DestDir: "{%HOMEPATH}\navx-mxp\cpp\examples\vscode\DataMonitor"; Flags: createallsubdirs recursesubdirs
Source: "..\roborio\c++\VSCodeExamples\navXMXP_CPP_FieldCentricDrive\*.*"; Excludes: "build,dist,bin"; DestDir: "{%HOMEPATH}\navx-mxp\cpp\examples\vscode\FieldCentricDrive"; Flags: createallsubdirs recursesubdirs
Source: "..\roborio\c++\VSCodeExamples\navXMXP_CPP_RotateToAngle\*.*"; Excludes: "build,dist,bin"; DestDir: "{%HOMEPATH}\navx-mxp\cpp\examples\vscode\RotateToAngle"; Flags: createallsubdirs recursesubdirs
Source: "..\roborio\c++\VSCodeExamples\navXMXP_CPP_AutoBalance\*.*"; Excludes: "build,dist,bin"; DestDir: "{%HOMEPATH}\navx-mxp\cpp\examples\vscode\AutoBalance"; Flags: createallsubdirs recursesubdirs
Source: "..\roborio\c++\VSCodeExamples\navXMXP_CPP_CollisionDetection\*.*"; Excludes: "build,dist,bin"; DestDir: "{%HOMEPATH}\navx-mxp\cpp\examples\vscode\CollisionDetection"; Flags: createallsubdirs recursesubdirs
Source: "..\roborio\c++\VSCodeExamples\navXMXP_CPP_MotionDetection\*.*"; Excludes: "build,dist,bin"; DestDir: "{%HOMEPATH}\navx-mxp\cpp\examples\vscode\MotionDetection"; Flags: createallsubdirs recursesubdirs
Source: "..\roborio\c++\VSCodeExamples\navXMXP_CPP_ExpansionIO\*.*"; Excludes: "build,dist,bin"; DestDir: "{%HOMEPATH}\navx-mxp\cpp\examples\vscode\ExpansionIO"; Flags: createallsubdirs recursesubdirs

; Remove installation of now-defunct navX-MXP library (this is now replaced by labviewae)
;Source: "..\roborio\labview\*.*"; DestDir: "{%HOMEPATH}\navx-mxp\labview"; Flags: createallsubdirs recursesubdirs
Source: "..\roborio\labviewae\*.*"; DestDir: "{code:GetLabVIEWDirName}vi.lib\Rock Robotics\WPI\ThirdParty\sensors\navX"; Flags: createallsubdirs recursesubdirs

Source: "..\android\navx_ftc\build\outputs\aar\*.aar"; DestDir: "{%HOMEPATH}\navx-mxp\android\libs" 
Source: "..\android\OpModes\*.*"; DestDir: "{%HOMEPATH}\navx-mxp\android\examples"; Flags: createallsubdirs recursesubdirs
Source: "..\android\navx_ftc\docs\*.*"; DestDir: "{%HOMEPATH}\navx-mxp\android\docs"; Flags: recursesubdirs

Source: "..\stm32\bin\navx-mxp*.hex"; DestDir: "{%HOMEPATH}\navx-mxp\firmware"; Flags:

[Run]
Filename: "{app}\installers\vcp\Win7\dpinst_amd64.exe"; Parameters: "/SW"; Flags: 64bit; OnlyBelowVersion: 0,6.2; Check: IsWin64
Filename: "{app}\installers\vcp\Win7\dpinst_x86.exe"; Parameters: "/SW"; Flags: 32bit; OnlyBelowVersion: 0,6.2; Check: not IsWin64
Filename: "{app}\installers\vcp\Win8\dpinst_amd64.exe"; Parameters: "/SW"; Flags: 64bit; MinVersion: 0,6.2; Check: IsWin64
Filename: "{app}\installers\vcp\Win8\dpinst_x86.exe"; Parameters: "/SW"; Flags: 32bit; MinVersion: 0,6.2; Check: not IsWin64
Filename: "{app}\installers\dfu\dpinst_amd64.exe"; Parameters: "/SW"; Flags: 64bit; Check: IsWin64
Filename: "{app}\installers\dfu\dpinst_x86.exe"; Parameters: "/SW"; Flags: 32bit; Check: not IsWin64
Filename: "{app}\navXFirmwareUpdater\vcredist_x86.exe"; Parameters: "/install /passive /norestart"; Flags:
Filename: "{app}\navXFirmwareUpdater\vcredist_x86_vs2005SP1_redist_KB2538242.exe"; Parameters: "/q:a"; Flags:

[Dirs]
Name: "{app}\navXFirmwareUpdater"; Flags: uninsalwaysuninstall
Name: "{app}\navXConfig"; Flags: uninsalwaysuninstall
Name: "{app}\navXMagCalibrator"; Flags: uninsalwaysuninstall
Name: "{app}\installers"
Name: "{app}\navXUI"; Flags: uninsalwaysuninstall

[Code]

function GetLabVIEWDirName(Value: string): string;
var          
  InstallPath: string;
begin
  // initialize default path, which will be returned when the following registry
  // key queries fail due to missing keys or for some different reason
  Result := 'C:\Program Files (x86)\National Instruments\LabVIEW 2019\';
  // query the first registry value; if this succeeds, return the obtained value
  if RegQueryStringValue(HKLM, 'Software\National Instruments\LabVIEW\Current Version', 'Path', InstallPath) then
    Result := InstallPath;
end;

function InitializeSetup(): Boolean;
var
 ErrorCode: Integer;
 JavaInstalled : Boolean;
 ResultMsg : Boolean;
 Versions: TArrayOfString;
 I: Integer;
 regRoot: Integer;
begin
 // Check which view of registry should be taken:
 regRoot := HKLM
 begin
  if IsWin64 then
  begin
   regRoot := HKLM64
  end;
 end;
 if (RegGetSubkeyNames(regRoot, 'SOFTWARE\JavaSoft\Java Runtime Environment', Versions)) or (RegGetSubkeyNames(regRoot, 'SOFTWARE\JavaSoft\Java Development Kit', Versions)) then
 begin
  for I := 0 to GetArrayLength(Versions)-1 do
   if JavaInstalled = true then
   begin
    //do nothing
   end else
   begin
    if ( Versions[I][2]='.' ) and ( ( StrToInt(Versions[I][1]) > 1 ) or ( ( StrToInt(Versions[I][1]) = 1 ) and ( StrToInt(Versions[I][3]) >= 7 ) ) ) then
    begin
     JavaInstalled := true;
    end else
    begin
     JavaInstalled := false;
    end;
   end;
 end else
 begin
  JavaInstalled := false;
 end;

 if JavaInstalled then
 begin
  Result := true;
 end else
    begin
  ResultMsg := MsgBox('Oracle Java v1.7 or newer not found in the system. Java 1.7 or later is required to run this application (can be installed after this installation too). Do you want to continue?',
   mbConfirmation, MB_YESNO) = idYes;
  if ResultMsg = false then
  begin
   Result := false;
  end else
  begin
   Result := true;
   ShellExec('open',
    'http://www.java.com/getjava/',
    '','',SW_SHOWNORMAL,ewNoWait,ErrorCode);
  end;
    end;
end;

end.
