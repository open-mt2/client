{
  system ? builtins.currentSystem,
  #   * `pkgs`   – recent nixos-unstable, used for the toolchain and most libraries.
  pkgs ? import (fetchTarball {
    url = "https://github.com/NixOS/nixpkgs/archive/31ecbaca1bdefa45dffc409f0250bd8039ea0010.tar.gz";
    sha256 = "1lzqyss0pznfh3az0llsp4a7c8gqcyq8wf96fnl5cnh5b3lic6kn";
  }) { inherit system; },
  #   * `legacy` – nixos-25.05, the last release that still ships Python 2.7 and
  legacy ? import (fetchTarball {
    url = "https://github.com/NixOS/nixpkgs/archive/ac62194c3917d5f474c1a844b6fd6da2db95077d.tar.gz";
    sha256 = "0v6bd1xk8a2aal83karlvc853x44dg1n4nk08jg3dajqyy0s98np";
  }) { inherit system; config.permittedInsecurePackages = [ "python-2.7.18.8" ]; },
  # Sources of the engine repositories, handed to FetchContent instead of
  # cloning them from GitHub. Default to sibling checkouts.
  neoeter ? ../neoeter,
  neoeter-python ? ../neoeter-python,
}:

let
  inherit (pkgs) lib;
  fs = lib.fileset;

  # Only what CMake reads, so .git, docs, nix files and local build trees
  # never change the derivation.
  cmakeEntries = [ "CMakeLists.txt" "cmake" "include" "src" ];

  cmakeSource = root: fs.toSource {
    inherit root;
    fileset = fs.unions (map (p: fs.maybeMissing (root + "/${p}")) cmakeEntries);
  };

  # Store paths (flake inputs, fetchFromGitHub, ...) can't be filesets.
  storeSource = src: builtins.path {
    name = "source";
    path = src;
    filter = path: _type:
      lib.elem
        (lib.head (lib.splitString "/" (lib.removePrefix "${toString src}/" path)))
        cmakeEntries;
  };

  repoSource = src: if lib.isPath src then cmakeSource src else storeSource src;

  src = cmakeSource ./.;

  # nixpkgs marks miniaudio Linux-only; on macOS drop the Linux audio
  # backends (it uses CoreAudio there). Untested on darwin.
  miniaudio =
    if pkgs.stdenv.hostPlatform.isLinux then pkgs.miniaudio
    else (pkgs.miniaudio.override {
      alsaSupport = false;
      pulseSupport = false;
      sndioSupport = false;
    }).overrideAttrs (old: {
      meta = old.meta // { platforms = lib.platforms.unix; };
    });
in
pkgs.stdenv.mkDerivation {
  pname = "metin2client";
  version = "0.1.0";

  inherit src;

  nativeBuildInputs = with pkgs; [
    cmake
    ninja
    pkg-config
    python3
  ];

  buildInputs = (with pkgs; [
    lzo
    libzip
    libjpeg
    SDL2
    freetype
    # system font lookup for the FreeType text renderer
    fontconfig
    libjack2
    libxcrypt # crypt.h, included by the vendored Python 2.7 headers
    zlib
  ]) ++ lib.optionals pkgs.stdenv.hostPlatform.isLinux (with pkgs; [
    # macOS takes OpenGL from the SDK.
    libGL
    libGLU
    # Requires.private of miniaudio.pc on Linux
    libpulseaudio
    alsa-lib
    sndio
  ]) ++ [
    miniaudio
    legacy.python27
    legacy.libdevil
  ];

  # Python 2.7 is located via FindPythonLibs / find_library in the packages.
  PYTHON_INCLUDE_DIR = "${legacy.python27}/include/python2.7";

  cmakeFlags = [
    "-DCMAKE_BUILD_TYPE=Release"
    "-DBUILD_TESTING=OFF"
    "-DBUILD_TESTS=OFF"
    "-DNEOETER_BUILD_TESTS=OFF"
    "-DCLIENT_STANDALONE=ON"
    "-DFETCHCONTENT_FULLY_DISCONNECTED=ON"
    "-DFETCHCONTENT_SOURCE_DIR_NEOETER=${repoSource neoeter}"
    "-DFETCHCONTENT_SOURCE_DIR_NEOETER-PYTHON=${repoSource neoeter-python}"
  ];

  # Legacy code passes non-literal format strings (fprintf(f, buf)).
  hardeningDisable = [ "format" ];

  ninjaFlags = [ "metin2client" ];

  installPhase = ''
    runHook preInstall
    install -Dm755 src/ui/metin2client $out/bin/metin2client
    runHook postInstall
  '';

  meta = {
    description = "OpenMT2 Metin2 client";
    mainProgram = "metin2client";
    platforms = lib.platforms.linux ++ lib.platforms.darwin;
  };
}
