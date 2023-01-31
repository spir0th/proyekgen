;; Setup script for Inno Setup (Windows x64 build)

#define AppName "proyekgen"
#define AppVersion "1.0.0"
#define AppMainExec "{app}\proyekgen.exe"

[Setup]
AppName={#AppName}
AppVerName={cm:NameAndVersion, {#AppName}, {#AppVersion}}
AppVersion={#AppVersion}
AppComments=Project Generator
AppPublisher=spirothXYZ
AppPublisherURL=https://github.com/spirothdev
AppContact=https://github.com/spirothdev/proyekgen
AppCopyright=Copyright (c) 2021-2023 spirothXYZ
AppendDefaultDirName=no
AppendDefaultGroupName=no
ArchitecturesAllowed=x64 ia64
ArchitecturesInstallin64BitMode=x64 ia64
ChangesEnvironment=yes
DefaultDirName={autopf}\proyekgen
DefaultGroupName=proyekgen
DisableWelcomePage=no
LicenseFile={#SourcePath}\license.txt
OutputBaseFilename={#AppName}-{#AppVersion}-setup
OutputDir={#SourcePath}\..\build\windows-x64-setup
PrivilegesRequired=lowest
PrivilegesRequiredOverridesAllowed=commandline dialog
SetupIconFile=compiler:SetupClassicIcon.ico
SourceDir={#SourcePath}\..\build\windows-x64-release
UninstallDisplayIcon={#AppMainExec}, 1
UninstallDisplayName={#AppName}
WizardStyle=modern

[Types]
Name: "full"; Description: "Complete installation"
Name: "standard"; Description: "Minimal installation"
Name: "custom"; Description: "Custom installation"; Flags: iscustom

[Components]
Name: "core"; Description: "Main"; Types: full standard custom; Flags: fixed
Name: "templates"; Description: "Templates"; Types: full
Name: "templates\ccpp"; Description: "C/C++"; Types: full
Name: "templates\java"; Description: "Java"; Types: full
Name: "templates\python"; Description: "Python"; Types: full

[Files]
Source: "{#AppName}\*"; DestDir: "{app}"; Excludes: "*.ilk, *.pdb, *.manifest, *.cmake, CMakeFiles, templates"; Components: "core"
Source: "{#AppName}\templates\cmake-c\*"; DestDir: "{autoappdata}\{#AppName}\templates\cmake-c"; Flags: recursesubdirs uninsneveruninstall skipifsourcedoesntexist; Components: "templates/ccpp"
Source: "{#AppName}\templates\cmake-cpp\*"; DestDir: "{autoappdata}\{#AppName}\templates\cmake-cpp"; Flags: recursesubdirs uninsneveruninstall skipifsourcedoesntexist; Components: "templates/ccpp"
Source: "{#AppName}\templates\gradle\*"; DestDir: "{autoappdata}\{#AppName}\templates\gradle"; Flags: recursesubdirs uninsneveruninstall skipifsourcedoesntexist; Components: "templates/java"
Source: "{#AppName}\templates\maven\*"; DestDir: "{autoappdata}\{#AppName}\templates\maven"; Flags: recursesubdirs uninsneveruninstall skipifsourcedoesntexist; Components: "templates/java"
Source: "{#AppName}\templates\python\*"; DestDir: "{autoappdata}\{#AppName}\templates\python"; Flags: recursesubdirs uninsneveruninstall skipifsourcedoesntexist; Components: "templates/python"

[Run]
Filename: "{#AppMainExec}"; Description: "Run {#AppName}"; Flags: postinstall nowait skipifsilent unchecked
