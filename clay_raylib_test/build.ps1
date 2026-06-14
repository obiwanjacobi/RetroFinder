$ErrorActionPreference = "Stop"

$projectRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$srcDir = Join-Path $projectRoot "src"
$libsDir = Join-Path $projectRoot "libs"
$buildDir = Join-Path $projectRoot "build"
$resDir = Join-Path $projectRoot "res"
$outputPath = Join-Path $buildDir "app.exe"

$localIncludeDirs = @(
    (Join-Path $projectRoot "libs"),
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

$cSourceFiles = $sourceFiles | Where-Object { [System.IO.Path]::GetExtension($_).ToLowerInvariant() -eq '.c' }
$cppSourceFiles = $sourceFiles | Where-Object { @('.cc', '.cpp', '.cxx') -contains [System.IO.Path]::GetExtension($_).ToLowerInvariant() }

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
        & $compiler.Source @clArgs
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

        $gnuArgs = @('-std=c++20', '-Wall', '-Wextra', '-Wno-missing-field-initializers', '-g3', '-O0', '-fno-omit-frame-pointer')
        foreach ($includeDir in $localIncludeDirs) {
            $gnuArgs += "-I$includeDir"
        }
        if ($useRaylib) {
            $gnuArgs += @("-I$raylibIncludeDir", "-L$raylibLibDir")
        }

        if ($cSourceFiles) {
            $gnuArgs += @('-x', 'c')
            $gnuArgs += $cSourceFiles
        }
        if ($cppSourceFiles) {
            $gnuArgs += @('-x', 'c++')
            $gnuArgs += $cppSourceFiles
        }

        $gnuArgs += @('-o', $outputPath)
        if ($useRaylib) {
            $gnuArgs += @('-lraylib', '-lopengl32', '-lgdi32', '-lwinmm')
        }
        & $compiler.Source @gnuArgs
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

        $clangArgs = @('-std=c++20', '-Wall', '-Wextra', '-Wno-missing-field-initializers', '-g3', '-O0', '-fno-omit-frame-pointer')
        foreach ($includeDir in $localIncludeDirs) {
            $clangArgs += "-I$includeDir"
        }
        if ($useRaylib) {
            $clangArgs += @("-I$raylibIncludeDir", "-L$raylibLibDir")
        }

        if ($cSourceFiles) {
            $clangArgs += @('-x', 'c')
            $clangArgs += $cSourceFiles
        }
        if ($cppSourceFiles) {
            $clangArgs += @('-x', 'c++')
            $clangArgs += $cppSourceFiles
        }

        $clangArgs += @('-o', $outputPath)
        if ($useRaylib) {
            $clangArgs += @('-lraylib', '-lopengl32', '-lgdi32', '-lwinmm')
        }
        & $compiler.Source @clangArgs
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
