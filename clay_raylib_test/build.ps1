$ErrorActionPreference = "Stop"

$projectRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$srcDir = Join-Path $projectRoot "src"
$libsDir = Join-Path $projectRoot "libs"
$buildDir = Join-Path $projectRoot "build"
$resDir = Join-Path $projectRoot "res"
$outputPath = Join-Path $buildDir "app.exe"

$localIncludeDirs = @(
    (Join-Path $projectRoot "libs\clay"),
    (Join-Path $projectRoot "libs\raylib")
)

$raylibRoot = if ($env:RAYLIB_DIR) { $env:RAYLIB_DIR } else { "C:\raylib" }
$raylibIncludeDir = $null
$raylibLibDir = $null

foreach ($candidate in @(
    (Join-Path $raylibRoot "raylib\src"),
    (Join-Path $raylibRoot "include")
)) {
    if (Test-Path $candidate) {
        $raylibIncludeDir = $candidate
        break
    }
}

foreach ($candidate in @(
    (Join-Path $raylibRoot "raylib\src"),
    (Join-Path $raylibRoot "lib")
)) {
    if (Test-Path $candidate) {
        $raylibLibDir = $candidate
        break
    }
}

$raylibLibMsvc = if ($raylibLibDir) { Join-Path $raylibLibDir "raylib.lib" } else { $null }
$raylibLibMingw = if ($raylibLibDir) { Join-Path $raylibLibDir "libraylib.a" } else { $null }

$hasAnyRaylibLib = $raylibIncludeDir -and $raylibLibDir -and ((Test-Path $raylibLibMsvc) -or (Test-Path $raylibLibMingw))
if ($hasAnyRaylibLib) {
    Write-Host "Raylib detected at $raylibRoot"
} else {
    Write-Host "Raylib not detected (checked $raylibRoot). Building without Raylib link flags."
}

$sourceRoots = @($srcDir)
if (Test-Path $libsDir) {
    $sourceRoots += $libsDir
}

$sourceFiles = Get-ChildItem -Path $sourceRoots -Recurse -File |
    Where-Object { $_.Extension -in @('.c', '.cc', '.cpp', '.cxx') } |
    Sort-Object FullName |
    ForEach-Object { $_.FullName }

$thirdPartySourceFiles = if (Test-Path $libsDir) {
    $sourceFiles | Where-Object { $_.StartsWith($libsDir, [System.StringComparison]::OrdinalIgnoreCase) }
} else {
    @()
}
$projectSourceFiles = $sourceFiles | Where-Object { -not $_.StartsWith($libsDir, [System.StringComparison]::OrdinalIgnoreCase) }

$cSourceFiles = $sourceFiles | Where-Object { [System.IO.Path]::GetExtension($_).ToLowerInvariant() -eq '.c' }
$cppSourceFiles = $sourceFiles | Where-Object { @('.cc', '.cpp', '.cxx') -contains [System.IO.Path]::GetExtension($_).ToLowerInvariant() }
$thirdPartyCSourceFiles = $thirdPartySourceFiles | Where-Object { [System.IO.Path]::GetExtension($_).ToLowerInvariant() -eq '.c' }
$thirdPartyCppSourceFiles = $thirdPartySourceFiles | Where-Object { @('.cc', '.cpp', '.cxx') -contains [System.IO.Path]::GetExtension($_).ToLowerInvariant() }
$projectCSourceFiles = $projectSourceFiles | Where-Object { [System.IO.Path]::GetExtension($_).ToLowerInvariant() -eq '.c' }
$projectCppSourceFiles = $projectSourceFiles | Where-Object { @('.cc', '.cpp', '.cxx') -contains [System.IO.Path]::GetExtension($_).ToLowerInvariant() }

if (-not $sourceFiles -or $sourceFiles.Count -eq 0) {
    Write-Error "No C/C++ source files found in $srcDir or $libsDir"
}

$usesRaylib = Select-String -Path $sourceFiles -Pattern '^\s*#\s*include\s*[<\"]raylib\.h[>\"]' -Quiet

New-Item -ItemType Directory -Path $buildDir -Force | Out-Null

function Import-VsDevShell {
    $vswhere = Join-Path ${env:ProgramFiles(x86)} "Microsoft Visual Studio\Installer\vswhere.exe"
    if (-not (Test-Path $vswhere)) {
        return $false
    }

    $installationPath = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
    if (-not $installationPath) {
        return $false
    }

    $vsDevCmd = Join-Path $installationPath "Common7\Tools\VsDevCmd.bat"
    if (-not (Test-Path $vsDevCmd)) {
        return $false
    }

    $developerCommand = "`"$vsDevCmd`" -no_logo && set"
    $environmentBlock = & cmd.exe /s /c $developerCommand
    if ($LASTEXITCODE -ne 0) {
        return $false
    }

    foreach ($line in $environmentBlock) {
        if ($line -match "^(.*?)=(.*)$") {
            [System.Environment]::SetEnvironmentVariable($matches[1], $matches[2], "Process")
        }
    }

    return $true
}

function Invoke-CheckedCompiler {
    param(
        [string]$CompilerPath,
        [string[]]$CompilerArgs,
        [string]$CompilerDisplayName
    )

    & $CompilerPath @CompilerArgs
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Compilation failed with exit code $LASTEXITCODE using $CompilerDisplayName."
    }
}

function Add-SourceGroup {
    param(
        [ref]$ArgListRef,
        [string[]]$Files,
        [string]$Language,
        [string[]]$Flags = @()
    )

    if (-not $Files) {
        return
    }

    if ($Flags -and $Flags.Count -gt 0) {
        $ArgListRef.Value += $Flags
    }
    $ArgListRef.Value += @('-x', $Language)
    $ArgListRef.Value += $Files
}

function Invoke-GnuStyleBuild {
    param(
        [string]$CompilerPath,
        [string[]]$ProjectWarningFlags,
        [bool]$UseRaylib
    )

    $compileArgs = @('-std=c++20', '-g3', '-O0', '-fno-omit-frame-pointer')
    foreach ($includeDir in $localIncludeDirs) {
        $compileArgs += "-I$includeDir"
    }
    if ($UseRaylib) {
        $compileArgs += @("-I$raylibIncludeDir", "-L$raylibLibDir")
    }

    Add-SourceGroup -ArgListRef ([ref]$compileArgs) -Files $thirdPartyCSourceFiles -Language 'c' -Flags @('-w')
    Add-SourceGroup -ArgListRef ([ref]$compileArgs) -Files $thirdPartyCppSourceFiles -Language 'c++' -Flags @('-w')
    Add-SourceGroup -ArgListRef ([ref]$compileArgs) -Files $projectCSourceFiles -Language 'c' -Flags $ProjectWarningFlags
    Add-SourceGroup -ArgListRef ([ref]$compileArgs) -Files $projectCppSourceFiles -Language 'c++' -Flags $ProjectWarningFlags

    if (-not $projectCSourceFiles -and -not $projectCppSourceFiles) {
        # If only third-party sources exist, keep default project warnings enabled.
        $compileArgs += $ProjectWarningFlags
    }

    $compileArgs += @('-o', $outputPath)
    if ($UseRaylib) {
        $compileArgs += @('-lraylib', '-lopengl32', '-lgdi32', '-lwinmm')
    }

    Invoke-CheckedCompiler -CompilerPath $CompilerPath -CompilerArgs $compileArgs -CompilerDisplayName $compiler.Name
}

$compilerPreference = @('cl', 'g++', 'clang++')
if ((Test-Path $raylibLibMingw) -and -not (Test-Path $raylibLibMsvc)) {
    $w64DevkitBin = Join-Path $raylibRoot "w64devkit\bin"
    if (Test-Path $w64DevkitBin) {
        $env:PATH = "$w64DevkitBin;$env:PATH"
    }
    $compilerPreference = @('g++', 'clang++', 'cl')
}

$compiler = Get-Command $compilerPreference -ErrorAction SilentlyContinue | Select-Object -First 1

if (-not $compiler) {
    Import-VsDevShell | Out-Null
    $compiler = Get-Command $compilerPreference -ErrorAction SilentlyContinue | Select-Object -First 1
}

if (-not $compiler) {
    Write-Error "No supported C++ compiler found. Install Visual Studio Build Tools, GCC, or Clang, or start VS Code from a developer shell."
}

$compilerName = [System.IO.Path]::GetFileNameWithoutExtension($compiler.Name).ToLowerInvariant()

switch ($compilerName) {
    "cl" {
        $useRaylib = $hasAnyRaylibLib -and (Test-Path $raylibLibMsvc)
        if ($usesRaylib -and -not $useRaylib) {
            Write-Error "main.cpp includes raylib.h but MSVC raylib.lib was not found. Install raylib.lib for MSVC or use g++ with MinGW raylib."
        }
        if ($hasAnyRaylibLib -and -not $useRaylib) {
            Write-Host "MSVC selected but raylib.lib was not found. Building without Raylib link flags."
        }

        $clArgs = @('/nologo', '/EHsc', '/std:c++20', '/Zi', '/Od', "/Fe:$outputPath")
        foreach ($includeDir in $localIncludeDirs) {
            $clArgs += "/I$includeDir"
        }
        if ($useRaylib) {
            $clArgs += "/I$raylibIncludeDir"
        }
        $clArgs += $sourceFiles
        if ($useRaylib) {
            $clArgs += @('/link', '/DEBUG', "/LIBPATH:$raylibLibDir", 'raylib.lib', 'opengl32.lib', 'gdi32.lib', 'winmm.lib', 'user32.lib', 'shell32.lib')
        }

        Invoke-CheckedCompiler -CompilerPath $compiler.Source -CompilerArgs $clArgs -CompilerDisplayName $compiler.Name
        break
    }
    "g++" {
        $useRaylib = $hasAnyRaylibLib -and (Test-Path $raylibLibMingw)
        if ($usesRaylib -and -not $useRaylib) {
            Write-Error "main.cpp includes raylib.h but MinGW libraylib.a was not found for g++."
        }
        if ($hasAnyRaylibLib -and -not $useRaylib) {
            Write-Host "g++ selected but libraylib.a was not found. Building without Raylib link flags."
        }

        $gppWarnings = @('-Wall', '-Wextra', '-Wno-missing-field-initializers', '-Wmisleading-indentation')
        Invoke-GnuStyleBuild -CompilerPath $compiler.Source -ProjectWarningFlags $gppWarnings -UseRaylib $useRaylib
        break
    }
    "clang++" {
        $useRaylib = $hasAnyRaylibLib -and (Test-Path $raylibLibMingw)
        if ($usesRaylib -and -not $useRaylib) {
            Write-Error "main.cpp includes raylib.h but MinGW libraylib.a was not found for clang++."
        }
        if ($hasAnyRaylibLib -and -not $useRaylib) {
            Write-Host "clang++ selected but libraylib.a was not found. Building without Raylib link flags."
        }

        $clangWarnings = @('-Wall', '-Wextra', '-Wno-missing-field-initializers')
        Invoke-GnuStyleBuild -CompilerPath $compiler.Source -ProjectWarningFlags $clangWarnings -UseRaylib $useRaylib
        break
    }
    default {
        Write-Error "Unsupported compiler: $($compiler.Name)"
    }
}

Write-Host "Built $outputPath"

if (Test-Path $resDir) {
    Copy-Item -Path (Join-Path $resDir "*") -Destination $buildDir -Recurse -Force
    Write-Host "Copied resources from $resDir to $buildDir"
}

Write-Host "Done."